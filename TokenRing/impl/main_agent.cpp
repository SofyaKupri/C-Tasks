#include "../headers/main_agent.h"
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