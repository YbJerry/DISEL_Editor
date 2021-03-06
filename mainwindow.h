//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// qt libraries
#include <QMainWindow>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>
#include <QVector>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QTabWidget>
#include <QDesktopServices>
#include <QUrl>

// DISEL API libraries
#include "DISXMLWriter.h"
#include "DISXMLReader.h"
#include "DISOntology.h"

// dialog headers
#include "newatomdialog.h"
#include "newconceptdialog.h"
#include "newedgedialog.h"
#include "newrelationdialog.h"
// others
#include "booleanlatticemodel.h"
#include "rootedgraphmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

//    friend class BaseBLItem;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setDefination(const QModelIndex &idx);
    void createRootedGraph(QString name);

    void on_addAtomDomainButton_clicked();
    void on_delAtomDomainButton_clicked();

    void on_addConceptButton_clicked();
    void on_delConceptButton_clicked();

    void on_addEdgeButton_clicked();
    void on_delEdgeButton_clicked();

    void on_addRelationButton_clicked();
    void on_delRelationButton_clicked();

    void on_actionNew_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_as_triggered();
    void on_actionDocument_triggered();

    void on_atomDomainView_doubleClicked(const QModelIndex &index);
    void on_conceptView_doubleClicked(const QModelIndex &index);
    void on_relationView_doubleClicked(const QModelIndex &index);
    void on_actionAbout_triggered();

    void on_addOntoButton_clicked();
    void on_delOntoButton_clicked();

private:
    void createBooleanLattice();


    void updateRootedGraph();

    void clearModel(QStandardItemModel* model);

    void appendConceptModel(DISEL::Ontology* onto);
    void addRowToConceptModel(const DISEL::Concept& con);
    void bindConceptModel();
    void updateConceptModel();
    void clearConceptModel();


    void createAtomDomainModel(std::vector<DISEL::Atom*> ad);
    void addRowToAtomDomainModel(const DISEL::Atom& at);
    void bindAtomDomainModel();
    void updateAtomDomainModel();
    void clearAtomDomainModel();
    void releaseOntology();

    void createEdgeModel(DISEL::Graph *gra);
    void addRowToEdgeModel(DISEL::RelationTag rt, DISEL::Edge *edge, size_t index);
    void addRowToEdgeModel(DISEL::Edge *edge, size_t index);
    void bindEdgeModel();
    void clearEdgeModel();

    void createRelationModel(std::vector<DISEL::Relation*> relas);
    void addRowToRelationModel(DISEL::Relation* rela);
    void bindRelationModel();
    void clearRelationModel();

    void createIncludedOntoModel(std::vector<std::pair<DISEL::OntologyTag, std::filesystem::path>> includedOntos);
    void addRowToIncludedOntoModel(DISEL::OntologyTag ot, std::filesystem::path path);
    void bindIncludedOntoModel();
    void updateIncludedOntoModel();
    void clearIncludedOntoModel();

    // test init
    void testInit();

    Ui::MainWindow *ui;
    DISEL::Ontology *onto;
    DISEL::Graph *graph;
    QStandardItemModel *conceptModel;
    QStandardItemModel *specModel;
    QStandardItemModel *atomDomainModel;
    QStandardItemModel *edgeModel;
    QStandardItemModel *relationModel;
    QStandardItemModel *includedOntoModel;
    QGraphicsScene *blScene;
    QGraphicsScene *rgScene;
    QString fileName;
    BooleanLatticeModel *blModel;

    QDir currentDirectory;
    bool isChanged;
};
#endif // MAINWINDOW_H
