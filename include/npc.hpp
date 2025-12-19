#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <vector>

struct NPC;
struct Bear;
struct Werewolf;
struct Rogue;

using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType { Unknown = 0, BearType = 1, WerewolfType = 2, RogueType = 3 };

struct Observer {
  virtual ~Observer() = default;
  virtual void OnFight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender,
                       bool attacker_win) = 0;
};

struct Visitor {
  virtual ~Visitor() = default;

  virtual bool VisitBear(const std::shared_ptr<NPC>& defender) = 0;

  virtual bool VisitWerewolf(const std::shared_ptr<NPC>& defender) = 0;

  virtual bool VisitRogue(const std::shared_ptr<NPC>& defender) = 0;
};

struct NPC : public std::enable_shared_from_this<NPC> {
 protected:
  NpcType type;
  int x;
  int y;
  std::string name;
  std::vector<std::shared_ptr<Observer>> observers;

 public:
  NPC(NpcType t, int _x, int _y, const std::string& _name);
  NPC(NpcType t, std::istream& is);
  virtual ~NPC() = default;

  NpcType GetType() const {
    return type;
  }
  int GetX() const {
    return x;
  }
  int GetY() const {
    return y;
  }
  const std::string& GetName() const {
    return name;
  }

  std::string GetTypeStr() const;

  void Subscribe(std::shared_ptr<Observer> observer);

  void FightNotify(const std::shared_ptr<NPC> defender, bool win);

  virtual bool IsClose(const std::shared_ptr<NPC>& other, size_t distance) const;

  virtual bool Accept(std::shared_ptr<Visitor> visitor) = 0;

  virtual void Print() const = 0;

  virtual void Save(std::ostream& os) const = 0;

  friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
};

std::ostream& operator<<(std::ostream& os, const NPC& npc);