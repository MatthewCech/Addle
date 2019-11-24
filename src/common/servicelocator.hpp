/**
 * Addle source code
 * @file common/servicelocator.h
 * @copyright Copyright 2019 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef SERVICELOCATOR_HPP
#define SERVICELOCATOR_HPP

#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <QHash>
#include <QMutex>
#include <QSharedPointer>

#include "common/interfaces/servicelocator/ifactory.hpp"
#include "common/interfaces/servicelocator/iservicelocator.hpp"
#include "common/interfaces/services/iservice.hpp"

#include "common/exceptions/servicelocatorexceptions.hpp"

#include "./interfaces/traits/initialize_traits.hpp"
#include "./utilities/qt_extensions/qhash.hpp"

/**
 * @class ServiceLocator
 * @brief Use the public static members of this class to access services and to
 * use object factories.
 * 
 * The ServiceLocator serves as a mediator between other classes in the
 * application, providing them access to each other without entangling them in
 * a dependency web. The Service Locator is itself essentially only an interface
 * relying on an external configuration to define its full behavior.
 * 
 * @note
 * Accessing ServiceLocator uses runtime type checking and QHash accessors. It 
 * is expected to be used infrequently by persistent objects. Be mindful of
 * situations where this may be too expensive.
 * 
 * @todo
 * ServiceLocator is closely tied to initialization, but I've also exposed some
 * options to willfully skip initialization. This feels necessary since I want
 * to model initialization in Addle as a convention and not a specialization of
 * C++, but I don't actually know under what circumstances an uninitialized
 * object would be useful -- and they could represent a major threading hazard.
 */
class ServiceLocator : public IServiceLocator
{
public:
    /**
     * @brief Get a service.
     * 
     * Provides a reference to the ServiceLocator's owned instance of the
     * service specified by `Interface`. If no such instance exists, then one is
     * created.
     * 
     * @tparam Interface
     * The interface of the desired service
     * 
     * @note
     * Interface must inherit IService.
     */
    template<class Interface>
    static Interface& get()
    {
        if (!_instance)
        {
            ADDLE_THROW(ServiceLocatorNotInitializedException());
        }

        return *_instance->get_p<Interface>();
    }

    /**
     * @brief Make an object.
     * 
     * Creates a new object that implements the interface specified by
     * Interface, and returns a pointer to this new object. If Interface
     * expects to be initialized, and it exposes an `initialize()` method
     * with no required parameters, it will be automatically initialized.
     * 
     * @tparam Interface
     * The interface of the desired object
     * 
     * @note
     * Interface must be make-able, i.e., must have trait is_makeable
     * 
     * @note
     * The caller is responsible to delete this object.
     *   
     * @note
     * If `SERVICELOCATOR_NO_AUTOINITIALIZE` is defined, then the object will
     * not be initialized.
     * 
     * @sa
     * expects_initialize, is_makeable
     */
    template<class Interface>
    static Interface* make()
    {
        if (!_instance)
        {
            ADDLE_THROW(ServiceLocatorNotInitializedException());
        }

        Interface* result = _instance->make_p<Interface>();

#ifndef SERVICELOCATOR_NO_AUTOINITIALIZE
        noParameterInitializeHelper(result, expects_initialize<Interface>());
#endif //SERVICELOCATOR_NO_AUTOINITIALIZE

        return result;
    }

#ifndef SERVICELOCATOR_NO_AUTOINITIALIZE
    /**
     * @brief Make an object.
     * 
     * Creates a new object that implements the interface specified by
     * Interface, then calls the initialize function on that object with the
     * arguments given.
     * 
     * @tparam Interface
     * The interface of the desired object
     * 
     * @param args
     * The arguments of this function must be the same as if calling
     * `Interface::initialize()`
     * 
     * @note
     * Interface must be make-able, i.e., must have trait is_makeable
     * 
     * @note
     * `Interface` must expect initialization. To make interfaces that don't
     * expect initialization, use make() instead.
     * 
     * @note
     * The caller is responsible to delete this object.
     * 
     * @note
     * If `SERVICELOCATOR_NO_AUTOINITIALIZE` is defined, this method will not
     * be declared.
     * 
     * @sa
     * expects_initialize, is_makeable
     */
    template<class Interface, typename... ArgTypes>
    static Interface* make(ArgTypes... args)
    {
        static_assert(
            expects_initialize<Interface>::value,
            "Calling ServiceLocator::make(...) requires Interface to expect initialization."
        );

        if (!_instance)
        {
            ADDLE_THROW(ServiceLocatorNotInitializedException());
        }

        Interface* result = _instance->make_p<Interface>();

        static_assert(
            std::is_same<void, decltype(result->initialize(args...))>::value,
            "Interface::initialize(...) must be return type void"
        );
        
        result->initialize(args...);

        return result;
    }

