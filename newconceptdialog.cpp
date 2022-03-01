#include "newconceptdialog.h"
#include "ui_newconceptdialog.h"

NewConceptDialog::NewConceptDialog(DISEL::Ontology * onto, bool alreadyCreated, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConceptDialog),
    alreadyCreated(alreadyCreated),
    onto(onto)
{
    ui->setupUi(this);
    this->setWindowTitle(templateAddStr("Concept", "Ontology"));

    for(auto at:onto->getAtomDomain()){
        auto *chkBox = new QCheckBox(at->getName().data(), this);
        chkBoxes.push_back(chkBox);
        ui->groupBox->layout()->addWidget(chkBox);
    }
    ui->groupBox->layout()->update();
}

NewConceptDialog::~NewConceptDialog()
{
    delete ui;
}

QString NewConceptDialog::getNameStr()
{
    return name;
}

QString NewConceptDialog::getDescStr()
{
    return desc;
}

QVector<QString> NewConceptDialog::getAtoms()
{
    QVector<QString> atoms;
    for(const auto &chkBox:chkBoxes){
        if(chkBox->isChecked()){
            atoms.push_back(chkBox->text());
        }
    }
    return atoms;
}

void NewConceptDialog::setContent(const DISEL::Concept &con)
{
    ui->lineEdit->setText(con.getName().data());
    ui->lineEdit->setReadOnly(true);
    ui->textEdit->setText(con.getDescription().data());

    for(auto at:con.getLatticeOfConcepts()){
        auto pred = [at](const QCheckBox *box){return box->text() == at.data();};
        if(auto iter = std::find_if(chkBoxes.begin(), chkBoxes.end(), pred); iter != chkBoxes.end()){
            (*iter)->setChecked(true);
        }
    }
}

void NewConceptDialog::on_buttonBox_accepted()
{
    name = ui->lineEdit->text().simplified();
    desc = ui->textEdit->toPlainText();
    auto pred = [&](DISEL::Concept* item){
        return item->getName() == name.toStdString();
    };

    if(name.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please input");
        msgBox.exec();
    }else if(auto cons = onto->getAllConcepts();
             !alreadyCreated && std::find_if(cons.begin(), cons.end(), pred) != cons.end()){
        QMessageBox msgBox;
        msgBox.setText(name + QString(" already exists"));
        msgBox.exec();
    }else{
        accept();
    }
}

void NewConceptDialog::on_comboBox_currentIndexChanged(int idx)
{
    // clear all the items in groupbox
    QLayoutItem* item;
    while((item = ui->vBoxLayout->takeAt(0)) != nullptr){
        delete item->widget();
        delete item;
    }

    chkBoxes.clear();
}
