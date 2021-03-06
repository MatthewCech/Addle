/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef RENDERDATA_HPP
#define RENDERDATA_HPP

#include <QSharedData>

#include <QRect>
#include <QPainter>
#include <QPainterPath>
namespace Addle {

class RenderData
{
public:
    enum Hint
    {
        None = 0x0,

        // Tells render steps to render at full opacity even if 
        FullOpacity = 0x1

    };
    Q_DECLARE_FLAGS(Hints, Hint);

private:
    struct RenderDataInner : QSharedData
    {
        RenderDataInner() = default;

        RenderDataInner(QRect area_, QPainter* painter_)
            : area(area_), painter(painter_)
        {
        }

        QPainterPath mask;
        QRect area;
        QPainter* painter = nullptr;
        bool aborted = false;
    };
public:

    RenderData()
        : _data(new RenderDataInner)
    {
    }

    RenderData(QRect area, QPainter* painter)
        : _data(new RenderDataInner(area, painter))
    {
    }

    RenderData(const RenderData& other)
        : _data(other._data)
    {
    }

    RenderData(RenderData&& other)
    {
        _data.swap(other._data);
    }

    void operator=(const RenderData& other)
    {
        _data = other._data;
    }

    QPainter* painter() const { return _data->painter; }

    void setArea(QRect area) { _data->area = area; }
    QRect area() const { return _data->area; }

    bool isAborted() const { return _data->aborted; }
    void abort() { _data->aborted = true; }

private:
    QSharedDataPointer<RenderDataInner> _data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RenderData::Hints);
} // namespace Addle

#endif // RENDERDATA_HPP