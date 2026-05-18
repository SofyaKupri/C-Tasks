#pragma once
#include "command.h"
#include <chrono>
#include <iostream>
#include <memory>

class TimerDecorator : public Command {
public:
  explicit TimerDecorator(std::unique_ptr<Command> cmd)
      : m_command(std::move(cmd)) {}
  void Execute() override {
    auto start = std::chrono::system_clock::now();
    m_command->Execute();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "elapsed " << elapsed.count() << " ms\n";
  }
  void Undo() override { m_command->Undo(); }

private:
  std::unique_ptr<Command> m_command;
};
