#pragma once
#include "interfaces.h"
#include "animals.h"
#include "things.h"
#include <memory>
#include <tuple>
#include <vector>

class Zoo: public IService {
public:
    Zoo(IVetClinic * vet_clinic, Journal * journal) : clinic(vet_clinic), journal(journal) {}
    using inject = std::tuple<IVetClinic *, Journal *>;
    bool AddAnimal(std::unique_ptr<Animal> animal);
    void AddThing(std::unique_ptr<Thing> thing);
    int FoodPerDay() const;
    std::vector<Animal*> GetContactAnimals() const;
    std::vector<IInventory*> GetInventory() const;
    ~Zoo() = default;
private:
    IVetClinic* clinic;
    Journal* journal;
    std::vector<std::unique_ptr<Animal>> _animals;
    std::vector<std::unique_ptr<Thing>> _things;
    int next_number = 1;
};