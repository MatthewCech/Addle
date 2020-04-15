#ifndef RENDERUTILS_HPP
#define RENDERUTILS_HPP

#include <QList>
#include <QWeakPointer>
#include "interfaces/rendering/irenderstep.hpp"
#include "servicelocator.hpp"

inline void render(QList<QWeakPointer<IRenderStep>> steps, RenderData data)
{
    auto stack = ServiceLocator::makeUnique<IRenderStack>(steps);
    stack->render(data);
}

inline void render(QList<QWeakPointer<IRenderStep>> steps, QRect area, QPainter* painter)
{
    RenderData data(area, painter);
    render(steps, data);
}

inline void render(QWeakPointer<IRenderStep> step, QRect area, QPainter* painter)
{
    render(QList<QWeakPointer<IRenderStep>> { step }, area, painter);
}
// void isolatedRender(IRenderStep& step, RenderData data)
// {
//     data.getPainter()->save();

//     step.onPush(data);
//     step.onPop(data);

//     data.getPainter()->restore();
// }

// void isolatedRender(IRenderStep& step, QRect area, QPainter* painter)
// {
//     isolatedRender(step, RenderData(area, painter));
// }

#endif // RENDERUTILS_HPP