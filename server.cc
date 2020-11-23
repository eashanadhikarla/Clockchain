/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
**
** async_tcp_echo_server.cpp
** Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
** Distributed under the Boost Software License, Version 1.0. (See accompanying
** file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
**
*/

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include <sstream>
#include <libgen.h>
#include <unistd.h>

#include "Blockchain.h"

using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::asio::async_write;
using boost::asio::buffer;
using boost::asio::buffer_copy;

using std::cout;
using std::cerr;
using std::enable_shared_from_this;
using std::endl;
using std::exception;
using std::make_shared;
using std::move; // from <utility>
using std::size_t;
using std::string;
using std::strtoul;

class session
  : public enable_shared_from_this<session> {
public:
  session(tcp::socket socket, Blockchain& blockchain, unsigned &counter)
    : socket_(move(socket)), blockchain_(blockchain), counter_(counter) {}

  void start() {
    do_read();
  }

private:
  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(buffer(read_buf, max_length),
        [this, self](boost::system::error_code ec, size_t length) {
          if (!ec) {
            // check for newline char
            string const message(read_buf);
            size_t pos = message.find("\n");
            if (pos != string::npos)
                handle_message(string(read_buf).substr(0, pos));
            else
                do_read();
          }
        });
  }

  void do_write(size_t length) {
    auto self(shared_from_this());
    async_write(socket_, buffer(write_buf, length),
        [this, self, length](boost::system::error_code ec, size_t
            len_written) {
          if (!ec && len_written > 0) {
            do_write(length - len_written);
          }
        });
    do_read();
  }

  void handle_message(string message) {
    size_t len = message.size();

    string response;

    string const EOM = "\n\n"; // end-of-message signal

    // commands go here
    if (len >= 3 && message.substr(0, 3) == "add") {
        if (len < 5) {
            response = "Add what?" + EOM;
        } else {
            ++counter_;

            stringstream ss;
            string argstr = message.substr(4);
            blockchain_.AddBlock(argstr);
            Block newBlock = blockchain_.GetBlock(counter_);

            ss << "Blocks in chain: " << counter_ << "\n";
            ss << "Block data: " << newBlock.GetData() << "\n";
            ss << "Block hash: " << newBlock.GetHash() << EOM;

            // write changes to file
            blockchain_.ToCSV("chain.csv");

            response = ss.str();
        }
    } else if (len >= 3 && message.substr(0, 3) == "get") {
        string data, hash;
        if (len < 5) {
            Block reqBlock = blockchain_.GetBlock(counter_);
            data = reqBlock.GetData();
            hash = reqBlock.GetHash();
        } else {
            string argstr = message.substr(4);
            unsigned index = strtoul(argstr.c_str(), NULL, 10);

            Block reqBlock = blockchain_.GetBlock(index);
            data = reqBlock.GetData();
            hash = reqBlock.GetHash();
        }
        stringstream ss;
        ss << "Block data: " << data << "\n";
        ss << "Block hash: " << hash << EOM;
        response = ss.str();
    } else {
        response = "Unrecognized command" + EOM;
    }

    buffer_copy(buffer(write_buf, max_length),
            buffer(response.c_str(), response.size()));
    do_write(response.size());
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char read_buf[max_length];
  char write_buf[max_length];
  Blockchain& blockchain_;
  unsigned& counter_;
};

class server {
public:
  server(io_service& io_service, short port, Blockchain& blockchain,
          unsigned& counter)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service),
      blockchain_(blockchain),
      counter_(counter) {
    do_accept();
  }

private:
  void do_accept() {
    acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec) {
          if (!ec) {
            make_shared<session>(move(socket_), blockchain_,
                    counter_)->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  tcp::socket socket_;
  Blockchain blockchain_;
  unsigned& counter_;
};

void usage(char const *progname) {
    cout << "ClockChain Server\n";
    cout << "  Usage: " << progname << " [options]\n";
    cout << "    -p <int> : Port on which to listen (default 41100)\n";
    cout << "    -h       : print this message\n";
}

int main(int argc, char* argv[])
{
  // Config vars that we get via getopt
  int port = 4000;        // default port for connection
  bool show_help = false; // whether to show usage info

  // Parse the command line options:
  int o;
  while ((o = getopt(argc, argv, "p:h")) != -1) {
    switch (o) {
    case 'h':
      show_help = true;
      break;
    case 'p':
      port = atoi(optarg);
      break;
    default:
      show_help = true;
      break;
    }
  }

  // Print help and exit
  if (show_help) {
    usage(basename(argv[0]));
    exit(0);
  }

  cout << "p = " << port << endl;
  Blockchain blockchain = Blockchain();
  unsigned counter = 0;

  try
  {
    io_service io_service;

    server s(io_service, port, blockchain, counter);

    io_service.run();
  }
  catch (exception& e)
  {
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
