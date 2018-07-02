#ifndef UCHARCONVERTDIALOG_H
#define UCHARCONVERTDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>

namespace Ui {
class UCharConvertDialog;
}

class UCharConvertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UCharConvertDialog(QWidget *parent = 0);
    int getCombo();
    int getOffset();


public slots:
    void enableOffset(int);

protected:
    QLabel* _label2;
    QSpinBox* _spinBox;
    QComboBox* _comboBox;

};

#endif // UCHARCONVERTDIALOG_H
