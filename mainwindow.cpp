#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , conceptModel(new QStandardItemModel())
    , atomDomainModel(new QStandardItemModel())
    , edgeModel(new QStandardItemModel())
    , relationModel(new QStandardItemModel())
    , onto(new DISEL::Ontology())
    , blScene(new QGraphicsScene())
    , rgScene(new QGraphicsScene())
    , blModel(new BooleanLatticeModel())
    , graph(nullptr)
    , isChanged(false)
{
    ui->setupUi(this);

    connect(blModel, &BooleanLatticeModel::blItemClicked, this, &MainWindow::createRootedGraph);

    conceptModel->setHorizontalHeaderLabels(QStringList({tr("Name"), tr("Description")}));
    atomDomainModel->setHorizontalHeaderLabels(QStringList({tr("Name"), tr("Description")}));
    edgeModel->setHorizontalHeaderLabels(QStringList({tr("Relation"), tr("Index"), tr("Ontology(from)"), tr("Concept(from)"), tr("Ontology(to)"), tr("Concept(to)")}));
    relationModel->setHorizontalHeaderLabels(QStringList({tr("Relation"), tr("Property")}));

    ui->atomDomainView->verticalHeader()->hide();
    ui->conceptView->verticalHeader()->hide();
    ui->edgeView->verticalHeader()->hide();
    ui->relationView->verticalHeader()->hide();

    ui->atomDomainView->setModel(atomDomainModel);
    ui->conceptView->setModel(conceptModel);
    ui->edgeView->setModel(edgeModel);
    ui->relationView->setModel(relationModel);

    ui->booleanLatticeView->setScene(blScene);
    ui->rootedGraphView->setScene(rgScene);

    //testInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendConceptModel(DISEL::Ontology *onto)
{
    for(auto con:onto->getAllConcepts()){
        QStandardItem *nameItem = new QStandardItem(QString(con->getName().data()));
        nameItem->setCheckable(true);
        QStandardItem *desItem = new QStandardItem(QString(con->getDescription().data()));
        conceptModel->appendRow(QList<QStandardItem*>{nameItem, desItem});
    }
}

void MainWindow::addRowToConceptModel(const DISEL::Concept &con)
{
    QStandardItem *nameItem = new QStandardItem(QString(con.getName().data()));
    nameItem->setCheckable(true);
    QStandardItem *desItem = new QStandardItem(QString(con.getDescription().data()));
    conceptModel->appendRow(QList<QStandardItem*>{nameItem, desItem});
}

void MainWindow::bindConceptModel()
{
    ui->conceptView->setModel(conceptModel);
    connect(ui->conceptView, &QTableView::clicked, this, &MainWindow::setDefination);
}

void MainWindow::updateConceptModel()
{
    clearModel(conceptModel);
    appendConceptModel(onto);
    bindConceptModel();
}

void MainWindow::createAtomDomainModel(std::vector<DISEL::Atom*> ad)
{
    for(auto a:ad){
        QStandardItem *nameItem = new QStandardItem(QString(a->getName().data()));
        nameItem->setCheckable(true);
        QStandardItem *desItem = new QStandardItem(QString(a->getDescription().data()));
        atomDomainModel->appendRow(QList<QStandardItem*>{nameItem, desItem});
    }
}

void MainWindow::addRowToAtomDomainModel(const DISEL::Atom &at)
{
    QStandardItem *nameItem = new QStandardItem(QString(at.getName().data()));
    nameItem->setCheckable(true);
    QStandardItem *descItem = new QStandardItem(QString(at.getDescription().data()));
    atomDomainModel->appendRow(QList<QStandardItem*>{nameItem, descItem});
}

void MainWindow::bindAtomDomainModel()
{
    ui->atomDomainView->setModel(atomDomainModel);
}

void MainWindow::updateAtomDomainModel()
{
    clearAtomDomainModel();
    createAtomDomainModel(onto->getAtomDomain());
    bindAtomDomainModel();
}

void MainWindow::clearAtomDomainModel()
{
    clearModel(atomDomainModel);
}

void MainWindow::releaseOntology()
{
    delete onto;
}

void MainWindow::createEdgeModel(DISEL::Graph *gra)
{
    size_t idx = 0;
    for(auto edge:gra->getUnattributableEdges()){
        addRowToEdgeModel(&edge, idx++);
    }
    for(auto [rt, vec]:gra->getAttributableEdges()){
        idx = 0;
        for(auto edge:vec){
            addRowToEdgeModel(rt, &edge, idx++);
        }
    }
}

void MainWindow::addRowToEdgeModel(DISEL::Edge *edge, size_t index)
{
    QStandardItem *relaItem = new QStandardItem(QString());
    relaItem->setCheckable(true);
    QStandardItem *idxItem = new QStandardItem(QString::number(index));
    QStandardItem *fromOntoItem = new QStandardItem(QString(edge->getFromTag().getOntologyBelong().data()));
    QStandardItem *fromNameItem = new QStandardItem(QString(edge->getFromTag().getName().data()));
    QStandardItem *toOntoItem = new QStandardItem(QString(edge->getToTag().getOntologyBelong().data()));
    QStandardItem *toNameItem = new QStandardItem(QString(edge->getToTag().getName().data()));
    edgeModel->appendRow(QList<QStandardItem*>{relaItem, idxItem, fromOntoItem, fromNameItem, toOntoItem, toNameItem});
}

void MainWindow::addRowToEdgeModel(DISEL::RelationTag rt, DISEL::Edge *edge, size_t index)
{
    QStandardItem *relaItem = new QStandardItem(QString::fromStdString(rt));
    relaItem->setCheckable(true);
    QStandardItem *idxItem = new QStandardItem(QString::number(index));
    QStandardItem *fromOntoItem = new QStandardItem(QString(edge->getFromTag().getOntologyBelong().data()));
    QStandardItem *fromNameItem = new QStandardItem(QString(edge->getFromTag().getName().data()));
    QStandardItem *toOntoItem = new QStandardItem(QString(edge->getToTag().getOntologyBelong().data()));
    QStandardItem *toNameItem = new QStandardItem(QString(edge->getToTag().getName().data()));
    edgeModel->appendRow(QList<QStandardItem*>{relaItem, idxItem, fromOntoItem, fromNameItem, toOntoItem, toNameItem});
}

void MainWindow::bindEdgeModel()
{
    ui->edgeView->setModel(edgeModel);
}

void MainWindow::clearEdgeModel()
{
    edgeModel->removeRows(0, edgeModel->rowCount());
}

void MainWindow::createRelationModel(std::vector<DISEL::Relation> relas)
{
    for(auto rela:relas){
        QStandardItem *nameItem = new QStandardItem(QString(rela.getName().data()));
        nameItem->setCheckable(true);
        QString props;
        for(auto p:rela.getAllProperties()){
            props += QString::fromStdString(rptos(p)) + " ";
        }
        QStandardItem *propItem = new QStandardItem(props);
        relationModel->appendRow(QList<QStandardItem*>{nameItem, propItem});
    }
}

void MainWindow::addRowToRelationModel(DISEL::Relation rela)
{
    QStandardItem *nameItem = new QStandardItem(QString::fromStdString(rela.getName()));
    QString props;
    for(auto p:rela.getAllProperties()){
        props += QString::fromStdString(rptos(p)) + " ";
    }
    QStandardItem *propItem = new QStandardItem(props);
    relationModel->appendRow(QList<QStandardItem*>{nameItem, propItem});
}

void MainWindow::bindRelationModel()
{
    ui->relationView->setModel(relationModel);
}

void MainWindow::testInit()
{
    DISXMLReader reader(R"(C:\Users\YbJerry\Desktop\dis.xml)");
    onto = reader.read();

    appendConceptModel(onto);
    bindConceptModel();
    createAtomDomainModel(onto->getAtomDomain());
    bindAtomDomainModel();

    for(auto at:onto->getAtomDomain()){
        blModel->addAtom(at->getName().data());
    }

    for(auto con:onto->getAllConcepts()){
        auto lattices = con->getLatticeOfConcepts();
        QVector<QString> transLattices;
        for(auto l:lattices){
            transLattices.push_back(l.data());
        }
        blModel->addConcept(con->getName().data(), transLattices);
    }

    blModel->createBLItems();
    blModel->createLines();
    blModel->bindScene(blScene);

    ui->booleanLatticeView->setScene(blScene);
    ui->booleanLatticeView->show();

//    RootedGraphModel g;
//    auto rg = g.createRootedGraph(onto->getAllGraphs().front());
//    rgScene->addPixmap(QPixmap::fromImage(rg));
}

void MainWindow::setDefination(const QModelIndex &idx)
{
    auto conName = idx.siblingAtColumn(0).data().toString().toStdString();
    if(auto con = onto->getConcept(DISEL::ConceptTag(conName))){
        QString str = QString::fromStdString(con.value()->getDefination());
        ui->definationTextEdit->setText(str);
    }else{
        ui->definationTextEdit->clear();
    }
}

void MainWindow::createRootedGraph(QString name)
{
    auto graphs = onto->getAllGraphs();
    auto func = [&name](DISEL::Graph *gra){return gra->getRoot() == name.toStdString();};
    if(auto iter = std::find_if(graphs.begin(), graphs.end(), func); iter != graphs.end()){
        graph = *iter;
        RootedGraphModel g;
        auto rg = g.createRootedGraph(graph);
        rgScene->clear();
        rgScene->addPixmap(QPixmap::fromImage(rg));

        createEdgeModel(*iter);
        bindEdgeModel();
        createRelationModel(graph->getRelations());
        bindRelationModel();

        ui->rightTabWidget->setCurrentIndex(1);
    }else{
        QMessageBox msgBox;
        msgBox.setText("Create Rooted Graph for " + name + "?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:{
                ui->rootedGraphView->scene()->clear();

                auto newGra = new DISEL::Graph(name.toStdString());
                newGra->setRoot(name.toStdString());
                onto->addGraph(newGra);
                clearEdgeModel();
                createEdgeModel(newGra);
                bindEdgeModel();

                ui->rightTabWidget->setCurrentIndex(1);
            }
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_addAtomDomainButton_clicked()
{
    NewAtomDialog dialog(onto, false, this);
    if(dialog.exec() == QDialog::Accepted){
        DISEL::Atom at(dialog.getAtomStr().toStdString(), dialog.getDescStr().toStdString());
        onto->addAtom(at);
        addRowToAtomDomainModel(at);
        bindAtomDomainModel();
        createBooleanLattice();
    }
}


void MainWindow::on_addConceptButton_clicked()
{
    NewConceptDialog dialog(onto, this);
    if(dialog.exec() == QDialog::Accepted){
        DISEL::Concept *con = new DISEL::Concept(dialog.getNameStr().toStdString());
        for(auto at:dialog.getAtoms()){
            con->addAtomTypeLattice(at.toStdString());
        }
        con->setDescription(dialog.getDescStr().toStdString());
        onto->addConcept(con);
        addRowToConceptModel(*con);
        bindConceptModel();
        createBooleanLattice();
    }
}


void MainWindow::on_delAtomDomainButton_clicked()
{
    for(int i = 0; i < atomDomainModel->rowCount();){
        auto it = atomDomainModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto name = it->data().toString().toStdString();
            atomDomainModel->removeRow(i);
            onto->delAtom(name);
        }else{
            ++i;
        }
    }
    bindAtomDomainModel();
    createBooleanLattice();
}


void MainWindow::on_delConceptButton_clicked()
{
    for(int i = 0; i < conceptModel->rowCount();){
        auto it = conceptModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto name = it->data().toString().toStdString();
            conceptModel->removeRow(i);
            onto->delConcept(name);
        }else{
            ++i;
        }
    }
    bindAtomDomainModel();
    createBooleanLattice();
}



void MainWindow::on_addEdgeButton_clicked()
{
    NewEdgeDialog dialog(graph, this);
    if(dialog.exec() == QDialog::Accepted){
        DISEL::ConceptTag from(dialog.getFromConceptTag().toStdString());
        DISEL::ConceptTag to(dialog.getFromConceptTag().toStdString());

        if(auto onto = dialog.getFromOntologyTag(); onto){
            from.setOntologyBelong(onto->toStdString());
        }

        if(auto onto = dialog.getToOntologyTag(); onto){
            to.setOntologyBelong(onto->toStdString());
        }

        DISEL::Edge *edge = new DISEL::Edge(from, to);

        if(auto rela = dialog.getRelationTag(); rela){
            DISEL::RelationTag rt(rela->toStdString());
            graph->addEdge(from, to, rt);
        }else{
            graph->addEdge(from, to);
        }

        clearEdgeModel();
        createEdgeModel(graph);
        bindEdgeModel();
    }
}

void MainWindow::on_delEdgeButton_clicked()
{
    for(int i = 0; i < edgeModel->rowCount();){
        auto it = edgeModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto row =  edgeModel->takeRow(i);

            auto rela = row[0]->data().toString().toStdString();
            auto idx = row[1]->data().toInt();

            if(rela.empty()){
                graph->delUnattributableEdge(idx);
            }else{
                graph->delAttributableEdge(rela, idx);
            }
        }else{
            ++i;
        }
    }

    clearEdgeModel();
    createEdgeModel(graph);
    bindEdgeModel();
}

