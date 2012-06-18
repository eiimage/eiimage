#ifndef EIIMAGESERVICE_H
#define EIIMAGESERVICE_H

#include <Service.h>
#include <Plugin.h>

#include "../Widgets/OperationBar.h"

class EIImageService : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    EIImageService(genericinterface::GenericInterface* gi); 
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);

  public slots:

  private:  
    genericinterface::GenericInterface* _gi;
    OperationBar* _operationBar;
};

#endif
