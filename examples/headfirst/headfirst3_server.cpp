#include <x2boost/flows/single_threaded_flow.hpp>
#include <x2boost/links/asio_tcp_server.hpp>

#include "headfirst_cases.hpp"

namespace x2 { namespace examples { namespace head_first {

    class capitalizer_server : public asio_tcp_server
    {
    public:
        capitalizer_server()
            : asio_tcp_server("capitalizer_server")
        {
        }
    protected:
        virtual void setup()
        {
            asio_tcp_server::setup();
            listen(6789);
        }
    };

} } }  // namespace x2::examples::head_first

using namespace x2;
using namespace x2::examples::head_first;

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
            ->add(case_ptr(new capitalizer_server())));

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