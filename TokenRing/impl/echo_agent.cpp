#include "../headers/echo_agent.h"

EchoAgent::EchoAgent(unsigned id, Token &token) : BaseAgent(id, token) {};

std::string EchoAgent::ReverseMessege(const std::string &s) {
  std::vector<std::string> words;
  std::istringstream iss(s);
  std::string cur;
  while (iss >> cur) {
    words.push_back(cur);
  }
  std::string res = "";
  for (int i = (int)words.size() - 1; i >= 0; i--) {
    res += words[i];
    if (i > 0) {
      res += " ";
    }
  }
  return res;
}

Data &EchoAgent::Process(Data &recv_data) {
  if (recv_data.data_type == DataType::kToken) {
    if (HasDataToSend()) {
      return NextMessage();
    }
    return GetToken();
  }
  Message &message = static_cast<Message &>(recv_data);
  int sender_id = message.header.sender_id;
  int receiver_id = message.header.reciever_id;
  if (sender_id == (int)Id()) {
    PopMessage();
    return GetToken();
  }
  if (receiver_id == (int)Id() || receiver_id == -1) {
    SaveChunk(message);
    if (IsFull(sender_id)) {
      std::string res = GetFullMessege(sender_id);
      PrintMessage(sender_id, Id(), res);
      std::string echo = ReverseMessege(res);
      SendMessage(sender_id, std::move(echo));
    }
  }
  return recv_data;
}