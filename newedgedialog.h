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
