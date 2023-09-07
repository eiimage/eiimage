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

#include "ClassAnalysisOp.h"
#include <QApplication>
#include "ClassAnalysisDialog.h"
#include "../Algorithms/ClassAnalysis.h"

#include <cstdio>
#include <Converter.h>
#include <GrayscaleImage.h>
#include <QMessageBox>

using namespace std;
using namespace imagein;

ClassAnalysisOp::ClassAnalysisOp() : Operation(qApp->translate("Operations", "Supervised classification").toStdString())
{
}

bool ClassAnalysisOp::needCurrentImg() const {
    return true;
}

void ClassAnalysisOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {

    ClassAnalysisDialog* dialog = new ClassAnalysisDialog(QApplication::activeWindow(), img);
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);


    if(dialog->isLearningStep()) {
//        FILE* f = fopen(dialog->getFileName().toLatin1(), "wt" );
        FILE* f = fopen(dialog->getFileName().toLocal8Bit(), "wt" );
        if(f == NULL) {
            QMessageBox::critical(NULL, "Error", "Could not open file for write access");
            return;
        }
        try {
            Image_t<double> *meanImg, *devImg;
            ClassAnalysis::write_to_file(image, dialog->getSelections(), f, dialog->getWindowSize(), &meanImg, &devImg);
            outDoubleImage(meanImg, qApp->translate("ClassAnalysis", "mean").toStdString());
            outDoubleImage(devImg, qApp->translate("ClassAnalysis", "standard deviation").toStdString());
        }
        catch(const char*e) {
            QMessageBox::critical(NULL, "Error", e);
            return;
        }
        fclose(f);
    }
    else if(dialog->isClassificationStep()) {
        FILE* f = fopen(dialog->getFileName().toLatin1(), "rt" );
        if(f == NULL) {
            QMessageBox::critical(NULL, "Error", "Could not open file for read access");
            return;
        }
        try {
            Image_t<double> *meanImg, *devImg;
            Image *resImg = ClassAnalysis::classify_from_file(image, f, dialog->getWindowSize(), &meanImg, &devImg);
            outImage(resImg, qApp->translate("ClassAnalysis", "classified").toStdString());
            outDoubleImage(meanImg, qApp->translate("ClassAnalysis", "mean").toStdString());
            outDoubleImage(devImg, qApp->translate("ClassAnalysis", "standard deviation").toStdString());
        }
        catch(const char*e) {
            QMessageBox::critical(NULL, "Error", e);
            return;
        }
        fclose(f);
    }
    else {
        FILE* f = fopen(dialog->getFileName().toLatin1(), "rt" );
        if(f == NULL) {
            QMessageBox::critical(NULL, "Error", "Could not open file for read access");
            return;
        }
        try {
            string s = ClassAnalysis::print_file_info(f);
            outText(s);
            outText("-------------------------------------------");
        }
        catch(const char*e) {
            QMessageBox::critical(NULL, "Error", e);
            return;
        }
        fclose(f);
    }


}
