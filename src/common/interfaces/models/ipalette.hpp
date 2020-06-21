#ifndef ICOLORPALETTE_HPP
#define ICOLORPALETTE_HPP

#include "utilities/rectangulararray.hpp"
#include <QColor>

#include "interfaces/traits/qobject_trait.hpp"
#include "interfaces/traits/makeable_trait.hpp"
#include "interfaces/traits/initialize_trait.hpp"
#include "interfaces/traits/by_id_traits.hpp"

#include "utilities/model/colorinfo.hpp"
#include "idtypes/paletteid.hpp"
#include "utilities/model/palettebuilder.hpp"

class IPalette
{
public:
    virtual ~IPalette() = default;

    virtual PaletteId id() const = 0;

    virtual void initialize(const PaletteBuilder& builder) = 0;

    virtual RectangularArray<ColorInfo> colors() const = 0;
    virtual void setColors(RectangularArray<ColorInfo> colors) = 0;

signals: 
    virtual void colorsChanged() = 0;
};

DECL_IMPLEMENTED_AS_QOBJECT(IPalette);
DECL_MAKEABLE(IPalette)
DECL_EXPECTS_INITIALIZE(IPalette);
DECL_PERSISTENT_OBJECT_TYPE(IPalette, PaletteId);

#endif // ICOLORPALETTE_HPP