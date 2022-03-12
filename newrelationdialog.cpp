#include "newrelationdialog.h"
#include "ui_newrelationdialog.h"

NewRelationDialog::NewRelationDialog(DISEL::Graph * gra, bool alreadyCreated, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRelationDialog),
    alreadyCreated(alreadyCreated),
    gra(gra)
{
    ui->setupUi(this);
    buttonGroup.setExclusive(false);

    int i = 0;
    for(auto prop:DISEL::RelationPropertiesVec){
        QCheckBox *checkBox = new QCheckBox(tr(rptos(prop).data()), this);
        buttonGroup.addButton(checkBox, i);
        ui->groupBox->layout()->addWidget(checkBox);
        ++i;
    }
}

NewRelationDialog::~NewRelationDialog()
{
//    QLayoutItem* item;
//    while((item = ui->groupBox->layout()->takeAt(0)) != nullptr){
//        delete item->widget();
//        delete item;
//    }
    delete ui;
}

QString NewRelationDialog::getName()
{
    return name;
}

QVector<DISEL::RelationProperty> NewRelationDialog::getProperties()
{
    return props;
}

void NewRelationDialog::setContent(const DISEL::Relation &rela)
{
    ui->lineEdit->setText(rela.getName().data());
    ui->lineEdit->setReadOnly(true);


    for(auto prop:rela.getAllProperties()){
        auto pred = [&](const QAbstractButton *chkBox){
            return chkBox->text().toStdString() == rptos(prop);
        };
        auto chkBoxes = buttonGroup.buttons();
        if(auto iter = std::find_if(chkBoxes.begin(), chkBoxes.end(), pred); iter != chkBoxes.end()){
            (*iter)->setChecked(true);
        }
    }
}

void NewRelationDialog::on_buttonBox_accepted()
{
    name = ui->lineEdit->text().simplified();

    for(int i = 0; buttonGroup.button(i); ++i){
        auto chkBox = buttonGroup.button(i);
        if(chkBox->isChecked()){
            props.push_back(DISEL::RelationPropertiesVec[i]);
        }
    }

    auto pred = [&](const DISEL::Relation* rela){
        return name.toStdString() == rela->getName();
    };

    if(name.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please input");
        msgBox.exec();
    }else if(auto relas = gra->getRelations();
             !alreadyCreated && std::find_if(relas.begin(), relas.end(), pred) != relas.end()){
        QMessageBox msgBox;
        msgBox.setText(name + QString(" already exists"));
        msgBox.exec();
    }else{
        accept();
    }
}

