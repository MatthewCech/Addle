#ifndef IPALETTEPRESENTER_HPP
#define IPALETTEPRESENTER_HPP

#include "interfaces/traits.hpp"



#include "utilities/model/colorinfo.hpp"
#include "utilities/qmultiarray.hpp"

#include <QMetaType>

class IPalette;
class IPalettePresenter
{
public: 
    virtual ~IPalettePresenter() = default;

    virtual void initialize(IPalette& model) = 0;

    virtual IPalette& model() const = 0;
    virtual QMultiArray<ColorInfo, 2> colors() const = 0;
};

Q_DECLARE_METATYPE(QSharedPointer<IPalettePresenter>);

DECL_EXPECTS_INITIALIZE(IPalettePresenter);
DECL_IMPLEMENTED_AS_QOBJECT(IPalettePresenter);
DECL_MAKEABLE(IPalettePresenter);

#endif // IPALETTEPRESENTER_HPP