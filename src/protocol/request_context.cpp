#include "web_server/request_context.h"

#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <cassert>
#include <utility>

namespace web_server {

ResponseBuilder::ResponseBuilder(unsigned version, bool keep_alive) : keep_alive_{keep_alive} {
    header_.version(version);
    header_.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
}

ResponseBuilder::Header ResponseBuilder::move_header() {
    assert(!used_);
    used_ = true;
    return std::move(header_);
}

ResponseBuilder::MessageGenerator
ResponseBuilder::plaintext_response(boost::beast::http::status status, std::string content) {
    header_.result(status);
    boost::beast::http::response<boost::beast::http::string_body> response{move_header(),
                                                                           std::move(content)};
    response.keep_alive(keep_alive_);
    response.set(boost::beast::http::field::content_type, "text/plain");
    response.prepare_payload();
    return response;
}

ResponseBuilder::MessageGenerator ResponseBuilder::json_response(std::string body) {
    boost::beast::http::response<boost::beast::http::string_body> response{move_header(),
                                                                           std::move(body)};
    response.keep_alive(keep_alive_);
    response.set(boost::beast::http::field::content_type, "application/json");
    response.prepare_payload();
    return response;
}

ResponseBuilder::MessageGenerator ResponseBuilder::empty_response() {
    header_.result(boost::beast::http::status::no_content);
    boost::beast::http::response<boost::beast::http::empty_body> response{move_header()};
    response.keep_alive(keep_alive_);
    return response;
}

ResponseBuilder::MessageGenerator ResponseBuilder::bad_request_text(std::string why) {
    return plaintext_response(boost::beast::http::status::bad_request, std::move(why));
}

ResponseBuilder::MessageGenerator ResponseBuilder::not_found_text() {
    return plaintext_response(boost::beast::http::status::not_found, "Not found");
}

ResponseBuilder::MessageGenerator ResponseBuilder::method_not_allowed() {
    return plaintext_response(boost::beast::http::status::method_not_allowed, "Method not allowed");
}

ResponseBuilder::MessageGenerator ResponseBuilder::internal_server_error(std::string_view what) {
    return plaintext_response(boost::beast::http::status::internal_server_error, std::string{what});
}

RequestContext::RequestContext(Request &&request)
    : request_{std::move(request)}, response_{request_.version(), request_.keep_alive()} {}

} // namespace web_server
