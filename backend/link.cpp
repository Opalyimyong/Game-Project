#include "link.h"
#include <algorithm>

Link::Link(const Node& node_a, const Node& node_b)
    : node_a_(node_a), node_b_(node_b), distance_(Node::GetDistanceN(node_a, node_b)) {}

const Node& Link::GetNodeA() const {
    return node_a_;
}

const Node& Link::GetNodeB() const {
    return node_b_;
}

float Link::GetDistance() const {
    return distance_;
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
