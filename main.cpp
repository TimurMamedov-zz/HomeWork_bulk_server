#include <iostream>
#include "bulk_server.h"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage:  bulk_server <port> <bulk_size>\n";
            return 1;
        }

        ba::io_service io_service;

        ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), std::atoi(argv[1]));
        bulk_server server(io_service, endpoint, std::atoi(argv[2]));

        io_service.run();

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
