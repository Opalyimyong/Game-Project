#include "node.h"

Node::Node(float x, float y, NodeType type) : pose_{x, y}, type_(type), building_(BuildingType::None) {}


float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB) {
    return Node::GetDistanceP(poseA, poseB);
}
