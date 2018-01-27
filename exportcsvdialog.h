#ifndef EXPORTCSVDIALOG_H
#define EXPORTCSVDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class ExportCSVDialog : public QDialog
{
    Q_OBJECT
public:
    ExportCSVDialog();
    void getDataToProcess(bool *integral, bool *rawData, int *interval);
    void getDataToProcess(bool *integral, bool *rawData, bool *derivative, int *interval);
    bool getExportRawColumns();
    bool getExportStackedColumns();
    bool getExportProcessedData();
    int getInterval();

public slots:
    void set_active_interval_spinBox(bool active);
    void set_active_processing_checkboxes(bool active);

private:
    QLabel *dataSelectLabel;
    QLabel *processedDataParamLabel;
    QLabel *intervalLayoutLabel;

    QVBoxLayout *exportTypeLayout;

    QCheckBox *exportRawColumns;
    QCheckBox *exportStackedColumns;
    QCheckBox *exportProcessedData;

    QCheckBox *integralCheckBox;
    QCheckBox *derivativeCheckBox;
    QCheckBox *rawDataCheckBox;
    QSpinBox *intervalSpinBox;
    QLabel *intervalLabel;

    QPushButton *cancelButton;
    QPushButton *okButton;

    QHBoxLayout *intervalLayout;
    QVBoxLayout *checkBoxesLayout;
    QHBoxLayout *rawDataIntervalLayout;

    QHBoxLayout *buttons_layout;

    QVBoxLayout *mainLayout;

    void updateOkButtonCheckable();

};

#endif // EXPORTCSVDIALOG_H
