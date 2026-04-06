#include "command.h"
#include "../facade/account_facade.h"

class DepositCommand : public Command {
public:
    DepositCommand(AccountFacade& facade, size_t id, double amount) 
        : m_facade(facade), m_id(id), m_amount(amount) {}
    void Execute() override {
        if (m_amount > 0) {
            m_facade.Deposit(m_id, m_amount);
        } else {
            m_facade.Withdraw(m_id, -m_amount);
        }
    }
    void Undo() override {
        if (m_amount < 0) {
            m_facade.Deposit(m_id, m_amount);
        } else {
            m_facade.Withdraw(m_id, -m_amount);
        }
    }

private:
    AccountFacade& m_facade;
    size_t m_id;
    double m_amount;
};