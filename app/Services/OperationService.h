#ifndef OPERATIONSERVICE_H
#define OPERATIONSERVICE_H

#include <Service.h>
#include <Plugin.h>

#include <QToolBar>
#include <QMenu>

class OperationService : public QObject, public genericinterface::Service
{
  Q_OBJECT
  public:
    OperationService(Operation* operation, QMenu* menu); 
    void display(genericinterface::GenericInterface* gi);
    void connect(genericinterface::GenericInterface* gi);
    inline Operation* getOperation() { return _operation; }
    ~OperationService();
    void setEnabled(bool);

  public slots:
    void operation();

  private:  
    genericinterface::GenericInterface* _gi;
    Operation* _operation;
    QMenu* _menu;
    QAction* _action;
};

#endif
