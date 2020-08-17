#ifndef PALETTE_HPP
#define PALETTE_HPP

#include "compat.hpp"
#include <QObject>
#include <QHash>
#include <QPair>
#include "interfaces/models/ipalette.hpp"

#include "utilities/initializehelper.hpp"
namespace Addle {
class ADDLE_CORE_EXPORT Palette : public QObject, public IPalette
{
    Q_OBJECT
    IAMQOBJECT_IMPL
public:
    virtual ~Palette() = default;

    PaletteId id() const { _initHelper.check(); return _id; }

    void initialize(const PaletteBuilder& builder);

    ColorArray colors() const { _initHelper.check(); return _colors; }

    bool contains(QColor color) const { return _index.contains(color.rgb()); }
    ColorInfo infoFor(QColor color) const { return _index[color.rgb()]; }

public slots:
    void setColors(ColorArray colors);

signals: 
    void colorsChanged(ColorArray colors);

private:
    void buildIndex();

    PaletteId _id;

    ColorArray _colors;
    QHash<QRgb, ColorInfo> _index;

    InitializeHelper _initHelper;
};

} // namespace Addle

#endif // PALETTE_HPP