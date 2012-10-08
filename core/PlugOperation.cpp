/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of EIImage.
 * 
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>

#include "PlugOperation.h"
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>


using namespace std;
using namespace imagein;
using namespace genericinterface;

PlugOperation::PlugOperation(string name) : GenericOperation(name), _needCurrentImg(false), _currentStdImg(NULL), _currentDblImg(NULL), _doubleCurrentImg(false) {
}

void PlugOperation::addInput(Input* input) {
    this->_inputs.push_back(input);
}

void PlugOperation::addInput(const Input& input) {
    this->_inputs.push_back(input.clone());
}

bool PlugOperation::isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const {
    if(!this->needCurrentImg()) {
        return true;
    }
    if(_doubleCurrentImg) {
        const DoubleImageWindow* diw = dynamic_cast<const DoubleImageWindow*>(imgWnd);
        return diw != NULL;
    }
    else {
        const StandardImageWindow* siw = dynamic_cast<const StandardImageWindow*>(imgWnd);
        return siw != NULL;
    }
}

void PlugOperation::operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& imgWndList) {

    if(this->needCurrentImg()) {
        if(_doubleCurrentImg) {
            const DoubleImageWindow* diw = dynamic_cast<const DoubleImageWindow*>(currentWnd);
            if(diw ==NULL) return;
            *_currentDblImg = *diw->getImage();
        }
        else {
            const StandardImageWindow* siw = dynamic_cast<const StandardImageWindow*>(currentWnd);
            if(siw == NULL) return;
            *_currentStdImg = *siw->getImage();
        }
    }
    
    if(_inputs.size()>0) {
        QDialog* dialog = new QDialog();
        dialog->setWindowTitle(qApp->translate("PlugOperation", "Parameters"));
        dialog->setMinimumWidth(160);
        QVBoxLayout* layout = new QVBoxLayout();
        dialog->setLayout(layout);

        for(vector<Input*>::iterator it = _inputs.begin(); it < _inputs.end(); ++it) {
            (*it)->fillDialog(dialog, currentWnd, imgWndList);
        }
        
        QPushButton *okButton = new QPushButton(qApp->translate("PlugOperation", "Validate"), dialog);
        okButton->setDefault(true);
        layout->addWidget(okButton);
        QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
        
        QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
        cout << code << endl;
        
        if(code!=QDialog::Accepted) {
            return;
        }
        
        for(vector<Input*>::iterator it = _inputs.begin(); it < _inputs.end(); ++it) {
            (*it)->pickValue();
        } 
    }
    
    this->operation();
}
