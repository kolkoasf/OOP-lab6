#pragma once

#include <memory>
#include "npc.hpp"

class FightVisitor : public Visitor {
 private:
  std::shared_ptr<NPC> attacker;

 public:
  explicit FightVisitor(const std::shared_ptr<NPC>& atk) : attacker(atk) {
  }

  bool VisitBear(const std::shared_ptr<NPC>& defender) override;

  bool VisitWerewolf(const std::shared_ptr<NPC>& defender) override;

  bool VisitRogue(const std::shared_ptr<NPC>& defender) override;

 private:
  bool DoesAttackWin(NpcType attacker_type, NpcType defender_type) const;
};