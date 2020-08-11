#ifndef ILAYERPRESENTER_HPP
#define ILAYERPRESENTER_HPP

#include "interfaces/traits.hpp"



#include "idocumentpresenter.hpp"

#include "interfaces/models/ilayer.hpp"

#include <QWeakPointer>
#include <QPainter>
#include <QRectF>

class IRenderStack;
class ILayerPresenter
{
public:
    virtual ~ILayerPresenter() = default;
    
    virtual void initialize(IDocumentPresenter* documentPresenter, QSharedPointer<ILayer> layer = nullptr) = 0;
    virtual IDocumentPresenter* getDocumentPresenter() = 0;

    virtual QSharedPointer<ILayer> getModel() = 0;

    virtual QString name() const = 0;
    virtual void setName(QString name) = 0;

    virtual IRenderStack& getRenderStack() = 0;

signals: 
    virtual void nameChanged(QString name) = 0;

    virtual void updated(QRect area) = 0;
};

DECL_MAKEABLE(ILayerPresenter)
DECL_EXPECTS_INITIALIZE(ILayerPresenter)
DECL_IMPLEMENTED_AS_QOBJECT(ILayerPresenter)

#endif // ILAYERPRESENTER_HPP