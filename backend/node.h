#pragma once

#include <cmath>
#include <memory>
#include "building.h"
#include "game_const.h"
#include "player.h"

enum class NodeType
{
    City,
    Power,
    Resource,
    None
};

struct NodePose
{
    int x;
    int y;

    bool operator==(const NodePose &other) const
    {
        return x == other.x && y == other.y;
    }
};

class Node
{
public:
    //Constructor
    Node(int x, int y, NodeType type)
        : pose_{x, y}, type_(type), building_(nullptr) {}

    //Getter
    const NodePose &GetPose() const { return pose_; }
    int GetX() const { return pose_.x; }
    int GetY() const { return pose_.y; }
    NodeType GetType() const { return type_; }
    Building *GetBuilding() const { return building_.get(); }
    bool HasBuilding() const { return building_ != nullptr; }
    
    //Setter
    virtual void SetBuilding(std::unique_ptr<Building> building) = 0;
    virtual void recieveItem(Player* player, Item item) {}
    virtual void fetchPlantType() {}
    virtual PlantType getPlantType() {}

    virtual ~Node() = default;

    static double GetDistanceN(const Node& a, const Node& b) {
        return GetDistanceP(a.GetPose(), b.GetPose());
    }
    static double GetDistanceP(const NodePose& a, const NodePose& b) {
        const double dx = a.x - b.x;
        const double dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

protected:
    NodePose pose_; //ตำแหน่งของ Node
    NodeType type_; //City, Power, Resource
    std::unique_ptr<Building> building_;
};

double GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB);

class CityNode : public Node {
    public:
        CityNode(int x, int y, CityType city_type) : Node(x, y, NodeType::City), city_data_{city_type} {} //Contructor
        double getElectricityPrice() const { return electricity_price_; } //เช็ค rate ค่าไฟ
        double getCurrentDemandTillActive() const { return city_data_.min_Energy - energy_now_; } //ต้องการไฟเท่าไหร่ ถึงจะ Active
        double getCurrentDemandTillFullyPowered() const { return city_data_.max_Energy - energy_now_; } //ต้องการไฟเท่าไหร่ ถึงจะ Active
        double EnergyNow() const { return energy_now_; } //Initial Energy
        bool isPowered() const { return is_powered_; } //ไฟพอไหม
        bool isFullyPowered() const{ return energy_now_ >= city_data_.max_Energy; }; //max หรือยัง
        CityData getCityData() const { return city_data_; } //เอาข้อมูลเมืองมาใช้

        void recieveItem(Player* player, Item item) override {
            energy_now_ += item.amount; if (energy_now_ >= city_data_.min_Energy) { is_powered_ = true; }
            newContract(player, item.amount * city_data_.Elec_Charge); 
            
        }
        //รับไฟจาก link;
        const std::vector<CityContract>& getContracts() const { return contracts_; } //เก็บว่าใครจ่ายอยู่บ้าง
        void newContract(Player* player, double amount); //รับไฟจากใคร เท่าไหร่
        void setEnergyRange(){}
    private:
        double energy_now_; //ไฟ้ฟ้าในปัจจุบัน realtime
        double current_demand_;  //ความต้องการไฟฟ้าปัจจุบัน realtime
        double electricity_price_;  //ราคาค่าไฟฟ้าต่อหน่วย set ใน map
        bool is_powered_; //เมือง active หรือยัง realtime
        CityData city_data_; //ข้อมูลเมือง max, min, now, small, big
        std::vector<CityContract> contracts_; //who and howmuch he pays
};

class PowerPlantNode : public Node {
    public:
        //contructor
        PowerPlantNode(int x, int y) : Node(x, y, NodeType::Power), source_type_(SourceType::None) {} 
        //getter
        double getSolarIndex() const { return solar_index_; } //passive gain จาก solar
        double getWindIndex() const { return wind_index_; } //passive gain จาก wind
        bool hasWater() const { return has_water_; } //มีแหล่งนํ้าหรือไม่
        double getEnvironmentMultiplier(PlantType type) const; 
        void checkInputType(){}
        void fetchPlantType() override{} //get building data
        PlantType getPlantType() override { return plant_type_; }
        
        void SetBuilding(std::unique_ptr<Building> building) override {}
    //get building data
        void recieveItem(Player* player, Item item) override {itemFromResource_ = item;} //รับ resource input จาก link

    private:
        Item itemFromResource_;
        double solar_index_; //set from map
        double wind_index_; //set from map
        bool has_water_; //set from map
        SourceType source_type_; //Resource_Based, Passive, None
        PlantType plant_type_; //ประเภทของโรงไฟฟ้า from list
};

class ResourceNode : public Node {
    public:
        ResourceNode(int x, int y) : Node(x, y, NodeType::Resource) {} //contructor
        ResourceType getResourceType() const { return resource_type_; } //ประเภทของทรัพยากร
        int getPurityLevel() const { return purity_level_; } //ระดับความบริสุทธิ์ของทรัพยากร
        void resourceOwner(Player* player) { owner_ = player; } //กําหนดเจ้าของทรัพยากร
        double getPurityMultiplier() const{}
        void fetchResourceType(){}//get building data
        //เดี๋ยวต้องทําดึงข้อมูลจาก turn มาใส่ใน owner node ด้วย เพื่อให้รู้ว่าใครเป็นเจ้าของทรัพยากรนี้
        void SetBuilding(std::unique_ptr<Building> building) override {}
    private:
        Item item_;
        ResourceType resource_type_; //set from map
        int purity_level_; //set from map
        Player* owner_; 
};


