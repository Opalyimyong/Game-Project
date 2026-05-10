#include "node.h"

Node::Node(float x, float y, NodeType type)
    : pose_{x, y}, type_(type), building_(nullptr) {}

float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB) {
    return Node::GetDistanceP(poseA, poseB);
}

NodePassiveGain PowerPlantNode::getPassiveGainType() const {
    if (solar_index_ > wind_index_ && solar_index_ > 0) {
        return NodePassiveGain::solar;
    } else if (wind_index_ > solar_index_ && wind_index_ > 0) {
        return NodePassiveGain::wind;
    } else if (has_water_) {
        return NodePassiveGain::water;
    } else {
        
        return NodePassiveGain::none;
    }
}