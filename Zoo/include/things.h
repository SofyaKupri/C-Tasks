#pragma once
#include "interfaces.h"

class Thing : public IInventory {
public:
    Thing() {};
    virtual ~Thing() = default;
};

class Table : public Thing {
public:
    Table() {};
};

class Computer : public Thing {
public:
    Computer() {};
};
