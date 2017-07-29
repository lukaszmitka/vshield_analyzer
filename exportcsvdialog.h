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

public slots:
    void set_active_raw_data(bool active);

private:
    QLabel *dataSelectLabel;

    QCheckBox *integralCheckBox;

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
};

#endif // EXPORTCSVDIALOG_H
