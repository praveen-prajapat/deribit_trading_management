//#include "utils.h"
//#include "order_manager.h"
//#include <iostream>
//
//int main() {
//    OrderManager orderManager;
//
//    try {
//        std::string token = UtilityNamespace::authenticate();
//        std::cout << "Access Token: " << token <<'\n'<< std::endl;
//
//        // Place 
//    /*    std::string response1 = orderManager.placeOrder("BTC-PERPETUAL", "buy", 10, 30000);
//        std::cout << "Place Order Response: " << response1 <<'\n'<< std::endl;*/
//
//        // Cancel 
//        std::string orderId = "29241100157";
//      /*  std::string response2 = orderManager.cancelOrder(orderId);
//        std::cout << "Cancel Order Response: " << response2 <<'\n'<< std::endl;*/
//
//        // Modify 
//        /*std::string response3 = orderManager.modifyOrder(orderId, 20, 32000);
//        std::cout << "Modify Order Response: " << response3 <<'\n'<< std::endl;*/
//
//        //Order Book
//        /*std::string response4 = orderManager.getOrderBook("BTC-1NOV24");
//        std::cout << "Order Book Response: " << response4 << '\n' << std::endl;*/
//
//        // Current Positions
//        std::string response5 = orderManager.getCurrentPositions("USDT", "future");
//        std::cout << "Current Positions Response: " << response5 << '\n' << std::endl;
//
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//    std::cout << "Press Enter to exit...";
//    std::cin.get();
//    return 0;
//}

// WebSocket

#include "websocket_server.h"
#include "order_manager.h"
#include <iostream>

int main() {
     OrderManager orderManager;

     std::string response = orderManager.placeOrder("BTC-PERPETUAL", "buy", 50, 40000);
     std::cout << "Order Response: " << response << std::endl;

     // start WebSocket server for streaming orderbook
     WebSocketServer wsServer;
     wsServer.startServer(9002);

     return 0;
 }