#include "Session.hpp"

#include <random>

std::vector<std::string> splitString(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::size_t pos = str.find(delimiter);
    if (pos != std::string::npos) {
        std::string first = str.substr(0, pos);
        std::string second = str.substr(pos + 1);
        return { first, second };
    }
    return result;
}

Service::Session::Session(tcp::socket socket, 
                            Database::Session& dbsession, 
                            std::list<std::shared_ptr<Session>>& sessions)
    : m_socket(std::move(socket)), m_dbsession(dbsession), m_sessions(sessions)
{
}

void Service::Session::start()
{
    m_sessions.push_back(shared_from_this());
    read();
}

void Service::Session::deliver(const Database::Model::Message message)
{
    std::string m = "message@" + bsoncxx::to_json(message.toBson());
    write(m);
}

void Service::Session::read()
{
    auto self(shared_from_this());
    auto buffer = std::make_shared<std::array<char, 512>>();

    m_socket.async_read_some(boost::asio::buffer(*buffer),
        [self, buffer](boost::system::error_code ec, std::size_t length) mutable
        {
            if (!ec)
            {
                self->handle_request(buffer, length);
                self->read();
            }
        });

}

void Service::Session::handle_request(std::shared_ptr<std::array<char, 512>> buffer, std::size_t length)
{
    std::string msg(buffer->data(), length);
    std::cout << "Received: " << msg << std::endl;
    std::vector<std::string> arr = splitString(msg, '@');

    if (arr.size() != 2)
    {
        write("Invalid command");
        return;
    }
    std::string command(arr[0]), params(arr[1]);
    auto commander = m_actions.find(command);
    if (commander != m_actions.end())
    {
        commander->second(params);
    }
    else
    {
        write("Invalid command");
    }
    
}

void Service::Session::write(std::string message)
{
    auto self(shared_from_this());
    m_message = message;
    boost::asio::async_write(m_socket, boost::asio::buffer(m_message),
        [self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "Echo \"" + self->m_message + "\" sent." << std::endl;
            }
            else
            {
                self->m_socket.close();
                self->m_sessions.remove(self);
                std::cerr << "Error to send" << std::endl;
            }
        });
}

void Service::Session::pUser(std::string params)
{
    try 
    {
        User user = User::fromJson(params);
        auto us = m_dbsession.findUserByLogin(user.getLogin());
        if (!us)
        {
            m_dbsession.insertUser(user);
            write("auth@succes");
            return;
        }
        bool check = us->checkPassword(user);
        std::string msg = std::string("auth@") + (check ? "succes" : "password isn't correct");
        write(msg);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        std::string msg("auth@somethings going banana");
        write(msg);
    }
}


void Service::Session::pMessage(std::string params)
{
    auto messages = m_dbsession.getMessages();
    if (messages.size() == 0)
    {
        write("message@");
        return;
    }
    std::string m("message@");
    for (auto& msg : messages)
    {
        m += bsoncxx::to_json(msg.toBson()) + " ";
    }
    m.resize(m.size() - 1);
    write(m);
}

void Service::Session::pSend(std::string params)
{
    Message message = Message::fromJson(params);
    m_dbsession.insertMessage(message);
    for (auto& session : m_sessions) 
    {
        session->deliver(message);
    }
}



