#pragma once

#include "interfaces.h"

class Animal : public IAlive, public IInventory {
public:
    Animal(int food);
    virtual ~Animal() = default;
};
class Herbo : public Animal, public IPlayerAnimal {
public:
    explicit Herbo(int food, int kindness);
};
class Predator : public Animal {
public:
    explicit Predator(int food);
};
class Monkey : public Herbo {
public:
    explicit Monkey(int food, int kindness);
};
class Rabbit : public Herbo {
public:
    explicit Rabbit(int food, int kindness);
};
class Tiger : public Predator {
public:
    explicit Tiger(int food);
};
class Wolf : public Predator {
public:
    explicit Wolf(int food);
};