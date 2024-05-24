#include "Client.hpp"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>

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

Service::Client::Client(boost::asio::io_context& io_context, 
                        const tcp::resolver::results_type& endpoints)
    : m_socket(io_context), thread([&]() { post(); })
{
    boost::asio::async_connect(m_socket, endpoints, [this](boost::system::error_code ec, tcp::endpoint) {
        if (!ec) {
            std::cout << "Connected to server.\n";
            initUser();
            read();
        }
        else {
            std::cerr << "Failed to connect: " << ec.message() << "\n";
        }
    });
    
}

Service::Client::~Client()
{
    thread.join();
}

void Service::Client::initUser()
{
    std::string login, password;
    std::cout << "Enter login: ";
    std::cin >> login;
    std::cout << "Enter password: ";
    std::cin >> password;
    userName = std::string(login);
    bsoncxx::builder::basic::document doc{};
    doc.append(bsoncxx::builder::basic::kvp("login", login),
        bsoncxx::builder::basic::kvp("password", password));
    std::string message = "user@" + bsoncxx::to_json(doc.view());
    write(message);
}

void Service::Client::post()
{
    while (true)
    {
        if (m_isLogined)
            break;
    }
    std::cin;

    while (true)
    {
        std::string message;
        std::getline(std::cin, message);
        if (message.empty())
            continue;
        bsoncxx::builder::basic::document doc{};
        doc.append(bsoncxx::builder::basic::kvp("login_user", userName),
            bsoncxx::builder::basic::kvp("message", message));
        std::string messageToServer = "send@" + bsoncxx::to_json(doc.view());
        std::cout << "\033[A\33[2K\r";
        write(messageToServer);
    }
}

void Service::Client::write(const std::string message)
{
    m_message = message;
    m_socket.async_write_some(boost::asio::buffer(m_message),
        [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
               
            }
        });
}

void Service::Client::pAuth(std::string params)
{
    if (params == "succes")
    {
        std::cout << "Join to chat\n" << std::endl;
        m_isLogined = true;
        write("message@getall");
        return;
    }
    std::cerr << "Error:" << params << std::endl;
    initUser();
}

void Service::Client::pMessage(std::string params)
{
    std::regex re("\\{([^}]*)\\}");
    std::sregex_iterator it(params.begin(), params.end(), re);
    std::sregex_iterator end;

    for (; it != end; ++it) {
        auto token(it->str());
        auto doc = bsoncxx::from_json(token);
        std::string login_user = doc["login_user"].get_utf8().value.data();
        std::string message = doc["message"].get_utf8().value.data();
        std::cout << login_user << ":" << message << std::endl;
    }
}

void Service::Client::read()
{
    m_buffer = std::make_shared<std::array<char, 512>>();

    m_socket.async_read_some(boost::asio::buffer(*m_buffer),
        [this](boost::system::error_code ec, std::size_t length)
        {
            if (!ec) {
                std::string response_message(m_buffer->data());
                std::vector<std::string> arr = splitString(response_message, '@');
                if (arr.size() != 2)
                    return;
                std::string command(arr[0]), params(arr[1]);
                auto commander = m_actions.find(command);
                if (commander != m_actions.end())
                {
                    commander->second(params);
                }
                read();
            }
        });
}
