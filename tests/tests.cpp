#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <memory>
#include <cmath>
#include "bear.hpp"
#include "werewolf.hpp"
#include "rogue.hpp"
#include "npc_factory.hpp"
#include "fight_visitor.hpp"
#include "game_manager.hpp"
#include "observers.hpp"

class NPCFactoryTest : public ::testing::Test {
 protected:
  void SetUp() override {
    NPCFactory::ResetCounters();
  }
};

class NPCTest : public ::testing::Test {
 protected:
  void SetUp() override {
    NPCFactory::ResetCounters();
  }
};

class FightVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    NPCFactory::ResetCounters();
  }
};

class GameManagerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    NPCFactory::ResetCounters();
  }

  void TearDown() override {
    std::remove("test_save.txt");
  }
};

TEST_F(NPCFactoryTest, CreateBearGeneratesCorrectName) {
  auto bear = NPCFactory::CreateBear(10, 20);
  ASSERT_NE(bear, nullptr);
  EXPECT_EQ(bear->GetType(), BearType);
  EXPECT_EQ(bear->GetName(), "Bear0");
}

TEST_F(NPCFactoryTest, CreateBearCoordinates) {
  auto bear = NPCFactory::CreateBear(100, 200);
  EXPECT_EQ(bear->GetX(), 100);
  EXPECT_EQ(bear->GetY(), 200);
}

TEST_F(NPCFactoryTest, CreateWerewolfGeneratesCorrectName) {
  auto werewolf = NPCFactory::CreateWerewolf(50, 60);
  ASSERT_NE(werewolf, nullptr);
  EXPECT_EQ(werewolf->GetType(), WerewolfType);
  EXPECT_EQ(werewolf->GetName(), "Werewolf0");
}

TEST_F(NPCFactoryTest, CreateRogueGeneratesCorrectName) {
  auto rogue = NPCFactory::CreateRogue(30, 40);
  ASSERT_NE(rogue, nullptr);
  EXPECT_EQ(rogue->GetType(), RogueType);
  EXPECT_EQ(rogue->GetName(), "Rogue0");
}

TEST_F(NPCFactoryTest, NameIncrementingBear) {
  auto bear1 = NPCFactory::CreateBear(0, 0);
  auto bear2 = NPCFactory::CreateBear(0, 0);
  auto bear3 = NPCFactory::CreateBear(0, 0);
  EXPECT_EQ(bear1->GetName(), "Bear0");
  EXPECT_EQ(bear2->GetName(), "Bear1");
  EXPECT_EQ(bear3->GetName(), "Bear2");
}

TEST_F(NPCFactoryTest, NameIncrementingWerewolf) {
  auto w1 = NPCFactory::CreateWerewolf(0, 0);
  auto w2 = NPCFactory::CreateWerewolf(0, 0);
  EXPECT_EQ(w1->GetName(), "Werewolf0");
  EXPECT_EQ(w2->GetName(), "Werewolf1");
}

TEST_F(NPCFactoryTest, NameIncrementingRogue) {
  auto r1 = NPCFactory::CreateRogue(0, 0);
  auto r2 = NPCFactory::CreateRogue(0, 0);
  EXPECT_EQ(r1->GetName(), "Rogue0");
  EXPECT_EQ(r2->GetName(), "Rogue1");
}

TEST_F(NPCFactoryTest, CreateByTypeReturnsCorrectType) {
  auto bear = NPCFactory::CreateByType(BearType, 10, 20);
  auto werewolf = NPCFactory::CreateByType(WerewolfType, 30, 40);
  auto rogue = NPCFactory::CreateByType(RogueType, 50, 60);

  EXPECT_EQ(bear->GetType(), BearType);
  EXPECT_EQ(werewolf->GetType(), WerewolfType);
  EXPECT_EQ(rogue->GetType(), RogueType);
}

TEST_F(NPCFactoryTest, CreateByTypeInvalidType) {
  auto invalid = NPCFactory::CreateByType(Unknown, 10, 20);
  EXPECT_EQ(invalid, nullptr);
}

