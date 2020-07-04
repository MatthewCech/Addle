#ifndef IBRUSHMODEL_HPP
#define IBRUSHMODEL_HPP

#include "idtypes/brushid.hpp"
#include "idtypes/brushengineid.hpp"

#include "interfaces/traits/initialize_trait.hpp"
#include "interfaces/traits/makeable_trait.hpp"
#include "utilities/model/brushbuilder.hpp"

#include "interfaces/traits/by_id_traits.hpp"

class IBrushModel
{
public:
    virtual ~IBrushModel() = default;

    virtual void initialize(const BrushBuilder& builder) = 0;

    virtual BrushId id() const = 0;
    virtual BrushEngineId engineId() const = 0;

    virtual QIcon icon() const = 0;

    // Common engine parameters, like pressure and velocity dynamics, will be
    // given properties here.

    // virtual QVariantHash& customEngineParameters() = 0;
    virtual QVariantHash customEngineParameters() const = 0;

    virtual bool isSizeInvariant() const = 0;
    virtual bool isPixelAliased() const = 0;
    virtual bool eraserMode() const = 0;

    virtual double minSize() const = 0;
    virtual double maxSize() const = 0;
    virtual QList<double> preferredSizes() const = 0;
    virtual bool strictSizing() const = 0;
    virtual double preferredStartingSize() const = 0;
};

DECL_MAKEABLE(IBrushModel);
DECL_PERSISTENT_OBJECT_TYPE(IBrushModel, BrushId); // TODO: isn't actually makeable by ID ..?

#endif // IBRUSHMODEL_HPP