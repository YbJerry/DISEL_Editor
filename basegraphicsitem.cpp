#include "basegraphicsitem.h"

BaseGraphicsItem::BaseGraphicsItem()
{

}

void BaseGraphicsItem::addNotifyLine(QGraphicsLineItem *line)
{
    notifyLineVec.push_back(line);
}

void BaseGraphicsItem::addNotifyItem(BaseGraphicsItem *item)
{
    notifyItemVec.push_back(item);
}

void BaseGraphicsItem::setFocused(bool focus)
{
    isFocused = focus;
}