    /**
     * @brief Make an uninitialized object.
     * 
     * Creates a new object that implements the interface specified by 
     * `Interface`, and returns a pointer to this new object. Specifically does
     * not initialize the object.
     *  
     * @note
     * Interface must be make-able, i.e., must have trait is_makeable
     * 
     * @note
     * `Interface` must expect initialization. To make interfaces that don't
     * expect initialization, use make() instead.
     * 
     * @note
     * The caller is responsible to delete this object.
     * 
     * @note
     * If `SERVICELOCATOR_NO_AUTOINITIALIZE` is defined, this method will not
     * be declared.
     * 
     * @sa
     * expects_initialize, is_makeable
     */
    template<class Interface>
    static Interface* make_uninitialized()
    {
        static_assert(
            expects_initialize<Interface>::value,
            "Calling ServiceLocator::make_uninitialized() requires Interface to expect initialization."
        );

        if (!_instance)
        {
            ADDLE_THROW(ServiceLocatorNotInitializedException());
        }

        return _instance->make_p<Interface>();;
    }
#endif // SERVICELOCATOR_NO_AUTOINITIALIZE 

private:

    ServiceLocator() = default;
    virtual ~ServiceLocator();

    static ServiceLocator* _instance;
    
    QHash<std::type_index, IFactory*> _factoryregistry;
    QHash<std::type_index, IService*> _serviceregistry;
    QHash<std::type_index, QSharedPointer<QMutex>> _serviceInitMutexes;
    
    // Private internals of getting a service
    template<class Interface>
    Interface* get_p()
    {
        //todo: thread safety
        static_assert(
            std::is_base_of<IService, Interface>::value,
            "Interface must inherit IService"
        );
        static_assert(
            !std::is_same<IService, Interface>::value,
            "IService is the common base interface of services. It cannot be gotten directly."
        );
        
        std::type_index interfaceIndex(typeid(Interface));
        
        if (_serviceInitMutexes.contains(interfaceIndex))
        {
            // An initialization mutex exists for this service, i.e., it is
            // currently being constructed in another thread. We will wait
            // until it is finished.

            auto mutex = _serviceInitMutexes.value(interfaceIndex);
            mutex->lock();
            mutex->unlock();
        }

        if (_serviceregistry.contains(interfaceIndex))
        {   
            // This service has already been made. Return the locator's instance.

            IService* service = _serviceregistry[interfaceIndex];
            return dynamic_cast<Interface*>(service);
        }
        else
        {
            // This service has not yet been made. We must make it now.

            if (!_factoryregistry.contains(interfaceIndex))
            {
#ifdef ADDLE_DEBUG
                ServiceNotFoundException ex(
                        typeid(Interface).name(),
                        _serviceregistry.size(),
                        _factoryregistry.size()
                );
#else
                ServiceNotFoundException ex;
#endif
                ADDLE_THROW(ex);
            }
            
            auto mutex = QSharedPointer<QMutex>(new QMutex);
            _serviceInitMutexes.insert(interfaceIndex, mutex);
            mutex->lock();

            Interface* service = make_p<Interface>();
            service->setServiceLocator(this);
            _serviceregistry[interfaceIndex] = service;

            _serviceInitMutexes.remove(interfaceIndex);
            mutex->unlock();

            return service;
        }
    }

    // Private internals of making an object
    template<class Interface>
    Interface* make_p()
    {
        static_assert(
            std::is_base_of<IMakeable, Interface>::value,
            "Interface must inherit type IMakeable"
        );
        static_assert(
            !std::is_same<IMakeable, Interface>::value,
            "IMakeable is an abstract base type: it cannot be made directly."
        );

        std::type_index interfaceIndex(typeid(Interface));
        
        if (!_factoryregistry.contains(interfaceIndex))
        {
#ifdef ADDLE_DEBUG
            FactoryNotFoundException ex(
                    typeid(Interface).name(),
                    _factoryregistry.size()
            );
#else
            FactoryNotFoundException ex;
#endif
            ADDLE_THROW(ex);
        }
        
        IFactory* factory = _factoryregistry[interfaceIndex];
        IMakeable* product = factory->make();

        Interface* result = dynamic_cast<Interface*>(product);

        if (!result)
        {
#ifdef ADDLE_DEBUG
            InvalidFactoryProductException ex(
                    typeid(Interface).name(),
                    factory->getProductTypeName(),
                    factory->getFactoryTypeName()
            );
#else
            InvalidFactoryProductException ex;
#endif
            ADDLE_THROW(ex);
        }

        return result;
    }

#ifndef SERVICELOCATOR_NO_AUTOINITIALIZE
    // Helper functions to conditionally call no-parameter initialize() on
    // interfaces than expect it.

    template<class Interface>
    static void noParameterInitializeHelper(Interface* object, std::true_type)
    {
        static_assert(
            std::is_same<void, decltype(object->initialize())>::value,
            "Interface::initialize() must be return type void"
        );

        object->initialize();
    }
    
    template<class Interface>
    static void noParameterInitializeHelper(Interface* object, std::false_type)
    {
        Q_UNUSED(object)
    }
#endif // SERVICELOCATOR_NO_AUTOINITIALIZE

    friend class BaseServiceConfiguration;

};

#endif // SERVICELOCATOR_HPP
