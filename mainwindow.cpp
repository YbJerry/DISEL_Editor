//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.hpp"

#include "benchmarking.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , conceptModel(new QStandardItemModel())
    , atomDomainModel(new QStandardItemModel())
    , edgeModel(new QStandardItemModel())
    , relationModel(new QStandardItemModel())
    , includedOntoModel(new QStandardItemModel())
    , onto(new DISEL::Ontology())
    , blScene(new QGraphicsScene())
    , rgScene(new QGraphicsScene())
    , blModel(new BooleanLatticeModel())
    , graph(nullptr)
    , isChanged(false)
{
    ui->setupUi(this);

    connect(blModel, &BooleanLatticeModel::blItemClicked, this, &MainWindow::createRootedGraph);
    connect(blModel, &BooleanLatticeModel::blItemAddCon, this, &MainWindow::addConceptFromLattice);

    conceptModel->setHorizontalHeaderLabels(QStringList({tr("Name"), tr("Description")}));
    atomDomainModel->setHorizontalHeaderLabels(QStringList({tr("Name"), tr("Description")}));
    edgeModel->setHorizontalHeaderLabels(QStringList({tr("Relation"), tr("Index"), tr("Ontology(from)"), tr("Concept(from)"), tr("Ontology(to)"), tr("Concept(to)")}));
    relationModel->setHorizontalHeaderLabels(QStringList({tr("Relation"), tr("Property")}));
    includedOntoModel->setHorizontalHeaderLabels(QStringList({tr("Name"), tr("Path")}));

    ui->atomDomainView->verticalHeader()->hide();
    ui->conceptView->verticalHeader()->hide();
    ui->edgeView->verticalHeader()->hide();
    ui->relationView->verticalHeader()->hide();
    ui->includedOntoView->verticalHeader()->hide();

    ui->atomDomainView->setModel(atomDomainModel);
    ui->conceptView->setModel(conceptModel);
    ui->edgeView->setModel(edgeModel);
    ui->relationView->setModel(relationModel);
    ui->includedOntoView->setModel(includedOntoModel);

    ui->booleanLatticeView->setScene(blScene);
    ui->rootedGraphView->setScene(rgScene);

    testInit();
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

void MainWindow::clearConceptModel()
{
    clearModel(conceptModel);
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

void MainWindow::createRelationModel(std::vector<DISEL::Relation*> relas)
{
    for(auto rela:relas){
        QStandardItem *nameItem = new QStandardItem(QString(rela->getName().data()));
        nameItem->setCheckable(true);
        QString props;
        for(auto p:rela->getAllProperties()){
            props += QString::fromStdString(rptos(p)) + " ";
        }
        QStandardItem *propItem = new QStandardItem(props);
        relationModel->appendRow(QList<QStandardItem*>{nameItem, propItem});
    }
}

void MainWindow::addRowToRelationModel(DISEL::Relation* rela)
{
    QStandardItem *nameItem = new QStandardItem(QString::fromStdString(rela->getName()));
    nameItem->setCheckable(true);
    QString props;
    for(auto p:rela->getAllProperties()){
        props += QString::fromStdString(rptos(p)) + " ";
    }
    QStandardItem *propItem = new QStandardItem(props);
    relationModel->appendRow(QList<QStandardItem*>{nameItem, propItem});
}

void MainWindow::bindRelationModel()
{
    ui->relationView->setModel(relationModel);
}

void MainWindow::clearRelationModel()
{
    relationModel->removeRows(0, relationModel->rowCount());
}

void MainWindow::createIncludedOntoModel(std::vector<std::pair<DISEL::OntologyTag, std::filesystem::path> > includedOntos)
{
    for(auto [ot, path]:includedOntos){
        addRowToIncludedOntoModel(ot, path);
    }
}

void MainWindow::addRowToIncludedOntoModel(DISEL::OntologyTag ot, std::filesystem::path path)
{
    QStandardItem *nameItem = new QStandardItem(QString(ot.data()));
    nameItem->setCheckable(true);
    QStandardItem *pathItem = new QStandardItem(QString(path.string().data()));
    includedOntoModel->appendRow(QList<QStandardItem*>{nameItem, pathItem});
}

void MainWindow::bindIncludedOntoModel()
{
    ui->includedOntoView->setModel(includedOntoModel);
}

void MainWindow::updateIncludedOntoModel()
{
    clearIncludedOntoModel();
    createIncludedOntoModel(onto->getAllIncludeOntology());
    bindIncludedOntoModel();
}

void MainWindow::clearIncludedOntoModel()
{
    includedOntoModel->removeRows(0, includedOntoModel->rowCount());
}

void MainWindow::testInit()
{
    DISXMLReader reader(R"(D:/DISEL_file/dis.xml)");
    onto = reader.read();

    clearConceptModel();
    appendConceptModel(onto);
    bindConceptModel();

    clearAtomDomainModel();
    createAtomDomainModel(onto->getAtomDomain());
    bindAtomDomainModel();

    clearIncludedOntoModel();
    createIncludedOntoModel(onto->getAllIncludeOntology());
    bindIncludedOntoModel();

    blModel->clear();

    for(auto at:onto->getAtomDomain()){
        blModel->addAtom(at->getName().data());
    }

    for(auto rt:onto->getAllRoots()){
        blModel->addRoot(rt.getName().data());
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
    blScene->clear();
    blModel->bindScene(blScene);

    ui->booleanLatticeView->setScene(blScene);
    ui->booleanLatticeView->show();

    rgScene->clear();
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
//        auto rg = g.createRootedGraph(graph);
//        rgScene->clear();
//        rgScene->addPixmap(QPixmap::fromImage(rg));
        g.createRootedGraph1(graph);
        rgScene->clear();
        g.bindScene(rgScene);

        clearEdgeModel();
        createEdgeModel(*iter);
        bindEdgeModel();
        clearRelationModel();
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
                graph = newGra;
                newGra->setRoot(name.toStdString());
                onto->addGraph(newGra);
                clearEdgeModel();
                createEdgeModel(newGra);
                bindEdgeModel();
                clearRelationModel();

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

void MainWindow::addConceptFromLattice(QVector<QString> atoms)
{
    NewConceptDialog dialog(onto, false, this);
    dialog.setCheckedAtoms(atoms);
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
    NewConceptDialog dialog(onto, false, this);
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
    NewEdgeDialog dialog(onto, graph, currentDirectory, this);
    if(dialog.exec() == QDialog::Accepted){
        DISEL::ConceptTag from(dialog.getFromConceptTag().toStdString());
        DISEL::ConceptTag to(dialog.getToConceptTag().toStdString());

        if(auto onto = dialog.getFromOntologyTag(); onto){
            from.setOntologyBelong(onto->toStdString());
        }

        if(auto onto = dialog.getToOntologyTag(); onto){
            to.setOntologyBelong(onto->toStdString());
        }

        if(auto rela = dialog.getRelationTag(); rela){
            DISEL::RelationTag rt(rela->toStdString());
            graph->addEdge(from, to, rt);
        }else{
            graph->addEdge(from, to);
        }

        clearEdgeModel();
        createEdgeModel(graph);
        bindEdgeModel();
        updateRootedGraph();
    }
}

void MainWindow::on_delEdgeButton_clicked()
{
    for(int i = 0; i < edgeModel->rowCount();){
        auto it = edgeModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto row =  edgeModel->takeRow(i);

            auto rela = row[0]->data(Qt::DisplayRole).toString().toStdString();
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
    updateRootedGraph();
}

void MainWindow::on_addRelationButton_clicked()
{
    NewRelationDialog dialog(graph, false, this);
    if(dialog.exec() == QDialog::Accepted){
        DISEL::Relation *rela = new DISEL::Relation(dialog.getName().toStdString());
        for(auto prop:dialog.getProperties()){
            rela->addProperty(prop);
        }
        graph->addRelation(*rela);
        addRowToRelationModel(rela);
        bindRelationModel();
        updateRootedGraph();
    }
}

void MainWindow::on_delRelationButton_clicked()
{
    for(int i = 0; i < relationModel->rowCount();){
        auto it = relationModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto name = it->data(Qt::DisplayRole).toString().toStdString();
            relationModel->removeRow(i);
            graph->delRelation(name);
        }else{
            ++i;
        }
    }
    bindRelationModel();
    clearEdgeModel();
    createEdgeModel(graph);
    bindEdgeModel();
    updateRootedGraph();
}

// new file
void MainWindow::on_actionNew_triggered()
{
    delete onto;
    onto = new DISEL::Ontology();
    graph = nullptr;
    isChanged = false;
    fileName.clear();

    clearConceptModel();
    clearAtomDomainModel();
    clearRelationModel();
    clearEdgeModel();
    blScene->clear();
    rgScene->clear();
    blModel->clear();
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
    if(onto->getName().empty()){
        bool ok;
        auto ontoName = QInputDialog::getText(this, tr("Set the name of saved ontology"), tr("Ontology name: "), QLineEdit::Normal, "", &ok);
        if(ok && !ontoName.isEmpty()){
            onto->setName(ontoName.toStdString());
        }else{
            QMessageBox msgBox;
            msgBox.setText(tr("Please input the name of ontology"));
            msgBox.exec();
            return;
        }
    }
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

    if(onto->getName().empty()){
        bool ok;
        auto ontoName = QInputDialog::getText(this, tr("Set the name of saved ontology"), tr("Ontology name: "), QLineEdit::Normal, "", &ok);
        if(ok && !ontoName.isEmpty()){
            onto->setName(ontoName.toStdString());
        }else{
            QMessageBox msgBox;
            msgBox.setText(tr("Please input the name of ontology"));
            msgBox.exec();
            return;
        }
    }

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
    currentDirectory.setPath(fileName);
    currentDirectory.cdUp();
    qDebug() << currentDirectory.path();
    qDebug() << fileName;

    if(fileName.isEmpty())
        return;

    delete onto;
    graph = nullptr;
    isChanged = false;

    clearRelationModel();
    clearEdgeModel();

    DISXMLReader reader(fileName.toStdString());
    onto = reader.read();

    clearConceptModel();
    appendConceptModel(onto);
    bindConceptModel();

    clearAtomDomainModel();
    createAtomDomainModel(onto->getAtomDomain());
    bindAtomDomainModel();

    clearIncludedOntoModel();
    createIncludedOntoModel(onto->getAllIncludeOntology());
    bindIncludedOntoModel();

    blModel->clear();

    for(auto at:onto->getAtomDomain()){
        blModel->addAtom(at->getName().data());
    }

    for(auto rt:onto->getAllRoots()){
        blModel->addRoot(rt.getName().data());
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
    blScene->clear();
    blModel->bindScene(blScene);

    ui->booleanLatticeView->setScene(blScene);
    ui->booleanLatticeView->show();

    rgScene->clear();
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

    for(auto rt:onto->getAllRoots()){
        blModel->addRoot(rt.getName().data());
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

void MainWindow::on_actionDocument_triggered()
{
    QDesktopServices::openUrl(QUrl("https://ybjerry.github.io/DISEL_Editor_helpdoc/"));
}


void MainWindow::on_relationView_doubleClicked(const QModelIndex &index)
{
    auto name = relationModel->item(index.row())->data(Qt::DisplayRole).toString().toStdString();
    if(auto op = graph->getRelation(name); op){
        auto *rela = op.value();
        NewRelationDialog dlg(graph, true, this);
        dlg.setContent(*rela);
        if(dlg.exec() == QDialog::Accepted){
            rela->clearProperties();
            for(auto prop:dlg.getProperties()){
                rela->addProperty(prop);
            }
        }
        clearRelationModel();
        createRelationModel(graph->getRelations());
        updateRootedGraph();
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QDesktopServices::openUrl(QUrl("https://ybjerry.github.io/DISEL_Editor_helpdoc/about/"));
}


void MainWindow::on_addOntoButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Ontology"), ".", tr("DIS Files (*.xml)"));
    QString ontoName;
    try {
        auto reader = DISXMLReader(fileName.toStdString());
        auto includedOnto = reader.read();
        ontoName = QString::fromStdString(includedOnto->getName());
    }  catch (const std::exception& e) {
        QMessageBox msgBox;
        msgBox.setText("The file is not a valid DIS file!");
        msgBox.exec();
        return;
    }
    fileName = currentDirectory.relativeFilePath(fileName);
    onto->addIncludeOntology(ontoName.toStdString(), std::filesystem::path(fileName.toStdString()));
    addRowToIncludedOntoModel(ontoName.toStdString(), std::filesystem::path(fileName.toStdString()));
    bindIncludedOntoModel();
}


void MainWindow::on_delOntoButton_clicked()
{
    for(int i = 0; i < includedOntoModel->rowCount();){
        auto it = includedOntoModel->item(i, 0);
        if(it->checkState() == Qt::Checked){
            auto name = it->data(Qt::DisplayRole).toString().toStdString();
            includedOntoModel->removeRow(i);
            onto->delIncludeOntology(name.c_str());
        }else{
            ++i;
        }
    }
    clearIncludedOntoModel();
    createIncludedOntoModel(onto->getAllIncludeOntology());
    bindIncludedOntoModel();
}
