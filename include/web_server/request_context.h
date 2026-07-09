#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast/http/message_generator.hpp>

#include <string>
#include <string_view>

namespace web_server {

class ResponseBuilder {
public:
    using MessageGenerator = boost::beast::http::message_generator;

    [[nodiscard]] MessageGenerator json_response(std::string body);
    [[nodiscard]] MessageGenerator empty_response();
    [[nodiscard]] MessageGenerator bad_request_text(std::string why);
    [[nodiscard]] MessageGenerator not_found_text();
    [[nodiscard]] MessageGenerator method_not_allowed();
    [[nodiscard]] MessageGenerator internal_server_error(std::string_view what);

private:
    using Header = boost::beast::http::response_header<boost::beast::http::fields>;

    ResponseBuilder(unsigned version, bool keep_alive);

    [[nodiscard]] MessageGenerator plaintext_response(
        boost::beast::http::status status,
        std::string content);
    [[nodiscard]] Header move_header();

    bool keep_alive_{};
    Header header_;
    bool used_{};

    friend class RequestContext;
};

class RequestContext {
public:
    using Request = boost::beast::http::request<boost::beast::http::string_body>;

    explicit RequestContext(Request&& request);

    [[nodiscard]] boost::beast::http::verb method() const noexcept
    {
        return request_.method();
    }

    [[nodiscard]] std::string_view target() const noexcept
    {
        return request_.target();
    }

    [[nodiscard]] std::string_view body() const noexcept
    {
        return request_.body();
    }

    [[nodiscard]] ResponseBuilder& response() noexcept
    {
        return response_;
    }

private:
    Request request_;
    ResponseBuilder response_;
};

}  // namespace web_server

