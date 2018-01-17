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

struct single_entry{
    int begin;
    int end;
    int strength;
};

class CompressiveStrengthDialog : public QDialog
{
    Q_OBJECT
public:
    CompressiveStrengthDialog();
    CompressiveStrengthDialog(std::vector<single_entry> current_data);
    std::vector<single_entry> getCompressiveStrengths();
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
    void init_layout();
    void addNewEntry();
    void removeLastEntry();
    //void initHeader();
    void initExitFooter();
    //void initStrengthTable();
    void initFooter();
    bool verify_data();
    bool verify_single_field(int row, int col);
    single_entry read_single_entry(int row);


    QTableWidget* qTableWidget;
    QStringList qTableHeader;
public slots:
    void addEntry();
    void removeEntry();
    void runDataValidation();
};

#endif // COMPRESSIVESTRENGTHDIALOG_H
