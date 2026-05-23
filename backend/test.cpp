#include <iostream>
#include <string>

int main() {
    std::string payload = "{\"action\":\"link\",\"from\":{\"r\":2,\"c\":3,\"type\":\"Resource\"},\"to\":{\"r\":5,\"c\":6,\"type\":\"Power\"},\"player\":\"p1\",\"transport_type\":\"Resource\"}";
    
    int from_r = 0, from_c = 0;
    size_t fr_pos = payload.find("\"from\":{\"r\":");
    if (fr_pos != std::string::npos) {
        from_r = std::stoi(payload.substr(fr_pos + 12));
        size_t fc_pos = payload.find("\"c\":", fr_pos);
        if (fc_pos != std::string::npos) from_c = std::stoi(payload.substr(fc_pos + 4));
    }
    
    std::cout << "from_r: " << from_r << ", from_c: " << from_c << std::endl;
    return 0;
}
