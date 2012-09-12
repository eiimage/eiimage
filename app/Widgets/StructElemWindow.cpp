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

#include "StructElemWindow.h"
#include "StructElemViewer.h"


#include <Image.h>
#include <QDialogButtonBox>

using namespace genericinterface;
using namespace imagein;
using namespace imagein::algorithm;
using namespace imagein::MorphoMat;

StructElemWindow::StructElemWindow(StructElem*& elem, QAction* tbButton) : _structElem(elem), _serviceStructElem(elem), _tbButton(tbButton)
{
    _realSize = *elem;
    
//    _widget = new QWidget;
//    this->setWidget(_widget);

    QVBoxLayout* layout = new QVBoxLayout();
    _toolBar = new QToolBar;
    _openFile = _toolBar->addAction("&Open file");
    _saveFile = _toolBar->addAction("&Save as...");
    layout->addWidget(_toolBar);
    this->setLayout(layout);

    
    
    QFormLayout* formLayout = new QFormLayout();
    layout->addLayout(formLayout);
    
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout->addLayout(layout2);
    layout2->addWidget(new QLabel("Basic shapes :"));
    _shapeToGen = new QComboBox;
    _shapeToGen->setEditable(false);
    _shapeToGen->insertItem(0, "Diamond");
    _shapeToGen->insertItem(1, "Disc");
    _shapeToGen->insertItem(2, "Empty");
    layout2->addWidget(_shapeToGen);
    _shapeSize = new QSpinBox();
    _shapeSize->setRange(1,64);
    _shapeSize->setSingleStep(1);
    _shapeSize->setValue(3);
    layout2->addWidget(_shapeSize);
    _genButton = new QPushButton("Generate");
    layout2->addWidget(_genButton);
    
    _viewer = new StructElemViewer(_structElem);
    _view = new QGraphicsView;
    _view->setScene(_viewer);
    layout->addWidget(_view);
    _viewer->draw(0,0);

    QHBoxLayout* layout3 = new QHBoxLayout();
    layout->addLayout(layout3);
    layout3->addWidget(new QLabel("Scale :"));
    QSpinBox* spinBox = new QSpinBox();
    spinBox->setRange(1,32);
    spinBox->setSingleStep(1);
    spinBox->setSuffix("");
    spinBox->setValue(_structElem->getScale());
    layout3->addWidget(spinBox);
    
//    QPushButton* button = new QPushButton("OK");
//    layout->addWidget(button);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->addWidget(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

//    QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(ok()));
    QObject::connect(_openFile, SIGNAL(triggered()), this, SLOT(openFile()));
    QObject::connect(_saveFile, SIGNAL(triggered()), this, SLOT(saveFile()));
    QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(resize(int)));
    QObject::connect(_genButton, SIGNAL(clicked(bool)), this, SLOT(generate()));
}

void StructElemWindow::ok() {
    _serviceStructElem = _structElem;
//    _tbButton->setEnabled(true);
    //this->close();
//    this->hide();
    this->accept();
}

//void StructElemWindow::closeEvent ( QCloseEvent * event) {
//    _tbButton->setEnabled(true);
//    event->ignore();
//    this->hide();

//}

void rasterCircle(GrayscaleImage_t<bool>& img, int x0, int y0, int radius)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  img.setPixel(x0, y0 + radius, true);
  img.setPixel(x0, y0 - radius, true);
  img.setPixel(x0 + radius, y0, true);
  img.setPixel(x0 - radius, y0, true);
 
  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    img.setPixel(x0 + x, y0 + y, true);
    img.setPixel(x0 - x, y0 + y, true);
    img.setPixel(x0 + x, y0 - y, true);
    img.setPixel(x0 - x, y0 - y, true);
    img.setPixel(x0 + y, y0 + x, true);
    img.setPixel(x0 - y, y0 + x, true);
    img.setPixel(x0 + y, y0 - x, true);
    img.setPixel(x0 - y, y0 - x, true);
  }
}

void closing(GrayscaleImage_t<bool>& img) {
    for(unsigned int j = 1; j < img.getHeight()-1; ++j) {
        bool last = false, swap = false;
        for(unsigned int i = 0; i < img.getWidth(); ++i) {
            const bool st = img.getPixel(i,j);
            const bool down = last && !st;
            if(down) swap = !swap;
            img.setPixel(i, j, st || swap);
            last  = st;
        }
    }
}

