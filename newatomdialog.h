//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NEWATOMDIALOG_H
#define NEWATOMDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "DISOntology.h"

#include "utils.h"

namespace Ui {
class NewAtomDialog;
}

class NewAtomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewAtomDialog(DISEL::Ontology * onto, bool alreadyCreated = false, QWidget *parent = nullptr);
    ~NewAtomDialog();

    QString getAtomStr();
    QString getDescStr();
    void setContent(const DISEL::Atom &atom);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewAtomDialog *ui;
    QString atom;
    QString desc;
    bool alreadyCreated;
    DISEL::Ontology * onto;
};

#endif // NEWATOMDIALOG_H
