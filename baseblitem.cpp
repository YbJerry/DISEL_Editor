#include "baseblitem.h"

BaseBLItem::BaseBLItem(QString name, BLTYPE type)
    : BaseBLItem(name, 0, 0, 80, 60, type)
{

}

BaseBLItem::BaseBLItem(QString name, qreal left, qreal top, qreal width, qreal height, BLTYPE type)
    : name(name)
    , left(left)
    , top(top)
    , width(width)
    , height(height)
    , penWidth(1)
    , type(type)
    , isFocused(false)
{
    setAcceptHoverEvents(true);
}

BaseBLItem::BaseBLItem(QString name, QPointF pos, qreal width, qreal height, BLTYPE type)
    : name(name)
    , left(pos.x())
    , top(pos.y())
    , width(width)
    , height(height)
    , penWidth(1)
    , type(type)
    , isFocused(false)
{
    setAcceptHoverEvents(true);
}

QRectF BaseBLItem::boundingRect() const
{
    return QRectF(left-penWidth, top-penWidth, width+penWidth, height+penWidth);
}

void BaseBLItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect(left, top, width, height);
    if(isFocused){
        painter->setBrush(Qt::gray);
    }
    switch (type) {
    case BLTYPE::ANONY:
        painter->setBrush(Qt::Dense7Pattern);
        break;
    case BLTYPE::NORMAL:
        break;
    case BLTYPE::ROOT:
        painter->drawRoundedRect(left+3, top+3, width-6, height-6, 5, 5);
        break;
    default:
        break;
    }

    painter->drawRoundedRect(rect, 5, 5);
    painter->drawText(rect, Qt::AlignCenter, name);
}

QPointF BaseBLItem::getCenterPos()
{
    return QPointF{left+width/2, top+height/2};
}

QPointF BaseBLItem::getUpperCenterPos()
{
    return QPointF{left+width/2, top};
}

QPointF BaseBLItem::getLowerCenterPos()
{
    return QPointF{left+width/2, top+height};
}

void BaseBLItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isFocused = true;
    update(QRectF(left, top, width, height));
}

void BaseBLItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isFocused = false;
    update(QRectF(left, top, width, height));
}

void BaseBLItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(name);
}
