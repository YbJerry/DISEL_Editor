#include "newrelationdialog.h"
#include "ui_newrelationdialog.h"

NewRelationDialog::NewRelationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRelationDialog)
{
    ui->setupUi(this);

    for(auto prop:DISEL::RelationPropertiesVec){
        QCheckBox *checkBox = new QCheckBox(tr(rptos(prop).data()), this);
        buttonGroup.addButton(checkBox);
        ui->groupBox->layout()->addWidget(checkBox);
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

void NewRelationDialog::on_buttonBox_accepted()
{
    name = ui->lineEdit->text().simplified();

    for(int i = 0; buttonGroup.button(i); ++i){
        auto chkBox = buttonGroup.button(i);
        if(chkBox->isChecked()){
            props.push_back(DISEL::RelationPropertiesVec[i]);
        }
    }

    if(name.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please input");
        msgBox.exec();
    }else{
        accept();
    }
}

