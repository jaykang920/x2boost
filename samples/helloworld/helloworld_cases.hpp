#ifndef X2_SAMPLES_HELLOWORLD_HELLOWORLD_CASES_HPP_
#define X2_SAMPLES_HELLOWORLD_HELLOWORLD_CASES_HPP_

#include <boost/algorithm/string.hpp>

#include <x2boost/x2boost.hpp>

#include "hello_world.hpp"

//using namespace x2;

namespace x2 {
namespace samples {
namespace hello_world {

    class capitalizer_case : public caze
    {
    protected:
        virtual void setup()
        {
            flow::bind(hello_req::_new(), &capitalizer_case::on_hello_req, this);
        }
    private:
        void on_hello_req(hello_req_ptr req)
        {
            hello_resp_ptr resp = hello_resp::_new();
            resp->_handle(req->_handle());
            resp->message(boost::to_upper_copy(req->name()));
            hub::post(resp);
        }
    };

    class output_case : public caze
    {
    protected:
        virtual void setup()
        {
            flow::bind(hello_resp::_new(), &output_case::on_hello_resp, this);
        }
    private:
        void on_hello_resp(hello_resp_ptr e)
        {
            std::cout << e->message() << std::endl;
        }
    };

}  // namespace hello_world
}  // namespace samples
}  // namespace x2

#endif  // X2_SAMPLES_HELLOWORLD_HELLOWORLD_CASES_HPP_
