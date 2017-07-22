#ifndef PRESSURELIMITDIALOG_H
#define PRESSURELIMITDIALOG_H

#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QAbstractButton>

class PressureLimitDialog : public QDialog
{
    Q_OBJECT
public:
    PressureLimitDialog(double limit_min, double limit_max);
    void getPressureLimts(double *limit_min, double *limit_max);
private:
    QDoubleSpinBox *press_min;
    QDoubleSpinBox *press_max;
};

#endif // PRESSURELIMITDIALOG_H
