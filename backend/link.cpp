#include "link.h"
#include <algorithm>

Link::Link(const Node& node_a, const Node& node_b)
    : node_a_(node_a), node_b_(node_b), distance_(Node::GetDistance(node_a, node_b)) {}

const Node& Link::GetNodeA() const {
    return node_a_;
}

const Node& Link::GetNodeB() const {
    return node_b_;
}

float Link::GetDistance() const {
    return distance_;
}

float Link::CalculateEarnings() const {
    BuildingType buildingA = node_a_.GetBuilding();
    BuildingType buildingB = node_b_.GetBuilding();

    // Simple earnings calculation - can be expanded based on game rules
    float earnings = 0.0f;

    // Power plant connected to city generates energy
    if ((buildingA == BuildingType::PowerPlant && node_b_.GetType() == NodeType::City) ||
        (buildingB == BuildingType::PowerPlant && node_a_.GetType() == NodeType::City)) {
        earnings += 10.0f; // base energy generation
    }

    // Resource plant connected to resource node generates resources
    if ((buildingA == BuildingType::ResourcePlant && node_b_.GetType() == NodeType::Resource) ||
        (buildingB == BuildingType::ResourcePlant && node_a_.GetType() == NodeType::Resource)) {
        earnings += 5.0f; // base resource generation
    }

    // Distance penalty - longer links are less efficient
    if (distance_ > 100.0f) {
        earnings *= 0.8f;
    }

    return earnings;
}

void LinkManager::AddLink(const Node& node_a, const Node& node_b) {
    links_.emplace_back(node_a, node_b);
}

const std::vector<Link>& LinkManager::GetAllLinks() const {
    return links_;
}

std::vector<Link> LinkManager::GetLinksForNode(const Node& node) const {
    std::vector<Link> result;
    for (const auto& link : links_) {
        if (&link.GetNodeA() == &node || &link.GetNodeB() == &node) {
            result.push_back(link);
        }
    }
    return result;
}

std::optional<Link> LinkManager::GetLinkBetweenNodes(const Node& node_a, const Node& node_b) const {
    for (const auto& link : links_) {
        if ((&link.GetNodeA() == &node_a && &link.GetNodeB() == &node_b) ||
            (&link.GetNodeA() == &node_b && &link.GetNodeB() == &node_a)) {
            return link;
        }
    }
    return std::nullopt;
}
