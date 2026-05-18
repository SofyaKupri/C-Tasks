#pragma once
#include "base_agent.h"
#include <sstream>

class EchoAgent : public BaseAgent {
public:
  EchoAgent(unsigned id, Token &);
  Data &Process(Data &recv_data) override;

private:
  std::string ReverseMessege(const std::string &);
};