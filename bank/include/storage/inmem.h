#pragma once
#include "storage.h"

#include <memory>
#include <unordered_map>

template <typename T> class InMemoryStorage : public Storage<T> {
public:
  void Add(const std::shared_ptr<T> &entity) override {
    m_data[entity->GetId()] = entity;
  }
  void Remove(size_t id) override { m_data.erase(id); }
  void Update(const std::shared_ptr<T> &entity) override {
    if (m_data.find(entity->GetId()) != m_data.end()) {
      m_data[entity->GetId()] = entity;
    }
  }
  std::shared_ptr<T> Get(size_t id) const override {
    auto it = m_data.find(id);
    return (it != m_data.end()) ? it->second : nullptr;
  }

private:
  std::unordered_map<size_t, std::shared_ptr<T>> m_data;
};
