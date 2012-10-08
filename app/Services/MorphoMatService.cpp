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

#include "MorphoMatService.h"

#include <GenericInterface.h>

#include <Algorithm/MorphoMat.h>
#include <Converter.h>
#include <typeinfo>

#include <QMessageBox>

using namespace genericinterface;
using namespace imagein::algorithm;

using namespace imagein::MorphoMat;

MorphoMatService::MorphoMatService() {

    bool elem[] = {
        false, true, false,
        true,  true, true,
        false, true, false
    };

    _structElem = new StructElem(GrayscaleImage_t<bool>(3, 3, elem), 1, 1);
    
}

void MorphoMatService::display(GenericInterface* gi)
{
  AlgorithmService::display(gi);
    

//  _editStructElem = _toolBar->addAction("&Structuring element");
//  _erosion = _toolBar->addAction("&Erosion");
//  _dilatation = _toolBar->addAction("&Dilatation");
  
  QMenu* menu = gi->menu("&Morpho. math.");

  _erosion2 = menu->addAction(tr("&Erosion"));
  _dilatation2 = menu->addAction(tr("&Dilatation"));
  _opening = menu->addAction(tr("&Opening"));
  _closing = menu->addAction(tr("&Closing"));
  _gradient = menu->addAction(tr("&Gradient"));
  _wtophat = menu->addAction(tr("&White top hat"));
  _btophat = menu->addAction(tr("&Black top hat"));
  menu->addSeparator();
  _editStructElem = menu->addAction(tr("&Structuring element"));

  
//  _erosion->setEnabled(false);
//  _dilatation->setEnabled(false);
  _gradient->setEnabled(false);
  _erosion2->setEnabled(false);
  _dilatation2->setEnabled(false);
  _opening->setEnabled(false);
  _closing->setEnabled(false);
  _gradient->setEnabled(false);
  _wtophat->setEnabled(false);
  _btophat->setEnabled(false);
    
//    QMdiArea* area = (QMdiArea*)gi->centralWidget();
//    _structElemWindow = new StructElemWindow(_structElem, _editStructElem);
//    area->addSubWindow(_structElemWindow);
//    _structElemWindow->setWindowTitle(tr("Editing structuring element"));
//    _structElemWindow->hide();
}

void MorphoMatService::connect(GenericInterface* gi)
{
    AlgorithmService::connect(gi);

    QObject::connect(_editStructElem, SIGNAL(triggered()), this, SLOT(editStructElem()));
//    QObject::connect(_erosion, SIGNAL(triggered()), this, SLOT(applyErosion()));
    QObject::connect(_erosion2, SIGNAL(triggered()), this, SLOT(applyErosion()));
//    QObject::connect(_dilatation, SIGNAL(triggered()), this, SLOT(applyDilatation()));
    QObject::connect(_dilatation2, SIGNAL(triggered()), this, SLOT(applyDilatation()));
    QObject::connect(_opening, SIGNAL(triggered()), this, SLOT(applyOpening()));
    QObject::connect(_closing, SIGNAL(triggered()), this, SLOT(applyClosing()));
    QObject::connect(_gradient, SIGNAL(triggered()), this, SLOT(applyGradient()));
    QObject::connect(_wtophat, SIGNAL(triggered()), this, SLOT(applyWhiteTopHat()));
    QObject::connect(_btophat, SIGNAL(triggered()), this, SLOT(applyBlackTopHat()));
	//connexion des changements d'images
    QObject::connect(_ws, SIGNAL(activeWidgetChanged(const QWidget*)), this, SLOT(checkActionsValid(const QWidget*)));
}

void MorphoMatService::checkActionsValid(const QWidget* activeWidget) {
    const StandardImageWindow* window = (activeWidget) ? dynamic_cast<const StandardImageWindow*>(activeWidget) : NULL;
	if(window) {
//      _erosion->setEnabled(true);
//      _dilatation->setEnabled(true);
      _gradient->setEnabled(true);
      _erosion2->setEnabled(true);
      _dilatation2->setEnabled(true);
      _opening->setEnabled(true);
      _closing->setEnabled(true);
      _gradient->setEnabled(true);
      _wtophat->setEnabled(true);
      _btophat->setEnabled(true);
    }
    else {
//      _erosion->setEnabled(false);
//      _dilatation->setEnabled(false);
      _gradient->setEnabled(false);
      _erosion2->setEnabled(false);
      _dilatation2->setEnabled(false);
      _opening->setEnabled(false);
      _closing->setEnabled(false);
      _gradient->setEnabled(false);
      _wtophat->setEnabled(false);
      _btophat->setEnabled(false);
    }
}


void MorphoMatService::editStructElem()
{
//    _editStructElem->setEnabled(false);
    QDialog* structElemWindow = new StructElemWindow(_structElem, _editStructElem);
    structElemWindow->exec();

    //WindowService* ws = dynamic_cast<WindowService*>(_gi->getService(GenericInterface::WINDOW_SERVICE));
    
    //StandardImageWindow* current_siw = dynamic_cast<StandardImageWindow*>(ws->getCurrentImageWindow());
    //ws->addWidget(ws->getWidgetId(current_siw), structElemWindow);
//    _structElemWindow->show();
}

void MorphoMatService::applyOperator(MorphoMat::Operator<depth_default_t>& op)
{
	 applyAlgorithm(&op);
}

void MorphoMatService::applyErosion()
{
    MorphoMat::Erosion<depth8_t> op(*_structElem);
    this->applyOperator(op);
}

void MorphoMatService::applyDilatation() {
    MorphoMat::Dilatation<depth8_t> op(*_structElem);
    this->applyOperator(op);
}

void MorphoMatService::applyGradient() {
    MorphoMat::Gradient<depth8_t> op(*_structElem);
    this->applyOperator(op);
}

void MorphoMatService::applyOpening() {
    MorphoMat::Opening<depth8_t> op(*_structElem);
    this->applyOperator(op);
}

void MorphoMatService::applyClosing() {
    MorphoMat::Closing<depth8_t> op(*_structElem);
    this->applyOperator(op);
}

void MorphoMatService::applyWhiteTopHat() {
    MorphoMat::WhiteTopHat<depth8_t> op(*_structElem);
    this->applyOperator(op);
}

void MorphoMatService::applyBlackTopHat() {
    MorphoMat::BlackTopHat<depth8_t> op(*_structElem);
    this->applyOperator(op);
}
