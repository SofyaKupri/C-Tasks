#include "../include/zoo.h"

bool Zoo::AddAnimal(std::unique_ptr<Animal> animal) {
    if (!clinic->IsHealthy(animal.get())) {
        return false;
    }
    animal->SetNumber(next_number++);
    _animals.push_back(std::move(animal));
    journal->Print("Animal added with number ", next_number-1, "\n");
    return true;
}

void Zoo::AddThing(std::unique_ptr<Thing> thing) {
    thing->SetNumber(next_number++);
    _things.push_back(std::move(thing));
    journal->Print("Thing added with number ", next_number-1, "\n");
}

int Zoo::FoodPerDay() const {
    int sum = 0;
    for (const auto& a : _animals) {
        sum += a->Food();
    }
    return sum;
}

std::vector<Animal*> Zoo::GetContactAnimals() const {
    std::vector<Animal*> res;
    for (const auto& a : _animals) {
        if (auto* player = dynamic_cast<IPlayerAnimal*>(a.get())) {
            Herbo* herbo = dynamic_cast<Herbo*>(a.get());
            if (herbo && herbo->IsContactable()) {
                res.push_back(a.get());
            }
        }
    }
    return res;
}

std::vector<IInventory*> Zoo::GetInventory() const {
    std::vector<IInventory*> inv;
    for (const auto& a : _animals) {
        inv.push_back(a.get());
    }
    for (const auto& t : _things) {
        inv.push_back(t.get());
    }
    return inv;
}