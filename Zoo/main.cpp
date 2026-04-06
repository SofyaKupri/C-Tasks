#include "include/di_container.h"
#include "include/interfaces.h"
#include "include/vet_clinic.h"
#include "include/journal.h"
#include "include/zoo.h"
#include "include/animals.h"
#include "include/things.h"

IService * clinic_maker(Container &) {
    return new VetClinic();
}

IService * journal_maker(Container &) {
    return new ConsoleJournal();
}

IService * zoo_maker(Container &container) {
    return new Zoo(dynamic_cast<IVetClinic*>(container.Resolve("clinic"))
        , dynamic_cast<Journal*>(container.Resolve("journal")));
}

void RunZooApp(Zoo* zoo, Journal* journal) {
    int op;
    journal->Print("\nEnter operaton:\n");
    journal->Print("1. Add animal\n");
    journal->Print("2. Add thing\n");
    journal->Print("3. Show total food per day\n");
    journal->Print("4. Show contactable animals\n");
    journal->Print("5. Show inventory\n");
    journal->Print("0. Exit\n");
    do {
        std::cin >> op;
        if (op == 1) {
            journal->Print("Select animal:\n");
            journal->Print("1. Monkey\n2. Rabbit\n3. Tiger\n4. Wolf\n");
            int t;
            std::cin >> t;
            journal->Print("Enter food per day: ");
            int food;
            std::cin >> food;
            int kindness = 0;
            if (t == 1 || t == 2) {
                journal->Print("Enter kindness: ");
                std::cin >> kindness;
            }
            std::unique_ptr<Animal> animal;
            switch (t) {
                case 1: animal = std::make_unique<Monkey>(food, kindness); break;
                case 2: animal = std::make_unique<Rabbit>(food, kindness); break;
                case 3: animal = std::make_unique<Tiger>(food); break;
                case 4: animal = std::make_unique<Wolf>(food); break;
                default: journal->Print("Invalid type\n"); continue;
            }
            if (zoo->AddAnimal(std::move(animal))) {
                journal->Print("Animal added\n");
            } else {
                journal->Print("Animal rejected\n");
            }
        } else if (op == 2) {
            journal->Print("Select thing:\n");
            journal->Print("1. Table\n2. Computer\n");
            int t;
            std::cin >> t;
            std::unique_ptr<Thing> thing;
            if (t == 1) {
                thing = std::make_unique<Table>();
            } else if (t == 2) {
                thing = std::make_unique<Computer>();
            } else {
                journal->Print("Invalid type\n");
                continue;
            }
            zoo->AddThing(std::move(thing));
            journal->Print("Thing added.\n");
        } else if (op == 3) {
            journal->Print("Food per day: ", zoo->FoodPerDay(), " kg\n");
        } else if (op == 4) {
            auto animals = zoo->GetContactAnimals();
            journal->Print("Contactable animals:\n");
            for (auto* a : animals) {
                journal->Print("Animal #", a->Number(), " (food: ", a->Food(), " kg)\n");
            }
        } else if (op == 5) {
            auto inv = zoo->GetInventory();
            journal->Print("Inventory:\n");
            for (auto* item : inv) {
                if (dynamic_cast<Animal*>(item)) {
                    journal->Print("Animal #", item->Number(), "\n");
                } else {
                    journal->Print("Thing #", item->Number(), "\n");
                }
            }
        }
    } while (op != 0);
}

int main() {
    Container advContainer;
    advContainer.Register("journal", journal_maker, true);
    advContainer.Register("clinic", clinic_maker, true);
    advContainer.Register("zoo", zoo_maker, true);

    Zoo* zooAdv = dynamic_cast<Zoo*>(advContainer.Resolve("zoo"));
    Journal* journalAdv = dynamic_cast<Journal*>(advContainer.Resolve("journal"));

    RunZooApp(zooAdv, journalAdv);

    return 0;
}