void MainWindow::on_addRelationButton_clicked()
{
    NewRelationDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted){
        DISEL::Relation *rela = new DISEL::Relation(dialog.getName().toStdString());
        for(auto prop:dialog.getProperties()){
            rela->addProperty(prop);
        }
        graph->addRelation(*rela);
        addRowToRelationModel(*rela);
        bindRelationModel();
    }
}

void MainWindow::on_delRelationButton_clicked()
{
    for(int i = 0; i < relationModel->rowCount();){
        auto it = relationModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto name = it->data().toString().toStdString();
            relationModel->removeRow(i);
            graph->delRelation(name);
        }else{
            ++i;
        }
    }
    bindRelationModel();
}

// new file
void MainWindow::on_actionNew_triggered()
{
    delete onto;
    DISEL::Ontology *onto = new DISEL::Ontology();

    appendConceptModel(onto);
    bindConceptModel();
    createAtomDomainModel(onto->getAtomDomain());
    bindAtomDomainModel();
}


void MainWindow::on_actionExit_triggered()
{
    if(isChanged){
        return;
    }
    this->close();
}


void MainWindow::on_actionSave_triggered()
{
    DISXMLWriter writer;
    if(fileName.isEmpty()){
        fileName = QFileDialog::getSaveFileName(this, tr("Save DIS File"), "./source.xml", tr("DIS Files (*.xml)"));

        if(fileName.isEmpty())
            return;
    }
    writer.write(fileName.toStdString(), *onto);
}

