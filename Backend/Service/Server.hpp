#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <string>
#include <list>

#include "Session.hpp"
#include "Database/Session.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

namespace Service
{
	class Server
	{
	public:
		Server(asio::io_context& ioc, short port, std::string hostdb);
	private:
		void accept();
		tcp::acceptor m_acceptor;
		Database::Session m_dbsession;
		std::list<std::shared_ptr<Session>> m_sessions;
	};
}