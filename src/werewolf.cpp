#include "werewolf.hpp"

Werewolf::Werewolf(int _x, int _y, const std::string& _name) : NPC(WerewolfType, _x, _y, _name) {
}

Werewolf::Werewolf(std::istream& is) : NPC(WerewolfType, is) {
}

bool Werewolf::Accept(std::shared_ptr<Visitor> visitor) {
  return visitor->VisitWerewolf(std::static_pointer_cast<Werewolf>(shared_from_this()));
}

void Werewolf::Print() const {
  std::cout << "Werewolf: " << *this << std::endl;
}

void Werewolf::Save(std::ostream& os) const {
  os << (int) WerewolfType << " " << x << " " << y << " " << name << std::endl;
}