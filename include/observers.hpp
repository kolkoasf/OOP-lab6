#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include "npc.hpp"

class ConsoleObserver : public Observer {
 private:
  ConsoleObserver() = default;
  static std::shared_ptr<ConsoleObserver> instance;

 public:
  static std::shared_ptr<ConsoleObserver> get() {
    if (!instance) {
      instance = std::shared_ptr<ConsoleObserver>(new ConsoleObserver());
    }
    return instance;
  }

  void OnFight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender,
               bool attacker_win) override;
};

class FileObserver : public Observer {
 private:
  std::ofstream file;

 public:
  FileObserver(const std::string& filename = "log.txt");
  ~FileObserver();

  void OnFight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender,
               bool attacker_win) override;
};