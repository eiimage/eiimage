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
#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>



#include "DMMDialog.h"

using namespace std;
using namespace imagein;
using namespace imagein::algorithm;
using namespace imagein::MorphoMat;

DMMDialog::DMMDialog(QWidget *parent) :
    QDialog(parent), _centerX(0), _centerY(0)
{
    QStringList opList;
    opList << tr("Erosion") << tr("Dilatation") << tr("Opening") << tr("Closing") << tr("DMM");

    //! Création de la fenêtre générée lors de la sélection de la fontionnalité "DMM"
    this->setWindowTitle(tr("DMM"));
    this->setMinimumWidth(512);
    this->setMinimumHeight(512);
    auto* layout = new QVBoxLayout();
    this->setLayout(layout);

    //! Création de la liste déroulante des opérations morphologiques disponibles
    auto* box1layout = new QHBoxLayout();
    _operatorBox = new QComboBox();
    _operatorBox->insertItems(0, opList);
    box1layout->addWidget(_operatorBox);
    _operatorBox->setCurrentIndex(Dmm);
    layout->addLayout(box1layout);

    //! Création du bouton DMM new level
    _newLevelButton = new QPushButton("DMM new level");
    _newLevelButton->setEnabled(false);
    layout->addWidget(_newLevelButton);

    auto* structElemBox = new QGroupBox(tr("Structuring element"));
    auto* structElemLayout = new QHBoxLayout();

    //! création d'un QGraphicsView : _structElemView, qui va contenir l'affichage de l'ES : _structElemViewer
    _views.push_back(StructElemViewer::ElemView(new StructElem(GrayscaleImage_t<bool>(1,1,new bool(true)), 0, 0), 0, 0));
    _structElemViewer = new StructElemViewer(_views.back().elem,false);
    _views.push_back(StructElemViewer::ElemView(new StructElem(GrayscaleImage_t<bool>(1,1,new bool(true)), 0, 0), 0, 0));
    _structElemViewer = new StructElemViewer(_views.back().elem, false);
    _structElemView = new QGraphicsView();
    _structElemView->setScene(_structElemViewer);
    structElemLayout->addWidget(_structElemView);
    _structElemViewer->draw(0, 0);

    //! Création des outils de modification de l'ES (boutons fléchés)
    auto* rightPanelLayout = new QVBoxLayout();
    auto* dilatButtonWidget = new QWidget();
    dilatButtonWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    auto* dilatButtonLayout = new QGridLayout(dilatButtonWidget);
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

    //! Création du bouton auto center lié à la manière de modifier l'ES
    auto* centerBox = new QCheckBox("Auto center");
    centerBox->setChecked(true);
    rightPanelLayout->addWidget(centerBox);

    //! Pour un affichage plus cohérent du panel de droite
    auto* fillWidget = new QWidget();
    fillWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    rightPanelLayout->addWidget(fillWidget);

    //! Ajout du panel de droite sur la fenêtre du DMM
    structElemLayout->addLayout(rightPanelLayout);
    structElemBox->setLayout(structElemLayout);
    layout->addWidget(structElemBox);

    //! Création du bouton de validation de l'opération à réaliser
    auto *okButton = new QPushButton(tr("Validate"), this);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    //! Connections Qt pour la modification de l'ES
    connect(dilatLeftButton, SIGNAL(clicked()), this, SLOT(dilateLeft()));
    connect(dilatTopLeftButton, SIGNAL(clicked()), this, SLOT(dilateTopLeft()));
    connect(dilatTopButton, SIGNAL(clicked()), this, SLOT(dilateTop()));
    connect(dilatTopRightButton, SIGNAL(clicked()), this, SLOT(dilateTopRight()));
    connect(dilatRightButton, SIGNAL(clicked()), this, SLOT(dilateRight()));
    connect(dilatBottomRightButton, SIGNAL(clicked()), this, SLOT(dilateBottomRight()));
    connect(dilatBottomButton, SIGNAL(clicked()), this, SLOT(dilateBottom()));
    connect(dilatBottomLeftButton, SIGNAL(clicked()), this, SLOT(dilateBottomLeft()));

    //! Connections Qt pour désactiver des directions de dilatation de l'ES
    connect(centerBox, SIGNAL(toggled(bool)), this, SLOT(setCenter(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatRightButton, SLOT(setDisabled(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatBottomRightButton, SLOT(setDisabled(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatBottomButton, SLOT(setDisabled(bool)));
    connect(centerBox, SIGNAL(toggled(bool)), dilatBottomLeftButton, SLOT(setDisabled(bool)));

    //! Connections Qt pour les boutons OK et DMM new level
    connect(_newLevelButton, SIGNAL(clicked()), this, SLOT(newLevel()));
    connect(_operatorBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(opChanged(QString)));
    _center = true;
}

DMMDialog::~DMMDialog() {
   delete _structElemView;
}

void DMMDialog::refreshStructElemView() {
    //! Stockage temporaire de l'ES actuel
    StructElemViewer* oldViewer = _structElemViewer;

    //! Création du nouvel ES avec ses dimensions modifiées
    //! front() retourne une référence vers le premier élément du vecteur _views
    _structElemViewer = new StructElemViewer(_views.front().elem, false);
    for(auto it = _views.begin()+1; it < _views.end(); ++it) {
        _structElemViewer->addStructElem(it->elem, (int)it->x, (int)it->y);
    }

    //! _structElemView affiche le contenu du _structElemViewer
    _structElemView->setScene(_structElemViewer);

    //! Coloriage du _structElemViewer pour faire apparaître l'ES
    _structElemViewer->draw(0, 0);

    //! Libération de la mémoire du précédent affichage
    delete oldViewer;

    //! Actiation du bouton DMM new level
    _newLevelButton->setEnabled(getSelectedOp() == Dmm && !_basicElements.empty());
}

void DMMDialog::dilateLeft() {
    StructElem::Dir dir;
    if(!_center || _centerX >= 0) {
        dir = StructElem::Right;
        _views.back().x--;
        --_centerX;
    }
    else {
        dir = StructElem::Left;
        ++_centerX;
    }
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}

void DMMDialog::dilateTopLeft() {
    StructElem::Dir dir;
    if(!_center || _centerX + _centerY >= 0 ) {
        dir = StructElem::BottomRight;
        _views.back().x--;
        _views.back().y--;
        --_centerX;
        --_centerY;
    }
    else {
        dir = StructElem::TopLeft;
        ++_centerX;
        ++_centerY;
    }
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}

void DMMDialog::dilateTop() {
    StructElem::Dir dir;
    if(!_center || _centerY >= 0 ) {
        dir = StructElem::Bottom;
        _views.back().y--;
        --_centerY;
    }
    else {
        dir = StructElem::Top;
        ++_centerY;
    }
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}

void DMMDialog::dilateTopRight() {
    StructElem::Dir dir;
    if(!_center || -_centerX + _centerY >= 0 ) {
        dir = StructElem::BottomLeft;
        _views.back().y--;
        ++_centerX;
        --_centerY;
    }
    else {
        dir = StructElem::TopRight;
        _views.back().x--;
        --_centerX;
        ++_centerY;
    }
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}

void DMMDialog::dilateRight() {
    StructElem::Dir dir;
    dir = StructElem::Left;
    ++_centerX;
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}

void DMMDialog::dilateBottomRight() {
    StructElem::Dir dir;
    dir = StructElem::TopLeft;
    ++_centerX;
    ++_centerY;
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}
void DMMDialog::dilateBottom() {
    StructElem::Dir dir;
    dir = StructElem::Top;
    ++_centerY;
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}
void DMMDialog::dilateBottomLeft() {
    StructElem::Dir dir;
    dir = StructElem::TopRight;
    _views.back().x--;
    --_centerX;
    ++_centerY;
    _views.back().elem->dilate(dir);
    _basicElements.push_back(dir);
    refreshStructElemView();
}

DMMDialog::Op DMMDialog::getSelectedOp() {
    QString opString = _operatorBox->currentText();
    if(opString == tr("Dilatation")) {
        return Dilatation;
    }
    else if(opString == tr("Erosion")) {
        return Erosion;
    }
    else if(opString == tr("Opening")) {
        return Opening;
    }
    else if(opString == tr("Closing")) {
        return Closing;
    }
    else {
        return Dmm;
    }
}

void DMMDialog::newLevel() {
    StructElem* newElem = new StructElem(*_views.back().elem);
    _views.push_back(StructElemViewer::ElemView(newElem, 0, 0));
    _levels.push_back(_basicElements.size());
}

void DMMDialog::opChanged(QString) {
    _newLevelButton->setEnabled(getSelectedOp() == Dmm && _basicElements.size()>0);
}
