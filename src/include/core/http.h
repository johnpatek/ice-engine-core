#ifndef HTTP_H
#define HTTP_H
#include <cpprest/http_msg.h>
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

namespace ice
{
namespace core
{   
    typedef web::http::http_headers http_headers;
    typedef web::http::experimental::listener::http_listener http_listener;
    typedef web::http::http_request http_request;
    typedef web::http::http_response http_response;
    typedef web::http::methods http_methods;
    typedef web::http::status_codes status_codes;

    class http_threaded_server
    {
    private: 
        http_listener _listener;

        
    };
}
}
#endif