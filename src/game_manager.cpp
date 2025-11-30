#include <algorithm>
#include <fstream>
#include <unordered_set>
#include "fight_visitor.hpp"
#include "game_manager.hpp"
#include "npc_factory.hpp"

GameManager::GameManager() {
}

void GameManager::AddNpc(std::shared_ptr<NPC> npc) {
  for (auto& obs : observers_) {
    npc->Subscribe(obs);
  }
  npcs_.push_back(npc);
}

void GameManager::AddObserver(std::shared_ptr<Observer> obs) {
  observers_.push_back(obs);
  for (auto& npc : npcs_) {
    npc->Subscribe(obs);
  }
}

size_t GameManager::GetNpcCount() const {
  return npcs_.size();
}

void GameManager::PrintAll() const {
  std::cout << "Total: " << npcs_.size() << " NPCs\n" << std::endl;
  for (const auto& npc : npcs_) {
    npc->Print();
  }
}

bool GameManager::SaveToFile(const std::string& filename) {
  std::ofstream ofs(filename);
  if (!ofs) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }
  ofs << npcs_.size() << std::endl;
  for (const auto& npc : npcs_) {
    npc->Save(ofs);
  }
  return true;
}

bool GameManager::LoadFromFile(const std::string& filename) {
  std::ifstream ifs(filename);
  if (!ifs) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }
  size_t count;
  if (!(ifs >> count)) {
    std::cerr << "Error reading NPC count\n";
    return false;
  }

  npcs_.clear();
  for (size_t i = 0; i < count; ++i) {
    auto npc = NPCFactory::LoadFromStream(ifs);
    if (npc) {
      AddNpc(npc);
    }
  }
  return true;
}

void GameManager::SimulateBattle(size_t distance) {
  std::unordered_set<std::shared_ptr<NPC>> dead_npcs;

  auto npcs_copy = npcs_;

  for (size_t i = 0; i < npcs_copy.size(); ++i) {
    auto& attacker = npcs_copy[i];

    if (dead_npcs.find(attacker) != dead_npcs.end()) {
      continue;
    }

    for (size_t j = 0; j < npcs_copy.size(); ++j) {
      if (i == j) {
        continue;
      }
      auto& defender = npcs_copy[j];

      if (dead_npcs.find(defender) != dead_npcs.end()) {
        continue;
      }

      if (attacker->IsClose(defender, distance)) {
        FightVisitor visitor(attacker);
        if (defender->Accept(std::make_shared<FightVisitor>(attacker))) {
          dead_npcs.insert(defender);
          attacker->FightNotify(defender, true);
        }
      }
    }
  }

  std::vector<std::shared_ptr<NPC>> survivors;
  survivors.reserve(npcs_.size() - dead_npcs.size());
  for (const auto& npc : npcs_) {
    if (dead_npcs.find(npc) == dead_npcs.end()) {
      survivors.push_back(npc);
    }
  }
  npcs_ = std::move(survivors);
}