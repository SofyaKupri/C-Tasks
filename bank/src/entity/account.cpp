#include "../../include/entity/account.h"
#include <stdexcept>

Account::Account(int id, std::string name) : m_id(id), m_name(name) {}

void Account::ApplyOperation(int64_t amount, Operation::Type op) {
  if (op == Operation::Type::Income) {
    m_balance += amount;
  } else {
    if (m_balance < amount) {
      throw std::runtime_error("No money");
    }
    m_balance -= amount;
  }
}

int64_t Account::GetBalance() const { return m_balance; }
int Account::GetId() const { return m_id; }
std::string Account::GetName() const { return m_name; }
