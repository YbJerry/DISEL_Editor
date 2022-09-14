#ifndef ATTRLINEITEM_H
#define ATTRLINEITEM_H

#include <QGraphicsLineItem>
#include <QPainter>

class AttrLineItem : public QGraphicsLineItem
{
public:
    AttrLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void setText(QString text);

private:
    // attr tag position
    qreal x, y;
    QString text;
};

#endif // ATTRLINEITEM_H