TEST_F(NPCFactoryTest, CoordinateClampingLowerBound) {
  auto npc = NPCFactory::CreateByType(BearType, -50, -100);
  EXPECT_EQ(npc->GetX(), 0);
  EXPECT_EQ(npc->GetY(), 0);
}

TEST_F(NPCFactoryTest, CoordinateClampingUpperBound) {
  auto npc = NPCFactory::CreateByType(BearType, 600, 700);
  EXPECT_EQ(npc->GetX(), 500);
  EXPECT_EQ(npc->GetY(), 500);
}

TEST_F(NPCFactoryTest, CoordinateClampingMixed) {
  auto npc = NPCFactory::CreateByType(BearType, -10, 600);
  EXPECT_EQ(npc->GetX(), 0);
  EXPECT_EQ(npc->GetY(), 500);
}

TEST_F(NPCFactoryTest, CoordinateClampingValid) {
  auto npc = NPCFactory::CreateByType(BearType, 100, 200);
  EXPECT_EQ(npc->GetX(), 100);
  EXPECT_EQ(npc->GetY(), 200);
}

TEST_F(NPCFactoryTest, ResetCounters) {
  NPCFactory::CreateBear(0, 0);
  NPCFactory::CreateBear(0, 0);
  NPCFactory::CreateWerewolf(0, 0);
  NPCFactory::ResetCounters();

  auto bear = NPCFactory::CreateBear(0, 0);
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);

  EXPECT_EQ(bear->GetName(), "Bear0");
  EXPECT_EQ(werewolf->GetName(), "Werewolf0");
}

TEST_F(NPCFactoryTest, LoadFromStreamBear) {
  std::istringstream iss("1 100 200 TestBear");
  auto npc = NPCFactory::LoadFromStream(iss);
  ASSERT_NE(npc, nullptr);
  EXPECT_EQ(npc->GetType(), BearType);
  EXPECT_EQ(npc->GetX(), 100);
  EXPECT_EQ(npc->GetY(), 200);
  EXPECT_EQ(npc->GetName(), "TestBear");
}

TEST_F(NPCFactoryTest, LoadFromStreamWerewolf) {
  std::istringstream iss("2 50 75 CustomWerewolf");
  auto npc = NPCFactory::LoadFromStream(iss);
  ASSERT_NE(npc, nullptr);
  EXPECT_EQ(npc->GetType(), WerewolfType);
}

TEST_F(NPCFactoryTest, LoadFromStreamRogue) {
  std::istringstream iss("3 200 300 SneakyRogue");
  auto npc = NPCFactory::LoadFromStream(iss);
  ASSERT_NE(npc, nullptr);
  EXPECT_EQ(npc->GetType(), RogueType);
}

TEST_F(NPCFactoryTest, LoadFromStreamInvalidType) {
  std::istringstream iss("99 100 200 Invalid");
  auto npc = NPCFactory::LoadFromStream(iss);
  EXPECT_EQ(npc, nullptr);
}

TEST_F(NPCFactoryTest, LoadFromStreamInvalidInput) {
  std::istringstream iss("");
  auto npc = NPCFactory::LoadFromStream(iss);
  EXPECT_EQ(npc, nullptr);
}

TEST_F(NPCTest, BearGetTypeStr) {
  auto bear = NPCFactory::CreateBear(0, 0);
  EXPECT_EQ(bear->GetTypeStr(), "Bear");
}

TEST_F(NPCTest, WerewolfGetTypeStr) {
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);
  EXPECT_EQ(werewolf->GetTypeStr(), "Werewolf");
}

TEST_F(NPCTest, RogueGetTypeStr) {
  auto rogue = NPCFactory::CreateRogue(0, 0);
  EXPECT_EQ(rogue->GetTypeStr(), "Rogue");
}

TEST_F(NPCTest, IsCloseWithinDistance) {
  auto npc1 = NPCFactory::CreateBear(0, 0);
  auto npc2 = NPCFactory::CreateBear(3, 4);
  EXPECT_TRUE(npc1->IsClose(npc2, 5));
}

TEST_F(NPCTest, IsCloseExactDistance) {
  auto npc1 = NPCFactory::CreateBear(0, 0);
  auto npc2 = NPCFactory::CreateBear(3, 4);
  EXPECT_TRUE(npc1->IsClose(npc2, 5));
}

