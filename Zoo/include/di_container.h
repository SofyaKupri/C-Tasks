#pragma once

#include "interfaces.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>


class Container {
public:
    using Factory = std::function<IService*(Container&)>;

    void Register(const std::string& name, Factory factory, bool singleton = true);
    IService* Resolve(const std::string name);

private:
    struct ServiceEntry {
        Factory factory;
        bool singleton;
        std::unique_ptr<IService> instance;
    };
    std::unordered_map<std::string, ServiceEntry> services;
};