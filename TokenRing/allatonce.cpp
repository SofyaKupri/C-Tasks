#include <cstring>
#include <stdio.h>

enum class DataType { kToken, kMessage };

class Data {
public:
  const DataType data_type;
  virtual ~Data() = default;

protected:
  explicit Data(DataType dt) : data_type(dt) {}
};

class Token : public Data {
public:
  explicit Token() : Data(DataType::kToken) {}
};

struct Header {
  int sender_id, reciever_id;
};
struct Chunk {
  unsigned total; /* на сколько всего частей разбили */
  unsigned id;    /* номер текущей части */
  unsigned size;  /* длина строки, записанная в поле data */
};

constexpr size_t MAX_PACKEGE_SIZE = 32;
constexpr size_t MESSAGE_SIZE =
    MAX_PACKEGE_SIZE - (sizeof(Header) + sizeof(Chunk));

class Message : public Data {
public:
  Header header;
  Chunk chunk;

  char data[MESSAGE_SIZE]{'\0'};
  explicit Message(int sender_id, int reciever_id, const char *s, int id,
                   int total);
};

Message::Message(int sender_id, int reciever_id, const char *s, int id,
                 int total)
    : Data(DataType::kMessage) {
  header.sender_id = sender_id;
  header.reciever_id = reciever_id;

  chunk.total = total;
  chunk.id = id;
  chunk.size = strlen(s);
  for (size_t i = 0; i < chunk.size; i++) {
    data[i] = s[i];
  }
}

#include <iostream>
#include <map>
#include <queue>
#include <string>

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

#include <map>
#include <vector>

class SimpleAgent : public BaseAgent {
public:
  SimpleAgent(unsigned id, Token &token);
  Data &Process(Data &recv_data) override;
};

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

#include <sstream>
class EchoAgent : public BaseAgent {
public:
  EchoAgent(unsigned id, Token &);
  Data &Process(Data &recv_data) override;

private:
  std::string ReverseMessege(const std::string &);
};
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

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
class IListener {
public:
  virtual ~IListener() = default;
  virtual bool OnAddAgent(BaseAgent *agent) = 0;
  virtual bool OnDeleteAgent(unsigned id) = 0;
  virtual bool OnSendMessage(unsigned from, unsigned to, std::string &&msg) = 0;
  virtual bool HasActiveAgents() const = 0;
  virtual void OnExit() = 0;
};

class MainAgent : public BaseAgent {
public:
  MainAgent(unsigned id, Token &);
  Data &Process(Data &recv_data) override;

  void AddListener(IListener *);

private:
  std::vector<IListener *> listeners_;
  bool exit_ = false;
};

MainAgent::MainAgent(unsigned id, Token &token) : BaseAgent(id, token) {}

void MainAgent::AddListener(IListener *l) {
  listeners_.push_back(l);
  l->OnAddAgent(this);
}

