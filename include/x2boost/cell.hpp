// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#ifndef X2BOOST_CELL_HPP_
#define X2BOOST_CELL_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/fingerprint.hpp"

namespace x2
{
    template<typename T, int I>
    class property {
        T value;
    public:
        T& operator=(const T& value)
        {
            // touch(I)
            return this->value = value;
        }
        operator T const& () const
        {
            return value;
        }
    };

    class X2BOOST_API cell
    {
    public:
        virtual ~cell() {}

        // Determines whether this cell is equal to the specified one.
        virtual bool equals(const cell& other) const;
        // Determines whether this cell is equivalent to the specified one.
        virtual bool equivalent(const cell& other) const;
        // Returns the hash code for the current object.
        virtual std::size_t hash_code() const;
        // Returns the hash code based on the specified fingerprint.
        virtual std::size_t hash_code(const fingerprint& fingerprint) const;
        // Returns a string that describes the current object.
        std::string string() const;

        /** Supports light-weight custom type hierarchy for cell and its subclasses. */
        class tag
        {
        public:
            // Initializes an empty cell::tag object.
            tag() {}
            
            // Initializes this cell::tag object with the specified fields.
            void set(const tag* base, int num_props)
            {
                base_ = base;
                num_props_ = num_props;
                offset_ = 0;
                if (base)
                {
                    offset_ = base->offset() + base->num_props();
                }
            }

            // Gets the pointer to the immediate base type tag.
            const tag* base() const { return base_; }
            // Gets the number of immediate (directly defined) properties in this type.
            int num_props() const { return num_props_; }
            // Gets the fingerprint base offset for immediate properties in this type.
            int offset() const { return offset_; }

        private:
            const tag* base_;
            int num_props_;
            int offset_;
        };

        // Returns the custom type tag for this class.
        static const tag* _tag();

        bool is_kind_of(const cell& other) const;

        // Returns the custom type tag of this object.
        virtual const tag* type_tag() const;

    protected:
        explicit cell(std::size_t length) : fingerprint_(length) {}

        // Describes the immediate properties into the specified stream.
        virtual void describe(std::ostream& stream) const;

        fingerprint fingerprint_;
    };
}

#endif  // X2BOOST_CELL_HPP_
