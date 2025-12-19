#include "npc.hpp"
#include <iostream>

NPC::NPC(NpcType t, int _x, int _y, const std::string& _name) : type(t), x(_x), y(_y), name(_name) {
  x = std::max(0, std::min(500, x));
  y = std::max(0, std::min(500, y));
}

NPC::NPC(NpcType t, std::istream& is) : type(t) {
  is >> x >> y >> name;
}

std::string NPC::GetTypeStr() const {
  switch (type) {
    case BearType:
      return "Bear";
    case WerewolfType:
      return "Werewolf";
    case RogueType:
      return "Rogue";
    default:
      return "Unknown";
  }
}

void NPC::Subscribe(std::shared_ptr<Observer> observer) {
  observers.push_back(observer);
}

void NPC::FightNotify(const std::shared_ptr<NPC> defender, bool win) {
  for (auto& obs : observers) {
    obs->OnFight(shared_from_this(), defender, win);
  }
}

bool NPC::IsClose(const std::shared_ptr<NPC>& other, size_t distance) const {
  double dx = x - other->x;
  double dy = y - other->y;
  double dist_squared = dx * dx + dy * dy;
  double max_dist_squared = distance * distance;
  return dist_squared <= max_dist_squared;
}

std::ostream& operator<<(std::ostream& os, const NPC& npc) {
  os << "{ x:" << npc.x << ", y:" << npc.y << ", name:\"" << npc.name << "\" }";
  return os;
}