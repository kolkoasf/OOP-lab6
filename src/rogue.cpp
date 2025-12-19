#include "rogue.hpp"

Rogue::Rogue(int _x, int _y, const std::string& _name) : NPC(RogueType, _x, _y, _name) {
}

Rogue::Rogue(std::istream& is) : NPC(RogueType, is) {
}

bool Rogue::Accept(std::shared_ptr<Visitor> visitor) {
  return visitor->VisitRogue(std::static_pointer_cast<Rogue>(shared_from_this()));
}

void Rogue::Print() const {
  std::cout << "Rogue: " << *this << std::endl;
}

void Rogue::Save(std::ostream& os) const {
  os << (int) RogueType << " " << x << " " << y << " " << name << std::endl;
}