#include <iostream>

#include <boost/asio.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include "Service/Client.hpp"

using boost::asio::ip::tcp;


int main(int argc, char* argv[]) {
    try {
        /* if (argc != 3) {
             std::cerr << "Usage: tcp_client <host> <port>\n";
             return 1;
         }*/

        boost::asio::io_context ioc;
        tcp::resolver resolver(ioc);
        auto endpoints = resolver.resolve("127.0.0.1", "8080");
        Service::Client client(ioc, endpoints);

        ioc.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}