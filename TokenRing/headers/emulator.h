#pragma once
#include "data.h"
#include "main_agent.h"
#include "echo_agent.h"
#include "simple_agent.h"
#include <algorithm>

class BaseAgent;

class Emulator : public IListener {
public:
    Emulator();
    bool OnAddAgent(BaseAgent* agent) override;
    bool OnDeleteAgent(unsigned agent_id) override;
    void Run();

    bool HasActiveAgents() const override;
    bool OnSendMessage(unsigned from, unsigned to, std::string&& msg) override;
    void OnExit() override;

    ~Emulator();
private:
  Token token_;
  std::vector<BaseAgent *> agents_;
  bool running_ = false;
};