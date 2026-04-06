#pragma once
#include "../entity/account.h"
#include "../entity/factory.h"
#include "../storage/storage.h"
#include <memory>

class AccountFacade {
public:
  explicit AccountFacade(std::shared_ptr<Storage<Account>> storage)
      : m_storage(std::move(storage)) {}
  std::shared_ptr<Account> CreateAccount(const std::string &name) {
    auto newAccount = Factory::CreateAccount(name);
    m_storage->Add(newAccount);
    return newAccount;
  }
  void Deposit(size_t id, uint64_t amount) {
    auto acc = m_storage->Get(id);
    if (acc) {
      acc->ApplyOperation(amount, Operation::Type::Income);
    }
  }
  void Withdraw(size_t id, uint64_t amount) {
    auto acc = m_storage->Get(id);
    if (acc) {
      acc->ApplyOperation(amount, Operation::Type::Outcome);
    }
  }
  void DeleteAccount(size_t id) { m_storage->Remove(id); }

private:
  std::shared_ptr<Storage<Account>> m_storage;
};
