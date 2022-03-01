#ifndef NEWRELATIONDIALOG_H
#define NEWRELATIONDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include <QButtonGroup>

#include "DISRelation.h"
#include "utils.hpp"

namespace Ui {
class NewRelationDialog;
}

class NewRelationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRelationDialog(QWidget *parent = nullptr);
    ~NewRelationDialog();

    QString getName();
    QVector<DISEL::RelationProperty> getProperties();
    void setRelation(const DISEL::Relation &rela);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewRelationDialog *ui;
    QString name;
    QVector<DISEL::RelationProperty> props;
    QButtonGroup buttonGroup;
};

#endif // NEWRELATIONDIALOG_H
