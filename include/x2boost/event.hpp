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
    // Common base class for all events.
    class X2BOOST_API event : public cell
    {
    public:
        // Supports custom type hierarchy for event and its subclasses.
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

        virtual ~event() {}

        // Determines whether this cell is equal to the other.
        virtual bool _equals(const cell& other) const;
        // Initializes this event object.
        void _initialize()
        {
            _channel_ = NULL;
        }
        // Instantiates a new event object.
        static event_ptr _new()
        {
            return event_ptr(new event);
        }
        // Returns the custom type tag of this class.
        static const tag* _tag();
        // Returns the type identifier of this event.
        int _type_id() const
        {
            const tag* t = static_cast<const tag*>(_type_tag());
            return t->type_id();
        }
        // Returns the custom type tag of the current object.
        virtual const cell::tag* _type_tag() const;

        // Built-in properties
        const char* _channel() const { return _channel_; }
        event& _set_channel(const char* value) { _channel_ = value; }
    
    protected:
        explicit event(std::size_t length) : cell(length + _tag()->num_props())
        {
            _initialize();
        }

        // Describes the immediate properties into the specified stream.
        virtual void _describe(std::ostream& stream) const;

    private:
        event() : cell(_tag()->num_props())
        {
            _initialize();
        }

        const char* _channel_;
    };

    class event_ptr_equivalent : public event_ptr
    {
    public:
        explicit event_ptr_equivalent(const event_ptr& e, const fingerprint& fp)
            : e_(e), fp_(fp) {}
    
        
    private:
        const event_ptr& e_;
        const fingerprint& fp_;
    };

    inline bool operator==(event_ptr const& lhs, event_ptr const& rhs)
    {
        return lhs->_equals(*rhs);
    }
}

#endif  // X2BOOST_EVENT_HPP_
