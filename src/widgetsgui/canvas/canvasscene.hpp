#ifndef CANVASSCENE_HPP
#define CANVASSCENE_HPP

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include "layeritem.hpp"

//#include "interfaces/presenters/icanvaspresenter.hpp"

class ICanvasPresenter;
class CanvasItem;
class CanvasScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CanvasScene(ICanvasPresenter& presenter, QObject* parent = nullptr);
    virtual ~CanvasScene() = default;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

    void drawBackground(QPainter* painter, const QRectF& rect);

private:
    CanvasItem* _canvasItem;

    ICanvasPresenter& _presenter;
};

#endif // CANVASSCENE_HPP