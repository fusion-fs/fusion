#include "io.pb.h"
#include "data_server.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace std;
namespace asio = boost::asio;
using asio::ip::tcp;
using boost::uint8_t;
using namespace io;


class Session : public boost::enable_shared_from_this<Session>
{
public:
    typedef boost::shared_ptr<Session> Pointer;
    typedef boost::shared_ptr<io::Request> RequestPointer;
    typedef boost::shared_ptr<io::Response> ResponsePointer;

    static Pointer create(asio::io_service& io_service)
    {
        return Pointer(new Session(io_service));
    }

    tcp::socket& get_socket()
    {
        return m_socket;
    }

    void start()
    {
        start_read_header();
    }

private:
    tcp::socket m_socket;
    vector<uint8_t> m_readbuf;
    boost::shared_ptr<io::Request> m_request;

    Session(asio::io_service& io_service)
        : m_socket(io_service), m_request(new io::Request())
    {
    }
    
    void handle_read_header(const boost::system::error_code& error)
    {
        if (!error) {
            unsigned msg_len;
            start_read_body(msg_len);
        }
    }

    void handle_read_body(const boost::system::error_code& error)
    {
        if (!error) {
            handle_request();
            start_read_header();
        }
    }

    void handle_request()
    {
        RequestPointer req;
        ResponsePointer resp = prepare_response(req);
        
        vector<uint8_t> writebuf;
        asio::write(m_socket, asio::buffer(writebuf));
    }

    void start_read_header()
    {
        asio::async_read(m_socket, asio::buffer(m_readbuf),
                boost::bind(&Session::handle_read_header, shared_from_this(),
                    asio::placeholders::error));
    }

    void start_read_body(unsigned msg_len)
    {
        // m_readbuf already contains the header in its first HEADER_SIZE
        // bytes. Expand it to fit in the body as well, and start async
        // read into the body.
        //
        m_readbuf.resize(msg_len);
        asio::mutable_buffers_1 buf = asio::buffer(m_readbuf, msg_len);
        asio::async_read(m_socket, buf,
                boost::bind(&Session::handle_read_body, shared_from_this(),
                    asio::placeholders::error));
    }

    ResponsePointer prepare_response(RequestPointer req)
    {
        string value;
        switch (req->type())
        {
            case io::Request::READ:
            {
                break; 
            }
            case io::Request::WRITE:
                break;
            case io::Request::TRIM:
            {
                break;
            }
            default:
                break;
        }
        ResponsePointer resp(new io::Response);
        //resp->set_value(value);
        return resp;
    }
};


struct DataServer::DataServerImpl
{
    asio::io_service& io_service_;
    tcp::acceptor acceptor;

    DataServerImpl(asio::io_service& io_service, unsigned port)
        : io_service_(io_service), 
          acceptor(io_service, tcp::endpoint(tcp::v4(), port))

    {
        start_accept();
    }

    void start_accept()
    {
        // Create a new connection to handle a client. Passing a reference
        // to db to each connection poses no problem since the server is 
        // single-threaded.
        //
        Session::Pointer new_connection = 
            Session::create(io_service_);

        // Asynchronously wait to accept a new client
        //
        acceptor.async_accept(new_connection->get_socket(),
            boost::bind(&DataServerImpl::handle_accept, this, new_connection,
                asio::placeholders::error));
    }

    void handle_accept(Session::Pointer connection,
            const boost::system::error_code& error)
    {
        // A new client has connected
        //
        if (!error) {
            // Start the connection
            //
            connection->start();

            // Accept another client
            //
            start_accept();
        }
    }
};


DataServer::DataServer(asio::io_service& io_service, unsigned port)
    : d(new DataServerImpl(io_service, port))
{
}


DataServer::~DataServer()
{
}


