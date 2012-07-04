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

#include "OperationService.h"
#include <GenericInterface.h>
#include <ImgWidget.h>

#include <QMessageBox>
#include <QLibrary>
#include <string>
#include <typeinfo>
#include <sstream>

using namespace genericinterface;
using namespace std;

OperationService::OperationService(Operation* operation, QMenu* menu) : _operation(operation), _menu(menu) {
    
}

void OperationService::display(GenericInterface* gi)
{
    _gi = gi;
    
    _action = _menu->addAction(_operation->getName().c_str());
}

void OperationService::connect(GenericInterface* gi)
{
    QObject::connect(_action, SIGNAL(triggered()), this, SLOT(operation()));
}


void OperationService::operation() {
    cout << _operation->getName() << endl;
    
    WindowService* ws = _gi->windowService();
    ImageWindow* curImgWnd = ws->getCurrentImageWindow();
    
    StandardImageWindow* curStdImgWnd = NULL;
    if (curImgWnd != NULL)
    {
        curStdImgWnd = dynamic_cast<StandardImageWindow*>(curImgWnd);
    }
    
    const Image* image = NULL;
    if(curStdImgWnd != NULL) {
        image = curStdImgWnd->getImage();
    }
    if(_operation->needCurrentImg() && image == NULL) return;
    
    vector<QWidget*> result = _operation->operator()(image, vector<const Image*>());
    
    for(vector<QWidget*>::iterator it = result.begin(); it < result.end(); ++it) {
        QWidget* widget = *it;
        ImgWidget* iwdgt = dynamic_cast<ImgWidget*>(widget);
        if(iwdgt != NULL) {
            Image* resImg = iwdgt->img;
           
            QString title;
            if(_operation->needCurrentImg()) {
                StandardImageWindow* siw = new StandardImageWindow(curStdImgWnd->getPath(), _gi, resImg);
                title = curStdImgWnd->windowTitle();
                title += " [";
                title += _operation->getName().c_str();
                 if(iwdgt->name != "") {
                    title += ", ";
                    title += iwdgt->name.c_str() ;
                }
                title += "]";
                siw->setWindowTitle(title);
                ws->addImage(ws->getNodeId(curStdImgWnd), siw);
            }
            else {
                title = _operation->getName().c_str();
                 if(iwdgt->name != "") {
                    title += " [";
                    title += iwdgt->name.c_str() ;
                    title += "]";
                }
                StandardImageWindow* siw = new StandardImageWindow(iwdgt->name.c_str(), _gi, resImg);
                siw->setWindowTitle(title);
                ws->addImage(NodeId(resImg), siw);
            }
            //delete iwdgt;
        }
        else {
            ws->addWidget(ws->getNodeId(curStdImgWnd), widget);
        }
        
    }
    
    
}

void OperationService::setEnabled(bool enabled) {
    _action->setEnabled(enabled);
}

OperationService::~OperationService() {
    _menu->removeAction(_action);
}
