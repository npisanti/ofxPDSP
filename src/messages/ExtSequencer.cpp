
#include "ExtSequencer.h"

std::vector<pdsp::ExtSequencer*> pdsp::ExtSequencer::instances;

pdsp::ExtSequencer::ExtSequencer() {
    instances.push_back(this);
}

pdsp::ExtSequencer::~ExtSequencer() {
    instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
}
