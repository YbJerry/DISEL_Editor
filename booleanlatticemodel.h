//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BOOLEANLATTICEMODEL_H
#define BOOLEANLATTICEMODEL_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QString>
#include <QVector>
#include <QQueue>
#include <QTabWidget>

#include "baseblitem.h"

#include "utils.h"

class BooleanLatticeModel : public QObject
{
    Q_OBJECT

public:
    BooleanLatticeModel();
    ~BooleanLatticeModel();

    void addAtoms(QVector<QString> ats);
    void addAtom(QString at);
    void addConcept(QString con, const QVector<QString> &conLat);
    void createBLItems();
    void createLines();
    void bindScene(QGraphicsScene* scene);
    void clear();

public slots:
    void emitBLItemClicked(QString conceptName);

signals:
    void blItemClicked(QString conceptName);

private:
    QPointF getCenterAlignedPosition(int currRowItemNum, int maxItemNum, int row, int idx);
    std::optional<QString> getNameOfEqualSet(QSet<QString>);

    QSet<QString> atoms;
    QMap<QString, QSet<QString>*> concepts;
    QVector<QVector<QSet<QString>>> latticeNumHash;
    QVector<QVector<BaseBLItem *>> blItems;
    QVector<QGraphicsLineItem *> lineItems;
    QMap<BaseBLItem *, QVector<BaseBLItem *>> notifyBLMap;
    QMap<BaseBLItem *, QVector<QGraphicsLineItem *>> notifyLineMap;
    qreal distWidth, distHeight;
    qreal width, height;
};

#endif // BOOLEANLATTICEMODEL_H
