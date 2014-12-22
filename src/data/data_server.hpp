#ifndef DATA_SERVER_H
#define DATA_SERVER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

class DataServer 
{
public:
    DataServer(boost::asio::io_service& io_service, unsigned port);
    ~DataServer();

private:
    DataServer();
    void start_accept();

    class Server;
    boost::shared_ptr<Server> server;
};

#endif /* DATA_SERVER_H */

