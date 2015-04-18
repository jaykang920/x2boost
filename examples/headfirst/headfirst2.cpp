#include <x2boost/flows/single_threaded_flow.hpp>

#include "head_first.hpp"
#include "headfirst_cases.hpp"

using namespace x2;
using namespace x2::examples::head_first;

int main()
{
    hub::instance()
        .attach(flow_ptr(new single_threaded_flow<>())
            ->add(case_ptr(new capitalizer_case())))
        .attach(flow_ptr(new single_threaded_flow<>())
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
            capitalize_req_ptr req = capitalize_req::_new();
            req->set_message(message);
            hub::post(req);
        }
    }
    return 0;
}