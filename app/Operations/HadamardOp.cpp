/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of ImageINSA.
 *
 * ImageINSA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ImageINSA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ImageINSA.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "HadamardOp.h"
#include <QApplication>
#include <GrayscaleImage.h>
#include "Transforms.h"
#include <Converter.h>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>

using namespace std;
using namespace imagein;

HadamardOp::HadamardOp() : Operation(qApp->translate("Operations", "8x8 transforms").toStdString())
{
}

bool HadamardOp::needCurrentImg() const {
    return true;
}

void HadamardOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {


    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(qApp->translate("Operations", "8x8 transforms"));
    QFormLayout* layout = new QFormLayout(dialog);
    QComboBox* transformBox = new QComboBox(dialog);
    transformBox->addItems(QStringList() << "Hadamard" << "Haar" << "Cosinus (DCT)");
    layout->insertRow(0, qApp->translate("Hadamard", "Transform : "), transformBox);

    QLabel* title = new QLabel(qApp->translate("Transforms", "<b>Select the coefficients to keep : </b>"));
    title->setAlignment(Qt::AlignCenter);
    layout->insertRow(1, title);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* selectAllButton = new QPushButton(qApp->translate("Transforms", "Clear selection"));
    QPushButton* selectNoneButton = new QPushButton(qApp->translate("Transforms", "Invert selection"));
    buttonLayout->addWidget(selectAllButton);
    buttonLayout->addWidget(selectNoneButton);
    layout->insertRow(2, buttonLayout);

    QWidget* coefWidget = new QWidget(dialog);
    QGridLayout* coefLayout = new QGridLayout(coefWidget);
    QCheckBox* checkBoxes[8][8];
    for(int i = 0; i < 8; ++i) {
        QLabel* labeli = new QLabel(QString("%1").arg(i));
        QLabel* labelj = new QLabel(QString("%1").arg(i));
        labeli->setAlignment(Qt::AlignCenter);
        labelj->setAlignment(Qt::AlignCenter);
        coefLayout->addWidget(labeli, 0, i + 1);
        coefLayout->addWidget(labelj, i + 1, 0);
    }
    for(int j = 0; j < 8; ++j) {
        for(int i = 0; i < 8; ++i) {
            checkBoxes[j][i] = new QCheckBox(coefWidget);
            coefLayout->addWidget(checkBoxes[j][i], j + 1, i + 1);
            checkBoxes[j][i]->setChecked(true);
            QObject::connect(selectAllButton, SIGNAL(clicked(bool)), checkBoxes[j][i], SLOT(setChecked(bool)));
            QObject::connect(selectNoneButton, SIGNAL(clicked()), checkBoxes[j][i], SLOT(toggle()));
        }
    }
    layout->insertRow(3, coefWidget);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(4, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code!=QDialog::Accepted) return;

    GrayscaleImage_t<bool>* selection = new GrayscaleImage_t<bool>(8, 8);
    for(int j = 0; j < 8; ++j) {
        for(int i = 0; i < 8; ++i) {
            selection->setPixelAt(i, j, checkBoxes[j][i]->isChecked());
        }
    }
//    GrayscaleImage* im = Converter<GrayscaleImage>::convert(*img);
    string s;
    Image_t<double> *resImg;
    Image* invImg;
    if(transformBox->currentIndex() == 0) {
        s = Transforms::Hadamard(img, &resImg, &invImg, selection);
        outDoubleImage(resImg, qApp->translate("Transforms", "Hadamard transform").toStdString(), AUTO, DISABLE, true, 256., true);
        outImage(invImg, qApp->translate("Transforms", "Hadamard reconstruction").toStdString());
    }
    else if(transformBox->currentIndex() == 1) {
        s = Transforms::Haar(img, &resImg, &invImg, selection);
        outDoubleImage(resImg, qApp->translate("Transforms", "Haar transform").toStdString(), AUTO, DISABLE, true, 256., true);
        outImage(invImg, qApp->translate("Transforms", "Haar reconstruction").toStdString());
    }
    else if(transformBox->currentIndex() == 2) {
        s = Transforms::cosinus(img, &resImg, &invImg, selection);
        outDoubleImage(resImg, qApp->translate("Transforms", "cosinus transform").toStdString(), AUTO, DISABLE, true, 256., true);
        outImage(invImg, qApp->translate("Transforms", "cosinus reconstruction").toStdString());
    }
    outText(s);
    outText("-------------------------------------------");
}

