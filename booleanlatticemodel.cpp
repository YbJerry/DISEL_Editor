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

void BooleanLatticeModel::addRoots(QVector<QString> rts)
{
    roots = rts;
}

void BooleanLatticeModel::addRoot(QString rt)
{
    roots.push_back(rt);
}

const QVector<QString> &BooleanLatticeModel::getAtoms() const
{
    return atoms;
}

void BooleanLatticeModel::addAtom(QString at)
{
    atoms.push_back(at);
    latticeNumHash.resize(atoms.size()+1);
}

void BooleanLatticeModel::addConcept(QString con, const QVector<QString> &conLat)
{
    quint64 bits = 0;
    for(auto &cl:conLat){
        auto it = std::find(atoms.begin(), atoms.end(), cl);
        bits |= (1i64 << it-atoms.begin());
    }
    concepts[bits]=con;
}

void BooleanLatticeModel::createBLItems()
{
    for(auto &vec : blItems){
        for(auto pItem : vec){
            delete pItem;
        }
    }
    blItems.clear();

    if(atoms.empty()){
        return;
    }

//    qreal left = 0, top = 0;
    int n = atoms.size();

    blItems.resize(n+1);

    // create bottom lattice
    blItems[0].push_back(new BaseBLItem("⊥", 0, BLTYPE::NORMAL));

    auto blNum = qPow(2, n);
    // When oneNum is 1, the new BaseBLItem need a value to store which atom it belongs to
    int atIdx = 0;

    for(quint64 bits = 1; bits < blNum-1; ++bits){
        int oneNum = countOneNum(bits);
        BaseBLItem* blItem;
        if(oneNum == 1){
            blItem = new BaseBLItem(atoms[atIdx++], bits, BLTYPE::NORMAL);
            blItems[1].push_back(blItem);
        }else if(concepts.count(bits)){
            if(std::find(roots.begin(), roots.end(), concepts[bits]) != roots.end()){
                blItem = new BaseBLItem(concepts[bits], bits,  BLTYPE::ROOT);
            }else{
                blItem = new BaseBLItem(concepts[bits], bits,  BLTYPE::NORMAL);
            }
            blItems[oneNum].push_back(blItem);
        }else{
            blItem = new BaseBLItem("", bits, BLTYPE::ANONY);
            blItems[oneNum].push_back(blItem);
        }
        connect(blItem, &BaseBLItem::clicked, this, &BooleanLatticeModel::emitBLItemClicked);
        connect(blItem, &BaseBLItem::addCon, this, &BooleanLatticeModel::emitBLItemAddCon);
    }

    // create top lattice
    if(concepts.count(blNum-1)){
        auto blItem = new BaseBLItem(concepts[blNum-1], blNum-1, BLTYPE::NORMAL);
        if(std::find(roots.begin(), roots.end(), concepts[blNum-1]) != roots.end()){
            blItem = new BaseBLItem(concepts[blNum-1], blNum-1,  BLTYPE::ROOT);
        }else{
            blItem = new BaseBLItem(concepts[blNum-1], blNum-1,  BLTYPE::NORMAL);
        }
        blItems[n].push_back(blItem);
        connect(blItem, &BaseBLItem::clicked, this, &BooleanLatticeModel::emitBLItemClicked);
        connect(blItem, &BaseBLItem::addCon, this, &BooleanLatticeModel::emitBLItemAddCon);
    }else{
        blItems[n].push_back(new BaseBLItem("ㄒ", blNum-1, BLTYPE::NORMAL));
    }

    int maxItemNum = 1;
    for(int i = 0; i < blItems.size(); ++i){
        maxItemNum = qMax(maxItemNum, blItems[i].size());
    }

    for(int i = 0; i < blItems.size(); ++i){
        for(int j = 0; j < blItems[i].size(); ++j){
            blItems[i][j]->setPosAndSize(getCenterAlignedPosition(blItems[i].size(), maxItemNum, i, j), width, height);
        }
    }
}

void BooleanLatticeModel::createLines()
{
    int n = atoms.size();
    for(int i = n; i > 0; --i){
        for(int j = 0; j < blItems[i].size(); ++j){
            for(int k = 0; k < blItems[i-1].size(); ++k){
                if(blItems[i][j]->contains(*blItems[i-1][k])){
                    auto pos1 = blItems[i][j]->getLowerCenterPos();
                    auto pos2 = blItems[i-1][k]->getUpperCenterPos();
                    blItems[i][j]->addNotifyBLItem(blItems[i-1][k]);
                    notifyBLMap[blItems[i][j]].push_back(blItems[i-1][k]);

                    auto lineItem = new QGraphicsLineItem(pos1.x(), pos1.y(), pos2.x(), pos2.y());
                    lineItems.push_back(lineItem);
                    blItems[i][j]->addNotifyLine(lineItem);
                    notifyLineMap[blItems[i][j]].push_back(lineItem);
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
    concepts.clear();
    latticeNumHash.clear();
    roots.clear();

    for(auto &vec : blItems){
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

void BooleanLatticeModel::emitBLItemAddCon(quint64 atomsBits)
{
    QVector<QString> resAtoms;
    int i = 0;
    while(atomsBits){
        if((atomsBits & 1i64) == 1){
            resAtoms.push_back(atoms[i]);
        }
        atomsBits >>= 1;
        ++i;
    }
    emit blItemAddCon(resAtoms);
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

inline int BooleanLatticeModel::countOneNum(quint64 n)
{
    int res = 0;
    while(n){
        ++res;
        n = n & (n-1);
    }
    return res;
}

bool BooleanLatticeModel::contains(quint64 a, quint64 b)
{
    return a == (a | b);
}
