#ifndef BASERGITEM_H
#define BASERGITEM_H

#include <QPainter>

#include "basegraphicsitem.h"

enum class RGTYPE{
    ROOT,
    NORMAL
};

class BaseRGItem: public BaseGraphicsItem
{
    Q_OBJECT
public:
    BaseRGItem(QString name, RGTYPE type = RGTYPE::NORMAL);
    BaseRGItem(QString name, qreal left, qreal top, qreal width, qreal height, RGTYPE type = RGTYPE::NORMAL);
    BaseRGItem(QString name, QPointF pos, qreal width, qreal height, RGTYPE type = RGTYPE::NORMAL);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void notifyLinkingItems(bool focus) override;

    void setPosAndSize(QPointF pos, qreal width, qreal height);
    void setCenterX(int x);
    void setCenterY(int y);
    qreal getCenterX() const;
    qreal getCenterY() const;

private:
    QString name;
    qreal penWidth;
    qreal left, top;
    qreal width, height;
    RGTYPE type;
};

#endif // BASERGITEM_H
