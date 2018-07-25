#ifndef PSEUDOCOLORDIALOG_H
#define PSEUDOCOLORDIALOG_H

#include <QDialog>
#include <QSpinBox>

namespace Ui {
class PseudoColorDialog;
}

class PseudoColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PseudoColorDialog(QWidget *parent = 0);
    int getHue();
    void setHue(int);

public slots:


protected:
    QSpinBox* _spinBox;
};

#endif //PSEUDOCOLORDIALOG_H
