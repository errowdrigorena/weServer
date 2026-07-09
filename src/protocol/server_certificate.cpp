#include "web_server/server_certificate.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>

#include <filesystem>
#include <fstream>
#include <string>

namespace web_server {
namespace {

[[nodiscard]] std::string read_file_content(std::filesystem::path const& path)
{
    std::ifstream file{path};
    return {std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
}

}  // namespace

void load_server_certificate(boost::asio::ssl::context& ctx)
{
    auto const cert =
        read_file_content(std::filesystem::current_path() / "server.cert.pem");
    auto const key =
        read_file_content(std::filesystem::current_path() / "server.key.pem");

    ctx.set_password_callback(
        [](std::size_t, boost::asio::ssl::context_base::password_purpose) {
            return "test";
        });

    ctx.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::single_dh_use);

    ctx.use_certificate_chain(boost::asio::buffer(cert.data(), cert.size()));
    ctx.use_private_key(
        boost::asio::buffer(key.data(), key.size()),
        boost::asio::ssl::context::file_format::pem);
}

}  // namespace web_server