Data &MainAgent::Process(Data &recv_data) {
  if (recv_data.data_type == DataType::kMessage) {
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
  bool stop = false;
  if (HasDataToSend()) {
    stop = true;
  } else {
    for (auto listener : listeners_) {
      if (listener->HasActiveAgents()) {
        stop = true;
        break;
      }
    }
  }
  if (stop) {
    if (HasDataToSend()) {
      return NextMessage();
    }
    return GetToken();
  }
  std::string action;
  while (!exit_) {
    std::cout << "> ";
    if (!(std::cin >> action)) {
      for (auto listener : listeners_) {
        listener->OnExit();
      }
      exit_ = true;
      break;
    }
    if (action[0] == 'r') {
      break;
    } else if (action[0] == 'a') {
      unsigned id;
      std::cin >> id;
      BaseAgent *agent = nullptr;
      if (action == "aa" || action == "aadd") {
        agent = new EchoAgent(id, GetToken());
      } else {
        agent = new SimpleAgent(id, GetToken());
      }
      bool ok = false;
      for (auto listener : listeners_) {
        if (listener->OnAddAgent(agent)) {
          ok = true;
        }
      }
      if (!ok) {
        std::cout << "Cannot add Agent with id #" << id << "\n";
        delete agent;
      }
    } else if (action[0] == 'd') {
      unsigned id;
      std::cin >> id;
      if (id == 0) {
        std::cout << "Cannot delete MainAgent\n";
      } else {
        bool ok = false;
        for (auto liestener : listeners_) {
          if (liestener->OnDeleteAgent(id)) {
            ok = true;
          }
        }
        if (!ok) {
          std::cout << "Cannot delete Agent with id #" << id << "\n";
        }
      }
    } else if (action[0] == 's') {
      unsigned sender_id, receiver_id;
      std::cin >> sender_id >> receiver_id;
      std::string message;
      std::getline(std::cin >> std::ws, message);
      bool ok = false;
      for (auto listener : listeners_) {
        std::string tmp = message;
        if (listener->OnSendMessage(sender_id, receiver_id, std::move(tmp))) {
          ok = true;
        }
      }
      if (!ok) {
        if (receiver_id != -1) {
          std::cout << "Cannot send message from agent #" << sender_id
                    << " to agent #" << receiver_id << "\n";
        } else {
          std::cout << "Cannot send message from agent #" << sender_id
                    << " to agent everyone\n";
        }
      }
    } else if (action[0] == 'e') {
      for (auto listener : listeners_) {
        listener->OnExit();
      }
      exit_ = true;
      break;
    }
  }
  return GetToken();
}

#include <algorithm>
class Emulator : public IListener {
public:
  Emulator();
  bool OnAddAgent(BaseAgent *agent) override;
  bool OnDeleteAgent(unsigned agent_id) override;
  void Run();

  bool HasActiveAgents() const override;
  bool OnSendMessage(unsigned from, unsigned to, std::string &&msg) override;
  void OnExit() override;

  ~Emulator();

private:
  Token token_;
  std::vector<BaseAgent *> agents_;
  bool running_ = false;
};

Emulator::Emulator() {
  auto *main_agent = new MainAgent(0, token_);
  agents_.push_back(main_agent);
  main_agent->AddListener(this);
}
bool Emulator::OnAddAgent(BaseAgent *agent) {
  for (const auto *cur_agnet : agents_) {
    if (cur_agnet->Id() == agent->Id()) {
      return false;
    }
  }
  agents_.push_back(agent);
  if (auto *main_agent = dynamic_cast<MainAgent *>(agent)) {
    main_agent->AddListener(this);
  }
  return true;
}
bool Emulator::OnDeleteAgent(unsigned agent_id) {
  if (agent_id == 0) {
    return false;
  }
  auto it = agents_.begin();
  for (; it != agents_.end(); it++) {
    if ((*it)->Id() == agent_id) {
      break;
    }
  }
  if (it != agents_.end()) {
    delete *it;
    agents_.erase(it);
    return true;
  }
  return false;
}

bool Emulator::OnSendMessage(unsigned from, unsigned to, std::string &&msg) {
  auto it = agents_.begin();
  for (; it != agents_.end(); it++) {
    if ((*it)->Id() == from) {
      break;
    }
  }
  if (it != agents_.end()) {
    if (dynamic_cast<EchoAgent *>(*it)) {
      return false;
    }
    (*it)->SendMessage(to, std::move(msg));
    return true;
  }
  return false;
}

bool Emulator::HasActiveAgents() const {
  for (const auto *cur_agent : agents_) {
    if (cur_agent->HasDataToSend()) {
      return true;
    }
  }
  return false;
}

void Emulator::OnExit() { running_ = false; }

void Emulator::Run() {
  if (agents_.empty()) {
    return;
  }
  running_ = true;
  size_t cur_idx = 0;
  Data *cur_data = &agents_[cur_idx]->GetToken();

  while (running_) {
    if (agents_.empty()) {
      break;
    }
    Data &next = agents_[cur_idx]->Process(*cur_data);
    cur_data = &next;
    cur_idx++;
    if (cur_idx >= agents_.size()) {
      cur_idx = 0;
    }
  }
}

Emulator::~Emulator() {
  for (auto *agent : agents_) {
    delete agent;
  }
  agents_.clear();
}
