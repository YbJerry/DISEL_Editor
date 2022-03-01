#include "newedgedialog.h"
#include "ui_newedgedialog.h"

NewEdgeDialog::NewEdgeDialog(DISEL::Graph *gra, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEdgeDialog),
    gra(gra)
{
    ui->setupUi(this);
}

NewEdgeDialog::~NewEdgeDialog()
{
    delete ui;
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
    fromOnto = ui->fromOntoLineEdit->text().simplified();
    fromCon = ui->fromConLineEdit->text().simplified();
    toOnto = ui->toOntoLineEdit->text().simplified();
    toCon = ui->toConLineEdit->text().simplified();

    if(fromCon.isEmpty() || toCon.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Please input");
        msgBox.exec();
    }else{
        accept();
    }
}

