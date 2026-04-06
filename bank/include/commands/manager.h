#pragma once
#include "command.h"
#include <memory>
#include <stack>

class CreateAccount;

class CommandManager {
public:
  void Do(std::unique_ptr<Command> cmd);
  void Undo();

private:
  std::stack<std::unique_ptr<Command>> m_history;
};
