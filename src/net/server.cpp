#include "web_server/server.h"

#include "web_server/error.h"
#include "web_server/http_session.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/this_coro.hpp>

#include <exception>
#include <iostream>

namespace web_server {
namespace {

void log_exception(std::exception_ptr ptr) {
    try {
        std::rethrow_exception(ptr);
    } catch (std::exception const &exc) {
        std::cerr << "Uncaught exception in session: " << exc.what() << '\n';
    }
}

} // namespace

boost::asio::awaitable<void> run_server(boost::asio::ip::tcp::endpoint listening_endpoint,
                                        boost::asio::ssl::context &ssl_ctx,
                                        std::shared_ptr<SharedState> state) {
    auto executor = co_await boost::asio::this_coro::executor;
    boost::asio::ip::tcp::acceptor acceptor{executor};

    acceptor.open(listening_endpoint.protocol());
    acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor.bind(listening_endpoint);
    acceptor.listen();

    while (true) {
        boost::asio::ip::tcp::socket socket = co_await acceptor.async_accept();
        boost::asio::co_spawn(executor, run_http_session(std::move(socket), ssl_ctx, state),
                              [](std::exception_ptr exc) {
                                  if (exc) {
                                      log_exception(exc);
                                  }
                              });
    }
}

} // namespace web_server
