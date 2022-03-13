//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "newatomdialog.h"
#include "ui_newatomdialog.h"

NewAtomDialog::NewAtomDialog(DISEL::Ontology * onto, bool alreadyCreated, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAtomDialog),
    alreadyCreated(alreadyCreated),
    onto(onto)
{
    ui->setupUi(this);
    this->setWindowTitle(templateAddStr("Atom", "Ontology"));

}

NewAtomDialog::~NewAtomDialog()
{
    delete ui;
}

QString NewAtomDialog::getAtomStr()
{
    return atom;
}

QString NewAtomDialog::getDescStr()
{
    return desc;
}

void NewAtomDialog::setContent(const DISEL::Atom &atom)
{
    ui->lineEdit->setText(atom.getName().data());
    ui->lineEdit->setReadOnly(true);
    ui->textEdit->setText(atom.getDescription().data());
}

void NewAtomDialog::on_buttonBox_accepted()
{
    atom = ui->lineEdit->text().simplified();
    desc = ui->textEdit->toPlainText();
    auto pred = [&](const DISEL::Atom* at){
        return at->getName() == atom.toStdString();
    };

    if(atom.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please input");
        msgBox.exec();
    }else if(auto at = onto->getAtomDomain();
             !alreadyCreated && std::find_if(at.begin(), at.end(), pred) != at.end()){
        QMessageBox msgBox;
        msgBox.setText(atom + QString(" already exists"));
        msgBox.exec();
    }else{
        accept();
    }
}
