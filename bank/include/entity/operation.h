#pragma once
#include <chrono>
#include <string>

class Operation {
public:
  enum class Type {
    Income,
    Outcome,
  };

  Operation(size_t id, Type op, uint64_t amount, size_t account_id,
            size_t category_id, std::string description = "")
      : m_id(id), m_type(op), m_amount(amount), m_account_id(account_id),
        m_category_id(category_id), m_description(std::move(description)),
        m_tp(std::chrono::system_clock::now()) {}

  size_t GetId() const { return m_id; }
  Type GetType() const { return m_type; }
  uint64_t GetAmount() const { return m_amount; }
  size_t GetAccountId() const { return m_account_id; }
  size_t GetCategoryId() const { return m_category_id; }

private:
  const size_t m_id;
  Type m_type;
  uint64_t m_amount;
  size_t m_account_id;
  size_t m_category_id;
  std::string m_description;
  std::chrono::system_clock::time_point m_tp;
};