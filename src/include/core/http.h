#ifndef HTTP_H
#define HTTP_H
#include <sstream>
#include <unordered_map>
#include <uWS/uWS.h>
#include <core/types.h>
#include <core/engine.h>

namespace ice
{
namespace core
{   
    constexpr const char * const http_endl = "\r\n";

    enum http_method
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

    enum http_status
    {
        OK=200,
        NOT_FOUND=404
    };

    struct http_headers : public std::unordered_map<string_type,string_type>
    {   
        string_type to_string() const
        {
            std::stringstream result;
            for(auto it = this->begin(); it != this->end(); it++)
            {
                result << it->first << ": " << it->second << http_endl;
            }
            return result.str();
        }
    };

    class http_request
    {
    private:
        string_type _string;
    public:
        http_request(
            const int method, 
            const string_type & path,
            const byte_type * payload_data,
            const size_type payload_size);

        string_type to_string() const;
    };

    class http_server
    {
    private:
        std::shared_ptr<ice::core::engine> _engine;
    public:
        http_server(
            const std::shared_ptr<ice::core::engine> & engine);
        http_server(
            const http_server& server) = delete;
        http_server(
            http_server&& server) = default;
        ~http_server();
        
        void start();

        void stop();

        virtual void handle_get();

        virtual void handle_post();
        
        virtual void handle_head();
        
        virtual void handle_put();
        
        virtual void handle_delete();
        
        virtual void handle_connect();
        
        virtual void handle_options();
        
        virtual void handle_trace();
        
        virtual void handle_patch();
    };

    class http_client
    {

    };
}
}

#endif