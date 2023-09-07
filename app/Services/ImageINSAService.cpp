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

#include <GenericInterface.h>

#include "ImageINSAService.h"

using namespace genericinterface;
using namespace std;

ImageINSAService::ImageINSAService(GenericInterface* gi) : WindowService(Qt::LeftDockWidgetArea) {
    _gi = gi;
    _operationBar = new OperationBar();
}

void ImageINSAService::display(GenericInterface* gi)
{
    WindowService::display(gi);
//    QDockWidget* _operationDock = new QDockWidget(tr("Operations"), gi);
//    _operationDock->setWidget(_operationBar);
//    gi->addDockWidget(Qt::LeftDockWidgetArea, _operationDock);

    _statusEdit = new QTextEdit();
    _statusEdit->setReadOnly(true);
    _statusEdit->setMinimumHeight(1);
    _statusEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    _statusEdit->setReadOnly(true);
    QDockWidget* statusDock = new QDockWidget(tr("Informations"), gi);
    statusDock->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    statusDock->setMinimumWidth(128);
    statusDock->setWidget(_statusEdit);
    statusDock->setFeatures(0x00);
    gi->addDockWidget(Qt::BottomDockWidgetArea, statusDock);
    _statusEdit->hide();
}

void ImageINSAService::connect(GenericInterface* gi)
{
    WindowService::connect(gi);
}

void ImageINSAService::addOpSet(OpSet* opSet) {
    for(vector<OpSetService*>::iterator it = _opSetServices.begin(); it != _opSetServices.end(); ++it) {
        if((*it)->getOpSet() == opSet) {
            return;
        }
    }
    OpSetService* opSetService = new OpSetService(opSet);
    _opSetServices.push_back(opSetService);
    _gi->addService(opSetService);
    QObject::connect(opSetService, SIGNAL(outputText(QString)), this, SLOT(outputText(QString)));
    if(_gi->running()) {
        opSetService->checkActionsValid(this->getCurrentImageWindow());
    }
}

void ImageINSAService::removeOpSet(OpSet* opSet) {
    for(vector<OpSetService*>::iterator it = _opSetServices.begin(); it != _opSetServices.end(); ++it) {
        if((*it)->getOpSet() == opSet) {
            _gi->removeService(*it);
            _opSetServices.erase(it);
            return;
        }
    }
}

void ImageINSAService::outputText(QString text) {
    _statusEdit->append(text);
    _statusEdit->show();
    if(_statusEdit->minimumHeight() < 92) _statusEdit->setMinimumHeight(_statusEdit->minimumHeight()+24);
}

void ImageINSAService::addText(std::string s) {
    _statusEdit->append(QString::fromStdString(s));
    _statusEdit->show();
    if(_statusEdit->minimumHeight() < 92) _statusEdit->setMinimumHeight(_statusEdit->minimumHeight()+24);
}
