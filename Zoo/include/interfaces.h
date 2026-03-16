#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>

class IAlive {
public:
    IAlive(int food = 0) : food(food) {}
    int Food() const noexcept { return food; };
    virtual ~IAlive() = default;
protected:
    int food = 0;
};

class IInventory {
public:
    IInventory(int number = 0) : number(number) {}
    int Number() const noexcept { return number; }
    void SetNumber(int value) { number = value; }
    virtual ~IInventory() = default;
protected:
    int number = 0;
};

class IPlayerAnimal {
public:
    IPlayerAnimal(int kindness) : kindness(kindness) {}
    int Kindness() const noexcept { return kindness; };
    bool IsContactable() const { return kindness > 5; }
    virtual ~IPlayerAnimal() = default;
protected:
    int kindness;
};

class IService {
public:
    virtual ~IService() = default;
};

class IVetClinic : public IService {
public:
    virtual bool IsHealthy(IAlive *) const noexcept = 0;
    virtual ~IVetClinic() = default;
};

class Journal: public IService {
public:
    template<typename T, typename... Types>
    void Print(T firstArg, Types... args) {
        Out() << firstArg;
        if constexpr (sizeof...(args)) {
            Print(std::forward<Types>(args)...);
        }
    }
    virtual ~Journal() = default;
protected:
    virtual std::ostream& Out() = 0;
};