#ifndef BASEGRAPHICSITEM_H
#define BASEGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>

class BaseGraphicsItem : public QGraphicsObject
{
public:
    BaseGraphicsItem();

    virtual void addNotifyLine(QGraphicsLineItem *line);
    virtual void addNotifyItem(BaseGraphicsItem *item);
    virtual void notifyLinkingItems(bool focus) = 0;
    virtual void setFocused(bool focus);

protected:
    QVector<QGraphicsLineItem *> notifyLineVec;
    QVector<BaseGraphicsItem *> notifyItemVec;
    bool isFocused;
};

#endif // BASEGRAPHICSITEM_H
