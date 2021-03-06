#include <x2boost/flows/single_threaded_flow.hpp>

#include "hello_world.hpp"
#include "helloworld_cases.hpp"

using namespace x2boost;
using namespace x2boost::samples::hello_world;

int main()
{
    hub::instance()
        .attach(flow_ptr(new single_threaded_flow<>())
            ->add(case_ptr(new capitalizer_case()))
            ->add(case_ptr(new output_case())));

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