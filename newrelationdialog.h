#ifndef NEWRELATIONDIALOG_H
#define NEWRELATIONDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include <QButtonGroup>

#include "DISRelation.h"
#include "DISGraph.h"
#include "utils.hpp"

namespace Ui {
class NewRelationDialog;
}

class NewRelationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRelationDialog(DISEL::Graph * gra, bool alreadyCreated = false, QWidget *parent = nullptr);
    ~NewRelationDialog();

    QString getName();
    QVector<DISEL::RelationProperty> getProperties();
    void setContent(const DISEL::Relation &rela);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewRelationDialog *ui;
    QString name;
    QVector<DISEL::RelationProperty> props;
    QButtonGroup buttonGroup;
    DISEL::Graph *gra;
    bool alreadyCreated;
};

#endif // NEWRELATIONDIALOG_H
