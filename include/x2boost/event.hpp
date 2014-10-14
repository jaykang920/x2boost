// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_EVENT_HPP_
#define X2BOOST_EVENT_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/cell.hpp"

namespace x2
{
    class X2BOOST_API event : public cell
    {
    public:
        virtual ~event() {}

        // Supports light-weight custom type hierarchy for event and its subclasses.
        class tag : public cell::tag
        {
        public:
            // Initializes an empty event::tag object.
            tag() {}

            // Initializes this event::tag object with the specified fields.
            void set(const tag* base, int num_props, int type_id)
            {
                cell::tag::set(base, num_props);
                type_id_ = type_id;
            }

            // Gets the integer type identifier.
            int type_id() const { return type_id_; }

        private:
            int type_id_;
        };

        // Returns the custom type tag for this class.
        static const tag* _tag();

        // Returns the custom type tag of this object.
        virtual const cell::tag* type_tag() const;

        const char* _channel() const
        {
            return _channel_;
        }

        event& _set_channel(const char* value)
        {
            _channel_ = value;
        }
    
    protected:
        event(std::size_t length) : cell(length + _tag()->num_props()) {}

    private:
        event() : cell(_tag()->num_props()) {}

        const char* _channel_;
    };
}

#endif  // X2BOOST_EVENT_HPP_
