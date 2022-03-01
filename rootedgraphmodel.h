#ifndef ROOTEDGRAPHMODEL_H
#define ROOTEDGRAPHMODEL_H

#include <QMap>
#include <QImage>
#include <QDebug>

#include "gvc.h"

#include "DISOntology.h"


class RootedGraphModel
{
public:
    RootedGraphModel();

    QImage createRootedGraph(DISEL::Graph *gra);

private:

};

#endif // ROOTEDGRAPHMODEL_H
