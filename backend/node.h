#pragma once

#include <cmath>
#include <memory>
#include "building.h"
#include "game_const.h"
#include "player.h"

enum class NodeType {
    City,
    Power,
    Resource
};
enum class PassiveType {
    Solar,
    Wind,
    Water
};
enum class SourceType {
    Passive,
    Resource_Based
};

struct NodePose {
    float x;
    float y;

    bool operator==(const NodePose& other) const {
        return x == other.x && y == other.y;
    }
};

class Node {
public:
    Node(float x, float y, NodeType type)
        : pose_{x, y}, type_(type), building_(nullptr) {}

    const NodePose& GetPose() const { return pose_; }
    float GetX() const { return pose_.x; }
    float GetY() const { return pose_.y; }
    NodeType GetType() const { return type_; }
    Building* GetBuilding() const { return building_.get(); }
    bool HasBuilding() const { return building_ != nullptr; }
    void SetBuilding(std::unique_ptr<Building> building) {
        building_ = std::move(building);
    }

    virtual ~Node() = default;

    static float GetDistanceP(const NodePose& a, const NodePose& b) {
        const float dx = a.x - b.x;
        const float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    static float GetDistanceN(const Node& a, const Node& b) {
        return GetDistanceP(a.GetPose(), b.GetPose());
    }

protected:
    NodePose pose_;
    NodeType type_;
    std::unique_ptr<Building> building_;
};

float GetDistanceBetweenPoses(const NodePose& poseA, const NodePose& poseB);

class CityNode : public Node {
    public:
        CityNode(float x, float y) : Node(x, y, NodeType::City) {} //Contructor
        float getElectricityPrice() const { return electricity_price_; } //เช็ค rate ค่าไฟ
        float getCurrentDemandTillActive() const { return city_data_.min_Energy - energy_now_; } //ต้องการไฟเท่าไหร่ ถึงจะ Active
        float getCurrentDemandTillFullyPowered() const { return city_data_.max_Energy - energy_now_; } //ต้องการไฟเท่าไหร่ ถึงจะ Active
        float EnergyNow() const { return energy_now_; } //Initial Energy
        bool isPowered() const { return is_powered_; } //ไฟพอไหม
        bool isFullyPowered() const{ return energy_now_ >= city_data_.max_Energy; }; //max หรือยัง
        CityData getCityData() const { return city_data_; } //เอาข้อมูลเมืองมาใช้

        const std::vector<CityContract>& getContracts() const { return contracts_; } //เก็บว่าใครจ่ายอยู่บ้าง
        void newContract(Player* player, float amount); //รับไฟจากใคร เท่าไหร่
        
    private:
        float energy_now_; //ไฟ้ฟ้าในปัจจุบัน
        float current_demand_;  //ความต้องการไฟฟ้าปัจจุบัน
        float electricity_price_;  //ราคาค่าไฟฟ้าต่อหน่วย
        bool is_powered_; //เมือง active หรือยัง
        CityData city_data_; //ข้อมูลเมือง
        std::vector<CityContract> contracts_; //เก็บข้อมูลว่าใครจ่ายอยู่บ้าง และจ่ายเท่าไหร่
};

class PowerPlantNode : public Node {
    public:
        PowerPlantNode(float x, float y) : Node(x, y, NodeType::Power) {} //contructor
        float getSolarIndex() const { return solar_index_; } //passive gain จาก solar
        float getWindIndex() const { return wind_index_; } //passive gain จาก wind
        bool hasWater() const { return has_water_; } //มีแหล่งนํ้าหรือไม่
        float getEnvironmentMultiplier(PlantType type) const; 
        void checkFactoryType() const{} 

    private:
        float solar_index_;
        float wind_index_;
        bool has_water_;
        SourceType source_type_;
};

class ResourceNode : public Node {
    public:
        ResourceNode(float x, float y) : Node(x, y, NodeType::Resource) {} //contructor
        ResourceType getResourceType() const { return resource_type_; } //ประเภทของทรัพยากร
        int getPurityLevel() const { return purity_level_; } //ระดับความบริสุทธิ์ของทรัพยากร
        void resourceOwner(Player* player) { owner_ = player; } //กําหนดเจ้าของทรัพยากร
        float getPurityMultiplier() const{}

    private:
        ResourceType resource_type_;
        int purity_level_;
        bool explored_;
        Player* owner_;

};

