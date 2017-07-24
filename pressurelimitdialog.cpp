#include "pressurelimitdialog.h"

PressureLimitDialog::PressureLimitDialog(double limit_min, double limit_max)
{
    QLabel *label_press_min = new QLabel(tr("Ciśnienie minimalne"));
    press_min = new QDoubleSpinBox();
    press_min->setDecimals(2);
    press_min->setMinimumWidth(60);
    press_min->setMaximumWidth(60);
    press_min->setValue(limit_min);
    QHBoxLayout *min_press_layout = new QHBoxLayout;
    min_press_layout->addWidget(label_press_min);
    min_press_layout->addWidget(press_min);

    QLabel *label_press_max = new QLabel(tr("Ciśnienie maksymalne"));
    press_max = new QDoubleSpinBox();
    press_max->setDecimals(2);
    press_max->setMinimumWidth(60);
    press_max->setMaximumWidth(60);
    press_max->setValue(limit_max);
    QHBoxLayout *max_press_layout = new QHBoxLayout;
    max_press_layout->addWidget(label_press_max);
    max_press_layout->addWidget(press_max);

    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    QHBoxLayout *buttons_layout = new QHBoxLayout;
    buttons_layout->addWidget(okButton);
    buttons_layout->addSpacing(20);
    buttons_layout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(min_press_layout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(max_press_layout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttons_layout);
    setLayout(mainLayout);
}

void PressureLimitDialog::getPressureLimts(double *limit_min, double *limit_max){
    *limit_min = press_min->value();
    *limit_max = press_max->value();
}
