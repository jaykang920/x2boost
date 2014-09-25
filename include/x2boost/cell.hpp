// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_CELL_HPP_
#define X2BOOST_CELL_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/thread/once.hpp>

namespace x2
{
    class X2BOOST_API cell
    {
    public:
        virtual ~cell() {}

        /** Supports light-weight custom type hierarchy for cell and its subclasses. */
        struct tag
        {
            /** Initializes a new instance of the cell::tag class. */
            tag(tag* base, int num_props)
            {
                this->base = base;
                this->num_props = num_props;
                offset = 0;
                if (base)
                {
                    offset = base->offset + base->num_props;
                }
            }

            /** Gets the pointer to the immediate base type tag. */
            tag* base;
            /** Gets the number of immediate (directly defined) properties in this type. */
            int num_props;
            /** Gets the fingerprint base offset for immediate properties in this type. */
            int offset;
        };

        //static const tag& _tag();

    private:
        //static tag _t;
        static boost::once_flag once_;
    };
}

#endif  // X2BOOST_CELL_HPP_
