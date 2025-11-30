#pragma once

#include <memory>
#include "npc.hpp"

struct Bear : public NPC {
 public:
  Bear(int _x, int _y, const std::string& _name);
  Bear(std::istream& is);

  bool Accept(std::shared_ptr<Visitor> visitor) override;

  void Print() const override;

  void Save(std::ostream& os) const override;
};