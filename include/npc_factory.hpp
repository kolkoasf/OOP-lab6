#pragma once

#include <algorithm>
#include <atomic>
#include <istream>
#include <memory>
#include <string>

#include "bear.hpp"
#include "npc.hpp"
#include "rogue.hpp"
#include "werewolf.hpp"

class NPCFactory {
 private:
  static std::atomic<int> bear_count;
  static std::atomic<int> werewolf_count;
  static std::atomic<int> rogue_count;

  static std::string GenerateBearName();

  static std::string GenerateWerewolfName();

  static std::string GenerateRogueName();

 public:
  static std::shared_ptr<NPC> CreateBear(int x, int y);

  static std::shared_ptr<NPC> CreateWerewolf(int x, int y);

  static std::shared_ptr<NPC> CreateRogue(int x, int y);

  static std::shared_ptr<NPC> CreateByType(NpcType type, int x, int y);

  static std::shared_ptr<NPC> LoadFromStream(std::istream& is);

  static void ResetCounters();
};
