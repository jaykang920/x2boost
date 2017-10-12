#include <x2boost/flows/single_threaded_flow.hpp>
#include <x2boost/links/asio_tcp_server.hpp>

#include "helloworld_cases.hpp"

namespace x2boost { namespace samples { namespace hello_world {

    class hello_server : public asio_tcp_server
    {
    public:
        hello_server()
            : asio_tcp_server("hello_server")
        {
        }
    protected:
        virtual void setup()
        {
            asio_tcp_server::setup();
            listen(6789);

            // TODO FIXME temp
            flow::bind((event_ptr)hello_resp::_new(), &hello_server::send, (asio_tcp_server*)this);
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
            ->add(case_ptr(new capitalizer_case()))
            ->add(case_ptr(new hello_server())));

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
    }
    return 0;
}