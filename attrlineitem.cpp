#include "attrlineitem.h"

AttrLineItem::AttrLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent)
    :QGraphicsLineItem(x1, y1, x2, y2, parent)
    ,x((x1+x2)/2)
    ,y((y1+y2)/2)
{

}

void AttrLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsLineItem::paint(painter, option, widget);
    painter->drawText(QPointF{x, y}, text);

    // arrow shape {0,0} {10, 3} {10, -3}
    QPolygonF arrowShape;
    QPointF center = this->line().center();
    painter->translate(center);
    arrowShape << QPointF{0,0};
    arrowShape << QPointF{-10,5};
    arrowShape << QPointF{-10,-5};
    qreal angle = this->line().angle();
    painter->rotate(-angle);

    painter->drawPolygon(arrowShape);
}

void AttrLineItem::setText(QString text)
{
    this->text = text;
}
