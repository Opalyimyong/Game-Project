#pragma once

#include "node.h"
#include <vector>
#include <optional>

class Link {
public:
    Link(const Node& node_a, const Node& node_b);

    const Node& GetNodeA() const;
    const Node& GetNodeB() const;
    float GetDistance() const;

    // Earnings calculation based on connected buildings
    float CalculateEarnings() const;

private:
    const Node& node_a_;
    const Node& node_b_;
    float distance_;
};






class LinkManager {
public:
    void AddLink(const Node& node_a, const Node& node_b);
    const std::vector<Link>& GetAllLinks() const;
    std::vector<Link> GetLinksForNode(const Node& node) const;
    std::optional<Link> GetLinkBetweenNodes(const Node& node_a, const Node& node_b) const;

private:
    std::vector<Link> links_;
};
