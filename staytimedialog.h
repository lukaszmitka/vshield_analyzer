#ifndef STAYTIMEDIALOG_H
#define STAYTIMEDIALOG_H

#include <QSpinBox>
#include <QVBoxLayout>
#include <QtWidgets>

class StayTimeDialog : public QDialog
{
    Q_OBJECT
public:
    StayTimeDialog(int time_min, int time_max);
    void getStayTime(int *min_stay_time, int *max_stay_time);
private:
    QSpinBox *minStayTimeSpinBox;
    QSpinBox *maxStayTimeSpinBox;
};

#endif // STAYTIMEDIALOG_H
