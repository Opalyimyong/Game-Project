#pragma once

#include <cmath>
#include <memory>
#include "building.h"

enum class NodeType {
    City,
    Power,
    Resource
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
    Node(float x, float y, NodeType type)
        : pose_{x, y}, type_(type), building_(nullptr) {}

    const NodePose& GetPose() const { return pose_; }
    float GetX() const { return pose_.x; }
    float GetY() const { return pose_.y; }
    NodeType GetType() const { return type_; }
    Building* GetBuilding() const { return building_.get(); }
    bool HasBuilding() const { return building_ != nullptr; }

    void SetBuilding(std::unique_ptr<Building> building) {
        building_ = std::move(building);
    }

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
    std::unique_ptr<Building> building_;
};

float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB);

