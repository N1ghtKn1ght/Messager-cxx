#include "Message.hpp"

using namespace Database::Model;

bsoncxx::document::value Message::toBson() const
{
	bsoncxx::builder::basic::document builder{};
	builder.append(bsoncxx::builder::basic::kvp("login_user", m_login_user),
		bsoncxx::builder::basic::kvp("message", m_message));
	return builder.extract();
}

Message Message::fromBson(const bsoncxx::document::view& doc)
{
	std::string login_user = doc["login_user"].get_utf8().value.data();
	std::string message = doc["message"].get_utf8().value.data();
	return Message(login_user, message);
}

Message Database::Model::Message::fromJson(const std::string json)
{
	return fromBson(bsoncxx::from_json(json).view());
}

std::string Message::getLoginUser() const
{
	return m_login_user;
}

std::string Message::getMessage() const
{
	return m_message;
}
