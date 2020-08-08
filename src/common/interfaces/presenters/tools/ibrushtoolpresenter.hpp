#ifndef IBRUSHTOOLPRESENTER_HPP
#define IBRUSHTOOLPRESENTER_HPP

#include "compat.hpp"

#include "itoolpresenter.hpp"

#include "idtypes/brushid.hpp"

#include "interfaces/traits/initialize_trait.hpp"
#include "interfaces/traits/qobject_trait.hpp"
#include "interfaces/traits/makeable_trait.hpp"
#include "interfaces/traits/metaobjectinfo.hpp"

namespace IBrushToolPresenterAux
{
    ADDLE_COMMON_EXPORT extern const ToolId BRUSH_ID;
    ADDLE_COMMON_EXPORT extern const ToolId ERASER_ID;

    enum Mode
    {
        Brush,
        Eraser
    };

    struct ADDLE_COMMON_EXPORT DefaultBrushes
    {
        static const BrushId Basic;
        static const BrushId Soft;
    };

    struct ADDLE_COMMON_EXPORT DefaultErasers
    {
        static const BrushId Basic;
    };

    ADDLE_COMMON_EXPORT extern const BrushId DEFAULT_BRUSH; // DefaultBrushes::Basic
    ADDLE_COMMON_EXPORT extern const BrushId DEFAULT_ERASER;
}

class IMainEditorPresenter;
class IAssetSelectionPresenter;
class IBrushPresenter;
class ICanvasPresenter;
class IViewPortPresenter;
class IColorSelectionPresenter;
class IBrushToolPresenter : public virtual IToolPresenter
{
public:
    INTERFACE_META(IBrushToolPresenter)

    typedef IBrushToolPresenterAux::Mode Mode;

    virtual ~IBrushToolPresenter() = default;

    virtual void initialize(IMainEditorPresenter* owner, Mode mode) = 0;

    virtual IAssetSelectionPresenter& brushSelection() = 0;

    virtual void selectBrush(BrushId id) = 0;
    virtual BrushId selectedBrush() = 0;

    virtual QSharedPointer<IBrushPresenter> selectedBrushPresenter() = 0;
signals:
    virtual void brushChanged(BrushId brush) = 0;
    virtual void refreshPreviews() = 0;
};

DECL_INTERFACE_META_PROPERTIES(IBrushToolPresenter,
    DECL_INTERFACE_PROPERTY(selectedBrush)
)

DECL_MAKEABLE(IBrushToolPresenter)
DECL_IMPLEMENTED_AS_QOBJECT(IBrushToolPresenter)
DECL_EXPECTS_INITIALIZE(IBrushToolPresenter)

#endif // IBRUSHTOOLPRESENTER_HPP