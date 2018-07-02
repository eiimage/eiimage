#include "UCharConvertDialog.h"
#include "ui_ucharconvertdialog.h"
#include <Converter.h>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>


UCharConvertDialog::UCharConvertDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(qApp->translate("Operations","Convert to UChar"));
    this->setMinimumWidth(180);

    QFormLayout* layout = new QFormLayout(this);

    QStringList text = (QStringList() << "crop" << "normalize" << "Add Offset" << "Scale" << "Add offset and scale");
    QLabel* label1 = new QLabel("Operation");
    _comboBox = new QComboBox();
    _comboBox->addItems(text);

    _label2 = new QLabel("Offset");
    _spinBox = new QSpinBox();
    _spinBox->setMaximum(255);
    _spinBox->setMinimum(0);
    _spinBox->setValue(127);
    _spinBox->setEnabled(false);
    _label2->setEnabled(false);
    _spinBox->setVisible(false);
    _label2->setVisible(false);

    layout->addRow(label1, _comboBox);
    layout->addRow(_label2, _spinBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->insertRow(3, buttonBox);

    QObject::connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableOffset(int)));
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


void UCharConvertDialog::enableOffset(int n){
    std::cout << "which case it is ? -> " << n << "\n";
    if(n==2){
        _spinBox->setEnabled(true);
        _label2->setEnabled(true);
        _spinBox->setVisible(true);
        _label2->setVisible(true);
    }else if(n==4){
        _spinBox->setEnabled(false);
        _label2->setEnabled(false);
        _spinBox->setValue(127);
        _spinBox->setVisible(true);
        _label2->setVisible(true);
    }else{
        _spinBox->setEnabled(false);
        _label2->setEnabled(false);
        _spinBox->setVisible(false);
        _label2->setVisible(false);
    }
}

int UCharConvertDialog::getCombo(){
    return _comboBox->currentIndex();
}

int UCharConvertDialog::getOffset(){
    return _spinBox->value();
}
