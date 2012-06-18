#include "PluginService.h"

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
    
    vector<QWidget*> result = _operation->operator()(image);
    
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