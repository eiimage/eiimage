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

#include "ClassResultOp.h"
#include <QApplication>
#include <string>
#include <cstring>
#include <cstdio>
#include "../Widgets/ImageZoneSelector.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QLabel>
#include <algorithm>

using namespace std;
using namespace imagein;

ClassResultOp::ClassResultOp() : Operation(qApp->translate("Operations", "Classification results").toStdString())
{
}

bool ClassResultOp::needCurrentImg() const {
    return true;
}

void ClassResultOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {

    QDialog* dialog = new QDialog(QApplication::activeWindow());
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QFormLayout* formLayout = new QFormLayout();
    QSpinBox* innerBox = new QSpinBox();
    QSpinBox* borderBox = new QSpinBox();
    innerBox->setRange(0, img->getWidth());
    borderBox->setRange(0, img->getWidth());
    innerBox->setSuffix(" px");
    borderBox->setSuffix(" px");
    innerBox->setValue(8);
    borderBox->setValue(2);
    formLayout->insertRow(0, qApp->translate("ClassResult", "Critère de zone intérieure : "), innerBox);
    formLayout->insertRow(1, qApp->translate("ClassResult", "Critère de zone frontière : "), borderBox);
    layout->addWidget(new QLabel(qApp->translate("ClassResult", "<b>Critère de zones (relatifs aux zones totales) : </b>")));
    layout->addLayout(formLayout);
    layout->addWidget(new QLabel(qApp->translate("ClassResult", "<b>Select the image's classes zones : </b>")));
    ImageZoneSelector* zoneSelector = new ImageZoneSelector(dialog, img);
    layout->addWidget(zoneSelector);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->addWidget(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    string returnval;

    int param1 = 2;
    int param2 = 8;
    vector<Rectangle> selection = zoneSelector->getSelections();
    int K = selection.size();
    int* classes = new int[K];
    outText(qApp->translate("ClassResult", "Voici les résultats du classement : \n").toStdString());
    outText(qApp->translate("ClassResult", "\nNombre de classes = %1 ").arg(K).toStdString());
    for(int i = 0; i < K; ++i) {
        Histogram histo = img->getHistogram(0, selection.at(i));
        classes[i] = (uint8_t) (std::max_element(histo.begin(), histo.end()) - histo.begin());
        outText(qApp->translate("ClassResult", "Valeur de la classe %1 = %2").arg(i+1).arg(classes[i]).toStdString());
    }


    //Zone frontire
    double *tauxF = new double[K];
    double *tauxI = new double[K];
    for(int i = 0; i < K; ++i){
        tauxF[i] = 0.;
        tauxI[i] = 0.;
    }

    for(int n = 0; n < K; ++n) {
        Rectangle zone = selection.at(n);
        for(unsigned int j = zone.top(); j < zone.bottom(); ++j) {
            for(unsigned int i = zone.left(); i < zone.right(); ++i) {
                if(img->getPixelAt(i, j) == classes[n]) {
                    if( i >= zone.left() + param2 && i < zone.right() - param2 && j >= zone.top() + param2 && j < zone.bottom() - param2 ) {
                        tauxI[n]++;
                    }
                    if( i < zone.left() + param1 || i >= zone.right() - param1 || j < zone.top() + param1 || j >= zone.bottom() - param1 ) {
                        tauxF[n]++;
                    }
                }
            }
        }
    }

    for(int n = 0; n < K; ++n) {
        Rectangle zone = selection.at(n);
        const double areaI = (zone.h - 2*param2) * (zone.w - 2*param2);
        const double areaF = (zone.h * zone.w) - (zone.h - 2*param1) * (zone.w - 2*param1);
        tauxI[n] = tauxI[n] * 100. / areaI;
        tauxF[n] = tauxF[n] * 100. / areaF;
    }
    for(int n = 0; n < K; ++n) {
        outText(qApp->translate("ClassResult", "Le taux de bon classement en zone intérieure %1 vaut: %2\%").arg(n+1).arg(tauxI[n], 0, 'f', 2).toStdString());
    }
    for(int n = 0; n < K; ++n) {
        outText(qApp->translate("ClassResult", "Le taux de bon classement en zone frontière %1 vaut: %2\%").arg(n+1).arg(tauxF[n], 0, 'f', 2).toStdString());
    }

    double tauxGI = 0., tauxGF = 0.;
    for(int i = 0; i < K; ++i){
        tauxGI += tauxI[i];
        tauxGF += tauxF[i];
    }
    tauxGI /= K;
    tauxGF /= K;

    outText(qApp->translate("ClassResult", "Le taux de bon classement en zone intérieure globale vaut: %1\%").arg(tauxGI, 0, 'f', 2).toStdString());
    outText(qApp->translate("ClassResult", "Le taux de bon classement en zone frontière globale vaut: %1\%").arg(tauxGF, 0, 'f', 2).toStdString());

    outText(returnval);
}
