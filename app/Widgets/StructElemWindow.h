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

#ifndef STRUCTELEMWINDOW_H 
#define STRUCTELEMWINDOW_H

#include <Algorithm/MorphoMat.h>
#include <Algorithm/Binarization.h>
#include <Algorithm/Otsu.h>
#include <Converter.h>

#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <Widgets/ImageWidgets/HistogramWindow.h>
#include <Widgets/ImageWidgets/GridView.h>

#include "StructElemViewer.h"

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>

enum OpMorpho {EditES ,ErosionOp, DilatationOp, OpeningOp, ClosingOp, GradientOp, WtophatOp, BtophatOp};

class StructElemWindow : public QDialog
{
    Q_OBJECT
  public:
    StructElemWindow(imagein::MorphoMat::StructElem*& elemi, QAction* button);
    void changeStructElem(imagein::MorphoMat::StructElem* elem);
    inline int getShapeSizeGenerate() const{ return _shapeToGenvalue;};

  public slots:
    void openFile();
    void saveFile();
    void ok();
    void resize(int size);
    void generate();

public slots:
    void dilateLeft();
    void dilateTopLeft();
    void dilateTop();
    void dilateTopRight();
    void dilateRight();
    void dilateBottomRight();
    void dilateBottom();
    void dilateBottomLeft();



    //! _shapeToGenvalue mis à jour avec le contenu de la box _shapeSize
    inline void setShapeSizeScaling(){ _shapeToGenvalue = _shapeSize->value();};
    inline void setShapeSizeScaling(int shapeToGenvalue){ _shapeToGenvalue = shapeToGenvalue;};

    signals:
    void sendOpMorph(OpMorpho);

  protected:
//    void closeEvent ( QCloseEvent * event );

  private:
    QWidget* _widget;
    QToolButton* _openFileButton;
    QToolButton* _saveFileButton;
    QAction* _ok;
    genericinterface::HistogramWindow* _histo;
    QLabel* _labelThreshold;
    imagein::MorphoMat::StructElem* _structElem;
    imagein::MorphoMat::StructElem*& _serviceStructElem;
    QGraphicsView* _view;
    StructElemViewer *_viewer;
    imagein::GrayscaleImage_t<bool> _realSize;
    QAction* _tbButton;
    QPushButton* _genButton; 
    QComboBox* _shapeToGen;
    QComboBox* _opMorphSelection;
    QSpinBox* _shapeSize;
    QSpinBox* _scale;
    int _shapeToGenvalue; //! Equivalent de _shapeToGen.value mais mis à jour uniquement à l'appuis sur le bouton "generate"
};

#endif
