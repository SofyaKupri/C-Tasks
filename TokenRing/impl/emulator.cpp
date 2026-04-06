#include "../headers/emulator.h"

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