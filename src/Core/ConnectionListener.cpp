/*
 * ConnectionListener.cc
 *
 *  Created on: Mar 25, 2022
 *      Author: iban
 */

#include <ConnectionListener.h>

namespace web_server {

ConectionListener::ConectionListener(
    net::io_context& ioc,
    ssl::context& ctx,
    tcp::endpoint endpoint,
    std::shared_ptr<std::string const> const& doc_root)
    : ioc_(ioc)
    , ctx_(ctx)
    , acceptor_(ioc)
    , doc_root_(doc_root)
{
    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if(ec)
    {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if(ec)
    {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if(ec)
    {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(
        net::socket_base::max_listen_connections, ec);
    if(ec)
    {
        fail(ec, "listen");
        return;
    }
}

void ConectionListener::run()
{
    do_accept();
}

void ConectionListener::do_accept()
{
    // The new connection gets its own strand
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(
            &ConectionListener::on_accept,
            shared_from_this()));
}

void ConectionListener::on_accept(beast::error_code ec, tcp::socket socket)
{
    if(ec)
    {
        fail(ec, "accept");
        return; // To avoid infinite loop
    }
    else
    {
        // Create the session and run it
        std::make_shared<session>(
            std::move(socket),
            ctx_,
            doc_root_)->run();
    }

    // Accept another connection
    do_accept();
}

} /* namespace web_server */
