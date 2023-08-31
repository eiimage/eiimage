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
#include "Services/MorphoMatService.h"


#include <Image.h>
#include <QDialogButtonBox>
#include <QGroupBox>

using namespace genericinterface;
using namespace imagein;
using namespace imagein::algorithm;
using namespace imagein::MorphoMat;

StructElemWindow::StructElemWindow(StructElem*& elem, QAction* tbButton) : _structElem(elem), _serviceStructElem(elem), _tbButton(tbButton)
{
    //! Taille de la fenêtre d'édition de l'élément structurant
    setMinimumSize(600,400);
    _realSize = *elem;

    auto* layout = new QVBoxLayout();

    //! sélection de la forme de l'élément  à générer
    auto* layout1 = new QHBoxLayout();
    layout->addLayout(layout1);
    layout1->addWidget(new QLabel(tr("Basic shapes :")));
    _shapeToGen = new QComboBox;
    _shapeToGen->setEditable(false);
    _shapeToGen->insertItem(0, tr("Diamond"));
    _shapeToGen->insertItem(1, tr("Disc"));
    _shapeToGen->insertItem(2, tr("Empty"));
    layout1->addWidget(_shapeToGen);

    //! menu déroulant sélection op. morph.
    auto* layout2 = new QHBoxLayout();
    layout->addLayout(layout2);
    layout2->addWidget(new QLabel(tr("Opération :")));
    _opMorphSelection = new QComboBox;
    _opMorphSelection->setEditable(false);
    _opMorphSelection->insertItem(EditES, tr("Edit structuring element"));
    _opMorphSelection->insertItem(ErosionOp, tr("Erosion"));
    _opMorphSelection->insertItem(DilatationOp, tr("Dilatation"));
    _opMorphSelection->insertItem(OpeningOp, tr("Opening"));
    _opMorphSelection->insertItem(ClosingOp, tr("Closing"));
    _opMorphSelection->insertItem(GradientOp, tr("Gradient"));
    _opMorphSelection->insertItem(WtophatOp, tr("White top hat"));
    _opMorphSelection->insertItem(BtophatOp, tr("Black top hat"));
    layout2->addWidget(_opMorphSelection);

    //! sélection de la taille de l'élément structurant à générer
    _shapeSize = new QSpinBox();
    _shapeSize->setRange(1,64);
    _shapeSize->setSingleStep(1);
    _shapeSize->setValue(_structElem->getShapeSizeMemory());
    layout1->addWidget(_shapeSize);
    _shapeToGenvalue = _structElem->getShapeSizeMemory();

    //! générer un nouvel élément structurant à partir de la forme et la taille sélectionnée
    _genButton = new QPushButton(tr("Generate"));
    layout1->addWidget(_genButton);

    auto* structElemLayout = new QHBoxLayout();

    //! Affichage de l'élément structurant
    _viewer = new StructElemViewer(_structElem, true);
    _view = new QGraphicsView;
    _view->setScene(_viewer);
    _viewer->draw(0,0);

// Création des outils de modification de l'ES (boutons fléchés)
    auto* rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->setAlignment(Qt::AlignTop);  // Alignement en haut

    auto* dilatButtonWidget = new QWidget();
    dilatButtonWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    auto* dilatButtonLayout = new QGridLayout(dilatButtonWidget);

    rightPanelLayout->setAlignment(Qt::AlignTop);  // Alignement en haut

    dilatButtonWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    auto* dilatLeftButton = new QPushButton(QIcon(":/img/arrow-left.png"), "");
    auto* dilatTopLeftButton = new QPushButton(QIcon(":/img/arrow-top-left.png"), "");
    auto* dilatTopButton = new QPushButton(QIcon(":/img/arrow-top.png"), "");
    auto* dilatTopRightButton = new QPushButton(QIcon(":/img/arrow-top-right.png"), "");
    auto* dilatRightButton = new QPushButton(QIcon(":/img/arrow-right.png"), "");
    auto* dilatBottomRightButton = new QPushButton(QIcon(":/img/arrow-bottom-right.png"), "");
    auto* dilatBottomButton = new QPushButton(QIcon(":/img/arrow-bottom.png"), "");
    auto* dilatBottomLeftButton = new QPushButton(QIcon(":/img/arrow-bottom-left.png"), "");
    auto* dilatCenterButton = new QPushButton("");
    dilatLeftButton->setFixedSize(32, 32);
    dilatTopLeftButton->setFixedSize(32, 32);
    dilatTopButton->setFixedSize(32, 32);
    dilatTopRightButton->setFixedSize(32, 32);
    dilatRightButton->setFixedSize(32, 32);
    dilatBottomRightButton->setFixedSize(32, 32);
    dilatBottomButton->setFixedSize(32, 32);
    dilatBottomLeftButton->setFixedSize(32, 32);
    dilatCenterButton->setFixedSize(32, 32);
    dilatRightButton->setEnabled(false);
    dilatBottomRightButton->setEnabled(false);
    dilatBottomButton->setEnabled(false);
    dilatBottomLeftButton->setEnabled(false);
    dilatCenterButton->setEnabled(false);
    dilatButtonLayout->addWidget(dilatLeftButton, 1, 0);
    dilatButtonLayout->addWidget(dilatTopLeftButton, 0, 0);
    dilatButtonLayout->addWidget(dilatTopButton, 0, 1);
    dilatButtonLayout->addWidget(dilatTopRightButton, 0, 2);
    dilatButtonLayout->addWidget(dilatRightButton, 1, 2);
    dilatButtonLayout->addWidget(dilatBottomRightButton, 2, 2);
    dilatButtonLayout->addWidget(dilatBottomButton, 2, 1);
    dilatButtonLayout->addWidget(dilatBottomLeftButton, 2, 0);
    dilatButtonLayout->addWidget(dilatCenterButton, 1, 1);
    rightPanelLayout->addWidget(dilatButtonWidget);

    rightPanelLayout->addWidget(dilatButtonWidget);

// Ajoutez _view et le widget contenant les boutons à la vue structElemBox
    auto* structElemBox = new QGroupBox(tr("Structuring element"));
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(_view);
    hLayout->addLayout(rightPanelLayout);
    structElemBox->setLayout(hLayout);

    layout->addWidget(structElemBox);
//////
//!bouton ouverture d'un ES sauvegardé
    _openFileButton = new QToolButton();
    _openFileButton->setToolTip(tr("Open file"));
    _openFileButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
    _openFileButton->setCheckable(true);
    _openFileButton->setIconSize (QSize(18, 18));

//!bouton sauvegarde d'un ES
    _saveFileButton = new QToolButton();
    _saveFileButton->setToolTip(tr("Save as..."));
    _saveFileButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    _saveFileButton->setCheckable(true);
    _saveFileButton->setIconSize (QSize(18, 18));

    //! Création layout horizontaux pour les boutons
    auto* layout3 = new QHBoxLayout();
    auto* hlayout = new QHBoxLayout();
    layout3->addStretch(); // Ajouter un espace flexible pour pousser les boutons vers la droite
    layout3->addWidget(_openFileButton);
    layout3->addWidget(_saveFileButton);

    hlayout->addLayout(layout3); // Ajouter le layout vertical au layout horizontal (ie en dessous de l'ES)
    layout->addLayout(hlayout); //Ajouter le layout horizontal au layout principal
    this->setLayout(layout);

    auto* formLayout = new QFormLayout();
    layout->addLayout(formLayout);

    //! modification de l'échelle de l'élément structurant
    auto* layout4 = new QHBoxLayout();
    layout->addLayout(layout4);
    layout4->addWidget(new QLabel(tr("Scale :")));
    _scale = new QSpinBox();
    _scale->setRange(1,32);
    _scale->setSingleStep(1);
    _scale->setSuffix("");
    _scale->setValue(_structElem->getScaleMemory());
    layout4->addWidget(_scale);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->addWidget(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    QObject::connect(buttonBox, SIGNAL(rejected()),     this, SLOT(reject()));


    QObject::connect(_openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(_saveFileButton, SIGNAL(clicked()), this, SLOT(saveFile()));
    QObject::connect(_scale, SIGNAL(valueChanged(int)), this, SLOT(resize(int)));
    QObject::connect(_genButton, SIGNAL(clicked(bool)), this, SLOT(generate()));

    //! Connections Qt pour la modification de l'ES
    connect(dilatLeftButton, SIGNAL(clicked()), this, SLOT(dilateLeft()));
    connect(dilatTopLeftButton, SIGNAL(clicked()), this, SLOT(dilateTopLeft()));
    connect(dilatTopButton, SIGNAL(clicked()), this, SLOT(dilateTop()));
    connect(dilatTopRightButton, SIGNAL(clicked()), this, SLOT(dilateTopRight()));
    connect(dilatRightButton, SIGNAL(clicked()), this, SLOT(dilateRight()));
    connect(dilatBottomRightButton, SIGNAL(clicked()), this, SLOT(dilateBottomRight()));
    connect(dilatBottomButton, SIGNAL(clicked()), this, SLOT(dilateBottom()));
    connect(dilatBottomLeftButton, SIGNAL(clicked()), this, SLOT(dilateBottomLeft()));

/*    //! Connections Qt pour désactiver des directions de dilatation de l'ES
    connect(centerBox, SIGNAL(toggled(bool)), this, SLOT(setCenter(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatRightButton, SLOT(setDisabled(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatBottomRightButton, SLOT(setDisabled(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatBottomButton, SLOT(setDisabled(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatBottomLeftButton, SLOT(setDisabled(bool)));*/
}

void StructElemWindow::ok() {
    _serviceStructElem = _structElem;
    //! Envoi du signal pour l'application de l'Op. Morpho. sur l'image
    emit sendOpMorph(OpMorpho(_opMorphSelection->currentIndex()));
    _structElem->setScaleMemory(_scale->value());
    _structElem->setShapeSizeMemory(_shapeToGenvalue);
    this->accept();
}

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
    //! Récupération de la taille de la forme à générer (choix de l'utilisateur)
    unsigned int size = _shapeSize->value();
    //! Création d'une nouvelle image en niveau de gris avec les nouvelles dimensions de l'ES
    GrayscaleImage_t<bool> elem(size, size);

    //! Initialisation de l'image de l'ES avec des 0 partout
    for(unsigned int j = 0; j < elem.getHeight(); ++j) {
        for(unsigned int i = 0; i < elem.getWidth(); ++i) {
            elem.setPixel(i, j, false);
        }
    }

    if(_shapeToGen->currentText()==tr("Disc")) {
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
    else if(_shapeToGen->currentText()==tr("Diamond")) {
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

    //! Création de l'ES à partir de l'image de celui-ci et de la position de son centre
    StructElem* structElem = new StructElem(elem, elem.getWidth()/2, elem.getHeight()/2);
    //! Reset du facteur d'échelle
    structElem->setScale(1);
    _scale->setValue(1);
    //! Sauvegarde du facteur d'échelle et de la taille de l'ES demandée
    _structElem->setScaleMemory(1);
    _structElem->setShapeSizeMemory(_shapeSize->value());
    //! Remplacement de l'ES par celui que l'on vient de générer
    changeStructElem(structElem);
    //! _shapeToGenvalue mis à jour avec le contenu de la box _shapeSize
    _shapeToGenvalue = _shapeSize->value();
}

void StructElemWindow::resize(int size) {
    //! On propage avec le plus proche voisin (imparfait pour les nombres pairs)
    GrayscaleImage_t<bool> elem(_shapeToGenvalue * size, _shapeToGenvalue * size);
    for(unsigned int j = 0; j < elem.getHeight(); ++j) {
        for(unsigned int i = 0; i < elem.getWidth(); ++i) {
            //! Calcul des coordonnées correspondantes dans l'image source
            int srcX = floor(i / (double)elem.getWidth() * _structElem->getWidth());
            int srcY = floor(j / (double)elem.getHeight() * _structElem->getHeight());

            //! Récupération de la valeur du pixel de l'image source (plus proche voisin)
            bool pixelValue = _structElem->getPixel(srcX, srcY);
            elem.setPixel(i, j, pixelValue);

        }
    }
    auto* structElem = new StructElem(elem, elem.getWidth() / 2, elem.getHeight() / 2);
    changeStructElem(structElem);
}

void StructElemWindow::changeStructElem(imagein::MorphoMat::StructElem* elem) {
    //! changement d'ES
    _structElem = elem;
    StructElemViewer* newViewer = new StructElemViewer(_structElem,true);
    _view->setScene(newViewer);
    delete _viewer;
    _viewer = newViewer;
    _viewer->draw(0,0);
}

void StructElemWindow::openFile() {
    QString file = QFileDialog::getOpenFileName(this, tr("Open a file"), QString(), tr("Images (*.png *.bmp *.jpg *.jpeg)"));
    if(file.size()==0) return;

    auto* structElem = new StructElem(file.toStdString());
    changeStructElem(structElem);
    _realSize = *structElem;
    _scale->setValue(1);
    setShapeSizeScaling(structElem->getWidth());
}

void StructElemWindow::saveFile() {
    QString file = QFileDialog::getSaveFileName(this, tr("Save file"), QString(), tr("Images (*.png *.bmp *.jpg *.jpeg)"));

    if(file.size()==0) return;

    /*Sur linux, l'extension ne se met pas toute seule comme sur windows*/
    QString extension = ".bmp";
    if (!file.endsWith(extension, Qt::CaseInsensitive)) {
        file += extension;
    }
    _structElem->save(file.toStdString());
}


void StructElemWindow::dilateLeft() {
    StructElem::Dir dir;
    dir = StructElem::Left;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}

void StructElemWindow::dilateTopLeft() {
    StructElem::Dir dir;
    dir = StructElem::BottomRight;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}

void StructElemWindow::dilateTop() {
    StructElem::Dir dir;
    dir = StructElem::Bottom;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}

void StructElemWindow::dilateTopRight() {
    StructElem::Dir dir;
    dir = StructElem::BottomLeft;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}

void StructElemWindow::dilateRight() {
    StructElem::Dir dir;
    dir = StructElem::Left;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}

void StructElemWindow::dilateBottomRight() {
    StructElem::Dir dir;
    dir = StructElem::TopLeft;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}
void StructElemWindow::dilateBottom() {
    StructElem::Dir dir;
    dir = StructElem::Top;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}
void StructElemWindow::dilateBottomLeft() {
    StructElem::Dir dir;
    dir = StructElem::TopRight;

    _structElem->dilate(dir);
    changeStructElem(_structElem);
}