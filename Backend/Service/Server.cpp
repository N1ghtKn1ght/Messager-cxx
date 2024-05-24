#include "Server.hpp"


Service::Server::Server(asio::io_context& ioc, short port, std::string hostdb)
    : m_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), m_dbsession(hostdb, "SimpleBase")
{
	accept();
}


void Service::Server::accept()
{
    m_acceptor.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket), m_dbsession, m_sessions)->start();
            }
            accept();
        });
}

