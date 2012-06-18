#include <GenericInterface.h>

#include "EIImageService.h"

using namespace genericinterface;
using namespace std;

EIImageService::EIImageService(GenericInterface* gi) {
    _gi = gi;
    _operationBar = new OperationBar();
}

void EIImageService::display(GenericInterface* gi)
{
    QDockWidget* _operationDock = new QDockWidget("Opérations", gi);
    _operationDock->setWidget(_operationBar);
    gi->addDockWidget(Qt::LeftDockWidgetArea, _operationDock);
}

void EIImageService::connect(GenericInterface* gi)
{
}