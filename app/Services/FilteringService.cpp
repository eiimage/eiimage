/*
 * Copyright 2011-2012 Benoit Averty, Samuel Babin, Matthieu Bergere, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
 *
 * This file is part of DETIQ-T.
 *
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FilteringService.h"

#include <QMdiArea>
#include <QString>
#include <QObject>

#include <GenericInterface.h>
#include <Converter.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <QApplication>
#include <Operation.h>

using namespace filtrme;
using namespace genericinterface;
using namespace imagein::algorithm;

void FilteringService::display(GenericInterface* gi)
{
    AlgorithmService::display(gi);
    QMenu* menu = gi->menu(qApp->translate("", "Filtering"));
    _filtering = menu->addAction(tr("&Apply filter"));
    _filtering->setEnabled(false);
    _filterEdition = menu->addAction(tr("&Edit filters"));
}

void FilteringService::checkActionsValid(const QWidget* activeWidget) {
    const ImageWindow* window = (activeWidget) ? dynamic_cast<const ImageWindow*>(activeWidget) : NULL;
    _filtering->setEnabled(window != NULL);
}

void FilteringService::connect(GenericInterface* gi)
{
    AlgorithmService::connect(gi);
    QObject::connect(_filtering, SIGNAL(triggered()), this, SLOT(applyFiltering()));
    QObject::connect(_filterEdition, SIGNAL(triggered()), this, SLOT(edition()));
    QObject::connect(gi->windowService(), SIGNAL(activeWidgetChanged(const QWidget*)), this, SLOT(checkActionsValid(const QWidget*)));
}

void FilteringService::applyFiltering()
{
    ImageWindow* siw = dynamic_cast<ImageWindow*>(_ws->getCurrentImageWindow());
    if (siw != NULL)
    {
        _siw = siw;

        _filterChoice = new FilterChoice(_gi);
        _filterChoice->setDoubleResult(siw->isDouble());
        QDialog::DialogCode code = static_cast<QDialog::DialogCode>(_filterChoice->exec());

        if(code!=QDialog::Accepted) {
            return;
        }
        _dblResult = _filterChoice->doubleResult();
        _scaling = _filterChoice->scalingResult();
        _offset = _filterChoice->offsetResult();

        Filtering* filtering = _filterChoice->getFiltering();
        this->applyAlgorithm(filtering);

    }
}

void FilteringService::edition()
{
    _filterEditor = new FilterEditor();
    _filterEditor->exec();
}

void FilteringService::applyAlgorithm(Filtering* algo)
{
    if (_siw != NULL)
    {
        const Image_t<double>* image;
        if(_siw->isStandard()) {
            const Image* whole_image = _siw->getDisplayImage();
            const Image* im = whole_image->crop(_siw->selection());
            image = Converter<Image_t<double> >::convert(*im);
            delete im;
        }
        else if(_siw->isDouble()) {
           DoubleImageWindow* diw = dynamic_cast<DoubleImageWindow*>(_siw);
           image = diw->getImage();
        }

        Image_t<double>* dblResImg = (*algo)(image);
        unsigned int i = 0;
        ImageWindow* riw;
        if(_siw->isStandard()) {
            delete image;
        }
        do{
            if(_dblResult) {
                DoubleImageWindow* diw = dynamic_cast<DoubleImageWindow*>(_siw);
                if(diw != NULL) {
                    riw = new DoubleImageWindow(dblResImg, _siw->getPath(), AUTO,AUTO , diw->isLogScaled());
                    /*Enable the output of description text concerning the display imgage processing options that the user can choose*/
                    QObject::connect(riw, SIGNAL(textToShow(QString)), this->_ws, SLOT(outputText(QString)));
                    if(dblResImg->min()<0){
                        std::string outputMessage = QObject::tr("Both Offset and Scaling applied "
                                                                "<br><br> pixel display = 127 +  pixel image * 127/Max(|minValue|,|minValue|)"
                                                                "<br><br> -------------------------------------------").toStdString();
                        _ws->addText(outputMessage);
                    }
                }
                else {
                    riw = new DoubleImageWindow(dblResImg, _siw->getPath(), AUTO,AUTO);
                    /*Enable the output of description text concerning the display imgage processing options that the user can choose*/
                    QObject::connect(riw, SIGNAL(textToShow(QString)), this->_ws, SLOT(outputText(QString)));
                    if(dblResImg->min()<0){
                        std::string outputMessage = QObject::tr("Both Offset and Scaling applied "
                                                                "<br><br> pixel display = 127 +  pixel image * 127/Max(|minValue|,|minValue|)"
                                                                "<br><br> -------------------------------------------").toStdString();
                        _ws->addText(outputMessage);
                    }
                }
            }
            else {
                std::string outputMessage = "";
                Image* resImg;
                Image_t<int>* intResImg = Converter<Image_t<int> >::convert(*dblResImg);
                if(_scaling && _offset ){
                    resImg  =  Converter<Image>::convertScaleAndOffset(*intResImg, &outputMessage);
                }
                else if(_scaling){
                    resImg  =  Converter<Image>::convertAndScale(*intResImg, &outputMessage);
                }
                else if(_offset){
                    resImg = Converter<Image>::convertAndOffset(*intResImg, &outputMessage);
                }
                else{
                    resImg = Converter<Image>::convertAndRound(*dblResImg);
                    outputMessage = qApp->translate("FilteringService","Pas de conversion [min : 0, max : 255]").toStdString();

                }
                riw = new StandardImageWindow(resImg, _siw->getPath());
                _ws->addText(outputMessage);
                _ws->addText("-------------------------------------------");
            }

            if(i == 0) riw->setWindowTitle(_siw->windowTitle() + " - " + _filterChoice->getFilterName() + qApp->translate("FilteringService"," Result ").toUtf8());
            else riw->setWindowTitle(_siw->windowTitle() + " - " + _filterChoice->getFilterName() + " - " + QString::number(i).toUtf8() );

            emit newImageWindowCreated(_ws->getNodeId(_siw), riw);
            i++;
            dblResImg = algo->getInterImg();
        }while(dblResImg != NULL);
        if(i >1){
            _ws->addText(tr("L'image résultat la racine des sommes des carrés").toStdString());
            _ws->addText("-------------------------------------------");
        }
    }
}
