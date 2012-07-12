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
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include <ImgWidget.h>

#include <QMessageBox>
#include <QLibrary>
#include <string>
#include <typeinfo>
#include <sstream>

using namespace genericinterface;
using namespace std;

OperationService::OperationService(GenericOperation* operation, QMenu* menu) : _operation(operation), _menu(menu) {
    
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
    
//    StandardImageWindow* curStdImgWnd = NULL;
//    if (curImgWnd != NULL)
//    {
//        curStdImgWnd = dynamic_cast<StandardImageWindow*>(curImgWnd);
//    }
    
//    const Image* image = NULL;
//    if(curStdImgWnd != NULL) {
//        image = curStdImgWnd->getImage();
//    }
    if(_operation->needCurrentImg() && !_operation->isValidImgWnd(curImgWnd)) return;

    map<const ImageWindow*, string> wndList;
    vector<ImageWindow*> windows = ws->getImageWindows();
    for(vector<ImageWindow*>::iterator it = windows.begin(); it < windows.end(); ++it) {
        wndList.insert(pair<const ImageWindow*, string>(*it, (*it)->windowTitle().toStdString()));
    }

    vector<QWidget*> result = _operation->operator()(curImgWnd, wndList);
    
    for(vector<QWidget*>::iterator it = result.begin(); it < result.end(); ++it) {
        QWidget* widget = *it;
        QLabel* twdgt = dynamic_cast<QLabel*>(widget);
        if((typeid(*widget) == typeid(ImgWidget)) || (typeid(*widget) == typeid(DoubleImgWidget))) {
            QString title = _operation->needCurrentImg() ? (curImgWnd->windowTitle() + " - ") : "";
            ImageWindow* siw;
            if(typeid(*widget)==typeid(ImgWidget)) {
                ImgWidget* w = dynamic_cast<ImgWidget*>(widget);
                title += w->name.c_str();
                siw = new StandardImageWindow(_operation->needCurrentImg() ? curImgWnd->getPath() : w->name.c_str(), _gi, w->img);
            }
            else {
                DoubleImgWidget* w = dynamic_cast<DoubleImgWidget*>(widget);
                title += w->name.c_str();
                siw = new DoubleImageWindow(_operation->needCurrentImg() ? curImgWnd->getPath() : w->name.c_str(), _gi, w->img, w->normalize, w->logScale);
            }
            NodeId id = _operation->needCurrentImg() ? ws->getNodeId(curImgWnd) : NodeId(siw->getDisplayImage());
            ws->addImage(id, siw);
            siw->setWindowTitle(title);
        }
        else if(twdgt != NULL) {
            emit outputText(twdgt->text());
        }
        else {
            ws->addWidget(ws->getNodeId(curImgWnd), widget);
        }
        
    }
    
    
}

void OperationService::setEnabled(bool enabled) {
    _action->setEnabled(enabled);
}

OperationService::~OperationService() {
    _menu->removeAction(_action);
}
