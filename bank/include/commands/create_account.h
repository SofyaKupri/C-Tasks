#pragma once
#include "command.h"
#include <string>

class AccountFacade;

class CreateAccount : public Command {
public:
  CreateAccount(AccountFacade &, std::string name);
  void Execute() override;
  void Undo() override;

private:
  AccountFacade &m_facade;
  std::string m_name;
  size_t m_id;
};