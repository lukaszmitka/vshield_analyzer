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
    Q_OBJECT
public:
    CompressiveStrengthDialog();
    std::vector<double> getCompressiveStrengths();
};

#endif // COMPRESSIVESTRENGTHDIALOG_H
