#ifndef BASEBLITEM_H
#define BASEBLITEM_H

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QPainter>
#include <QTabWidget>

#include "rootedgraphmodel.h"

enum class BLTYPE{
    ANONY,
    ROOT,
    NORMAL
};

class BaseBLItem : public QGraphicsObject
{
    Q_OBJECT

public:
    BaseBLItem(QString name, BLTYPE type = BLTYPE::ANONY);
    BaseBLItem(QString name, qreal left, qreal top, qreal width, qreal height, BLTYPE type = BLTYPE::ANONY);
    BaseBLItem(QString name, QPointF pos, qreal width, qreal height, BLTYPE type = BLTYPE::ANONY);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPointF getCenterPos();
    QPointF getUpperCenterPos();
    QPointF getLowerCenterPos();
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked(QString conceptName);

private:
    QString name;
    qreal penWidth;
    qreal left, top;
    qreal width, height;
    BLTYPE type;
    bool isFocused;
};

#endif // BASEBLITEM_H