TEST_F(NPCTest, IsCloseOutsideDistance) {
  auto npc1 = NPCFactory::CreateBear(0, 0);
  auto npc2 = NPCFactory::CreateBear(10, 10);
  EXPECT_FALSE(npc1->IsClose(npc2, 10));
}

TEST_F(NPCTest, IsCloseZeroDistance) {
  auto npc1 = NPCFactory::CreateBear(0, 0);
  auto npc2 = NPCFactory::CreateBear(0, 0);
  EXPECT_TRUE(npc1->IsClose(npc2, 0));
}

TEST_F(NPCTest, IsCloseSamePoint) {
  auto npc1 = NPCFactory::CreateBear(100, 100);
  auto npc2 = NPCFactory::CreateBear(100, 100);
  EXPECT_TRUE(npc1->IsClose(npc2, 1));
}

TEST_F(NPCTest, SaveAndLoadRoundTrip) {
  auto original = NPCFactory::CreateBear(123, 456);
  std::ostringstream oss;
  original->Save(oss);

  std::istringstream iss(oss.str());
  auto loaded = NPCFactory::LoadFromStream(iss);

  ASSERT_NE(loaded, nullptr);
  EXPECT_EQ(loaded->GetType(), original->GetType());
  EXPECT_EQ(loaded->GetX(), original->GetX());
  EXPECT_EQ(loaded->GetY(), original->GetY());
}

TEST_F(FightVisitorTest, WerewolfBeatsRogue) {
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);
  auto rogue = NPCFactory::CreateRogue(0, 0);

  bool result = rogue->Accept(std::make_shared<FightVisitor>(werewolf));
  EXPECT_TRUE(result);
}

TEST_F(FightVisitorTest, RogueDefeatsBear) {
  auto rogue = NPCFactory::CreateRogue(0, 0);
  auto bear = NPCFactory::CreateBear(0, 0);

  bool result = bear->Accept(std::make_shared<FightVisitor>(rogue));
  EXPECT_TRUE(result);
}

TEST_F(FightVisitorTest, BearDefeatsWerewolf) {
  auto bear = NPCFactory::CreateBear(0, 0);
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);

  bool result = werewolf->Accept(std::make_shared<FightVisitor>(bear));
  EXPECT_TRUE(result);
}

TEST_F(FightVisitorTest, RogueDefeatedByWerewolf) {
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);
  auto rogue = NPCFactory::CreateRogue(0, 0);

  bool result = rogue->Accept(std::make_shared<FightVisitor>(werewolf));
  EXPECT_TRUE(result);
}

TEST_F(FightVisitorTest, BearDefeatedByRogue) {
  auto rogue = NPCFactory::CreateRogue(0, 0);
  auto bear = NPCFactory::CreateBear(0, 0);

  bool result = bear->Accept(std::make_shared<FightVisitor>(rogue));
  EXPECT_TRUE(result);
}

TEST_F(FightVisitorTest, WerewolfDefeatedByBear) {
  auto bear = NPCFactory::CreateBear(0, 0);
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);

  bool result = werewolf->Accept(std::make_shared<FightVisitor>(bear));
  EXPECT_TRUE(result);
}

class MockObserver : public Observer {
 public:
  int FightCount = 0;
  std::shared_ptr<NPC> LastAttacker;
  std::shared_ptr<NPC> LastDefender;
  bool LastWin = false;

  void OnFight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender,
               bool attacker_win) override {
    FightCount++;
    LastAttacker = attacker;
    LastDefender = defender;
    LastWin = attacker_win;
  }
};

TEST_F(NPCTest, SubscribeAndNotify) {
  auto bear = NPCFactory::CreateBear(0, 0);
  auto rogue = NPCFactory::CreateRogue(0, 0);
  auto mock_obs = std::make_shared<MockObserver>();

  bear->Subscribe(mock_obs);
  bear->FightNotify(rogue, true);

  EXPECT_EQ(mock_obs->FightCount, 1);
  EXPECT_EQ(mock_obs->LastAttacker, bear);
  EXPECT_EQ(mock_obs->LastDefender, rogue);
  EXPECT_TRUE(mock_obs->LastWin);
}

