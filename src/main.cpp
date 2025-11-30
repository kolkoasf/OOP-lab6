#include <iostream>
#include <limits>
#include "game_manager.hpp"
#include "npc.hpp"
#include "npc_factory.hpp"
#include "observers.hpp"

int main() {
  GameManager game;
  auto consoleObs = ConsoleObserver::get();
  game.AddObserver(consoleObs);
  auto fileObs = std::make_shared<FileObserver>();
  game.AddObserver(fileObs);

  int choice;
  while (true) {
    std::cout << "\n== Main Menu ==\n";
    std::cout << "1. Add NPC\n";
    std::cout << "2. Save NPCs to file\n";
    std::cout << "3. Load NPCs from file\n";
    std::cout << "4. List all NPCs (" << game.GetNpcCount() << ")\n";
    std::cout << "5. Run Battle Simulation\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    switch (choice) {
      case 1: {
        int type;
        int x, y;
        std::cout << "Select NPC type:\n1=Bear\n2=Werewolf\n3=Rogue\nChoice: ";
        std::cin >> type;
        if (type < 1 || type > 3 || std::cin.fail()) {
          std::cout << "Invalid type.\n";
          std::cin.clear();
          std::cin.ignore();
          continue;
        }
        std::cout << "Enter X coordinate (0-500): ";
        std::cin >> x;
        std::cout << "Enter Y coordinate (0-500): ";
        std::cin >> y;
        if (x < 0 || x > 500 || y < 0 || y > 500 || std::cin.fail()) {
          std::cout << "Invalid coordinates.\n";
          std::cin.clear();
          std::cin.ignore();
          continue;
        }
        auto npc = NPCFactory::CreateByType(static_cast<NpcType>(type), x, y);
        if (npc) {
          game.AddNpc(npc);
          std::cout << "Added NPC: " << npc->GetName() << " (Type: " << npc->GetTypeStr() << ")\n";
        } else {
          std::cout << "Error creating NPC.\n";
        }
        break;
      }
      case 2: {
        std::string filename;
        std::cout << "Enter filename to Save (default: npc.txt): ";
        std::cin >> filename;
        if (filename.empty()) {
          filename = "npc.txt";
        }
        if (game.SaveToFile(filename)) {
          std::cout << "NPCs saved to " << filename << "\n";
        } else {
          std::cout << "Failed to Save NPCs.\n";
        }

        break;
      }
      case 3: {
        std::string filename;
        std::cout << "Enter filename to load (default: npc.txt): ";
        std::cin >> filename;
        if (filename.empty()) {
          filename = "npc.txt";
        }
        if (game.LoadFromFile(filename)) {
          std::cout << "NPCs loaded from " << filename << "\n";
        } else {
          std::cout << "Failed to load NPCs.\n";
        }

        break;
      }
      case 4:
        game.PrintAll();

        break;
      case 5: {
        size_t max_distance;
        std::cout << "Enter battle maximum distance: ";
        std::cin >> max_distance;
        if (std::cin.fail()) {
          std::cout << "Invalid input.\n";
          std::cin.clear();
          std::cin.ignore();
          continue;
        }
        game.SimulateBattle(max_distance);

        break;
      }
      case 0:
        std::cout << "Exiting...\n";
        return 0;
      default:
        std::cout << "Invalid choice. Try again.\n";
        break;
    }
  }
}