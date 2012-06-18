#include <QSpinBox>
#include <QDialog>
#include <QLayout>
#include <QFormLayout>
#include <QWidget>

#include "IntParam.h"

using namespace imagein;

IntParam::IntParam(std::string name, int min, int max, int def) : Parameter(name), _min(min), _max(max) {
    _def = std::min(def, _max);
    _def = std::max(def, _min);
}

void IntParam::fillDialog(QDialog* dialog) {
    QWidget *widget = new QWidget();
    QFormLayout* layout = new QFormLayout();
    _spinbox = new QSpinBox();
    _spinbox->setRange(_min, _max);
    _spinbox->setValue(_def);
    layout->insertRow(1, QString(_name.c_str()) + " : ", _spinbox);
    widget->setLayout(layout);
    dialog->layout()->addWidget(widget);
}

void IntParam::pickValue() {
    int value = _spinbox->value();
    value = std::min(value, _max);
    value = std::max(value, _min);
    *_ptr = value;
}

IntParam* IntParam::clone() const {
    return new IntParam(*this);
}
