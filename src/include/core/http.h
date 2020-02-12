#ifndef HTTP_H
#define HTTP_H
#include <cpprest/http_msg.h>
#include <core/memory.h>
namespace ice
{
namespace core
{
    class http_request : public web::http::http_request
    {

    };
    
    class http_response
    {

    };

    class http_client
    {
    public:
        virtual void get_request();
        virtual void post_request();
        virtual void put_request();
        virtual void patch_request();
        virtual void delete_request();  
    };

    class http_server
    {
        
    };
}
}
#endif