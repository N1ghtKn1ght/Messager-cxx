#include <iostream>
#include <map>
#include <regex>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Service
{
	class Client
	{
		public:

			Client(boost::asio::io_context& io_context, 
				   const tcp::resolver::results_type& endpoints);
			~Client();
		private:
			void initUser();
			void post();
			void read();
			void write(std::string message);
			void pAuth(std::string params);
			void pMessage(std::string params);
			std::thread thread;
			tcp::socket m_socket;
			std::string m_message;
			std::string userName;
			bool m_isLogined = false;
			std::shared_ptr<std::array<char, 512>> m_buffer;
			std::map<std::string, std::function<void(std::string)>> m_actions =
			{
				{"auth", [&](std::string params) { pAuth(params); }},
				{"message", [&](std::string params) { pMessage(params); }},
			};
	};
}