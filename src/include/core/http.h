#ifndef HTTP_H
#define HTTP_H
#include <sstream>
#include <unordered_map>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <uWS/uWS.h>
#include <core/engine.h>
#include <core/types.h>

namespace ice
{
namespace core
{   
    constexpr const char * const http_endl = "\r\n";

    enum http_methods
    {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    };

    class http_headers : public std::unordered_map<string_type,string_type>
    {

    };

    enum http_status_codes
    {
        OK=200,
        NOT_FOUND=404
    };

    class http_client
    {
    protected:
        std::shared_ptr<engine> _engine;
        std::string _base_url;
    public:
        http_client(
            const std::shared_ptr<engine> & engine, 
            const std::string & host, 
            const int port);

        http_client(const http_client& client) = default;
        
        http_client(http_client&& client) = default;
        
        ~http_client() = default;

        string_type get_request(
            const string_type & path,
            const http_headers & headers);

        string_type post_request(
            const string_type & path,
            const http_headers & headers,
            const byte_type * data,
            const size_type size);


    };
    


}
}

#endif