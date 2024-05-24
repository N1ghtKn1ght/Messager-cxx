#include "Session.hpp"

 
void Database::Session::insertUser(const Database::Model::User& user)
{
    try {
        auto collection = m_database[Userdb];
        collection.insert_one(user.toBson().view());
        std::cout << "Inserted user: " << bsoncxx::to_json(user.toBson()) << std::endl;
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
    }
}

void Database::Session::insertMessage(const Database::Model::Message& message)
{
    try {
        auto collection = m_database[Messagedb];
        collection.insert_one(message.toBson().view());
        std::cout << "Inserted message: " << bsoncxx::to_json(message.toBson()) << std::endl;
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB exception: " << e.what() << std::endl;
    }
}


std::unique_ptr<Database::Model::User> Database::Session::findUserByLogin(const std::string& login)
{
    try
    {
        auto collection = m_database[Userdb];
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "login" << login;
        auto filter = filter_builder.view();
        auto res = collection.find_one(filter);
        if (!res)
            return nullptr;
        bsoncxx::document::value doc = *res;
        auto user = std::make_unique<Database::Model::User>(Database::Model::User::fromBson(doc));
        return user;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return nullptr;
    }
}

std::vector<Database::Model::Message> Database::Session::getMessages()
{
    std::vector<Database::Model::Message> result;

    try
    {
        auto collection = m_database[Messagedb];
        //collection.delete_many({});
        auto cursor = collection.find({});
        for (auto&& doc : cursor) {
            result.push_back(Database::Model::Message::fromBson(doc));
        }

        return result;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return result;
    }
}
