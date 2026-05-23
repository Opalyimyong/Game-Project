#include "node.h"

Node::Node(int x, int y, NodeType type)
    : pose_{x, y}, type_(type), building_(nullptr) {}

double GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB) {
    return Node::GetDistanceP(poseA, poseB);
}

void CityNode::newContract(Player* player, double amount) {
    CityContract contract{player, amount};
    contracts_.push_back(contract);
}

void PowerPlantNode::checkInputType()
{
    if (HasBuilding())
    {
        Building *building = GetBuilding();
        if (auto *powerPlant = dynamic_cast<PowerPlant *>(building))
        {
            if (
            powerPlant->getType() == PlantType::SolarPlant || 
            powerPlant->getType() == PlantType::WindPlant || 
            powerPlant->getType() == PlantType::HydroPlant)
            {
                source_type_ = SourceType::Passive;
            }
            else
            {
                source_type_ = SourceType::Resource_Based;
            }
        }
    }
}

void PowerPlantNode::fetchPlantType()
{
    if (HasBuilding())
    {
        Building *building = GetBuilding();
        if (auto *powerPlant = dynamic_cast<PowerPlant *>(building))
        {
            plant_type_ = powerPlant->getType();
        }
    }
}

void ResourceNode::fetchResourceType()
{
    if (HasBuilding())
    {
        Building *building = GetBuilding();
        if (auto *resourcePlant = dynamic_cast<ResourcePlant *>(building))
        {
            resource_type_ = resourcePlant->getType();
        }
    }
}
void PowerPlantNode::SetBuilding(std::unique_ptr<Building> building)
{
    building_ = std::move(building);
    checkInputType();
    fetchPlantType();

    if(source_type_ == SourceType::Passive)
    {
        if (plant_type_ == PlantType::SolarPlant)
        {
            building_->addResourceInput({TransportType::Resource, getSolarIndex()});
        }
        else if (plant_type_ == PlantType::WindPlant)
        {
            building_->addResourceInput({TransportType::Resource, getWindIndex()});
        }
        else if (plant_type_ == PlantType::HydroPlant)
        {
            building_->addResourceInput({TransportType::Resource, hasWater() ? 5.0 : 0.0}); // สมมติว่าถ้ามีน้ำก็ได้ 5 หน่วย ถ้าไม่มีน้ำก็ไม่ได้อะไร
        }
    }
    else if (source_type_ == SourceType::Resource_Based)
    {
        // รอรับ resource input จาก link แล้วค่อยเช็คว่าทรัพยากรที่ได้รับตรงกับที่โรงไฟฟ้าต้องการไหม
    }
}
void CityNode::setEnergyRange()
{
    if (city_data_.type == CityType::Small)
    {
        city_data_.min_Energy = 20.0;
        city_data_.max_Energy = 50.0;
        city_data_.Elec_Charge = 1.0;
    }
    else if (city_data_.type == CityType::Big)
    {
        city_data_.min_Energy = 50.0;
        city_data_.max_Energy = 120.0;
        city_data_.Elec_Charge = 1.5;
    }
}