#include "fight_visitor.hpp"

bool FightVisitor::DoesAttackWin(NpcType attacker_type, NpcType defender_type) const {
  if (attacker_type == WerewolfType && defender_type == RogueType) {
    return true;
  }
  if (attacker_type == RogueType && defender_type == BearType) {
    return true;
  }
  if (attacker_type == BearType && defender_type == WerewolfType) {
    return true;
  }
  return false;
}

bool FightVisitor::VisitBear(const std::shared_ptr<NPC>& defender) {
  return DoesAttackWin(attacker->GetType(), BearType);
}

bool FightVisitor::VisitWerewolf(const std::shared_ptr<NPC>& defender) {
  return DoesAttackWin(attacker->GetType(), WerewolfType);
}

bool FightVisitor::VisitRogue(const std::shared_ptr<NPC>& defender) {
  return DoesAttackWin(attacker->GetType(), RogueType);
}