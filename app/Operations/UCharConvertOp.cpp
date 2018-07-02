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


#include "UCharConvertOp.h"
#include "../Tools.h"
#include <QApplication>
#include <QObject>
#include "Operation.h"
#include "UCharConvertDialog.h"
#include "Image.h"
#include <Converter.h>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>

using namespace imagein;

UCharConvertOp::UCharConvertOp() : DoubleOperation(qApp->translate("Operations", "UChar converter").toStdString())
{
}

bool UCharConvertOp::needCurrentImg() const{
    return true;
}

void UCharConvertOp::operator()(const imagein::Image_t<double>* from, const std::map<const imagein::Image_t<double>*, std::string>&){

    UCharConvertDialog* dialog = new UCharConvertDialog(QApplication::activeWindow());

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
    if(code!=QDialog::Accepted) return;

    Image * resImg;
    std::string LogMessage = "";
    Image_t<int> * tempIntImg;
    int offset;
    std::string windowName;

    switch(dialog->getCombo())
    {
        case 0 :
            resImg = Converter<Image>::convertAndRound(*from);
            windowName = qApp->translate("Operations","Cropped").toStdString();
            break;
                
        case 1 :
            tempIntImg = Converter<Image_t<double>>::convertToInt(*from);
            tempIntImg->normalize();
            resImg = Converter<Image>::convert(*tempIntImg);
            delete tempIntImg;
            windowName = qApp->translate("Operations","Normalized").toStdString();
            break;

        case 2 :
            std::cout << "offset : " << dialog->getOffset() << " \n";
            tempIntImg = Converter<Image_t<double>>::convertToInt(*from);
            offset = dialog->getOffset();
            resImg = Converter<Image>::convertAndOffset(*tempIntImg, &LogMessage, offset);
            delete tempIntImg;
            windowName = qApp->translate("Operations","Offset").toStdString();
            break;

        case 4 :
            tempIntImg = Converter<Image_t<double>>::convertToInt(*from);
            resImg = Converter<Image>::convertScaleAndOffset(*tempIntImg, &LogMessage);
            delete tempIntImg;
            windowName = qApp->translate("Operations","Offset and scaled").toStdString();
            break; 

        case 3 :
            tempIntImg = Converter<Image_t<double>>::convertToInt(*from);
            resImg = Converter<Image>::convertAndScale(*tempIntImg, &LogMessage);
            delete tempIntImg;
            windowName = qApp->translate("Operations","Scaled").toStdString();
            break;

        default: 
            std::cout << "Default conversion" << std::endl;
            resImg = Converter<Image>::convertAndRound(*from);
            windowName = qApp->translate("Operations","Cropped").toStdString();
            break;
    }

    //si erreur lors du chargement alors non de fenêtre vide
    outImage(resImg, windowName);
}


