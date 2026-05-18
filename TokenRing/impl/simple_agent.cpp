#include "../headers/simple_agent.h"

SimpleAgent::SimpleAgent(unsigned id, Token &token) : BaseAgent(id, token) {}

Data &SimpleAgent::Process(Data &recv_data) {
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
    }
  }
  return recv_data;
}