void MainWindow::on_actionSave_as_triggered()
{
    DISXMLWriter writer;
    fileName = QFileDialog::getSaveFileName(this, tr("Save DIS File"), "./source.xml", tr("DIS Files (*.xml)"));

    if(fileName.isEmpty())
        return;
    writer.write(fileName.toStdString(), *onto);
}

void MainWindow::updateRootedGraph()
{
    RootedGraphModel g;
    auto rg = g.createRootedGraph(graph);
    rgScene->clear();
    rgScene->addPixmap(QPixmap::fromImage(rg));
}

void MainWindow::clearModel(QStandardItemModel *model)
{
    model->removeRows(0, model->rowCount());
}

void MainWindow::on_actionLoad_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open DIS File"), ".", tr("DIS Files (*.xml)"));
    qDebug() << fileName;

    if(fileName.isEmpty())
        return;

    delete onto;

    DISXMLReader reader(fileName.toStdString());
    onto = reader.read();

    appendConceptModel(onto);
    bindConceptModel();
    createAtomDomainModel(onto->getAtomDomain());
    bindAtomDomainModel();

    for(auto at:onto->getAtomDomain()){
        blModel->addAtom(at->getName().data());
    }

    for(auto con:onto->getAllConcepts()){
        auto lattices = con->getLatticeOfConcepts();
        QVector<QString> transLattices;
        for(auto l:lattices){
            transLattices.push_back(l.data());
        }
        blModel->addConcept(con->getName().data(), transLattices);
    }

    blModel->createBLItems();
    blModel->createLines();
    blModel->bindScene(blScene);

    ui->booleanLatticeView->setScene(blScene);
    ui->booleanLatticeView->show();
}

