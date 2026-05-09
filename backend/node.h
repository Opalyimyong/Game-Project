#pragma once

#include <cmath>
#include <optional>
#include <string>
#include <unordered_map>

enum class NodeType {
    City,
    Power,
    Resource
};

enum class BuildingType {
    None,
    PowerPlant,
    ResourcePlant
};

struct NodePose {
    float x;
    float y;

    bool operator==(const NodePose& other) const {
        return x == other.x && y == other.y;
    }
};

class Node {
public:
    Node(float x, float y, NodeType type) : pose_{x, y}, type_(type), building_(BuildingType::None) {}

    const NodePose& GetPose() const { return pose_; }
    float GetX() const { return pose_.x; }
    float GetY() const { return pose_.y; }
    NodeType GetType() const { return type_; }
    BuildingType GetBuilding() const { return building_; }

    void SetBuilding(BuildingType building) { building_ = building; }

    static float GetDistanceP(const NodePose& a, const NodePose& b) {
        const float dx = a.x - b.x;
        const float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    static float GetDistanceN(const Node& a, const Node& b) {
        return GetDistanceP(a.GetPose(), b.GetPose());
    }

private:
    NodePose pose_;
    NodeType type_;
    BuildingType building_;
};

float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB);

