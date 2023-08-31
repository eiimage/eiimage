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
#include <QDebug>

using namespace genericinterface;
using namespace imagein::algorithm;

using namespace imagein::MorphoMat;

MorphoMatService::MorphoMatService() {

    //!Création de l'élément structurant de base (= une croix)
    bool elem[] = {
        false, true, false,
        true,  true, true,
        false, true, false
    };
    //! L'ES initial est une image binaire de taille 3*3 avec le motif dessiné ci-dessus et son centre en (1,1)
    _structElem = new StructElem(GrayscaleImage_t<bool>(3, 3, elem), 1, 1);
    
}

void MorphoMatService::display(GenericInterface* gi)
{
  AlgorithmService::display(gi);

  //! Ajout des différentes opérateurs dans le menu Morpho. math.
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

  //!Les opérateurs sont désactivés car il faut s'assurer qu'une image uchar est sélectionnée
  _gradient->setEnabled(false);
  _erosion2->setEnabled(false);
  _dilatation2->setEnabled(false);
  _opening->setEnabled(false);
  _closing->setEnabled(false);
  _gradient->setEnabled(false);
  _wtophat->setEnabled(false);
  _btophat->setEnabled(false);
}

void MorphoMatService::connect(GenericInterface* gi)
{
    AlgorithmService::connect(gi);

    //! Réalisation des connections sur chacun des éléments du menu Morpho. Math.
    QObject::connect(_editStructElem, SIGNAL(triggered()), this, SLOT(editStructElem()));
    QObject::connect(_erosion2, SIGNAL(triggered()), this, SLOT(applyErosion()));
    QObject::connect(_dilatation2, SIGNAL(triggered()), this, SLOT(applyDilatation()));
    QObject::connect(_opening, SIGNAL(triggered()), this, SLOT(applyOpening()));
    QObject::connect(_closing, SIGNAL(triggered()), this, SLOT(applyClosing()));
    QObject::connect(_gradient, SIGNAL(triggered()), this, SLOT(applyGradient()));
    QObject::connect(_wtophat, SIGNAL(triggered()), this, SLOT(applyWhiteTopHat()));
    QObject::connect(_btophat, SIGNAL(triggered()), this, SLOT(applyBlackTopHat()));
	//!connexion des changements d'images
    QObject::connect(_ws, SIGNAL(activeWidgetChanged(const QWidget*)), this, SLOT(checkActionsValid(const QWidget*)));
}

void MorphoMatService::checkActionsValid(const QWidget* activeWidget) {
    const StandardImageWindow* window = (activeWidget) ? dynamic_cast<const StandardImageWindow*>(activeWidget) : NULL;
    //! On active les boutons du menu Morpho. Math. ssi l'image sélectionnée est de type uchar
	if(window) {
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
    //! Création de la fenêtre d'édition de l'élément structurant
    QDialog* structElemWindow = new StructElemWindow(_structElem, _editStructElem);
    //! Connection Qt pour appliquer l'Op. Morph. sélectionné dans le sous menu "Structuring Element" lorsque l'on clique sur "OK"
    QObject::connect(structElemWindow, SIGNAL(sendOpMorph(OpMorpho)),this,SLOT(applyOpMorph(OpMorpho)));
    //! Affichage de la fenêtre liée au menu "Structuring Element"
    structElemWindow->exec();
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

void MorphoMatService::applyOpMorph(OpMorpho op){
    //! Switch case qui permet d'identifier quel opérateur a été sélectionné par l'utilisateur dans le menu déroulant
    switch(op){
        case ErosionOp:
        {
            applyErosion();
            break;
        }
        case DilatationOp:
        {
            applyDilatation();
            break;
        }
        case OpeningOp:
        {
            applyOpening();
            break;
        }
        case ClosingOp:
        {
            applyClosing();
            break;
        }
        case GradientOp:
        {
            applyGradient();
            break;
        }
        case WtophatOp:
        {
            applyWhiteTopHat();
            break;
        }
        case BtophatOp:
        {
            applyBlackTopHat();
            break;
        }
        default:
            break;
    }
}
