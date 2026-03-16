#include "../include/di_container.h"
#include <stdexcept>

void Container::Register(const std::string& name, Factory factory, bool singleton) {
    services[name] = ServiceEntry{factory, singleton, nullptr};
}

IService* Container::Resolve(const std::string name) {
    auto it = services.find(name);
    if (it == services.end()) {
        throw std::runtime_error("Service not found: " + name);
    }
    auto& entry = it->second;
    if (entry.singleton) {
        if (!entry.instance) {
            entry.instance.reset(entry.factory(*this));
        }
        return entry.instance.get();
    } else {
        return entry.factory(*this);
    }
}