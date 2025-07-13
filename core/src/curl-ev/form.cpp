/**
        curl-ev: wrapper for integrating libcurl with libev applications
        Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
        See COPYING for license information.

        C++ wrapper for constructing libcurl forms
*/

#include <curl-ev/error_code.hpp>
#include <curl-ev/form.hpp>
#include <curl-ev/native.hpp>
#include <curl-ev/wrappers.hpp>

USERVER_NAMESPACE_BEGIN

namespace curl {

form::form() {
    impl::CurlGlobal::Init(); 
    mime_ = native::curl_mime_init(nullptr);
}

form::~form() {
    if (mime_) {
        native::curl_mime_free(mime_);
        mime_ = nullptr;
    }
}

void form::add_content(std::string_view key, std::string_view content) {
    std::error_code ec;
    add_content(key, content, ec);
    throw_error(ec, "add_content");
}

void form::add_content(std::string_view key, std::string_view content, std::error_code& ec) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) { return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_data(
        part,
        content.data(),
        content.length()
    ))};
}

void form::add_content(std::string_view key, std::string_view content, const std::string& content_type) {
    std::error_code ec;
    add_content(key, content, content_type, ec);
    throw_error(ec, "add_content");
}

void form::add_content(
    std::string_view key,
    std::string_view content,
    const std::string& content_type,
    std::error_code& ec
) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) { return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_data(
        part,
        content.data(),
        content.length()
    ))};
    if (ec) { return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_type(
        part,
        content_type.data()
    ))};
}

void form::add_buffer(
    const std::string& key,
    const std::string& file_name,
    const char* buffer,
    size_t buffer_len,
    std::error_code& ec
) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) { return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_data(
        part,
        buffer,
        buffer_len
    ))};
    if (ec) { return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filename(
        part,
        file_name.c_str()
    ))};
}

void form::add_buffer(
    const std::string& key,
    const std::string& file_name,
    const std::shared_ptr<std::string>& buffer
) {
    std::error_code ec;
    add_buffer(key, file_name, buffer, ec);
    throw_error(ec, "add_buffer");
}

void form::add_buffer(
    const std::string& key,
    const std::string& file_name,
    const std::shared_ptr<std::string>& buffer,
    std::error_code& ec
) {
    buffers_.push_back(buffer);
    add_buffer(key, file_name, buffers_.back()->c_str(), buffers_.back()->size(), ec);
}

void form::add_buffer(
    const std::string& key,
    const std::string& file_name,
    const std::shared_ptr<std::string>& buffer,
    const std::string& content_type
) {
    std::error_code ec;
    add_buffer(key, file_name, buffer, content_type, ec);
    throw_error(ec, "add_buffer");
}

void form::add_buffer(
    const std::string& key,
    const std::string& file_name,
    const std::shared_ptr<std::string>& buffer,
    const std::string& content_type,
    std::error_code& ec
) {
    buffers_.push_back(buffer);
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_data(
        part,
        buffer->c_str(),
        buffer->length()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filename(
        part,
        file_name.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_type(
        part,
        content_type.data()
    ))};
}

void form::add_file(const std::string& key, const std::string& file_path) {
    std::error_code ec;
    add_file(key, file_path, ec);
    throw_error(ec, "add_file");
}

void form::add_file(const std::string& key, const std::string& file_path, std::error_code& ec) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filedata(
        part,
        file_path.c_str()
    ))};
}

void form::add_file(const std::string& key, const std::string& file_path, const std::string& content_type) {
    std::error_code ec;
    add_file(key, file_path, content_type, ec);
    throw_error(ec, "add_file");
}

void form::add_file(
    const std::string& key,
    const std::string& file_path,
    const std::string& content_type,
    std::error_code& ec
) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filedata(
        part,
        file_path.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_type(
        part,
        content_type.data()
    ))};
}

void form::add_file_using_name(const std::string& key, const std::string& file_path, const std::string& file_name) {
    std::error_code ec;
    add_file_using_name(key, file_path, file_name, ec);
    throw_error(ec, "add_file_using_name");
}

void form::add_file_using_name(
    const std::string& key,
    const std::string& file_path,
    const std::string& file_name,
    std::error_code& ec
) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filedata(
        part,
        file_path.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filename(
        part,
        file_name.c_str()
    ))};
}

void form::add_file_using_name(
    const std::string& key,
    const std::string& file_path,
    const std::string& file_name,
    const std::string& content_type
) {
    std::error_code ec;
    add_file_using_name(key, file_path, file_name, content_type, ec);
    throw_error(ec, "add_file_using_name");
}

void form::add_file_using_name(
    const std::string& key,
    const std::string& file_path,
    const std::string& file_name,
    const std::string& content_type,
    std::error_code& ec
) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filedata(
        part,
        file_path.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filename(
        part,
        file_name.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_type(
        part,
        content_type.data()
    ))};
}

void form::add_file_content(const std::string& key, const std::string& file_path) {
    std::error_code ec;
    add_file_content(key, file_path, ec);
    throw_error(ec, "add_file_content");
}

void form::add_file_content(const std::string& key, const std::string& file_path, std::error_code& ec) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filedata(
        part,
        file_path.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filename(
        part,
        nullptr
    ))};
}

void form::add_file_content(const std::string& key, const std::string& file_path, const std::string& content_type) {
    std::error_code ec;
    add_file_content(key, file_path, content_type, ec);
    throw_error(ec, "add_file_content");
}

void form::add_file_content(
    const std::string& key,
    const std::string& file_path,
    const std::string& content_type,
    std::error_code& ec
) {
    auto part = native::curl_mime_addpart(mime_);
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_name(
        part,
        key.data()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filedata(
        part,
        file_path.c_str()
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_filename(
        part,
        nullptr
    ))};
    if (ec) {  return; }
    ec = std::error_code{static_cast<errc::EasyErrorCode>(native::curl_mime_type(
        part,
        content_type.data()
    ))};
}

}  // namespace curl

USERVER_NAMESPACE_END
