/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef COLORINFO_HPP
#define COLORINFO_HPP

#include <boost/optional.hpp>
#include <QSharedData>
#include <QColor>
#include <QPoint>
#include <QMetaType>

#include "utilities/translatedstring.hpp"

namespace Addle {

class ColorInfo
{
    struct ColorInfoInner : QSharedData 
    {
        ColorInfoInner() = default;
        ColorInfoInner(QColor color_, TranslatedString name_, boost::optional<QPoint> pos_)
            : color(color_), name(name_), pos(pos_)
        {
        }

        QColor color;
        TranslatedString name;

        boost::optional<QPoint> pos;
    };

public: 
    ColorInfo()
        : _data(new ColorInfoInner)
    {
    }

    ColorInfo(QColor color, TranslatedString name = TranslatedString(), boost::optional<QPoint> pos = boost::optional<QPoint>())
        : _data( new ColorInfoInner(color, name, pos))
    {
    }

    ColorInfo(const ColorInfo&) = default;

    bool operator==(const ColorInfo& x) const
    { 
        return _data->color == x._data->color
            && _data->name == x._data->name
            && _data->pos == x._data->pos;
    }
    bool operator!=(const ColorInfo& x) const { return !(*this == x); }

    QColor color() const { return _data->color; }
    void setColor(QColor color) { _data->color = color; }

    TranslatedString name() const { return _data->name; }
    void setName(TranslatedString name) { _data->name = name; }

    boost::optional<QPoint> pos() const { return _data->pos; }
    void setPos(boost::optional<QPoint> pos) { _data->pos = pos; }

private: 
    QSharedDataPointer<ColorInfoInner> _data;
};

inline uint qHash(ColorInfo info, uint seed)
{ 
    uint hash = ::qHash(info.color().rgb(), seed);
    if (!info.name().isEmpty())
        hash ^= ::qHash(info.name(), seed);
    if (info.pos())
        hash ^= ::qHash(qMakePair(info.pos()->x(), info.pos()->y()), seed);
    return hash;
}

} // namespace Addle

Q_DECLARE_METATYPE(Addle::ColorInfo);

#endif // COLORINFO_HPP