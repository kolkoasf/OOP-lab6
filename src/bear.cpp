#include "bear.hpp"

Bear::Bear(int _x, int _y, const std::string& _name) : NPC(BearType, _x, _y, _name) {
}

Bear::Bear(std::istream& is) : NPC(BearType, is) {
}

bool Bear::Accept(std::shared_ptr<Visitor> visitor) {
  return visitor->VisitBear(std::static_pointer_cast<Bear>(shared_from_this()));
}

void Bear::Print() const {
  std::cout << "Bear: " << *this << std::endl;
}

void Bear::Save(std::ostream& os) const {
  os << (int) BearType << " " << x << " " << y << " " << name << std::endl;
}