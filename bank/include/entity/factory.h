#pragma once
#include "account.h"
#include "category.h"
#include "operation.h"
#include <memory>
#include <string>

class Factory {
private:
  static size_t GenId();

public:
  static std::shared_ptr<Account> CreateAccount(const std::string &name);
  static std::shared_ptr<Category> CreateCategory(const std::string &name,
                                                  Category::Type);
  static std::shared_ptr<Operation>
  CreateOperation(Operation::Type type, int64_t amount, size_t accountId,
                  size_t categoryId, const std::string &description);
};
