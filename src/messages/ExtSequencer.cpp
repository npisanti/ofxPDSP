
#include "ExtSequencer.h"

std::vector<pdsp::ExtSequencer*> pdsp::ExtSequencer::instances;

pdsp::ExtSequencer::ExtSequencer() {
    instances.push_back(this);
}

pdsp::ExtSequencer::~ExtSequencer() {
    for (std::size_t i = 0; i < instances.size(); ++i) {
        if (instances[i] == this) {
            instances.erase(instances.begin() + i);
            break;
        }
    }
}
