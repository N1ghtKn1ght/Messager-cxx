#include "User.hpp"

#include <bsoncxx/builder/basic/kvp.hpp>

using namespace Database::Model;

bsoncxx::document::value User::toBson() const
{
	bsoncxx::builder::basic::document builder{};
	builder.append(bsoncxx::builder::basic::kvp("login", m_login),
		bsoncxx::builder::basic::kvp("password", m_password));
	return builder.extract();
}

User User::fromBson(const bsoncxx::document::view& doc)
{
	std::string login = doc["login"].get_utf8().value.data();
	std::string password = doc["password"].get_utf8().value.data();
	return User(login, password);
}

User User::fromJson(const std::string json)
{
	return fromBson(bsoncxx::from_json(json).view());
}

std::string User::getLogin() const
{
	return m_login;
}

bool User::checkPassword(const User& user)
{
	return m_password == user.m_password;
}
