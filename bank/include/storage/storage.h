#pragma once

#include <memory>

template <typename T> class Storage {
public:
  virtual void Add(const std::shared_ptr<T> &entity) = 0;
  virtual void Remove(size_t id) = 0;
  virtual void Update(const std::shared_ptr<T> &entity) = 0;
  virtual std::shared_ptr<T> Get(size_t id) const = 0;
};
