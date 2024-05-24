#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <string>
#include <vector>

#include "Models/User.hpp"
#include "Models/Message.hpp"



#define Messagedb "Messages"
#define Userdb "Users"


namespace Database
{
	class Session
	{
		public:
			Session(const std::string& uri, const std::string& name)
				: m_client(mongocxx::uri{ uri }), m_database(m_client[name]) {};
			void insertUser(const Database::Model::User& user);
			void insertMessage(const Database::Model::Message& message);
			std::unique_ptr<Database::Model::User> findUserByLogin(const std::string& login);
			std::vector<Database::Model::Message> getMessages();
		private:
			mongocxx::instance m_instance;
			mongocxx::client m_client;
			mongocxx::database m_database;
	
	};
}