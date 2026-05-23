#pragma once

#include "node.h"
#include <vector>
#include <optional>
#include <string>


class Link
{
public:
    Link(Node* node_a, Node* node_b, Player* owner);

    Node* GetNodeA() const;
    Node* GetNodeB() const;
    Player* GetOwner() const;
    double GetDistance() const;

private:
    Node* node_a_;
    Node* node_b_;
    Player* owner_;
    double distance_;
};

class LinkManager
{
public:
    void AddLink(Node& node_a, Node& node_b);
    void OperateLink(Node& node_a, Node& node_b);
    const std::vector<Link>& GetAllLinks() const;
    std::vector<Link> GetLinksForNode(const Node& node) const;
    std::optional<Link> GetLinkBetweenNodes(const Node& node_a, const Node& node_b) const;
private:
    std::vector<Link> links_;
};
