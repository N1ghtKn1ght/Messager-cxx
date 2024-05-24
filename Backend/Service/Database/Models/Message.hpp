#pragma once

#include <string>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/json.hpp>


namespace Database::Model
{
	class Message
	{
	public:
		Message(const std::string& login_user, const std::string& message) :
			m_login_user(login_user), m_message(message) { };
		bsoncxx::document::value toBson() const;
		static Message fromBson(const bsoncxx::document::view& doc);
		static Message fromJson(const std::string json);
		std::string getLoginUser() const;
		std::string getMessage() const;
	private:
		std::string m_login_user;
		std::string m_message;

	};
}