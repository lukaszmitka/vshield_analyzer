#ifndef VSHIELDPROGRESSDIALOG_H
#define VSHIELDPROGRESSDIALOG_H

#include <QSpinBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QThread>
#include "vshieldreader.h"
#include "vshieldanalyzerworker.h"

class VShieldProgressDialog : public QDialog
{
    Q_OBJECT
public:
    //explicit VShieldProgressDialog(QObject *parent = nullptr);
    VShieldProgressDialog(QString directory, QStringList files, QSqlQuery query);
    // TODO ad some features ;P
    void getNumberOfShields(int *shield_no);

signals:
    //void setPBRange(int min, int max);
    //void setPBValue(int value);

public slots:
    //void setRange(int min, int max);
    //void setValue(int value);
    void filesProcessed(int shield_no);

private:
    QLabel *label_number_of_files;
    QSpinBox *numberOfFIlesSpinBox;

    QProgressBar *progressBar;

    QPushButton *cancelButton;
    QPushButton *okButton;

    QHBoxLayout *numberOfFilesLayout;
    QHBoxLayout *buttons_layout;

    QVBoxLayout *mainLayout;

    QSqlQuery insertQuery;

    VShieldAnalyzerWorker* worker;

    int number_of_shields;
};

#endif // VSHIELDPROGRESSDIALOG_H
