#include "web_server/constants.h"
#include "web_server/server.h"
#include "web_server/server_certificate.h"
#include "web_server/shared_state.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>

#include <exception>
#include <iostream>
#include <memory>

int main() {
    try {
        boost::asio::ssl::context ssl_ctx{boost::asio::ssl::context::tlsv12_server};
        web_server::load_server_certificate(ssl_ctx);

        auto state = std::make_shared<web_server::SharedState>();
        boost::asio::io_context io_context;

        boost::asio::ip::tcp::endpoint const endpoint{
            boost::asio::ip::make_address(web_server::default_listen_address),
            web_server::default_listen_port};

        boost::asio::co_spawn(io_context, web_server::run_server(endpoint, ssl_ctx, state),
                              [](std::exception_ptr exc) {
                                  if (!exc) {
                                      return;
                                  }
                                  try {
                                      std::rethrow_exception(exc);
                                  } catch (std::exception const &e) {
                                      std::cerr << "Fatal server error: " << e.what() << '\n';
                                  }
                              });

        std::cout << "weServer listening on https://" << web_server::default_listen_address << ':'
                  << web_server::default_listen_port << '\n';
        io_context.run();
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
