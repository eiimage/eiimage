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

#include <GenericInterface.h>

#include "EIImageService.h"

using namespace genericinterface;
using namespace std;

EIImageService::EIImageService(GenericInterface* gi) : WindowService(Qt::LeftDockWidgetArea) {
    _gi = gi;
    _operationBar = new OperationBar();
}

void EIImageService::display(GenericInterface* gi)
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
    gi->addDockWidget(Qt::BottomDockWidgetArea, statusDock);
    _statusEdit->hide();
}

void EIImageService::connect(GenericInterface* gi)
{
    WindowService::connect(gi);
}

void EIImageService::addOpSet(OpSet* opSet) {
    for(vector<OpSetService*>::iterator it = _opSetServices.begin(); it != _opSetServices.end(); ++it) {
        if((*it)->getOpSet() == opSet) {
            return;
        }
    }
    OpSetService* opSetService = new OpSetService(opSet);
    _opSetServices.push_back(opSetService);
    _gi->addService(opSetService);
    QObject::connect(opSetService, SIGNAL(outputText(QString)), this, SLOT(outputText(QString)));
}

void EIImageService::removeOpSet(OpSet* opSet) {
    for(vector<OpSetService*>::iterator it = _opSetServices.begin(); it != _opSetServices.end(); ++it) {
        if((*it)->getOpSet() == opSet) {
            _gi->removeService(*it);
            _opSetServices.erase(it);
            return;
        }
    }
}

void EIImageService::outputText(QString text) {
    _statusEdit->append(text);
    _statusEdit->show();
    if(_statusEdit->minimumHeight() < 92) _statusEdit->setMinimumHeight(_statusEdit->minimumHeight()+24);
//    _statusEdit->setMinimumHeight(32);
}

void EIImageService::addText(std::string s) {
    _statusEdit->append(QString::fromStdString(s));
    _statusEdit->show();
    if(_statusEdit->minimumHeight() < 92) _statusEdit->setMinimumHeight(_statusEdit->minimumHeight()+24);
}
