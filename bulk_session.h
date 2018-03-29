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
        boost::asio::async_read_until(socket_,
                                streambuf, '\n',
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::istream is(&streambuf);
                std::string line;
                std::getline(is, line);
                commandsStorage.addString(self, line);
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

    boost::asio::streambuf streambuf;
    ba::ip::tcp::socket socket_;
    std::set<bulk_session_ptr>& bulk_sessions_;
    CommandsStorage& commandsStorage;
    std::string str;
};
