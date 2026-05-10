#include "node.h"
#include "building.h"
Node::Node(float x, float y, NodeType type)
    : pose_{x, y}, type_(type), building_(nullptr) {}

float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB) {
    return Node::GetDistanceP(poseA, poseB);
}

void CityNode::newContract(Player* player, float amount) {
    CityContract contract{player, amount};
    contracts_.push_back(contract);
}

 void PowerPlantNode::checkFactoryType() const {
    if (HasBuilding()) {
        Building* building = GetBuilding();
        if (building->getType() == PlantType:: SolarPlant) {
   
          
          
    }
}
