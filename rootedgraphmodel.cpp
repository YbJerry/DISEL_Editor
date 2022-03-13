//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "rootedgraphmodel.h"

RootedGraphModel::RootedGraphModel()
{

}

QImage RootedGraphModel::createRootedGraph(DISEL::Graph *gra)
{
    Agraph_t *g = agopen((char*)"rooted graph", Agdirected, NULL);
    GVC_t *gvc = gvContext();
    char *imageData = (char *)malloc(5000 * sizeof(char));
    unsigned int length;

    QMap<DISEL::ConceptTag, Agnode_s*> nodeMap;

    for(auto node:gra->getAllNodes()){
        QString name;
        if(node.fromOther()){
            name += node.getOntologyBelong().data();
            name += '.';
        }
        name += node.getName().data();
        auto n = agnode(g, name.toStdString().data(), 1);

        if(name.toStdString() == gra->getRoot().getName()){
            agsafeset(n, (char*)"shape", (char*)"doubleoctagon", (char*)"ellipse");
        }

        nodeMap.insert(node, n);
    }

    for(auto edge:gra->getUnattributableEdges()){
        auto from = nodeMap[edge.getFromTag()];
        auto to = nodeMap[edge.getToTag()];
        auto e = agedge(g, from, to, 0, 1);
    }

    for(const auto & [rela, edgeVec]:gra->getAttributableEdges()){
        for(const auto &edge:edgeVec){
            auto from = nodeMap[edge.getFromTag()];
            auto to = nodeMap[edge.getToTag()];
            auto e = agedge(g, from, to, 0, 1);
//            agset(e, name.data(), rela.data());
            agsafeset(e, (char*)"label", rela.data(), (char*)"");
            qDebug() << (char *)rela.data();
        }
    }

    //agsafeset(n, (char*)"color", "red", "");
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", "test.png");

    gvRenderData(gvc, g, "png", &imageData, &length);
    gvFreeLayout(gvc, g);
    agclose(g);

    QImage res;
    QByteArray arr(imageData, length);
    res.loadFromData(arr);

    return res;
}
