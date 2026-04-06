#include "include/commands/create_account.h"
#include "include/commands/manager.h"
#include "include/commands/timer_decorator.h"
#include "include/commands/deposit.h"
#include "include/entity/account.h"
#include "include/entity/category.h"
#include "include/entity/operation.h"
#include "include/facade/account_facade.h"
#include "include/storage/cache.h"
#include "include/storage/inmem.h"
#include "include/storage/storage.h"
#include <iostream>

using std::cout;

void RunBankApp() {
  cout << "\nEnter action:\n";
  cout << "1. New account\n";
  cout << "2. Get balance\n";
  cout << "3. Cancel last action\n";
  cout << "4. Update balance\n";
  cout << "0. Exit\n";
  auto storage = std::make_shared<InMemoryStorage<Account>>();
  auto cachedStorage = std::make_shared<Cache<Account>>(storage);
  AccountFacade accountFacade(cachedStorage);
  CommandManager cmdManager;
  int action = -1;
  while (action != 0) {
    cout << "\nEnter action:\n";
    if (!(std::cin >> action))
      break;
    std::cin.ignore();

    switch (action) {
    case 1: {
      cout << "Enter account's name:\n";
      std::string name;
      std::getline(std::cin, name);
      auto baseCmd = std::make_unique<CreateAccount>(accountFacade, name);
      auto profiledCmd = std::make_unique<TimerDecorator>(std::move(baseCmd));
      cmdManager.Do(std::move(profiledCmd));
      cout << "Account created successfully\n";
      break;
    }
    case 2: {
      cout << "Enter account's ID:\n";
      size_t id;
      std::cin >> id;
      auto acc = cachedStorage->Get(id);
      if (acc) {
        cout << "Name: " << acc->GetName()
             << " | Balance: " << acc->GetBalance() << "\n";
      } else {
        cout << "Account not found\n";
      }
      break;
    }
    case 3: {
      cmdManager.Undo();
      cout << "Undid successfully\n";
      break;
    }
    case 4: {
      cout << "Enter account's ID:\n";
      size_t id;
      std::cin >> id;
      cout << "Balance change:\n";
      int64_t amount;
      std::cin >> amount;
      try {
        auto baseCmd = std::make_unique<DepositCommand>(accountFacade, id, amount);
        auto profiledCmd = std::make_unique<TimerDecorator>(std::move(baseCmd));
        cmdManager.Do(std::move(profiledCmd));
        std::cout << "Balance changes successfully\n";
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
      break;
    }
    case 0:
      std::cout << "Byebye\n";
      break;
    default:
      std::cout << "Command not found\n";
    }
  }
}

int main() { RunBankApp(); };
