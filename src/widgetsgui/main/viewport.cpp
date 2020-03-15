#include "viewport.hpp"
#include <QtDebug>

#include <QPaintEngine>

#include <QLayout>
#include <QGridLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include <QPolygonF>
#include <QResizeEvent>

#include "servicelocator.hpp"
#include "utilities/qtextensions/qobject.hpp"
#include "utilities/mathutils.hpp"

#include "interfaces/presenters/imaineditorpresenter.hpp"
#include "interfaces/presenters/toolpresenters/itoolpresenter.hpp"

#include "widgetsgui/utilities/graphicsmouseeventblocker.hpp"
#include "utilities/presenter/propertybinding.hpp"
#include "widgetsgui/utilities/guiutils.hpp"
#include "widgetsgui/canvas/canvasscene.hpp"

#include <QScrollBar>

ViewPort::ViewPort(IViewPortPresenter* presenter)
{
    _presenter = presenter;

    QGraphicsView::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsView::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QGraphicsView::setTransformationAnchor(ViewportAnchor::AnchorViewCenter);

    //_canvasScene = new CanvasScene(*canvasPresenter, this);
    //setScene(_canvasScene);

    verticalScrollBar()->disconnect(this);
    horizontalScrollBar()->disconnect(this);

    connect_interface(
        _presenter,
        SIGNAL(transformsChanged()),
        this,
        SLOT(onTransformsChanged()),
        // QueuedConnection is important for avoiding jitter with tools that move
        // the viewport, on X11. X11 mouse events are asynchronous so changing
        // the transforms inside the mouse event handler causes Qt to calculate
        // positions based on mismatched mouse positions and transforms.
        Qt::ConnectionType::QueuedConnection
    );

    new PropertyBinding(
        this,
        "_mainPresenterIsEmpty",
        qobject_interface_cast(_presenter->getMainEditorPresenter()), 
        IMainEditorPresenter::Meta::Properties::empty,
        PropertyBinding::ReadOnly
    );

    QFrame::setFrameShape(QFrame::Shape::StyledPanel);

    _backgroundTexture = checkerBoardTexture(35, Qt::gray, Qt::lightGray);
}

void ViewPort::resizeEvent(QResizeEvent *event)
{
    _cache_viewPortRect = QAbstractScrollArea::viewport()->contentsRect();

    //auto s_presenter = _presenter.toStrongRef();
    _presenter->setSize(_cache_viewPortRect.size());
}

void ViewPort::moveEvent(QMoveEvent *event)
{
    //auto s_presenter = _presenter.toStrongRef();
    _presenter->setGlobalOffset(QWidget::mapToGlobal(QWidget::contentsRect().topLeft()));
}

void ViewPort::onTransformsChanged()
{
    _cache_ontoCanvasTransform = _presenter->getOntoCanvasTransform();
    _cache_fromCanvasTransform = _presenter->getFromCanvasTransform();

    //auto s_presenter = _presenter.toStrongRef();

    if (!scene())
        return;

    //QPointF vptl = _cache_ontoCanvasTransform.map()

    QRectF bound = _presenter->getOntoCanvasTransform().mapRect(QRectF(QPointF(), _presenter->getSize()));

    // Moving the scene while the mouse is pressed can cause additional mouse
    // events to be sent with positions based on outdated transforms, and this
    // causes jitter when a tool is moving the viewport. We block these events
    // until the transform is done.
    GraphicsMouseEventBlocker blocker;
    QGraphicsView::scene()->installEventFilter(&blocker);

    QGraphicsView::setSceneRect(bound);
    QGraphicsView::setTransform(_cache_fromCanvasTransform);
    QGraphicsView::centerOn(_presenter->getPosition());

    QGraphicsView::scene()->removeEventFilter(&blocker);
}

void ViewPort::drawBackground(QPainter* painter, const QRectF& rect)
{
    if (_cache_mainPresenterIsEmpty) return;

    painter->save();

    painter->setTransform(painter->worldTransform());
    painter->drawTiledPixmap(
        _cache_fromCanvasTransform.mapRect(rect).intersected(_cache_viewPortRect),
        _backgroundTexture,
        _cache_viewPortRect.center()
    );

    painter->restore();
}