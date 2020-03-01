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
    // Provide correct line endings for HTTP
    constexpr const char * const http_endl = "\r\n";

    const int DEFAULT_HTTP_PORT = 80;

    // HTTP methods
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

    /**
     * Provide a simple layer of abstraction for HTTP headers
     * For example, to set the Content-Type header to json, 
     * we would do the following:
     * 
     * http_headers headers;
     * headers["Content-Type"] = "application/json"
     */
    class http_headers : public std::unordered_map<string_type,string_type>
    {

    };

    // Add http status codes as needed
    enum http_status_codes
    {
        OK=200,
        NOT_FOUND=404
    };

    // HTTP client class for sending simple HTTP requests
    class http_client
    {
    protected:
        // TODO: add async capabilities
        std::shared_ptr<engine> _engine;
        std::string _base_url;
    public:
        
        http_client(
            const std::shared_ptr<engine> & engine, 
            const std::string & host, 
            const int port = DEFAULT_HTTP_PORT);

        http_client(
            const http_client& client) = default;
        
        http_client(
            http_client&& client) = default;
        

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

    class http_server
    {
    private:
        std::vector<std::shared_ptr<std::thread>> _threads;
        std::vector<std::shared_ptr<uv_async_t>> _handles;
        int _port;
        size_type _thread_count;
        void handle_data(
            uWS::HttpResponse * response,
            char * data, 
            size_type size, 
            size_type remaining);
    protected:
        std::shared_ptr<ice::core::engine> _engine;
    public:
        http_server(
            const std::shared_ptr<ice::core::engine> engine,
            const int port = DEFAULT_HTTP_PORT,
            const size_type threads = std::thread::hardware_concurrency());

        ~http_server() = default;

        void start();

        void stop();

        virtual string_type handle_get(
            const string_type& path,
            const http_headers& headers) = 0;

        virtual string_type handle_post(
            const string_type & path,
            const http_headers & headers,
            const byte_type * data,
            const size_type size) = 0;
    };
    


}
}

#endif