//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "booleanlatticemodel.h"

BooleanLatticeModel::BooleanLatticeModel()
    : distWidth(20)
    , distHeight(80)
    , width(60)
    , height(30)
{
    latticeNumHash.resize(1);
}

BooleanLatticeModel::~BooleanLatticeModel()
{
    clear();
}

void BooleanLatticeModel::addAtoms(QVector<QString> ats)
{
    for(auto &at:ats){
        addAtom(at);
    }
}

void BooleanLatticeModel::addAtom(QString at)
{
    atoms.insert(at);
    latticeNumHash.resize(atoms.size()+1);
}

void BooleanLatticeModel::addConcept(QString con, const QVector<QString> &conLat)
{
    int idx = conLat.size();
    concepts[con] = new QSet<QString>{};
    for(auto &cl:conLat){
        concepts[con]->insert(cl);
    }
}

void BooleanLatticeModel::createBLItems()
{
    blItems.clear();

    qreal left = 0, top = 0;
    int n = atoms.size();

    blItems.resize(n+1);

    QQueue<int> atomStartIdxs;

    // create bottom lattice
    latticeNumHash[0].push_back(QSet<QString>{});
    int maxItemNum = 1;

    int idx = 1;
    for(auto iter = atoms.begin(); iter != atoms.end(); ++iter){
        latticeNumHash[1].push_back(QSet<QString>{*iter});
        atomStartIdxs.push_back(idx++);
    }
    maxItemNum = qMax(maxItemNum, latticeNumHash[1].size());

    for(int i = 2; i <= n; ++i){
        for(int j = 0; j < latticeNumHash[i-1].size(); ++j){
            int setAppendIdx = atomStartIdxs.first();
            atomStartIdxs.pop_front();
            QSet<QString> st(latticeNumHash[i-1][j]);
            for(int l = setAppendIdx; l < n; ++l){
                latticeNumHash[i].push_back(st.unite(latticeNumHash[1][l]));
                st.subtract(latticeNumHash[1][l]);
                atomStartIdxs.push_back(l+1);
            }
        }
        maxItemNum = qMax(maxItemNum, latticeNumHash[i].size());
    }

    for(int i = n; i >= 0; --i){
        int CNum = latticeNumHash[i].size();
        for(int j = 0; j < CNum; ++j){
//            QString name = "{";
//            for(auto &item:latticeNumHash[i][j]){
//                name += item + ", ";
//            }
//            name += "}";
            QString name;
            BaseBLItem *blItem;
            if(auto con = getNameOfEqualSet(latticeNumHash[i][j]); con){
                name += con.value();
                blItem = new BaseBLItem(name, getCenterAlignedPosition(latticeNumHash[i].size(), maxItemNum, i, j), width, height, BLTYPE::NORMAL);
            }else{
                blItem = new BaseBLItem(name, getCenterAlignedPosition(latticeNumHash[i].size(), maxItemNum, i, j), width, height, BLTYPE::ANONY);
            }
            connect(blItem, &BaseBLItem::clicked, this, &BooleanLatticeModel::emitBLItemClicked);
            blItems[i].push_back(blItem);
            left += distWidth + width;
        }
        left = 0;
        top += distHeight + height;
    }
}

void BooleanLatticeModel::createLines()
{
    int n = atoms.size();
    for(int i = n; i > 0; --i){
        for(int j = 0; j < latticeNumHash[i].size(); ++j){
            for(int k = 0; k < latticeNumHash[i-1].size(); ++k){
                if(latticeNumHash[i][j].contains(latticeNumHash[i-1][k])){
                    auto pos1 = blItems[i][j]->getLowerCenterPos();
                    auto pos2 = blItems[i-1][k]->getUpperCenterPos();
                    lineItems.push_back(new QGraphicsLineItem(pos1.x(), pos1.y(), pos2.x(), pos2.y()));
                }
            }
        }
    }
}

void BooleanLatticeModel::bindScene(QGraphicsScene *scene)
{
    for(auto item:lineItems){
        scene->addItem(item);
    }

    for(auto vec:blItems){
        for(auto item:vec){
            scene->addItem(item);
        }
    }
}

void BooleanLatticeModel::clear()
{
    atoms.clear();
    for(auto iter = concepts.begin(); iter != concepts.end(); ++iter){
        auto* p = iter.value();
        delete p;
    }
    concepts.clear();
    latticeNumHash.clear();

    for(auto vec : blItems){
        for(auto pItem : vec){
            delete pItem;
        }
    }
    blItems.clear();

    for(auto pItem : lineItems){
        delete pItem;
    }
    lineItems.clear();
}

void BooleanLatticeModel::emitBLItemClicked(QString conceptName)
{
    emit blItemClicked(conceptName);
}

QPointF BooleanLatticeModel::getCenterAlignedPosition(int currRowItemNum, int maxItemNum, int row, int idx)
{
    int n = atoms.size();
    qreal totalWidth = maxItemNum * width + (maxItemNum-1) * distWidth;
    qreal needWidth = currRowItemNum * width + (currRowItemNum-1) * distWidth;
    int realRow = n - row;
    qreal y = realRow * (height + distHeight);
    qreal margin = (totalWidth - needWidth) / 2;
    qreal x = margin + idx * (width + distWidth);
    return QPointF{x, y};
}

std::optional<QString> BooleanLatticeModel::getNameOfEqualSet(QSet<QString> targetSet)
{
    if(targetSet.size() == 0){
        return "⊥";
    }
    if(targetSet.size() == 1){
        return *targetSet.begin();
    }
    for(auto iter = concepts.begin(); iter != concepts.end(); ++iter){
        const auto &con = iter.key();
        const auto &set = *iter.value();
        if(set == targetSet){
            return con;
        }
    }
    if(targetSet.size() == atoms.size()){
        return "ㄒ";
    }
    return std::nullopt;
}
