#include "../../include/commands/command.h"
#include "../../include/commands/create_account.h"
#include "../../include/commands/manager.h"
#include "../../include/facade/account_facade.h"

void CommandManager::Do(std::unique_ptr<Command> cmd) {
  cmd->Execute();
  m_history.push(std::move(cmd));
}

void CommandManager::Undo() {
  if (!m_history.empty()) {
    m_history.top()->Undo();
    m_history.pop();
  }
}

CreateAccount::CreateAccount(AccountFacade &facade, std::string name)
    : m_facade(facade), m_name(std::move(name)), m_id(0) {}

void CreateAccount::Execute() {
  auto account = m_facade.CreateAccount(m_name);
  m_id = account->GetId();
}

void CreateAccount::Undo() {
  if (m_id != 0) {
    m_facade.DeleteAccount(m_id);
  }
}