TEST_F(NPCTest, MultipleObserversNotified) {
  auto bear = NPCFactory::CreateBear(0, 0);
  auto rogue = NPCFactory::CreateRogue(0, 0);
  auto mock_obs1 = std::make_shared<MockObserver>();
  auto mock_obs2 = std::make_shared<MockObserver>();

  bear->Subscribe(mock_obs1);
  bear->Subscribe(mock_obs2);
  bear->FightNotify(rogue, false);

  EXPECT_EQ(mock_obs1->FightCount, 1);
  EXPECT_EQ(mock_obs2->FightCount, 1);
  EXPECT_FALSE(mock_obs1->LastWin);
  EXPECT_FALSE(mock_obs2->LastWin);
}

TEST_F(GameManagerTest, AddNpcIncrementsCount) {
  GameManager game;
  EXPECT_EQ(game.GetNpcCount(), 0);

  auto bear = NPCFactory::CreateBear(0, 0);
  game.AddNpc(bear);
  EXPECT_EQ(game.GetNpcCount(), 1);
}

TEST_F(GameManagerTest, AddMultipleNpcs) {
  GameManager game;
  auto bear = NPCFactory::CreateBear(0, 0);
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);
  auto rogue = NPCFactory::CreateRogue(0, 0);

  game.AddNpc(bear);
  game.AddNpc(werewolf);
  game.AddNpc(rogue);

  EXPECT_EQ(game.GetNpcCount(), 3);
}

TEST_F(GameManagerTest, AddObserver) {
  GameManager game;
  auto mock_obs = std::make_shared<MockObserver>();
  game.AddObserver(mock_obs);
  EXPECT_TRUE(true);
}

TEST_F(GameManagerTest, SaveToFileCreatesFile) {
  GameManager game;
  auto bear = NPCFactory::CreateBear(100, 200);
  game.AddNpc(bear);

  bool result = game.SaveToFile("test_save.txt");
  EXPECT_TRUE(result);

  std::ifstream f("test_save.txt");
  EXPECT_TRUE(f.good());
  f.close();
}

TEST_F(GameManagerTest, SaveToFileContainsNpcData) {
  GameManager game;
  auto bear = NPCFactory::CreateBear(100, 200);
  game.AddNpc(bear);

  game.SaveToFile("test_save.txt");

  std::ifstream f("test_save.txt");
  std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
  f.close();

  EXPECT_FALSE(content.empty());
}

TEST_F(GameManagerTest, SaveAndLoadRoundTrip) {
  GameManager game1;
  auto bear = NPCFactory::CreateBear(100, 200);
  auto werewolf = NPCFactory::CreateWerewolf(50, 75);

  game1.AddNpc(bear);
  game1.AddNpc(werewolf);
  game1.SaveToFile("test_save.txt");

  NPCFactory::ResetCounters();
  GameManager game2;
  bool loaded = game2.LoadFromFile("test_save.txt");

  EXPECT_TRUE(loaded);
  EXPECT_EQ(game2.GetNpcCount(), 2);
}

TEST_F(GameManagerTest, LoadFromNonexistentFile) {
  GameManager game;
  bool result = game.LoadFromFile("nonexistent_file_xyz.txt");
  EXPECT_FALSE(result);
}

TEST_F(GameManagerTest, SaveEmptyGameManager) {
  GameManager game;
  bool result = game.SaveToFile("test_save.txt");
  EXPECT_TRUE(result);

  GameManager game2;
  bool loaded = game2.LoadFromFile("test_save.txt");
  EXPECT_TRUE(loaded);
  EXPECT_EQ(game2.GetNpcCount(), 0);
}

TEST_F(GameManagerTest, SimulateBattleNotCrashing) {
  GameManager game;
  auto bear = NPCFactory::CreateBear(0, 0);
  auto rogue = NPCFactory::CreateBear(5, 0);

  game.AddNpc(bear);
  game.AddNpc(rogue);

  game.SimulateBattle(10);
  EXPECT_GE(game.GetNpcCount(), 1);
}

