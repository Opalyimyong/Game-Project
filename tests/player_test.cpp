#include "player.h"
#include "building.h"

#include <cassert>
#include <iostream>

class TestBuilding : public Building
{
public:
    TestBuilding(int id, Player *owner, const std::vector<int> &location_node)
        : Building(id, owner, location_node, {TransportType::Resource, 0.0}) {}

    Item getItem() const override { return item_; }
    bool upgrade() override { return false; }
    void processWaste() override {}
    bool process() override { return true; }
    int getCurrentValue() const override { return 0; }
};

int main()
{
    Player player("test");
    assert(player.getCoins() == 500.0);
    assert(player.getActionPoints() == 3);

    // executeManualAction should use one AP and charge cost
    assert(player.executeManualAction(10));
    assert(player.getActionPoints() == 2);
    assert(player.getCoins() == 490.0);

    // failing cost should not consume AP
    assert(!player.executeManualAction(1000));
    assert(player.getActionPoints() == 2);

    player.resetActionPoints();
    assert(player.getActionPoints() == 3);

    assert(player.payAutoRunCost(50));
    assert(player.getCoins() == 440.0);
    assert(!player.payAutoRunCost(1000));
    assert(player.getCoins() == 440.0);

    assert(player.disposeWaste(10));
    assert(player.getActionPoints() == 2);
    assert(player.getCoins() == 410.0);

    player.addWaste(100);
    assert(player.getEfficiencyModifier() == 0.8);
    assert(player.isGameOver());

    Player owner("owner");
    assert(owner.getActionPoints() == 3);

    // Building toggle behavior with a valid owner
    TestBuilding buildA(1, &owner, {0, 1});
    assert(buildA.IsActive());
    assert(buildA.toggleStatus(false));
    assert(!buildA.IsActive());

    assert(buildA.toggleStatus(true));
    assert(buildA.IsActive());
    assert(owner.getActionPoints() == 2);

    // Already active, request to activate again should fail
    assert(!buildA.toggleStatus(true));
    assert(buildA.IsActive());

    // Building with no owner cannot reactivate
    TestBuilding buildB(2, nullptr, {2});
    assert(buildB.IsActive());
    assert(buildB.toggleStatus(false));
    assert(!buildB.toggleStatus(true));
    assert(!buildB.IsActive());

    // ResourcePlant tests
    Player resourceOwner("resourceOwner");
    ResourcePlant resourcePlant(10, &resourceOwner, {10}, ResourceType::Coal, 2);
    assert(resourcePlant.getType() == ResourceType::Coal);
    assert(resourcePlant.getItem().type == TransportType::Resource);
    assert(resourcePlant.process());
    assert(resourcePlant.getItem().amount == 20.0);
    assert(resourcePlant.getWasteOutput() == 13.0);
    assert(resourcePlant.getCurrentValue() == 84);
    assert(!resourcePlant.upgrade());

    // PowerPlant tests
    Player powerOwner("powerOwner");
    PowerPlant powerPlant(20, &powerOwner, {20}, PlantType::SolarPlant);
    assert(powerPlant.getType() == PlantType::SolarPlant);
    assert(powerPlant.getItem().type == TransportType::Energy);
    assert(!powerPlant.addResourceInput({TransportType::Energy, 5.0}));
    assert(powerPlant.addResourceInput({TransportType::Resource, 5.0}));
    assert(powerPlant.process());
    assert(powerPlant.getItem().amount == 5.0);
    powerPlant.clearResourceInput();
    assert(powerPlant.getResourceInput().amount == 0.0);
    assert(powerPlant.getCurrentValue() == 90);
    assert(!powerPlant.upgrade());
    powerOwner.addCoins(250.0);
    assert(powerPlant.upgrade());
    assert(powerPlant.getLevel() == 2);
    assert(powerOwner.getActionPoints() == 2);

    std::cout << "Player and Building tests passed.\n";
    return 0;
}
