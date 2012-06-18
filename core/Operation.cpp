#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "Operation.h"


using namespace std;
using namespace imagein;

Operation::Operation(string name) : _name(name), _needCurrentImg(false), _currentImg(NULL) {
}

std::string Operation::getName() {
    return _name;
}

void Operation::addInput(Input* input) {
    this->_inputs.push_back(input);
}

void Operation::addInput(const Input& input) {
    this->_inputs.push_back(input.clone());
}

void Operation::addOutput(const Output& output) {
    _outputs.push_back(output.clone());
}


std::vector<QWidget*> Operation::operator()(const Image* currentImg) {
    vector<QWidget*> result;
    
    if(this->needCurrentImg()) {
        if(currentImg==NULL) return result;
        *_currentImg = *currentImg;
    }
    
    if(_inputs.size()>0) {
        QDialog* dialog = new QDialog();
        dialog->setWindowTitle("Paramètres");
        dialog->setMinimumWidth(160);
        QVBoxLayout* layout = new QVBoxLayout();
        dialog->setLayout(layout);

        for(vector<Input*>::iterator it = _inputs.begin(); it < _inputs.end(); ++it) {
            (*it)->fillDialog(dialog);
        }
        
        QPushButton *okButton = new QPushButton("Valider", dialog);
        okButton->setDefault(true);
        layout->addWidget(okButton);
        QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
        
        QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
        cout << code << endl;
        
        if(code!=QDialog::Accepted) {
            return result;
        }
        
        for(vector<Input*>::iterator it = _inputs.begin(); it < _inputs.end(); ++it) {
            (*it)->pickValue();
        } 
    }
    
    
    _outputs.clear();
    this->operation();
    
    for(vector<Output*>::iterator it = _outputs.begin(); it < _outputs.end(); ++it) {
        result.push_back((*it)->getWidget());
    }
    return result;
}