/*
 * Server.h
 *
 *  Created on: Jun 13, 2021
 *      Author: iban
 */

#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ssl/context_base.hpp>
#include <boost/beast/ssl.hpp>

#include <algorithm>
#include <memory>
#include <string>

namespace web_server
{

const auto address{ boost::asio::ip::make_address("127.0.0.1") };
constexpr unsigned short port{8080 };
const auto doc_root{std::make_shared<std::string>("./") };
constexpr int threads{4 };

void run_server();

} /* namespace web_server */

#endif /* CORE_SERVER_H_ */
