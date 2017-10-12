#include <boost/asio.hpp>

#include <x2boost/flows/single_threaded_flow.hpp>
#include <x2boost/links/asio_tcp_client.hpp>

#include "hello_world.hpp"
#include "helloworld_cases.hpp"

namespace x2boost { namespace samples { namespace hello_world {

    class hello_client : public asio_tcp_client
    {
    public:
        hello_client()
            : asio_tcp_client("hello_client")
        {
        }
    protected:
        virtual void setup()
        {
            asio_tcp_client::setup();

            boost::asio::ip::tcp::endpoint endpoint;
            endpoint.address(boost::asio::ip::address_v4::from_string("127.0.0.1"));
            endpoint.port(6789);
            connect(endpoint);

            // TODO FIXME temp
            flow::bind((event_ptr)hello_req::_new(), &hello_client::send, (asio_tcp_client*)this);
        }
    };

} } }  // namespace x2boost::samples::hello_world

using namespace x2boost;
using namespace x2boost::samples::hello_world;

void log_handler(int level, const std::string& message)
{
    std::cout << "[x2] " << log_level::string(level) << " " << message << std::endl;
}

int main()
{
    log::level = log_level::all;
    log::handler = log_handler;

    hub::instance()
        .attach(flow_ptr(new single_threaded_flow<>())
            ->add(case_ptr(new output_case()))
            ->add(case_ptr(new hello_client())));

    hub::flows flows;
    flows.startup();

    while (true)
    {
        std::string message;
        std::getline(std::cin, message);
        if (message == "quit")
        {
            break;
        }
        else
        {
            hello_req_ptr req = hello_req::_new();
            req->name(message);
            hub::post(req);
        }
    }
    return 0;
}