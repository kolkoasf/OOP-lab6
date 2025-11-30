#include <memory>
#include <string>
#include <vector>
#include "npc.hpp"
#include "observers.hpp"

class GameManager {
 public:
  GameManager();

  void AddNpc(std::shared_ptr<NPC> npc);

  bool SaveToFile(const std::string& filename);

  bool LoadFromFile(const std::string& filename);

  void PrintAll() const;

  size_t GetNpcCount() const;

  void SimulateBattle(size_t max_distance);

  void AddObserver(std::shared_ptr<Observer> obs);

 private:
  std::vector<std::shared_ptr<NPC>> npcs_;
  std::vector<std::shared_ptr<Observer>> observers_;
};
