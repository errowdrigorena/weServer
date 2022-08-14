/*
 * Server.cc
 *
 *  Created on: Jun 13, 2021
 *      Author: iban
 */

#include <boost/asio/ip/tcp.hpp>
#include <ConnectionListener.h>
#include <Server.h>
#include <ServerCertificate.hpp>
#include <thread>

namespace web_server
{

void run_server()
{
	// The io_context is required for all I/O
	boost::asio::io_context ioc{threads };
	// The SSL context is required, and holds certificates
	boost::asio::ssl::context ctx{  boost::asio::ssl::context::tlsv12 };

	// This holds the self-signed certificate used by the server
	load_server_certificate(ctx);

	// Create and launch a listening port
	std::make_shared<ConectionListener>(ioc, ctx, boost::asio::ip::tcp::endpoint
	{ web_server::address, web_server::port }, web_server::doc_root)->run();

	// Run the I/O service on the requested number of threads
	std::vector<std::thread> thread_container;
	thread_container.reserve(web_server::threads - 1);
	for (auto i = web_server::threads - 1; i > 0; --i)
		thread_container.emplace_back([&ioc]
		{
			ioc.run();
		});
	ioc.run();
}

} /* namespace web_server */
