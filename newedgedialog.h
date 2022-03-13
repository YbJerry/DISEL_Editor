//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NEWEDGEDIALOG_H
#define NEWEDGEDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "DISOntology.h"

namespace Ui {
class NewEdgeDialog;
}

class NewEdgeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewEdgeDialog(DISEL::Graph *gra, QWidget *parent = nullptr);
    ~NewEdgeDialog();

    std::optional<QString> getFromOntologyTag();
    QString getFromConceptTag();
    std::optional<QString> getToOntologyTag();
    QString getToConceptTag();
    std::optional<QString> getRelationTag();

    void setEdge(const DISEL::Edge &e);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewEdgeDialog *ui;
    DISEL::Graph *gra;
    QString fromOnto, fromCon;
    QString toOnto, toCon;
    QString rela;
};

#endif // NEWEDGEDIALOG_H
