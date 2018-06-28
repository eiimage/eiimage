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

#include "OperationService.h"
#include <GenericInterface.h>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include "ImageINSAService.h"

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

    if(_operation->getName().length() > 0) {
        _action = _menu->addAction(_operation->getName().c_str());
    }
    else {
        _action = _menu->addSeparator();
    }
}

void OperationService::connect(GenericInterface* gi)
{
    QObject::connect(_action, SIGNAL(triggered()), this, SLOT(operation()));
}


void OperationService::operation() {
    WindowService* ws = _gi->windowService();
    ImageWindow* curImgWnd = ws->getCurrentImageWindow();
    if(_operation->needCurrentImg() && !_operation->isValidImgWnd(curImgWnd)) return;

    map<const ImageWindow*, string> wndList;
    vector<const ImageWindow*> windows = ws->getImageWindows();
    for(vector<const ImageWindow*>::const_iterator it = windows.begin(); it < windows.end(); ++it) {
        wndList.insert(pair<const ImageWindow*, string>(*it, (*it)->windowTitle().toStdString()));
    }

    _operation->operator()(dynamic_cast<ImageINSAService*>(ws));
    
}

void OperationService::setEnabled(bool enabled) {
    _action->setEnabled(enabled);
}

OperationService::~OperationService() {
    _menu->removeAction(_action);
}
