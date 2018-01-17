#ifndef COMPRESSIVESTRENGTHDIALOG_H
#define COMPRESSIVESTRENGTHDIALOG_H

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QMessageBox>
#include <iostream>

#define COAL_LINE_COLUMN_WIDTH 80
#define STRENGTH_COLUMN_WIDTH 80

class CompressiveStrengthDialog : public QDialog
{
    Q_OBJECT
public:
    CompressiveStrengthDialog();
    std::vector<double> getCompressiveStrengths();
private:
    QVBoxLayout *mainLayout;
    QLabel *min_coalLineLabel;
    QLabel *max_coalLineLabel;
    QLabel *compressionStrengthLabel;
    QHBoxLayout *header;
    QHBoxLayout *footer;
    QHBoxLayout *exitFooter;
    QPushButton *qPushButtonAddEntry;
    QPushButton *qPushButtonRemoveEntry;
    QPushButton *qPushButtonValidateData;
    QPushButton *qPushButtonOK;
    QPushButton *qPushButtonCancel;
    QGridLayout *strenghtTable;
    QVector<QSpinBox*> minSpinBoxes;
    QVector<QSpinBox*> maxSpinBoxes;
    QVector<QSpinBox*> compressionStrengths;
    void addNewEntry();
    void removeLastEntry();
    //void initHeader();
    void initExitFooter();
    //void initStrengthTable();
    void initFooter();
    bool verify_data();
    bool verify_single_field(int row, int col);

    QTableWidget* qTableWidget;
    QStringList qTableHeader;
public slots:
    void addEntry();
    void removeEntry();
    void runDataValidation();
};

#endif // COMPRESSIVESTRENGTHDIALOG_H
