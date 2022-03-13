//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
