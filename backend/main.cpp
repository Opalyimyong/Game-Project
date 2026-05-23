#include "mongoose.h"
#include <iostream>
#include <string>
void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        
        // This line fixes the "WS upgrade expected" issue. 
        // It catches the browser's GET request and upgrades it to a WebSocket.
        mg_ws_upgrade(c, hm, NULL); 

    } else if (ev == MG_EV_WS_OPEN) {
        // Helpful for debugging - tells you when the connection is officially stable
        std::cout << "Client connected successfully!" << std::endl;

    } else if (ev == MG_EV_WS_MSG) {
        struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
        
        // Correct naming: .buf and .len based on {6DACD7B2-715A-4711-95B7-39A0000A4B84}.png
        std::string payload(wm->data.buf, wm->data.len);

        std::cout << "Received: " << payload << std::endl;

        // Trigger your game logic based on the message content
        if (payload.find("START_GAME") != std::string::npos) {
            std::cout << "Initializing game world..." << std::endl;
            
            // Send a response back to the JavaScript
            std::string response = "{\"status\":\"started\"}";
            mg_ws_send(c, response.c_str(), response.length(), WEBSOCKET_OP_TEXT);
        } else if (payload.find("\"action\":\"build\"") != std::string::npos) {
            std::cout << "Player built a structure." << std::endl;
        } else if (payload.find("\"action\":\"link\"") != std::string::npos) {
            std::cout << "Player created a link." << std::endl;
        } else if (payload.find("\"action\":\"end_turn\"") != std::string::npos) {
            std::cout << "Player ended their turn." << std::endl;
        }
    }
}


int main() {
struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    
    // Using port 18888 as decided to avoid conflicts
    mg_http_listen(&mgr, "http://0.0.0.0:18888", fn, NULL); 
    
    std::cout << "Server started on http://localhost:18888" << std::endl;

    for (;;) mg_mgr_poll(&mgr, 100); // Faster polling for better game response
    
    mg_mgr_free(&mgr);
    return 0;
}