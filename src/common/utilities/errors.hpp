/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <cstdlib>
#include <iostream>

#include <QtGlobal>

#include "compat.hpp"

#include "interfaces/services/ierrorservice.hpp"

#include "exceptions/addleexception.hpp"
#include "exceptions/unhandledexception.hpp"

#include "servicelocator.hpp"

namespace Addle {

DECL_LOGIC_ERROR(GenericLogicError);

#ifdef ADDLE_DEBUG

/**
 * @brief A general-purpose AddleException for logic errors.
 */
class ADDLE_COMMON_EXPORT GenericLogicError : public AddleException
{
    ADDLE_EXCEPTION_BOILERPLATE(GenericLogicError);
public:
    GenericLogicError(const char* expression = nullptr, QString message = QString());
    virtual ~GenericLogicError() = default;

private:
    const char* const _expression;
    QString _message;
};

/**
 * @def
 * @brief Asserts that the given expression resolves to true.
 * 
 * If the assertion fails, a logic error AddleException with the given message
 * is thrown.
 */
#define ADDLE_ASSERT_M(expression, message) \
if (Q_UNLIKELY(!static_cast<bool>(expression))) { ADDLE_THROW(GenericLogicError(#expression, message)); }

/**
 * @def
 * @brief Asserts that the given expression resolves to true.
 * 
 * If the assertion fails, a logic error AddleException is thrown.
 */
#define ADDLE_ASSERT(expression) \
ADDLE_ASSERT_M(expression, QString())

/**
 * @def
 * Indiscriminately throws a logic error AddleException with the given message.
 */
#define ADDLE_LOGIC_ERROR_M(message) \
ADDLE_THROW(GenericLogicError(nullptr, message))

/**
 * @def
 * @brief Denotes a code path that is logically unreachable.
 * 
 * In debug builds this throws a logic error. In release builds, this is
 * equivalent to Q_UNREACHABLE().
 */
//% "A code path that was designated logically unreachable was reached."
#define ADDLE_LOGICALLY_UNREACHABLE() ADDLE_THROW(GenericLogicError(nullptr, qtTrId("debug-messages.bad-logically-unreachable")))

//% "An exception of an unknown type was caught."
#define _LAST_DITCH_CATCH(x) ServiceLocator::get<IErrorService>().reportUnhandledError(GenericLogicError(nullptr, qtTrId("debug-messages.last-ditch-catch")), x);

[[noreturn]] void ADDLE_COMMON_EXPORT _cannotReportError_impl(const std::exception* primaryEx);

#define ADDLE_SLOT_CATCH_SEVERITY(x) \
catch (const AddleException& ex) \
{ \
    if (static_cast<bool>(sender())) \
    { \
        try { ServiceLocator::get<IErrorService>().reportUnhandledError(ex, x); } \
        catch(...) { _cannotReportError_impl(&ex); } \
    } \
    else throw; \
} \
catch (const std::exception& ex) \
{ \
    if (static_cast<bool>(sender())) \
    { \
        try { ServiceLocator::get<IErrorService>().reportUnhandledError(ex, x); } \
        catch(...) { _cannotReportError_impl(&ex); } \
    } \
    else throw; \
} \
catch(...) \
{ \
    if (static_cast<bool>(sender())) \
    { \
        try { _LAST_DITCH_CATCH(x) } \
        catch(...) { _cannotReportError_impl(nullptr); } \
    } \
    else throw; \
}

#define ADDLE_EVENT_CATCH_SEVERITY(x) \
catch (const AddleException& ex) \
{ \
    try { ServiceLocator::get<IErrorService>().reportUnhandledError(ex, x); } \
    catch(...) { _cannotReportError_impl(&ex); } \
} \
catch (const std::exception& ex) \
{ \
    try { ServiceLocator::get<IErrorService>().reportUnhandledError(ex, x); } \
    catch(...) { _cannotReportError_impl(&ex); } \
} \
catch(...) \
{ \
    try { _LAST_DITCH_CATCH(x) } \
    catch(...) { _cannotReportError_impl(nullptr); } \
}

#else // ADDLE_DEBUG

class ADDLE_COMMON_EXPORT GenericLogicError : public AddleException
{
    ADDLE_EXCEPTION_BOILERPLATE(GenericLogicError);
public:
    virtual ~GenericLogicError() = default;
};

#define ADDLE_ASSERT_M(expression, message) if (Q_UNLIKELY(!static_cast<bool>(expression))) { ADDLE_THROW(GenericLogicError()); }
#define ADDLE_ASSERT(expression) ADDLE_ASSERT_M(expression, NULL)

#define ADDLE_LOGIC_ERROR_M(message) ADDLE_THROW(GenericLogicError())

#define ADDLE_LOGICALLY_UNREACHABLE() Q_UNREACHABLE()

#define ADDLE_SLOT_CATCH_SEVERITY(x) \
catch(...) \
{ \
    if (static_cast<bool>(sender())) \
    { \
        try { ServiceLocator::get<IErrorService>().reportUnhandledError(GenericLogicError(), x); } \
        catch(...) { std::terminate(); } \
    } \
    else throw; \
}

#define ADDLE_EVENT_CATCH_SEVERITY(x) \
catch(...) \
{ \
    try { ServiceLocator::get<IErrorService>().reportUnhandledError(GenericLogicError(), x); } \
    catch(...) { std::terminate(); } \
}

#endif //ADDLE_DEBUG

/**
 * @def
 * @brief A catch umbrella for use in the body of a Qt slot.
 * 
 * If the slot is invoked by the Qt event system, this will quietly report
 * exceptions to IErrorService. If the slot was called directly, the exceptions
 * will be propagated to the caller.
 * 
 * Example:
 * ```c++
 * void Spiff::freemSlot()
 * {
 *     try
 *     {
 *        // ...
 *     }
 *     ADDLE_SLOT_CATCH
 * }
 * ```
 */
#define ADDLE_SLOT_CATCH ADDLE_SLOT_CATCH_SEVERITY(UnhandledException::Normal)

/**
 * @def
 * Similar to ADDLE_SLOT_CATCH. For use in the body of methods that aren't slots 
 * but may be invoked by the Qt event system, such as `QObject::event`,
 * `QRunnable::run` or a `Q_INVOKABLE` method. This will never propagate
 * exceptions to the caller.
 */
#define ADDLE_EVENT_CATCH ADDLE_EVENT_CATCH_SEVERITY(UnhandledException::Normal)

} // namespace Addle

#endif // ERRORS_HPP