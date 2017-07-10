#ifndef MEDIANDIALOG_H
#define MEDIANDIALOG_H

#include <QDialog>

namespace Ui {
class MedianDialog;
}

class MedianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedianDialog(QWidget *parent = 0);
    ~MedianDialog();

    bool cross() const;
    bool square() const;
    unsigned int getSize() const;

private:
    Ui::MedianDialog *ui;
};

#endif // MEDIANDIALOG_H
