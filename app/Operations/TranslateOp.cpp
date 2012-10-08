#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>

#include "TranslateOp.h"
#include "../Tools.h"
using namespace std;
using namespace imagein;

TranslateOp::TranslateOp() : Operation(qApp->translate("Operations", "Translation").toStdString()) {
}

bool TranslateOp::needCurrentImg() const {
    return true;
}


void TranslateOp::operator()(const Image* img, const map<const Image*, string>& imgList) {
    vector<QWidget*> result;
    QString imgName(imgList.find(img)->second.c_str());
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Translation", "Translating %1").arg(imgName));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);


    layout->setSizeConstraint(QLayout::SetFixedSize);

    QSpinBox* xSpinBox = new QSpinBox(dialog);
    QSpinBox* ySpinBox = new QSpinBox(dialog);
    QCheckBox* expandBox = new QCheckBox(qApp->translate("TranslateOp", "Expand image"), dialog);
    QSpinBox* valueSpinBox = new QSpinBox(dialog);

    xSpinBox->setRange(-65536, 65535);
    ySpinBox->setRange(-65536, 65535);
    valueSpinBox->setRange(0, 255);
    valueSpinBox->setValue(0);

    layout->insertRow(0, qApp->translate("TranslateOp", "X offset : "), xSpinBox);
    layout->insertRow(1, qApp->translate("TranslateOp", "Y offset : "), ySpinBox);
    layout->insertRow(2, expandBox);
    layout->insertRow(3, qApp->translate("TranslateOp", "Fill value : "), valueSpinBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(4, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    int dx = xSpinBox->value();
    int dy = ySpinBox->value();
    Image::depth_t fillValue = valueSpinBox->value();
    Image* resImg;
    if(expandBox->isChecked()) {
        resImg = new Image(img->getWidth() + abs(dx), img->getHeight() + abs(dy), img->getNbChannels(), fillValue);
        dx = max(0, dx);
        dy = max(0, dy);
        for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < img->getHeight(); ++j) {
                for(unsigned int i = 0; i < img->getWidth(); ++i) {
                    Image::depth_t value = img->getPixel(i, j, c);
                    resImg->setPixel(i + dx, j + dy, c, value);
                }
            }
        }
    }
    else {
        resImg = new Image(img->getWidth(), img->getHeight(), img->getNbChannels(), fillValue);
        for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
            for(unsigned int j = max(dy, 0); j < resImg->getHeight()+min(dy,0); ++j) {
                for(unsigned int i = max(dx, 0); i < resImg->getWidth()+min(dx,0); ++i) {
                    Image::depth_t value = img->getPixel(i-dx, j-dy, c);
                    resImg->setPixel(i, j, c, value);
                }
            }
        }
    }

    QString name = qApp->translate("TranslateOp", "Translated %1:%2").arg(dx).arg(dy);
    this->outImage(resImg, name.toStdString());
    return;
}
