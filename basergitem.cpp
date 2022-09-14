#include "basergitem.h"

BaseRGItem::BaseRGItem(QString name, RGTYPE type)
    :BaseRGItem(name, 0, 0, 80, 60, type)
{

}

BaseRGItem::BaseRGItem(QString name, qreal left, qreal top, qreal width, qreal height, RGTYPE type)
    : name(name)
    , left(left)
    , top(top)
    , width(width)
    , height(height)
    , penWidth(1)
    , type(type)
{

}

BaseRGItem::BaseRGItem(QString name, QPointF pos, qreal width, qreal height, RGTYPE type)
    : name(name)
    , left(pos.x())
    , top(pos.y())
    , width(width)
    , height(height)
    , penWidth(1)
    , type(type)
{

}

QRectF BaseRGItem::boundingRect() const
{
    return QRectF{left-2*penWidth, top-2*penWidth, width+2*penWidth, height+2*penWidth};
}

void BaseRGItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect(left, top, width, height);
//    if(isFocused){
//        QPen pen;
//        pen.setColor(Qt::magenta);
//        painter->setPen(pen);
//    }
    painter->fillRect(rect, Qt::white);

    switch (type) {
    case RGTYPE::NORMAL:
        break;
    case RGTYPE::ROOT:
        painter->drawRoundedRect(left+3, top+3, width-6, height-6, 5, 5);
        break;
    default:
        break;
    }

    painter->drawRoundedRect(rect, 5, 5);
    if(name.contains('.')){
        auto splitedName = name.split('.');
        painter->drawText(rect, Qt::AlignCenter, splitedName[0]+".\n"+splitedName[1]);
    }else{
        painter->drawText(rect, Qt::AlignCenter, name);
    }
}

void BaseRGItem::notifyLinkingItems(bool focus)
{

}

void BaseRGItem::setPosAndSize(QPointF pos, qreal width, qreal height)
{
    this->left = pos.x();
    this->top = pos.y();
    this->width = width;
    this->height = height;
}

void BaseRGItem::setCenterX(int x)
{
    left = x - width/2;
}

void BaseRGItem::setCenterY(int y)
{
    top = y + height/2;
}

qreal BaseRGItem::getCenterX() const
{
    return left + width/2;
}

qreal BaseRGItem::getCenterY() const
{
    return top + height/2;
}