void MainWindow::on_atomDomainView_doubleClicked(const QModelIndex &index)
{
    auto name = atomDomainModel->item(index.row())->data(Qt::DisplayRole).toString().toStdString();
    if(auto opAt = onto->getAtom(name); opAt){
        auto *at = opAt.value();
        NewAtomDialog dlg(onto, true, this);
        dlg.setContent(*at);
        if(dlg.exec() == QDialog::Accepted){
            at->setDescription(dlg.getDescStr().toStdString());
        }
        updateAtomDomainModel();
    }
}


void MainWindow::on_conceptView_doubleClicked(const QModelIndex &index)
{
    auto name = conceptModel->item(index.row())->data(Qt::DisplayRole).toString().toStdString();
    if(auto op = onto->getConcept(name); op){
        auto *con = op.value();
        NewConceptDialog dlg(onto, true, this);
        dlg.setContent(*con);
        if(dlg.exec() == QDialog::Accepted){
            con->setDescription(dlg.getDescStr().toStdString());

            con->clearLatticeOfConcepts();
            for(auto at:dlg.getAtoms()){
                con->addAtomTypeLattice(at.toStdString());
            }
        }
        updateConceptModel();
        createBooleanLattice();
    }
}

void MainWindow::createBooleanLattice()
{
    blModel->clear();
    for(auto at:onto->getAtomDomain()){
        blModel->addAtom(at->getName().data());
    }

    for(auto con:onto->getAllConcepts()){
        auto lattices = con->getLatticeOfConcepts();
        QVector<QString> transLattices;
        for(auto l:lattices){
            transLattices.push_back(l.data());
        }
        blModel->addConcept(con->getName().data(), transLattices);
    }

    blModel->createBLItems();
    blModel->createLines();
    blModel->bindScene(blScene);
}

