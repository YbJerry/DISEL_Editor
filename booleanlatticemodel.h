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
#include <QBitArray>

#include "DISOntology.h"

#include "baseblitem.h"

#include "utils.h"

class BooleanLatticeModel : public QObject
{
    Q_OBJECT

public:
    BooleanLatticeModel();
    ~BooleanLatticeModel();

    void addAtoms(QVector<QString> ats);
    const QVector<QString> &getAtoms() const;
    void addAtom(QString at);
    void addRoots(QVector<QString> rts);
    void addRoot(QString rt);
    void addConcept(QString con, const QVector<QString> &conLat);
    void createBLItems();
    void createLines();
    void bindScene(QGraphicsScene* scene);
    void clear();

public slots:
    void emitBLItemClicked(QString conceptName);
    void emitBLItemAddCon(quint64 atoms);
//    void emitBLItemAddGra();

signals:
    void blItemClicked(QString conceptName);
    void blItemAddCon(QVector<QString> atoms);

private:
    QPointF getCenterAlignedPosition(int currRowItemNum, int maxItemNum, int row, int idx);

    inline int countOneNum(quint64 n);
    bool contains(quint64 a, quint64 b);

    QVector<QString> atoms;
    QVector<QString> roots;
    QMap<quint64, QString> concepts;
    QVector<QVector<QSet<QString>>> latticeNumHash;
    QVector<QVector<BaseBLItem *>> blItems;
    QVector<QGraphicsLineItem *> lineItems;
    QMap<BaseBLItem *, QVector<BaseBLItem *>> notifyBLMap;
    QMap<BaseBLItem *, QVector<QGraphicsLineItem *>> notifyLineMap;
    qreal distWidth, distHeight;
    qreal width, height;
};

#endif // BOOLEANLATTICEMODEL_H
