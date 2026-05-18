#pragma once
#include "data.h"
#include <queue>
#include <string>
#include <map>
#include <iostream>

class BaseAgent {
public:
  BaseAgent(unsigned id, Token &);
  virtual ~BaseAgent() = default;

  void SendMessage(int reciever_id, std::string &&data);
  unsigned Id() const;
  bool HasDataToSend() const;

  Message &NextMessage();
  void PopMessage();
  Token &GetToken();
  void PrintMessage(int sender, int receiver, const std::string &message);

  virtual Data &Process(Data &recv_data) = 0;

protected:
  std::map<int, std::vector<std::string>> parts_;

  void SaveChunk(const Message &);
  bool IsFull(int sender_id) const;
  std::string GetFullMessege(int sender_id);

private:
  unsigned id_;
  Token &token_;
  std::queue<Message> q_;
};