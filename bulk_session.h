#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <set>
#include "commands_storage.h"

namespace ba = boost::asio;

class bulk_session;

using bulk_session_ptr = std::shared_ptr<bulk_session>;

class bulk_session
        : public std::enable_shared_from_this<bulk_session>
{
public:
    bulk_session(ba::ip::tcp::socket socket,
                 std::set<bulk_session_ptr>& bulk_sessions,
                 CommandsStorage& commandsStorage_)
        : socket_(std::move(socket)),
          bulk_sessions_(bulk_sessions),
          commandsStorage(commandsStorage_)
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
                                boost::asio::buffer(read_str, 10),
                                [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                for(std::size_t i = 0; i < length; i++)
                {
                    if(read_str[i] != '\n')
                    {
                        str += read_str[i];
                    }
                    else
                    {
                        commandsStorage.addString(shared_from_this(), str);
                        str = "";
                    }
                }
                do_read();
            }
            else
            {
                bulk_sessions_.erase(shared_from_this());
                if(bulk_sessions_.empty())
                    commandsStorage.dumpResidues();
            }
        });
    }

    ba::ip::tcp::socket socket_;
    std::set<bulk_session_ptr>& bulk_sessions_;
    CommandsStorage& commandsStorage;
    char read_str[10];
    std::string str;
};
