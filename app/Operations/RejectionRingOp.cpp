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

#include "RejectionRingOp.h"
#include "../Tools.h"

#include <QDialog>
#include <QFormLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
using namespace imagein;
using namespace std;

RejectionRingOp::RejectionRingOp() : Operation(qApp->translate("Operations", "Rejection ring").toStdString())
{
}

void RejectionRingOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) {
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Rejection ring"));
    dialog->setMinimumWidth(180);
    QFormLayout *layout = new QFormLayout(dialog);

    QSpinBox *widthBox = new QSpinBox(dialog);
    widthBox->setRange(0, 65536);
    widthBox->setValue(512);
    layout->insertRow(0, qApp->translate("RejectionRingOp", "Width=Height : "), widthBox);

    QSpinBox *radiusBox = new QSpinBox(dialog);
    radiusBox->setRange(0, 65536);
    layout->insertRow(1, qApp->translate("RejectionRingOp", "Radius : "), radiusBox);

    QSpinBox *thickBox = new QSpinBox(dialog);
    thickBox->setRange(0, 65536);
    layout->insertRow(2, qApp->translate("RejectionRingOp", "Thickness (beyond radius) : "), thickBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal,
                                                       dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if (code != QDialog::Accepted) return;

    //On rcupre la taille de l'image a traiter
    int nb_ligne, nb_colonne;
    int rayon, epaisseur;

    nb_ligne = widthBox->value(); //1er parametre de l'algorithme
    rayon = radiusBox->value(); //2nd parametre de l'algorithme
    epaisseur = thickBox->value(); //3me parametre de l'algorithme

    nb_colonne = nb_ligne;

    Image_t<double> *result = new Image_t<double>(nb_ligne, nb_colonne, 1);//image resultat

    int i, j;
    int icentre, jcentre;

    icentre = (int) (nb_ligne / 2.);
    jcentre = (int) (nb_colonne / 2.);

    for (i = 0; i < nb_ligne; i++)
        for (j = 0; j < nb_colonne; j++) {
            result->setPixelAt(i, j, 1.);
        }

    for (i = 0; i < nb_ligne; i++)
        for (j = 0; j < nb_colonne; j++) {
            if ((i - icentre) * (i - icentre) + (j - jcentre) * (j - jcentre) >= rayon * rayon &&
                (i - icentre) * (i - icentre) + (j - jcentre) * (j - jcentre) <=
                (rayon + epaisseur) * (rayon + epaisseur))
                result->setPixelAt(i, j, 0.);
        }


    QString name(qApp->translate("RejectionRingOp", "Rejection ring (%1 %2 %3)"));
    name = name.arg(nb_ligne).arg(rayon).arg(epaisseur);
    outDoubleImage(result, name.toStdString(),ENABLE,DISABLE);
}

bool RejectionRingOp::needCurrentImg() const {
    return false;
}
