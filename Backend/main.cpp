#include <iostream>

#include <boost/asio.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>


#include "Service/Server.hpp"
#include "Service/Session.hpp"

int main() 
{
    try {
        boost::asio::io_context ioc;
        Service::Server server(ioc, 8080, "mongodb://root:root@localhost:27017");
        ioc.run();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}