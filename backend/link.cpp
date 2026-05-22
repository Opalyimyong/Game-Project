#include "link.h"
#include <algorithm>

Link::Link(const Node &node_a, const Node &node_b)
    : node_a_(node_a), node_b_(node_b), distance_(Node::GetDistanceN(node_a, node_b)) {}
    
const Node &Link::GetNodeA() const
{
    return node_a_;
}

const Node &Link::GetNodeB() const
{
    return node_b_;
}

double Link::GetDistance() const {
    return distance_;
};

const Item& Link::GetItem() const {
    return item;
}

void Link::SetItem(const Item& i) {
    item = i;
}

void LinkManager::AddLink(const Node &node_a, const Node &node_b)
{
    links_.emplace_back(node_a, node_b);
    // Compute initial item for the newly created link.
    OperateLink(node_a, node_b);
}

const std::vector<Link> &LinkManager::GetAllLinks() const
{
    return links_;
}

std::vector<Link> LinkManager::GetLinksForNode(const Node &node) const
{
    std::vector<Link> result;
    for (const auto &link : links_)
    {
        if (&link.GetNodeA() == &node || &link.GetNodeB() == &node)
        {
            result.push_back(link);
        }
    }
    return result;
}

std::optional<Link> LinkManager::GetLinkBetweenNodes(const Node &node_a, const Node &node_b) const
{
    for (const auto &link : links_)
    {
        if ((&link.GetNodeA() == &node_a && &link.GetNodeB() == &node_b) ||
            (&link.GetNodeA() == &node_b && &link.GetNodeB() == &node_a))
        {
            return link;
        }
    }
    return std::nullopt;
}

void LinkManager::OperateLink(const Node &node_a, const Node &node_b)
{
    for (auto &link : links_)
    {
        if ((&link.GetNodeA() == &node_a && &link.GetNodeB() == &node_b) ||
            (&link.GetNodeA() == &node_b && &link.GetNodeB() == &node_a))
        {
            Item out{TransportType::Resource, 0.0};
            bool linkStatus = false;
            NodeType aType = node_a.GetType();
            NodeType bType = node_b.GetType();

            // Resource -> Power: send resource amount
            if (aType == NodeType::Resource && bType == NodeType::Power)
            {
                if (node_a.HasBuilding())
                {
                    linkStatus = node_a.GetBuilding()->process();
                    out = node_a.GetBuilding()->getItem();
                    out.type = TransportType::Resource;
                }
            } //swap a<->b
            else if (bType == NodeType::Resource && aType == NodeType::Power)
            {
                if (node_b.HasBuilding())
                {
                    out = node_b.GetBuilding()->getItem();
                    out.type = TransportType::Resource;
                }
            }
       

            // Power -> City: send energy amount
            else if (aType == NodeType::Power && bType == NodeType::City)
            {
                if (node_a.HasBuilding())
                { 
                    
                    out = node_a.GetBuilding()->getItem();
                    out.type = TransportType::Energy;
                }
            }

        
            else if (bType == NodeType::Power && aType == NodeType::City)
            {
                if (node_b.HasBuilding())
                {
                    out = node_b.GetBuilding()->getItem();
                    out.type = TransportType::Energy;
                }
            }
            // Resource <-> City direct transport (if needed)
            else if ((aType == NodeType::Resource && bType == NodeType::City) ||
                     (aType == NodeType::City && bType == NodeType::Resource))
            {
                // Send resource toward city
                const Node &resNode = (aType == NodeType::Resource) ? node_a : node_b;
                if (resNode.HasBuilding())
                {
                    out = resNode.GetBuilding()->getItem();
                    out.type = TransportType::Resource;
                }
            }

            link.SetItem(out);
            return;
        }
    }
}
