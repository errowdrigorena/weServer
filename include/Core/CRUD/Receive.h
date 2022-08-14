/*
 * Receive.h
 *
 *  Created on: Aug 14, 2022
 *      Author: iban
 */

#ifndef RECEIVE_H_
#define RECEIVE_H_

#include <boost/beast/core/string_type.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http/basic_file_body.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>

template< class Body, class Allocator, class Send>
bool manage_reception(
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send);

template< class Body, class Allocator, class Send>
bool manage_reception(
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send)
{
    if(req.target() == "/API")
    {
    	std::cout << "We are in API" << std::endl;

    	if(req.method() == http::verb::get)
    	{

    		boost::property_tree::ptree root;
    		boost::property_tree::ptree arrayTree;
    		boost::property_tree::ptree flatTree;
    		std::stringstream ss;

    		// Populate JSON response
    		flatTree.put("bool", true);
    		flatTree.put("integer", 123);
    		flatTree.put("string", "String");
    		arrayTree.push_back(std::make_pair("", flatTree));
    		arrayTree.push_back(std::make_pair("", flatTree));
    		arrayTree.push_back(std::make_pair("", flatTree));
    		root.add_child("node_array", arrayTree);
    		root.add_child("node_1", flatTree);

    		// Export JSON response
    		boost::property_tree::json_parser::write_json(ss, root, true /* human */);
            http::response<http::string_body> res{http::status::ok, req.version(), ss.str()};
            send(std::move(res));
    	}
    	return true;
    }

    if(req.target() == "/API/users")
    {
    	std::cout << "We are in API/users" << std::endl;

    	if(req.method() == http::verb::post)
    	{
        	std::cout << "We received a post" << std::endl;

            http::response<http::string_body> res{http::status::ok, req.version()};

            send(std::move(res));

            std::cout << "data inside is " << req.body() << std::endl;

    	}

    	if(req.method() == http::verb::get)
    	{
        	std::cout << "We received a get" << std::endl;
    	}

    	return true;
    }

    std::cout << "req.target is " << req.target() << std::endl;
    return false;
}





#endif /* RECEIVE_H_ */
