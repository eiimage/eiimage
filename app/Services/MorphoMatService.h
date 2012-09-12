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

#ifndef MORPHOMATSERVICE_H
#define MORPHOMATSERVICE_H

#include <Algorithm/MorphoMat.h>

#include <Services/AlgorithmService.h>
#include "../Widgets/StructElemWindow.h"

#include <QToolBar>

namespace genericinterface
{
    class MorphoMatService : public AlgorithmService
    {
      Q_OBJECT
      public:
        MorphoMatService(); 
        void display(GenericInterface* gi);
        void connect(GenericInterface* gi);
        imagein::MorphoMat::StructElem* getStructElem() { return _structElem; }
        void setStructElem(imagein::MorphoMat::StructElem* structElem) { _structElem = structElem; }

      public slots:
	    void editStructElem();
        void applyErosion();
	    void applyDilatation();
	    void applyGradient();
	    void applyOpening();
	    void applyClosing();
	    void applyWhiteTopHat();
	    void applyBlackTopHat();
        void checkActionsValid(const QWidget* activeWidget);

      private:
        imagein::MorphoMat::StructElem* _structElem;
        QAction* _editStructElem;
	    QAction* _erosion;
	    QAction* _dilatation;
	    QAction* _erosion2;
	    QAction* _dilatation2;
	    QAction* _opening;
	    QAction* _closing;
	    QAction* _gradient;
	    QAction* _wtophat;
	    QAction* _btophat;
        void applyOperator(MorphoMat::Operator<depth_default_t>& op);
        StructElemWindow* _structElemWindow;
    };
}

#endif
