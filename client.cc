/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
**
** blocking_tcp_echo_client.cpp
** Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
** Distributed under the Boost Software License, Version 1.0. (See accompanying
** file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
**
*/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include <libgen.h>
#include <string>

using boost::asio::ip::tcp;

using boost::asio::buffer;
using boost::asio::connect;
using boost::asio::io_service;
using boost::asio::streambuf;
using boost::asio::write;

using std::exception;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::getline;
using std::istream;
using std::string;
using std::strlen;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      cerr << "Usage: " << basename(argv[0]) << " <host> <port>" << endl;
      return 1;
    }

    io_service io_service;

    cout << "Connecting to server ...";
    cout.flush();

    tcp::socket s(io_service);
    tcp::resolver resolver(io_service);
    connect(s, resolver.resolve({argv[1], argv[2]}));

    cout << " success" << endl;
    cout << "Commands: get #, add #" << endl;
    cout << "To quit, type <Ctrl><d>" << endl;

    while (true) {
        cout << "Enter a command: ";
        char request[max_length];
        cin.getline(request, max_length);

        // include '\n' in what we send to server
        const size_t chars_read = cin.gcount();
        if (chars_read > max_length)
            request[max_length-1] = '\n';
        else
            request[chars_read-1] = '\n';

        if (cin.eof()) {
            cout << "EOF" << endl;
            break;
        }

        size_t request_length = strlen(request);
        write(s, buffer(request, request_length));
        
        streambuf reply;
        istream is(&reply);
        string line;
        
        read_until(s, reply, "\n\n");

        cout << "Reply is: ";
        is >> cout.rdbuf();
    }
    
  }
  catch (exception& e)
  {
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
