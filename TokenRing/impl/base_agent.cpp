#include "../headers/base_agent.h"

BaseAgent::BaseAgent(unsigned id, Token &token) : id_(id), token_(token) {}

void BaseAgent::SendMessage(int reciever_id, std::string &&data) {
  int n = data.size();
  int total = (n + MESSAGE_SIZE - 1) / MESSAGE_SIZE;
  for (int i = 0; i < total; i++) {
    int chunk_size = std::min(MESSAGE_SIZE, n - i * MESSAGE_SIZE);
    std::string s = "";
    for (int k = 0; k < chunk_size; k++) {
      s += data[i * MESSAGE_SIZE + k];
    }
    q_.push(Message(id_, reciever_id, s.c_str(), i, total));
  }
}

unsigned BaseAgent::Id() const { return id_; }

bool BaseAgent::HasDataToSend() const { return !q_.empty(); }

Message &BaseAgent::NextMessage() { return q_.front(); }

void BaseAgent::PopMessage() { q_.pop(); }

Token &BaseAgent::GetToken() { return token_; }

void BaseAgent::PrintMessage(int sender, int receiver,
                             const std::string &message) {
  std::cout << "#" << receiver << " recv from #" << sender << ": " << message
            << "\n";
}

void BaseAgent::SaveChunk(const Message &message) {
  int sender_id = message.header.sender_id;

  if (parts_[sender_id].empty()) {
    parts_[sender_id].resize(message.chunk.total);
  }
  parts_[sender_id][message.chunk.id] =
      std::string(message.data, message.chunk.size);
}
bool BaseAgent::IsFull(int sender_id) const {
  auto it = parts_.find(sender_id);
  if (it == parts_.end() || (it->second).empty()) {
    return false;
  }
  for (const std::string &p : (it->second)) {
    if (p.empty()) {
      return false;
    }
  }
  return true;
}
std::string BaseAgent::GetFullMessege(int sender_id) {
  std::string res = "";
  for (const std::string &p : parts_[sender_id]) {
    res += p;
  }
  parts_.erase(sender_id);
  return res;
}