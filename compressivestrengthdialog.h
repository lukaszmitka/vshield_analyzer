#ifndef COMPRESSIVESTRENGTHDIALOG_H
#define COMPRESSIVESTRENGTHDIALOG_H

#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QAbstractButton>

class CompressiveStrengthDialog : public QDialog
{
public:
    CompressiveStrengthDialog();
    void getCompressiveStrengths(std::vector<double> *strengths);
};

#endif // COMPRESSIVESTRENGTHDIALOG_H
