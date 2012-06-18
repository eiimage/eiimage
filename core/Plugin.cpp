#include "Plugin.h"

Plugin::Plugin(std::string name) : _name(name) {
}

std::string Plugin::getName() {
    return _name;
}

std::vector<Operation*> Plugin::getOperations() {
    return _operations;
}

void Plugin::addOperation(Operation* operation) {
    _operations.push_back(operation);
}