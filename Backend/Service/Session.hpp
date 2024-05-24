#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <string>
#include <list>

#include "Database/Session.hpp"


namespace asio = boost::asio;
namespace beast = boost::beast;     
namespace http = beast::http;         
namespace net = boost::asio;   
using tcp = net::ip::tcp;     
using User = Database::Model::User;
using Message = Database::Model::Message;


namespace Service
{
    class Session : public std::enable_shared_from_this<Session>
    {
        public:
            explicit Session(tcp::socket socket, 
                                Database::Session& dbsession, 
                                std::list<std::shared_ptr<Session>>& sessions);
            void start();
            void deliver(const Database::Model::Message message);
        private:
            void read();
            void handle_request(std::shared_ptr<std::array<char, 512>> buffer, std::size_t length);
            void write(std::string message);
            void pUser(std::string params);
            void pMessage(std::string params);
            void pSend(std::string params);
            tcp::socket m_socket;
            Database::Session& m_dbsession;
            std::string m_message;
            std::list<std::shared_ptr<Session>>& m_sessions;
            std::map<std::string, std::function<void(std::string)>> m_actions = 
            {
                {"user", [&](std::string params) { pUser(params); }},
                {"message", [&](std::string params) { pMessage(params); }},
                {"send", [&](std::string params) { pSend(params); }},
            };
    };
}
