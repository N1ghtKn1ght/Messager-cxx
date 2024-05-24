#pragma once

#include <string>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/json.hpp>

namespace Database::Model
{
	class User
	{
		public:
			User(const std::string& login, const std::string& password) :
				m_login(login), m_password(password) { };
			bsoncxx::document::value toBson() const;
			static User fromBson(const bsoncxx::document::view& doc);
			static User fromJson(const std::string json);
			std::string getLogin() const;
			bool checkPassword(const User& user);
		private:

			std::string m_login;
			std::string m_password;

	};
}