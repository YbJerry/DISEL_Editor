//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "newedgedialog.h"
#include "ui_newedgedialog.h"

NewEdgeDialog::NewEdgeDialog(DISEL::Ontology *onto, DISEL::Graph *gra, QDir directory, QWidget *parent) :
    QDialog(parent),
    directory(directory),
    ui(new Ui::NewEdgeDialog),
    onto(onto),
    gra(gra)
{
    ui->setupUi(this);

    auto *pVec = new QVector<QString>;
    auto thisOntoName = "-- This Ontology";
    ontoGroup.push_back(thisOntoName);
    ontoConGroup.push_back(pVec);
    //ontoConMap[thisOntoName] = pVec;
    ui->fromOntoComboBox->addItem(thisOntoName);
    ui->toOntoComboBox->addItem(thisOntoName);
    for(auto con:onto->getAllConcepts()){
        auto conNameQStr = QString::fromStdString(con->getName());
        ui->fromConComboBox->addItem(conNameQStr);
        ui->toConComboBox->addItem(conNameQStr);
        pVec->push_back(conNameQStr);
    }

    for(auto [ontoName, filePath]:onto->getAllIncludeOntology()){
        auto completePath = directory.filePath(QString::fromStdString(filePath.string()));
        qDebug() << completePath;
        DISXMLReader reader(completePath.toStdString());
        auto *includeOnto = reader.read();
        pVec = new QVector<QString>;
        auto ontoNameQStr = QString::fromStdString(includeOnto->getName());
        ontoGroup.push_back(ontoNameQStr);
        ontoConGroup.push_back(pVec);
        //ontoConMap[ontoNameQStr] = pVec;

        ui->fromOntoComboBox->addItem(ontoNameQStr);
        ui->toOntoComboBox->addItem(ontoNameQStr);

        for(auto con:includeOnto->getAllConcepts()){
            pVec->push_back(QString::fromStdString(con->getName()));
        }

        delete includeOnto;
    }

    ui->relaComboBox->addItem("-- Without any relation");
    for(auto* rela:gra->getRelations()){
        ui->relaComboBox->addItem(QString::fromStdString(rela->getName()));
    }
}

NewEdgeDialog::~NewEdgeDialog()
{
    delete ui;
    for(auto it:ontoConMap){
        delete it;
    }
}

std::optional<QString> NewEdgeDialog::getFromOntologyTag()
{
    if(fromOnto.isEmpty())
        return {};
    return fromOnto;
}

QString NewEdgeDialog::getFromConceptTag()
{
    return fromCon;
}

std::optional<QString> NewEdgeDialog::getToOntologyTag()
{
    if(toOnto.isEmpty())
        return {};
    return toOnto;
}

QString NewEdgeDialog::getToConceptTag()
{
    return toCon;
}

std::optional<QString> NewEdgeDialog::getRelationTag()
{
    if(rela.isEmpty())
        return {};
    return rela;
}

void NewEdgeDialog::on_buttonBox_accepted()
{
//    fromOnto = ui->fromOntoLineEdit->text().simplified();
//    fromCon = ui->fromConLineEdit->text().simplified();
//    toOnto = ui->toOntoLineEdit->text().simplified();
//    toCon = ui->toConLineEdit->text().simplified();

    if(ui->fromOntoComboBox->currentIndex() != 0){
        fromOnto = ui->fromOntoComboBox->currentText().simplified();
    }
    fromCon = ui->fromConComboBox->currentText().simplified();
    if(ui->toOntoComboBox->currentIndex() != 0){
        toOnto = ui->toOntoComboBox->currentText().simplified();
    }
    toCon = ui->toConComboBox->currentText().simplified();
    if(ui->relaComboBox->currentIndex() != 0){
        rela = ui->relaComboBox->currentText().simplified();
    }

    if(fromCon.isEmpty() || toCon.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please input");
        msgBox.exec();
    }else{
        accept();
    }
}


void NewEdgeDialog::on_fromOntoComboBox_currentIndexChanged(int index)
{
    ui->fromConComboBox->clear();
    for(auto conName: *ontoConGroup[index]){
        ui->fromConComboBox->addItem(conName);
    }
}


void NewEdgeDialog::on_toOntoComboBox_currentIndexChanged(int index)
{
    ui->toConComboBox->clear();
    for(auto conName: *ontoConGroup[index]){
        ui->toConComboBox->addItem(conName);
    }
}

