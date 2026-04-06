#include "../../include/entity/factory.h"
#include <stdexcept>

size_t Factory::GenId() {
  static size_t current_id = 1;
  return current_id++;
}

std::shared_ptr<Account> Factory::CreateAccount(const std::string &name) {
  return std::make_shared<Account>(GenId(), name);
}

std::shared_ptr<Category> Factory::CreateCategory(const std::string &name,
                                                  Category::Type op) {
  return std::make_shared<Category>(GenId(), name, op);
}

std::shared_ptr<Operation>
Factory::CreateOperation(Operation::Type type, int64_t amount, size_t accountId,
                         size_t categoryId,
                         const std::string &description = "") {
  if (amount <= 0) {
    throw std::invalid_argument("No money");
  }
  return std::make_shared<Operation>(GenId(), type, amount, accountId,
                                     categoryId, description);
}
