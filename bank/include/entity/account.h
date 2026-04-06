#pragma once
#include "operation.h"
#include <string>

class Account {
public:
  Account(int id, std::string name);
  void ApplyOperation(int64_t amount, Operation::Type op);
  int64_t GetBalance() const;
  int GetId() const;
  std::string GetName() const;

private:
  const size_t m_id;
  const std::string m_name;
  int64_t m_balance{0};
};
