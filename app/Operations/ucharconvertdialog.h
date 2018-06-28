#ifndef UCHARCONVERTDIALOG_H
#define UCHARCONVERTDIALOG_H

#include <QDialog>

namespace Ui {
class UCharConvertDialog;
}

class UCharConvertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UCharConvertDialog(QWidget *parent = 0);
    ~UCharConvertDialog();

private:
    Ui::UCharConvertDialog *ui;
};

#endif // UCHARCONVERTDIALOG_H
