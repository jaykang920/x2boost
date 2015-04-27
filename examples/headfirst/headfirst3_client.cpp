#include <boost/asio.hpp>

#include <x2boost/flows/single_threaded_flow.hpp>
#include <x2boost/links/asio_tcp_client.hpp>

#include "head_first.hpp"
#include "headfirst_cases.hpp"

namespace x2 { namespace examples { namespace head_first {

    class capitalizer_client : public asio_tcp_client
    {
    public:
        capitalizer_client()
            : asio_tcp_client("capitalizer_server")
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
            flow::bind((event_ptr)capitalize_req::_new(), &capitalizer_client::send, (asio_tcp_client*)this);
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
            ->add(case_ptr(new output_case()))
            ->add(case_ptr(new capitalizer_client())));

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
            capitalize_req_ptr req = capitalize_req::_new();
            req->message(message);
            hub::post(req);
        }
    }
    return 0;
}