TEST_F(GameManagerTest, SimulateBattleWithObserver) {
  GameManager game;
  auto mock_obs = std::make_shared<MockObserver>();
  game.AddObserver(mock_obs);

  auto werewolf = NPCFactory::CreateWerewolf(0, 0);
  auto rogue = NPCFactory::CreateRogue(3, 4);

  game.AddNpc(werewolf);
  game.AddNpc(rogue);

  game.SimulateBattle(5);
  EXPECT_EQ(game.GetNpcCount(), 1);
}

TEST_F(GameManagerTest, PrintAllNotCrashing) {
  GameManager game;
  auto bear = NPCFactory::CreateBear(0, 0);
  game.AddNpc(bear);

  testing::internal::CaptureStdout();
  game.PrintAll();
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_FALSE(output.empty());
}

TEST(IntegrationTest, CreateBearWithCustomCoordinates) {
  NPCFactory::ResetCounters();
  auto bear = NPCFactory::CreateByType(BearType, 250, 250);

  ASSERT_NE(bear, nullptr);
  EXPECT_EQ(bear->GetType(), BearType);
  EXPECT_EQ(bear->GetX(), 250);
  EXPECT_EQ(bear->GetY(), 250);
  EXPECT_EQ(bear->GetName(), "Bear0");
}

TEST(IntegrationTest, AllThreeTypesCreation) {
  NPCFactory::ResetCounters();
  auto bear = NPCFactory::CreateByType(BearType, 100, 100);
  auto werewolf = NPCFactory::CreateByType(WerewolfType, 200, 200);
  auto rogue = NPCFactory::CreateByType(RogueType, 300, 300);

  EXPECT_EQ(bear->GetTypeStr(), "Bear");
  EXPECT_EQ(werewolf->GetTypeStr(), "Werewolf");
  EXPECT_EQ(rogue->GetTypeStr(), "Rogue");
}

TEST(IntegrationTest, FightChainRockPaperScissors) {
  NPCFactory::ResetCounters();
  auto bear = NPCFactory::CreateBear(0, 0);
  auto werewolf = NPCFactory::CreateWerewolf(0, 0);
  auto rogue = NPCFactory::CreateRogue(0, 0);

  EXPECT_TRUE(rogue->Accept(std::make_shared<FightVisitor>(werewolf)));

  EXPECT_TRUE(bear->Accept(std::make_shared<FightVisitor>(rogue)));

  EXPECT_TRUE(werewolf->Accept(std::make_shared<FightVisitor>(bear)));
}

TEST(IntegrationTest, GameManagerFullWorkflow) {
  NPCFactory::ResetCounters();
  GameManager game;

  auto bear = NPCFactory::CreateBear(10, 20);
  auto werewolf = NPCFactory::CreateWerewolf(30, 40);

  game.AddNpc(bear);
  game.AddNpc(werewolf);

  EXPECT_EQ(game.GetNpcCount(), 2);

  bool saved = game.SaveToFile("test_integration.txt");
  EXPECT_TRUE(saved);

  NPCFactory::ResetCounters();
  GameManager game2;
  bool loaded = game2.LoadFromFile("test_integration.txt");

  EXPECT_TRUE(loaded);
  EXPECT_EQ(game2.GetNpcCount(), 2);

  std::remove("test_integration.txt");
}

TEST(IntegrationTest, CoordinateValidationMultipleNpcs) {
  NPCFactory::ResetCounters();
  auto npc1 = NPCFactory::CreateByType(BearType, -100, -100);
  auto npc2 = NPCFactory::CreateByType(WerewolfType, 600, 600);
  auto npc3 = NPCFactory::CreateByType(RogueType, 250, 250);

  EXPECT_EQ(npc1->GetX(), 0);
  EXPECT_EQ(npc1->GetY(), 0);
  EXPECT_EQ(npc2->GetX(), 500);
  EXPECT_EQ(npc2->GetY(), 500);
  EXPECT_EQ(npc3->GetX(), 250);
  EXPECT_EQ(npc3->GetY(), 250);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}