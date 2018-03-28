#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <set>

namespace ba = boost::asio;

class bulk_session;

using bulk_session_ptr = std::shared_ptr<bulk_session>;

class bulk_session
        : public std::enable_shared_from_this<bulk_session>
{
public:
    bulk_session(ba::ip::tcp::socket socket, std::set<bulk_session_ptr>& bulk_sessions)
        : socket_(std::move(socket)), bulk_sessions_(bulk_sessions)
    {
    }

    void start()
    {
        bulk_sessions_.insert(shared_from_this());
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_str, 20),
                                [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
//                std::string str;
//                str += item->second.second;
//                item->second.second = "";
//                for(std::size_t i = 0; i < size; i++)
//                {
//                    if(data[i] != '\n')
//                    {
//                        str += data[i];
//                    }
//                    else
//                    {
//                        commandStorage.addString(handle_, str);
//                        str = "";
//                    }
//                    i++;
//                }
                std::cout << read_str << std::endl;
                do_read();
            }
            else
            {
                bulk_sessions_.erase(shared_from_this());
            }
        });
    }

    ba::ip::tcp::socket socket_;
    std::set<bulk_session_ptr>& bulk_sessions_;
    char read_str[20];
};
