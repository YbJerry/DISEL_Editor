//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
    return QRectF(left-2*penWidth, top-2*penWidth, width+2*penWidth, height+2*penWidth);
}

void BaseBLItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect(left, top, width, height);
    if(isFocused){
        QPen pen;
        pen.setColor(Qt::magenta);
        painter->setPen(pen);
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

void BaseBLItem::redraw()
{
    update(QRectF(left, top, width, height));
}

void BaseBLItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isFocused = true;
    notifyLinkingItems(isFocused);
    redraw();
}

void BaseBLItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isFocused = false;
    notifyLinkingItems(isFocused);
    redraw();
}

void BaseBLItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(name);
}

void BaseBLItem::addNotifyLine(QGraphicsLineItem *line)
{
    notifyLineVec.push_back(line);
}

void BaseBLItem::addNotifyBLItem(BaseBLItem *blItem)
{
    notifyBLVec.push_back(blItem);
}

void BaseBLItem::notifyLinkingItems(bool focus)
{
    QPen pen;
    pen.setColor(focus?Qt::magenta:Qt::black);

    for(auto line:notifyLineVec){
        line->setPen(pen);
    }

    for(auto blItem:notifyBLVec){
        blItem->setFocused(focus);
        blItem->notifyLinkingItems(focus);
        blItem->redraw();
    }
}

void BaseBLItem::setFocused(bool focus)
{
    isFocused = focus;
}
