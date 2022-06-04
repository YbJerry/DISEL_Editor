//This Source Code Form is subject to the terms of the Mozilla Public
//License, v. 2.0. If a copy of the MPL was not distributed with this
//file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NEWEDGEDIALOG_H
#define NEWEDGEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QMessageBox>
#include <QDir>

#include "DISOntology.h"
#include "DISXMLReader.h"

namespace Ui {
class NewEdgeDialog;
}

class NewEdgeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewEdgeDialog(DISEL::Ontology *onto, DISEL::Graph *gra, QDir directory, QWidget *parent = nullptr);
    ~NewEdgeDialog();

    std::optional<QString> getFromOntologyTag();
    QString getFromConceptTag();
    std::optional<QString> getToOntologyTag();
    QString getToConceptTag();
    std::optional<QString> getRelationTag();

    void setEdge(const DISEL::Edge &e);

private slots:
    void on_buttonBox_accepted();

    void on_fromOntoComboBox_currentIndexChanged(int index);

    void on_toOntoComboBox_currentIndexChanged(int index);

private:
    Ui::NewEdgeDialog *ui;
    DISEL::Ontology *onto;
    DISEL::Graph *gra;
    QMap<QString, QVector<QString>*> ontoConMap;
    QVector<QString> ontoGroup;
    QVector<QVector<QString>*> ontoConGroup;
    QString fromOnto, fromCon;
    QString toOnto, toCon;
    QString rela;
    QDir directory;
};

#endif // NEWEDGEDIALOG_H
