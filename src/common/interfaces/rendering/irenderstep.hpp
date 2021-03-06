/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef IRENDERSTEP_HPP
#define IRENDERSTEP_HPP

#include <QPainterPath>

#include "interfaces/traits.hpp"
#include "interfaces/iamqobject.hpp"
#include "utilities/render/renderdata.hpp"

namespace Addle {

class IRenderStep : public virtual IAmQObject
{
public:
    virtual ~IRenderStep() = default;

    virtual void onPush(RenderData& data) = 0;
    virtual void onPop(RenderData& data) = 0;

    // virtual bool isVisible() const = 0;
    // virtual void hide() = 0;
    // virtual void unhide() = 0;

    virtual QRect areaHint() = 0;

signals: 
    virtual void changed(QRect area) = 0;
    // virtual void removeFromStack() = 0;
};

} // namespace Addle

Q_DECLARE_INTERFACE(Addle::IRenderStep, "org.addle.IRenderStep")

#endif // IRENDERSTEP_HPP