/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of EIImage.
 *
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>

#include <cmath>

#include "RotateOp.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;

RotateOp::RotateOp() : Operation(qApp->translate("Operations", "Rotation").toStdString())
{
}

bool RotateOp::needCurrentImg() const {
    return true;
}

struct Point {
    int x;
    int y;
    Point(int x_=0, int y_=0) : x(x_), y(y_) {}
};

inline Point rotatePoint(Point toRotate, double sin, double cos)
{
    Point returnval;
    returnval.x = ((double)toRotate.x) * cos + ((double)toRotate.y) * sin;
    returnval.y = ((double)toRotate.y) * cos - ((double)toRotate.x) * sin;
    return returnval;
}

void RotateOp::operator()(const Image* img, const map<const Image*, string>& imgList) {

    QString imgName(imgList.find(img)->second.c_str());
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Rotation", "Rotating %1").arg(imgName));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);


    layout->setSizeConstraint(QLayout::SetFixedSize);

    QDoubleSpinBox* angleSpinBox = new QDoubleSpinBox(dialog);
    QCheckBox* expandBox = new QCheckBox(qApp->translate("Rotation", "Expand image"), dialog);
    QSpinBox* valueSpinBox = new QSpinBox(dialog);

    angleSpinBox->setRange(-180, 180);
    angleSpinBox->setWrapping(true);
    angleSpinBox->setSingleStep(45);
    valueSpinBox->setRange(0, 255);
    valueSpinBox->setValue(0);

    layout->insertRow(0, qApp->translate("Rotation", "Rotation angle : "), angleSpinBox);
    layout->insertRow(1, expandBox);
    layout->insertRow(2, qApp->translate("Rotation", "Fill value : "), valueSpinBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    double angle = angleSpinBox->value() * acos(0)/90;
    Image::depth_t fillValue = valueSpinBox->value();

    int Width = img->getWidth();
    int Height= img->getHeight();
    int nWidth=Width, nHeight=Height;

    double cos_angle = cos(angle);
    if(fabs(cos_angle)< 1e-10) cos_angle = 0;
    double cos_neg_ang = cos( -angle );
    if( fabs(cos_neg_ang) < 1e-10 ) cos_neg_ang = 0;

    double sin_angle = sin(angle);
    if( fabs(sin_angle) < 1e-10 ) sin_angle = 0;
    double sin_neg_ang = sin( -angle );
    if( fabs(sin_neg_ang) < 1e-10 ) sin_neg_ang = 0;

    // Calculate the size of the new bitmap
    Point rotation_point;
    rotation_point.x = 0;
    rotation_point.y = 0;

    Point p1 = rotatePoint(Point(0,0), sin_angle, cos_angle);
    Point p2 = rotatePoint(Point(img->getWidth()-1,0), sin_angle, cos_angle);
    Point p3 = rotatePoint(Point(0,img->getHeight()-1), sin_angle, cos_angle);
    Point p4 = rotatePoint(Point(img->getWidth()-1,img->getHeight()-1), sin_angle, cos_angle);

    int min_x = min(min(p1.x,p2.x),min(p3.x,p4.x));
    int max_x = max(max(p1.x,p2.x),max(p3.x,p4.x));
    int min_y = min(min(p1.y,p2.y),min(p3.y,p4.y));
    int max_y = max(max(p1.y,p2.y),max(p3.y,p4.y));

    if(expandBox->isChecked()) {
        nWidth = abs(max_x-min_x)+1;
        nHeight= abs(max_y-min_y)+1;
    }

    Image *resImg = new Image(nWidth, nHeight, img->getNbChannels(), fillValue);

    Point source_point;
    for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
        for(unsigned int hcounter=0;hcounter<nHeight;hcounter++)  {
            for(unsigned int wcounter=0;wcounter<nWidth;wcounter++)  {

                source_point.x = wcounter + min_x;
                source_point.y = hcounter + min_y;
                source_point = rotatePoint(source_point,sin_neg_ang,cos_neg_ang);
                source_point.x = source_point.x + rotation_point.x;
                source_point.y = source_point.y + rotation_point.y;

                if(     (source_point.x>=0)
                        &&(source_point.x<Width)
                        &&(source_point.y>=0)
                        &&(source_point.y<Height)) {
                    resImg->setPixel(wcounter,hcounter,c, img->getPixel(source_point.x,source_point.y, c));
                }

            }
        }
    }

    QString name = qApp->translate("Rotation", "rotated %1").arg(angleSpinBox->value());
    this->outImage(resImg, name.toStdString());
}
