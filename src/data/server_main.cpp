#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "data_server.hpp"
#include "io.pb.h"


using namespace std;
namespace asio = boost::asio;


int main(int argc, const char* argv[])
{
    unsigned port = 4050;
    if (argc > 1)
        port = atoi(argv[1]);
    cout << "Serving on port " << port << endl;

    try {
        asio::io_service io_service;
        DataServer server(io_service, 4050);
        io_service.run();
    }
    catch (std::exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
