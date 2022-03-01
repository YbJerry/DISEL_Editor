#ifndef NEWCONCEPTDIALOG_H
#define NEWCONCEPTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QMessageBox>

#include "DISOntology.h"

#include "utils.h"

namespace Ui {
class NewConceptDialog;
}

class NewConceptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewConceptDialog(DISEL::Ontology * onto, bool alreadyCreated = false, QWidget *parent = nullptr);
    ~NewConceptDialog();

    QString getNameStr();
    QString getDescStr();
    QVector<QString> getAtoms();
    void setContent(const DISEL::Concept &con);

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::NewConceptDialog *ui;
    QVector<QCheckBox *> chkBoxes;
    DISEL::Ontology * onto;

    bool alreadyCreated;
    QString name;
    QString desc;
    QVector<QString> atoms;
};

#endif // NEWCONCEPTDIALOG_H
