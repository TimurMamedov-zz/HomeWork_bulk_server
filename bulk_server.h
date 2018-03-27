#pragma once
#include "bulk_session.h"

class bulk_server
{
public:
    bulk_server(boost::asio::io_service& io_service,
                const ba::ip::tcp::endpoint& endpoint, const std::size_t& bulk_size)
        : acceptor_(io_service, endpoint),
          socket_(io_service),
          bulkSize(bulk_size)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::make_shared<bulk_session>(std::move(socket_), bulk_sessions)->start();
            }

            do_accept();
        });
    }
    std::set<bulk_session_ptr> bulk_sessions;
    ba::ip::tcp::acceptor acceptor_;
    ba::ip::tcp::socket socket_;
    const std::size_t bulkSize;
    //  chat_room room_;
};
