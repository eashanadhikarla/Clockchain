/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <chrono>

using namespace std;
using boost::asio::io_service;
using boost::asio::steady_timer;

int main() {
    cout << "stating a 1 second timer ... " << endl;
    io_service ioc;
    steady_timer timer(ioc, chrono::seconds(1));
    function<void(const boost::system::error_code&)> handler;
    handler = [&](const boost::system::error_code& error) {
            cout << "It's been 1 seconds" << endl;
            timer.expires_at(timer.expires_at() + chrono::seconds(1));
            timer.async_wait(handler);
    };
    timer.async_wait(handler);

    ioc.run();

    return 0;
}
