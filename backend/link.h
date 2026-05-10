#pragma once

#include "node.h"
#include <vector>
#include <optional>
#include <string>

struct LinkPreview {
    const Node* node_a;
    const Node* node_b;
    float distance;
    
    bool is_valid; // whether the link can be created
    std::string preview_info; // description of what would happen
};

class Link {
public:
    Link(const Node& node_a, const Node& node_b);

    const Node& GetNodeA() const;
    const Node& GetNodeB() const;
    float GetDistance() const;

private:
    const Node& node_a_;
    const Node& node_b_;
    float distance_;
    Item item;
};


class LinkManager {
public:

    LinkPreview PreviewLink(const Node& node_a, const Node& node_b);

    void ConfirmAndAddLink(const Node& node_a, const Node& node_b);
    
    void AddLink(const Node& node_a, const Node& node_b);
    const std::vector<Link>& GetAllLinks() const;
    std::vector<Link> GetLinksForNode(const Node& node) const;
    std::optional<Link> GetLinkBetweenNodes(const Node& node_a, const Node& node_b) const;
private:
    std::vector<Link> links_;
};
