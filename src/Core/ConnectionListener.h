/*
 * ConnectionListener.h
 *
 *  Created on: Mar 25, 2022
 *      Author: iban
 */

#ifndef CORE_CONNECTIONLISTENER_H_
#define CORE_CONNECTIONLISTENER_H_
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>

#include <boost/beast/ssl.hpp>
#include <boost/beast/core.hpp>

#include <string>
#include <memory>
#include <iostream>

#include <Session.h>
#include <Common.h>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace web_server {
// Accepts incoming connections and launches the sessions
class ConectionListener : public std::enable_shared_from_this<ConectionListener>
{
    net::io_context& ioc_;
    ssl::context& ctx_;
    tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;

public:
    ConectionListener(
        net::io_context& ioc,
        ssl::context& ctx,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root);

    // Start accepting incoming connections
    void run();

private:
    void do_accept();

    void on_accept(beast::error_code ec, tcp::socket socket);
};

} /* namespace web_server */

#endif /* CORE_CONNECTIONLISTENER_H_ */
