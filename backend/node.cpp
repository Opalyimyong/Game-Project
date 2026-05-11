#include "node.h"

Node::Node(int x, int y, NodeType type)
    : pose_{x, y}, type_(type), building_(nullptr) {}

float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB) {
    return Node::GetDistanceP(poseA, poseB);
}

void CityNode::newContract(Player* player, float amount) {
    CityContract contract{player, amount};
    contracts_.push_back(contract);
}

void PowerPlantNode::checkFactoryType() {
    if (HasBuilding()) {
        Building* building = GetBuilding();
        if (auto* powerPlant = dynamic_cast<PowerPlant*>(building)) {
            if (powerPlant->getType() == PlantType::SolarPlant
            || powerPlant->getType() == PlantType::WindPlant
            || powerPlant->getType() == PlantType::HydroPlant
        ) { source_type_ = SourceType::Passive;} 
        else { source_type_ = SourceType::Resource_Based;}
        }
    }
}

void PowerPlantNode::fetchPlantType() {
    if (HasBuilding()) {
        Building* building = GetBuilding();
        if (auto* powerPlant = dynamic_cast<PowerPlant*>(building)) {
            plant_type_ = powerPlant->getType();
        }
    }
}

void ResourceNode::fetchResourceType() {
    if (HasBuilding()) {
        Building* building = GetBuilding();
        if (auto* resourcePlant = dynamic_cast<ResourcePlant*>(building)) {
            resource_type_ = resourcePlant->getType();
        }
    }
}

