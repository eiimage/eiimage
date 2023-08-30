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

#include <QVBoxLayout>
#include <QLabel>
#include "GetHintOp.h"


GetHintOp::GetHintOp() : Operation(qApp->translate("Operations", "About HSV").toStdString())
{
}


bool GetHintOp::needCurrentImg() const {
    return false;
}

void GetHintOp::operator()(const imagein::Image *, const std::map<const imagein::Image *, std::string> &) {
    auto* hint = new QDialog(QApplication::activeWindow());
    hint->setWindowTitle(QString(qApp->translate("ColorDialog", "A small tip")));

    // Créer une disposition verticale pour le QDialog
    auto* mainLayout = new QVBoxLayout(hint);

    auto* huePic = new QLabel(hint);
    huePic->setPixmap(QPixmap(":/img/qcolor_hsv.png"));

    auto* hueLabel = new QLabel("<b>Hue</b> is the dominant color from 0 to 359 degrees on the color wheel below <br>", hint);
    auto* hueNote = new QLabel("note : Qt's HSV model uses a hue of -1 for achromatic colors <br>", hint);

    auto* satuPic = new QLabel(hint);
    satuPic->setPixmap(QPixmap(":/img/qcolor-saturation.png"));

    auto* satuLabel = new QLabel("<b>Saturation</b> is in the range 0 to 255 and refers to the vividness of the color <br>", hint);

    auto* valuePic = new QLabel(hint);
    valuePic->setPixmap(QPixmap(":/img/qcolor-value.png"));

    auto* valueLabel = new QLabel("<b>Value</b> is in the range 0 to 255 and represents the brightness of the color <br>", hint);

    // Ajouter les QLabel à la disposition principale
    mainLayout->addWidget(hueLabel);
    mainLayout->addWidget(huePic);
    mainLayout->addWidget(hueNote);
    mainLayout->addWidget(satuLabel);
    mainLayout->addWidget(satuPic);
    mainLayout->addWidget(valueLabel);
    mainLayout->addWidget(valuePic);

    // Afficher le QDialog
    hint->show();
}
