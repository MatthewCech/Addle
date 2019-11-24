#ifndef ITASK_HPP
#define ITASK_HPP

#include "common/interfaces/traits/initialize_traits.hpp"
#include "common/interfaces/traits/qobject_trait.hpp"

#include <QSharedPointer>

class ITaskController;

class ITask
{
public:
    virtual ~ITask() = default;

    virtual void run() = 0;

    virtual QSharedPointer<ITaskController> getController() = 0;
};

#include "itaskcontroller.hpp"

#endif // ITASK_HPP