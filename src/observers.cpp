#include <iostream>
#include <sstream>

#include "observers.hpp"

std::shared_ptr<ConsoleObserver> ConsoleObserver::instance = nullptr;

void ConsoleObserver::OnFight(const std::shared_ptr<NPC> attacker,
                              const std::shared_ptr<NPC> defender, bool attacker_win) {
  if (attacker_win) {
    std::cout << " [KILL] " << attacker->GetTypeStr() << " '" << attacker->GetName() << "' killed "
              << defender->GetTypeStr() << " '" << defender->GetName() << "'" << std::endl;
  }
}

FileObserver::FileObserver(const std::string& filename) {
  file.open(filename, std::ios::app);
  if (file.is_open()) {
    file << "\n=== Battle Log ===\n";
  }
}

FileObserver::~FileObserver() {
  if (file.is_open()) {
    file << "\n";
    file.close();
  }
}

void FileObserver::OnFight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender,
                           bool attacker_win) {
  if (!file.is_open())
    return;

  if (attacker_win) {
    file << "[KILL] " << attacker->GetTypeStr() << " '" << attacker->GetName() << "' killed "
         << defender->GetTypeStr() << " '" << defender->GetName() << "'\n";
    file.flush();
  }
}