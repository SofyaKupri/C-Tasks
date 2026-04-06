#pragma once
#include "storage.h"
#include <memory>
#include <unordered_map>

template <typename T> class Cache : public Storage<T> {
public:
  Cache(std::shared_ptr<Storage<T>> s) : m_storage(s) {}
  void Add(const std::shared_ptr<T> &entity) override {
    m_data[entity->GetId()] = entity;
    if (m_storage) {
      m_storage->Add(entity);
    }
  }
  void Remove(size_t id) override {
    m_data.erase(id);
    if (m_storage) {
      m_storage->Remove(id);
    }
  }
  void Update(const std::shared_ptr<T> &entity) override {
    m_data[entity->GetId()] = entity;
    if (m_storage) {
      m_storage->Update(entity);
    }
  }
  std::shared_ptr<T> Get(size_t id) const override {
    auto it = m_data.find(id);
    if (it != m_data.end()) {
      return it->second;
    }
    if (m_storage) {
      auto entity = m_storage->Get(id);
      return entity;
    }
    return nullptr;
  }

private:
  std::unordered_map<size_t, std::shared_ptr<T>> m_data;
  std::shared_ptr<Storage<T>> m_storage;
};