//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
    void setCheckedAtoms(const QVector<QString> &atoms);

private slots:
    void on_buttonBox_accepted();

    //void on_comboBox_currentIndexChanged(int index);

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
