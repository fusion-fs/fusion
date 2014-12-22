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
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace std;
namespace asio = boost::asio;
using asio::ip::tcp;
using boost::uint8_t;
using namespace io;
using namespace google;

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
            read_header();
        }

private:
    tcp::socket m_socket;
    asio::streambuf m_readbuf;
    boost::shared_ptr<io::Request> m_request;
    boost::shared_ptr<io::Response> m_resp;

    Session(asio::io_service& io_service)
        : m_socket(io_service), m_request(new io::Request())
        {
        }
    
    void handle_read_header(const boost::system::error_code& error)
        {
            if (!error) {
                istream stream(&m_readbuf);
                protobuf::io::IstreamInputStream in_stream(&stream);
                protobuf::io::CodedInputStream coded_in_stream(&in_stream);
                Request msg;
                msg.MergePartialFromCodedStream(&coded_in_stream);
                if (msg.has_io_read()) {
                    Request_IORead read /*= msg*/;
                    string key = read.key();
                    unsigned msg_len;
                    read_body(msg_len);
                }
            }
        }

    void handle_read_body(const boost::system::error_code& error)
        {
            if (!error) {
                handle_request();
                read_header();
            }
        }

    void handle_request()
        {
            RequestPointer req;
            ResponsePointer resp = prepare_response(req);
        
            vector<uint8_t> writebuf;
            asio::write(m_socket, asio::buffer(writebuf));
        }

    void read_header()
        {
            boost::asio::streambuf::mutable_buffers_type mutableBuffer =
                m_readbuf.prepare(4096);

            asio::async_read(m_socket, asio::buffer(mutableBuffer),
                             boost::bind(&Session::handle_read_header, 
                                         shared_from_this(),
                                         asio::placeholders::error));
        }

    void read_body(unsigned msg_len)
        {
            boost::asio::streambuf::mutable_buffers_type mutableBuffer =
                m_readbuf.prepare(msg_len);

            asio::async_read(m_socket, asio::buffer(mutableBuffer),
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


class DataServer::Server
{
    asio::io_service& io_service_;
    tcp::acceptor acceptor;
public:
    Server(asio::io_service& io_service, unsigned port)
        : io_service_(io_service), 
          acceptor(io_service, tcp::endpoint(tcp::v4(), port))

        {
            start_accept();
        }

    void start_accept()
        {
            Session::Pointer new_connection = 
                Session::create(io_service_);

            acceptor.async_accept(new_connection->get_socket(),
                                  boost::bind(&Server::handle_accept, 
                                              this, new_connection,
                                              asio::placeholders::error));
        }

    void handle_accept(Session::Pointer connection,
                       const boost::system::error_code& error)
        {
            if (!error) {
                connection->start();
                start_accept();
            }
        }
};


DataServer::DataServer(asio::io_service& io_service, unsigned port)
    : server(new Server(io_service, port))
{
}


DataServer::~DataServer()
{
}


