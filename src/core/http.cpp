#include <core/http.h>

ice::core::http_client::http_client(
    const std::shared_ptr<ice::core::engine> & engine, 
    const std::string & host, 
    const int port)
{
    _engine = engine;
    
    std::string address(host);
    
    // We need to add the port number if it's not 80
    if(port != DEFAULT_HTTP_PORT)
    {
        address.append(":");
        address.append(std::to_string(port));
    }
    
    _base_url = std::string(
        "http://" 
        + address);
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
        curlpp::options::Url(
            _base_url 
            + path));
    
    request.setOpt(curlpp::options::HttpHeader(request_headers)); 
    
    request.setOpt(curlpp::options::WriteStream(&ss));
    
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
        curlpp::options::Url(
            _base_url
            + path));

    request.setOpt(curlpp::options::HttpHeader(request_headers)); 
    
    request.setOpt(curlpp::options::WriteStream(&ss));
    
    request.setOpt(curlpp::options::PostFields((char*)data));

    request.setOpt(curlpp::options::PostFieldSize(size));

    request.perform();

    return ss.str();
}

struct http_data
{
    ice::core::http_methods method;
    std::string url;
    ice::core::http_headers headers;
    std::string path;
    
    http_data()
    {

    }

    http_data(uWS::HttpRequest request)
    {
        url = request.getUrl().toString();
        path = url.substr(
            0,
            url.find("?"));
        switch(request.getMethod())
        {
            case uWS::HttpMethod::METHOD_GET:
                method = ice::core::http_methods::GET;
                break;     
            case uWS::HttpMethod::METHOD_POST:
                method = ice::core::http_methods::POST;
                break;
            default:
                throw std::runtime_error(
                    "Invalid HTTP method");
        }
    }
};

// callback function for uv loop
void close_hub_event_loop(uv_async_t * handle)
{
    auto uv_walk_callback = [](uv_handle_t * handle, void* pass)
    {
        if(!uv_is_closing(handle))
        {
            uv_close(handle, nullptr);
        }
    };

    auto loop = handle->loop;
    auto hub = (uWS::Hub *)handle->data;
    uv_stop(loop);
    uv_walk(loop,uv_walk_callback,nullptr);
    uv_run(loop,UV_RUN_DEFAULT);
    uv_loop_close(loop);
}

ice::core::http_server::http_server(
    const std::shared_ptr<ice::core::engine> engine,
    const int port,
    const size_type threads)
{
    _engine = engine;
    _port = port;
    _thread_count = threads;
}

void ice::core::http_server::start()
{
    for(int i = 0 ; i < _thread_count; i++)
    {
        std::shared_ptr<uWS::Hub> hub(new uWS::Hub);
        std::shared_ptr<uv_async_t> async_handle(new uv_async_t);
        
        async_handle->data = hub.get();

        uv_async_init(
            (uv_loop_t*)hub->getLoop(),
            async_handle.get(),
            close_hub_event_loop);
                
        hub->onHttpData([this](
            uWS::HttpResponse * res, 
            char * buf, 
            size_t size, 
            size_t remain){
                this->handle_data(
                    res,
                    buf,
                    size,
                    remain);        
            });
        
        hub->onHttpRequest([this](
            uWS::HttpResponse * res, 
            uWS::HttpRequest req, 
            char * buf, 
            size_t size, 
            size_t remain){
                res->getHttpSocket()->setUserData(
                    (void*)new http_data(req));

                this->handle_data(
                    res,
                    buf,
                    size,
                    remain);
            });
        
        hub->listen(
            _port,
            nullptr,
            uS::ListenOptions::REUSE_PORT);

        hub->getDefaultGroup<uWS::SERVER>().addAsync();

        _handles.push_back(
            std::move(async_handle));

        _threads.push_back(std::make_shared<std::thread>(
            [](std::shared_ptr<uWS::Hub> && h)
            {   
                h->run();
                h.reset();
            }, std::move(hub)));
            
    }
}

void ice::core::http_server::stop()
{
    for(int i = 0; i < _thread_count; i++)
    {
        uv_async_send(_handles[i].get());
        _threads[i]->join();
        _handles[i].reset();
        _threads[i].reset();
    }
}

void ice::core::http_server::handle_data(
    uWS::HttpResponse * response,
    char * data, 
    ice::core::size_type size, 
    ice::core::size_type remaining)
{
    std::string result;
    http_data * request_data;
    
    request_data = (http_data*)
        response->getHttpSocket()->getUserData();

    try
    {
        if(remaining == 0)
        {
            // TODO: add more http methods
            if(request_data->method 
                == ice::core::http_methods::GET)
            {
                result = this->handle_get(
                    request_data->path,
                    request_data->headers);
            }
            else if(request_data->method 
                == ice::core::http_methods::POST)
            {
                result = this->handle_post(
                    request_data->path,
                    request_data->headers,
                    (ice::core::byte_type*)data,
                    size);
            }
            else
            {
                throw std::runtime_error(
                    "Invalid HTTP method");
            }
        }
    }
    catch(std::exception & e)
    {
        result = e.what();
    }

    response->end(
        result.data(),
        result.size());
    
    delete request_data;
}
