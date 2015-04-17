#include <boost/algorithm/string.hpp>

#include <x2boost/flows/single_threaded_flow.hpp>

#include "head_first.hpp"

using namespace x2;
using namespace x2::samples::head_first;

class capitalizer_case : public caze
{
protected:
    virtual void setup()
    {
        flow::bind(capitalize_req::_new(), &capitalizer_case::on_capitalize_req, this);
    }
private:
    void on_capitalize_req(capitalize_req_ptr req)
    {
        capitalize_resp_ptr resp = capitalize_resp::_new();
        resp->set_result(boost::to_upper_copy(req->message()));
        hub::post(resp);
    }
};

class output_case : public caze
{
protected:
    virtual void setup()
    {
        flow::bind(capitalize_resp::_new(), &output_case::on_capitalize_resp, this);
    }
private:
    void on_capitalize_resp(capitalize_resp_ptr e)
    {
        std::cout << e->result() << std::endl;
    }
};

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
            capitalize_req_ptr req = capitalize_req::_new();
            req->set_message(message);
            hub::post(req);
        }
    }

    return 0;
}