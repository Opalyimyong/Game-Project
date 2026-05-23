#include "link.h"
#include <algorithm>

Link::Link(Node* node_a, Node* node_b, Player* owner) 
    : node_a_(node_a), node_b_(node_b), owner_(owner), distance_(Node::GetDistanceN(*node_a, *node_b)) {}

Node* Link::GetNodeA() const { return node_a_; }

Node* Link::GetNodeB() const { return node_b_; }

Player* Link::GetOwner() const { return owner_; }

double Link::GetDistance() const { return distance_; };


void LinkManager::AddLink(Node &node_a, Node &node_b)
{
    links_.emplace_back(&node_a, &node_b, nullptr);
    // Compute initial item for the newly created link.
    OperateLink(node_a, node_b);
}

const std::vector<Link> &LinkManager::GetAllLinks() const
{
    return links_;
}

std::vector<Link> LinkManager::GetLinksForNode(const Node &node) const
{
    std::vector<Link> node_links;
    for (const auto &link : links_)
    {
        if (link.GetNodeA() == &node || link.GetNodeB() == &node)
        {
            node_links.push_back(link);
        }
    }
    return node_links;
}

std::optional<Link> LinkManager::GetLinkBetweenNodes(const Node &node_a, const Node &node_b) const
{
    for (const auto &link : links_)
    {
        if ((link.GetNodeA() == &node_a && link.GetNodeB() == &node_b) ||
            (link.GetNodeA() == &node_b && link.GetNodeB() == &node_a))
        {
            return link;
        }
    }
    return std::nullopt;
}

void LinkManager::OperateLink(Node &node_a, Node &node_b) //อย่าลืมเช็คว่า user ลําดับ input ถูกไหม
{
    for (auto &link : links_)
    {
        if ((link.GetNodeA() == &node_a && link.GetNodeB() == &node_b) ||
            (link.GetNodeA() == &node_b && link.GetNodeB() == &node_a))
        {
            Item out{TransportType::Resource, 0.0};
            NodeType aType = node_a.GetType();
            NodeType bType = node_b.GetType();

            // Resource -> Power: send resource amount
            if (aType == NodeType::Resource && bType == NodeType::Power)
            {
                if (node_a.HasBuilding() && node_b.HasBuilding())
                {
                    if (node_a.getPlantType() == PlantType::CoalPlant 
                    || node_a.getPlantType() == PlantType::GasPlant 
                    || node_a.getPlantType() == PlantType::BiomassPlant)
                    {
                        return; //ถ้าเป็น passive plant จะไม่ส่ง resource ไป ไม่มี item update เข้าไป
                    }
                    bool isSent = node_b.GetBuilding()->addResourceInput(node_a.GetBuilding()->getItem());
                    out = node_a.GetBuilding()->getItem();
                    out.type = TransportType::Resource;
                    node_b.recieveItem(nullptr, out); //รอใส่ player เข้าไป
                }
            }
            
            // Power -> City: send energy amount
            else if (aType == NodeType::Power && bType == NodeType::City)
            {
                if (node_a.HasBuilding() && node_b.HasBuilding())
                { 
                    out = node_a.GetBuilding()->getItem();
                    out.type = TransportType::Energy;
                    out.waste_amount += node_b.GetBuilding()->getItem().waste_amount; //บวก waste ของ powerplant
                    node_b.recieveItem(nullptr, out); //รอใส่ player เข้าไป
                }
            }
            return;
        }
    }
}