void StructElemWindow::generate() {
    unsigned int size = _shapeSize->value();
    GrayscaleImage_t<bool> elem(size, size);
    //if(size%2==0) --size;

    for(unsigned int j = 0; j < elem.getHeight(); ++j) {
        for(unsigned int i = 0; i < elem.getWidth(); ++i) {
            elem.setPixel(i, j, false);
        }
    }

    if(_shapeToGen->currentText()=="Disc") {
        const float radius = (size)/2.0f;
        const float center = radius;
        for(unsigned int j = 0; j < elem.getHeight(); ++j) {
            for(unsigned int i = 0; i < elem.getWidth(); ++i) {
                const float dx = i+0.5f-center;
                const float dy = j+0.5f-center;
                const float error = (dx*dx+dy*dy)/(radius*radius);


                if(error < (1.02f*1.02f)) {
                    elem.setPixel(i, j, true);
                }
                else {
                    elem.setPixel(i, j, false);
                }
            }
        }
        //rasterCircle(elem, size/2, size/2, size/2);    
    }
    else if(_shapeToGen->currentText()=="Diamond") {
        const float radius = (size)/2.0f;
        const float center = radius;
        for(unsigned int j = 0; j < elem.getHeight(); ++j) {
            for(unsigned int i = 0; i < elem.getWidth(); ++i) {
                const float dx = i+0.5f-center;
                const float dy = j+0.5f-center;
                const float error = (std::abs(dx)+std::abs(dy))/radius;


                if(error < (1.01f*1.01f)) {
                    elem.setPixel(i, j, true);
                }
                else {
                    elem.setPixel(i, j, false);
                }
            }
        }
        
    }


    //closing(elem);
    
    StructElem* structElem = new StructElem(elem, elem.getWidth()/2, elem.getHeight()/2);
    changeStructElem(structElem);
}

void StructElemWindow::resize(int size) {
    /*unsigned int size = std::abs(size_);
    GrayscaleImage_t<bool> elem(_realSize.getWidth()*size, _realSize.getHeight()*size);
    for(unsigned int j = 0; j < elem.getHeight(); ++j) {
        for(unsigned int i = 0; i < elem.getWidth(); ++i) {
            elem.setPixel(i, j, _realSize.getPixel(i/size, j/size));
        }
    }*/

    //StructElem<depth_default_t>* structElem = new StructElem<depth_default_t>(elem, elem.getWidth()/2, elem.getHeight()/2);
    _structElem->setScale(std::abs(size));
    changeStructElem(_structElem);
}

void StructElemWindow::changeStructElem(imagein::MorphoMat::StructElem* elem) {
    _structElem = elem;
    StructElemViewer* newViewer = new StructElemViewer(_structElem);
    _view->setScene(newViewer);
    delete _viewer;
    _viewer = newViewer;
    _viewer->draw(0,0);
}

void StructElemWindow::openFile() {
    QString file = QFileDialog::getOpenFileName(this, "Open a file", QString(), "Images (*.png *.bmp *.jpg *.jpeg)");
    if(file.size()==0) return;
    //Image image(file.toStdString());
    //Otsu algo;
    //GrayscaleImage* im_tmp = Converter<GrayscaleImage>::convert(image);
    //GrayscaleImage* im_res = algo(im_tmp);
    //GrayscaleImage_t<bool> elem(im_res->getWidth(), im_res->getHeight());
    //for(unsigned int j = 0; j < elem.getHeight(); ++j) {
        //for(unsigned int i = 0; i < elem.getWidth(); ++i) {
            //elem.setPixel(i, j, (im_res->getPixel(i, j) <= 0));
        //}
    //}

    //StructElem<depth_default_t>* structElem = new StructElem<depth_default_t>(elem, elem.getWidth()/2, elem.getHeight()/2);
    StructElem* structElem = new StructElem(file.toStdString());
    changeStructElem(structElem);
    _realSize = *structElem;
    
}

void StructElemWindow::saveFile() {
    QString file = QFileDialog::getSaveFileName(this, "Save file", QString(), "Images (*.png *.bmp *.jpg *.jpeg)");

    if(file.size()==0) return;
    //GrayscaleImage img(_structElem->getWidth(), _structElem->getHeight());
    //for(unsigned int j = 0; j < img.getHeight(); ++j) {
        //for(unsigned int i = 0; i < img.getWidth(); ++i) {
            //img.setPixel(i, j, _structElem->getPixel(i, j) ? 0 : 255);
        //}
    //}
    //img.save(file.toStdString());
    _structElem->save(file.toStdString());

    
}
