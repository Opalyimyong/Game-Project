#pragma once

#include "node.h"
#include <vector>
#include <optional>
#include <string>


class Link
{
public:
    Link(const Node &node_a, const Node &node_b);
    const Node& GetNodeA() const;
    const Node& GetNodeB() const;
    double GetDistance() const;

private:
    const Node& node_a_;
    const Node& node_b_;
    double distance_;
    Item item;
};

class LinkManager
{
public:

    void ConfirmAndAddLink(const Node& node_a, const Node& node_b);
    
    void AddLink(const Node& node_a, const Node& node_b);
    const std::vector<Link>& GetAllLinks() const;
    std::vector<Link> GetLinksForNode(const Node& node) const;
    std::optional<Link> GetLinkBetweenNodes(const Node& node_a, const Node& node_b) const;
private:
    std::vector<Link> links_;
};
