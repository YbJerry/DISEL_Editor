//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ROOTEDGRAPHMODEL_H
#define ROOTEDGRAPHMODEL_H

#include <QMap>
#include <QImage>
#include <QDebug>
#include <QGraphicsScene>
#include <QPair>
#include <QQueue>

#include "gvc.h"

#include "DISOntology.h"
#include "basergitem.h"
#include "attrlineitem.h"


class RootedGraphModel
{
public:
    RootedGraphModel();

    QImage createRootedGraph(DISEL::Graph *gra);
    void createRootedGraph1(DISEL::Graph *gra);
    void bindScene(QGraphicsScene* scene);

private:
    BaseRGItem *rootItem;
    QSet<BaseRGItem *> rgItems;
//    QMap<DISEL::ConceptTag, BaseRGItem *> rgItemsMap;
    QMap<BaseRGItem *, QSet<BaseRGItem *>> reversedEdges;
    QMap<QPair<BaseRGItem *, BaseRGItem *>, QString> attrs;
    QVector<QGraphicsLineItem *> lineItems;
    qreal distWidth, distHeight;
    qreal width, height;
};

#endif // ROOTEDGRAPHMODEL_H
