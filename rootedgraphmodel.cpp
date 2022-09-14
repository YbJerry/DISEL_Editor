//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "rootedgraphmodel.h"

RootedGraphModel::RootedGraphModel()
    :distWidth(30)
    ,distHeight(80)
    ,width(150)
    ,height(60)
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

void RootedGraphModel::createRootedGraph1(DISEL::Graph *gra)
{
    QMap<DISEL::ConceptTag, BaseRGItem *> nodeMap;
    for(auto node:gra->getAllNodes()){
        QString name;
        RGTYPE type = RGTYPE::NORMAL;
        if(node.fromOther()){
            name += node.getOntologyBelong().data();
            name += '.';
        }
        name += node.getName().data();

        if(name.toStdString() == gra->getRoot().getName()){
            type = RGTYPE::ROOT;
        }

        BaseRGItem *item = new BaseRGItem(name, type);
        rgItems.insert(item);
        nodeMap[node]= item;

        if(type == RGTYPE::ROOT){
            rootItem = item;
        }
    }

    for(auto edge:gra->getUnattributableEdges()){
        auto from = nodeMap[edge.getFromTag()];
        auto to = nodeMap[edge.getToTag()];
        reversedEdges[to].insert(from);
        attrs[{from, to}] = "";
    }

    for(const auto & [rela, edgeVec]:gra->getAttributableEdges()){
        for(const auto &edge:edgeVec){
            auto from = nodeMap[edge.getFromTag()];
            auto to = nodeMap[edge.getToTag()];
            reversedEdges[to].insert(from);
            attrs[{from, to}] = QString::fromStdString(rela);
        }
    }

    int maxItemNum = 0;
    QVector<QVector<BaseRGItem *>> hierVec;
    QSet<BaseRGItem *> searchedSet;
    QQueue<BaseRGItem *> queue;
    queue.enqueue(rootItem);
    for(int hier = 0; !queue.isEmpty(); ++hier){
        QQueue<BaseRGItem *> sameHierQueue;
        hierVec.push_back({});
        while(!queue.isEmpty()){
            auto parentItem = queue.dequeue();
            hierVec[hier].push_back(parentItem);
            sameHierQueue.enqueue(parentItem);
        }

        int currentItemNum = 0;
        while(!sameHierQueue.isEmpty()){
            auto to = sameHierQueue.dequeue();
            currentItemNum++;
            for(auto from:qAsConst(reversedEdges[to])){
                searchedSet.insert(to);
                if(!searchedSet.contains(from)){
                    queue.enqueue(from);
                }
            }
        }
        maxItemNum = qMax(maxItemNum, currentItemNum);
    }

    const qreal totalWidth = maxItemNum * (distWidth + width) + distWidth;
    qreal top = distHeight;
    for(int i = 0; i < hierVec.size(); ++i){
        int n = hierVec[i].size();
        qreal realDistWidth = (totalWidth - width*n) / (n + 1);
        qreal left = realDistWidth;

        for(int j = 0; j < hierVec[i].size(); ++j){
            auto &item = hierVec[i][j];
            item->setPosAndSize(QPointF{left, top}, width, height);
            left += realDistWidth + width;
        }
        top += distHeight + height;
    }

    for(auto iter = attrs.begin(); iter != attrs.end(); ++iter){
        auto [from, to] = iter.key();
        auto text = iter.value();

        auto line = new AttrLineItem(
                        from->getCenterX(),
                        from->getCenterY(),
                        to->getCenterX(),
                        to->getCenterY()
                    );
        line->setText(text);

        lineItems.push_back(line);
    }
}

void RootedGraphModel::bindScene(QGraphicsScene *scene)
{
    for(auto item:lineItems){
        scene->addItem(item);
    }

    for(auto item:rgItems){
        scene->addItem(item);
    }
}
