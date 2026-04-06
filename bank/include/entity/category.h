#pragma once
#include <string>

class Category {
public:
  enum class Type {
    Income,
    Outcome,
  };
  Category(int id, std::string name, Type op)
      : m_id(id), m_name(name), m_type(op) {}
  size_t GetId() const { return m_id; }

private:
  const size_t m_id;
  const std::string m_name;
  Type m_type;
};
