#include "../include/animals.h"

Animal::Animal(int food) : IAlive(food), IInventory(0) {}

Herbo::Herbo(int food, int kindness) : Animal(food), IPlayerAnimal(kindness) {}
Predator::Predator(int food) : Animal(food) {}

Monkey::Monkey(int food, int kindness) : Herbo(food, kindness) {}
Rabbit::Rabbit(int food, int kindness) : Herbo(food, kindness) {}
Tiger::Tiger(int food) : Predator(food) {}
Wolf::Wolf(int food) : Predator(food) {}