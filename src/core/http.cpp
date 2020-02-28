#include <core/http.h>

ice::core::http_client::http_client(
    const std::shared_ptr<ice::core::engine> & engine, 
    const std::string & host, 
    const int port)
{
    _engine = engine;

    _base_url = std::string(
        "http://" 
        + host + ":" 
        + std::to_string(port));
}

ice::core::string_type ice::core::http_client::get_request(
    const ice::core::string_type & path,
    const ice::core::http_headers & headers)
{
    std::stringstream ss;
    std::list<std::string> request_headers;
    curlpp::Cleanup cleanup;
    curlpp::Easy request;
    typedef std::pair<
        ice::core::string_type,
        ice::core::string_type> pair_type;
    for(const pair_type & header_pair : headers)
    {
        request_headers.push_back(
            header_pair.first 
            + ": " 
            + header_pair.second);    
    }
    
    request.setOpt(
        new curlpp::options::Url(
            _base_url 
            + path));
    
    request.setOpt(new curlpp::options::HttpHeader(request_headers)); 
    
    request.setOpt(new curlpp::options::WriteStream(&ss));
    
    request.perform();
    
    return ss.str();
}

ice::core::string_type ice::core::http_client::post_request(
    const string_type & path,
    const http_headers & headers,
    const byte_type * data,
    const size_type size)
{
    std::stringstream ss;
    std::list<std::string> request_headers;
    curlpp::Cleanup cleanup;
    curlpp::Easy request;
    typedef std::pair<
        ice::core::string_type,
        ice::core::string_type> pair_type;
    for(const pair_type & header_pair : headers)
    {
        request_headers.push_back(
            header_pair.first 
            + ": " 
            + header_pair.second);    
    }
    request.setOpt(
        new curlpp::options::Url(
            _base_url
            + path));

    request.setOpt(new curlpp::options::HttpHeader(request_headers)); 
    
    request.setOpt(new curlpp::options::WriteStream(&ss));
    
    request.setOpt(new curlpp::options::PostFields((char*)data));

    request.setOpt(new curlpp::options::PostFieldSize(size));

    request.perform();

    return ss.str();
}