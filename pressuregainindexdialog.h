#ifndef PRESSUREGAININDEXDIALOG_H
#define PRESSUREGAININDEXDIALOG_H

#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDialog>
#include <vector>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QAbstractButton>

class PressureGainIndexDialog : public QDialog
{
    Q_OBJECT
public:
    PressureGainIndexDialog(int shield_count);
    std::vector <int> getChosenShields();
    QVBoxLayout *mainLayout;
};

#endif // PRESSUREGAININDEXDIALOG_H
