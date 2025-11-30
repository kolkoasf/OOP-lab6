#include "npc_factory.hpp"

std::atomic<int> NPCFactory::bear_count{0};
std::atomic<int> NPCFactory::werewolf_count{0};
std::atomic<int> NPCFactory::rogue_count{0};

std::string NPCFactory::GenerateBearName() {
  return "Bear" + std::to_string(bear_count.fetch_add(1));
}

std::string NPCFactory::GenerateWerewolfName() {
  return "Werewolf" + std::to_string(werewolf_count.fetch_add(1));
}

std::string NPCFactory::GenerateRogueName() {
  return "Rogue" + std::to_string(rogue_count.fetch_add(1));
}

std::shared_ptr<NPC> NPCFactory::CreateBear(int x, int y) {
  return std::make_shared<Bear>(x, y, GenerateBearName());
}

std::shared_ptr<NPC> NPCFactory::CreateWerewolf(int x, int y) {
  return std::make_shared<Werewolf>(x, y, GenerateWerewolfName());
}

std::shared_ptr<NPC> NPCFactory::CreateRogue(int x, int y) {
  return std::make_shared<Rogue>(x, y, GenerateRogueName());
}

std::shared_ptr<NPC> NPCFactory::CreateByType(NpcType type, int x, int y) {
  x = std::max(0, std::min(500, x));
  y = std::max(0, std::min(500, y));
  switch (type) {
    case BearType:
      return CreateBear(x, y);
    case WerewolfType:
      return CreateWerewolf(x, y);
    case RogueType:
      return CreateRogue(x, y);
    default:
      return nullptr;
  }
}

std::shared_ptr<NPC> NPCFactory::LoadFromStream(std::istream& is) {
  int typeInt;
  if (!(is >> typeInt)) {
    return nullptr;
  }
  NpcType type = static_cast<NpcType>(typeInt);
  switch (type) {
    case BearType:
      return std::make_shared<Bear>(is);
    case WerewolfType:
      return std::make_shared<Werewolf>(is);
    case RogueType:
      return std::make_shared<Rogue>(is);
    default:
      return nullptr;
  }
}

void NPCFactory::ResetCounters() {
  bear_count = 0;
  werewolf_count = 0;
  rogue_count = 0;
}
