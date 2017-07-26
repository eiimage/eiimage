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

#include <GenericInterface.h>
#include <Converter.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <QApplication>

using namespace filtrme;
using namespace genericinterface;
using namespace imagein::algorithm;

void FilteringService::display(GenericInterface* gi)
{
    AlgorithmService::display(gi);

//    _filtering = _toolBar->addAction("&Filtering");
//    _filterEdition = _toolBar->addAction("&FilterEditor");
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
//        QMdiArea* area = (QMdiArea*)_gi->centralWidget();
//        area->addSubWindow(_filterChoice);
        QDialog::DialogCode code = static_cast<QDialog::DialogCode>(_filterChoice->exec());

        if(code!=QDialog::Accepted) {
            return;
        }
        _dblResult = _filterChoice->doubleResult();

        Filtering* filtering = _filterChoice->getFiltering();
        this->applyAlgorithm(filtering);

    }
}

void FilteringService::edition()
{  
    _filterEditor = new FilterEditor();
    _filterEditor->exec();
    //QMdiArea* area = (QMdiArea*)_gi->centralWidget();
    //area->addSubWindow(_filterEditor);
//    StandardImageWindow* siw = dynamic_cast<StandardImageWindow*>(_ws->getCurrentImageWindow());
//    _ws->addWidget(_ws->getNodeId(siw), _filterEditor);
    //_filterEditor->show();

}

void FilteringService::applyAlgorithm(Filtering* algo)
{
    //StandardImageWindow* siw = dynamic_cast<StandardImageWindow*>(_ws->getCurrentImageWindow());
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
        ImageWindow* riw;
        if(_siw->isStandard()) {
            delete image;
        }
        if(_dblResult) {
            DoubleImageWindow* diw = dynamic_cast<DoubleImageWindow*>(_siw);
            if(diw != NULL) {
                riw = new DoubleImageWindow(dblResImg, _siw->getPath(), diw->isNormalized(), diw->isLogScaled());
            }
            else {
                riw = new DoubleImageWindow(dblResImg, _siw->getPath(), true);
            }
        }
        else {
            Image_t<int>* intResImg = Converter<Image_t<int> >::convert(*dblResImg);
            delete dblResImg;
            Image* resImg = Converter<Image>::makeDisplayable(*intResImg);
            delete intResImg;
            riw = new StandardImageWindow(resImg, _siw->getPath());
        }
        riw->setWindowTitle(_siw->windowTitle());
        emit newImageWindowCreated(_ws->getNodeId(_siw), riw);
    }
}
