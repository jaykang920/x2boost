#ifndef X2_EXAMPLES_HEADFIRST_HEADFIRST_CASES_HPP_
#define X2_EXAMPLES_HEADFIRST_HEADFIRST_CASES_HPP_

#include <boost/algorithm/string.hpp>

#include <x2boost/x2boost.hpp>

#include "head_first.hpp"

//using namespace x2;

namespace x2 {
namespace examples {
namespace head_first {

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

}  // namespace head_first
}  // namespace examples
}  // namespace x2

#endif  // X2_EXAMPLES_HEADFIRST_HEADFIRST_CASES_HPP_
