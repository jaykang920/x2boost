// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_SERIALIZER_HPP_
#define X2BOOST_SERIALIZER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/buffer.hpp"

namespace x2boost
{
    /// Binary wire format serializer.
    class X2BOOST_API serializer
    {
    public:
        /// Constructs a new serializer object that works on the specified
        /// buffer.
        serializer(buffer& buffer) : buffer_(buffer) {}

        // Overloaded write for primitive types

        // Gets the number of bytes required to encode the specified boolean
        // value.
        static int get_encoded_length(bool value) { return 1; }
        // Encodes a boolean value into the underlying buffer.
        void write(bool value)
        {
            buffer_.ensure_capacity(1);
            buffer_.put_byte((byte_t)(value ? 1 : 0));
        }

        // Gets the number of bytes required to encode the specified single
        // byte.
        static int get_encoded_length(byte_t value) { return 1; }
        // Encodes a single byte into the underlying buffer.
        void write(byte_t value)
        {
            buffer_.ensure_capacity(1);
            buffer_.put_byte(value);
        }

        // Gets the number of bytes required to encode the specified 8-bit
        // signed integer.
        static int get_encoded_length(boost::int8_t value) { return 1; }
        // Encodes an 8-bit signed integer into the underlying buffer.
        void write(boost::int8_t value)
        {
            buffer_.ensure_capacity(1);
            buffer_.put_byte((byte_t)value);
        }

        // Gets the number of bytes required to encode the specified 16-bit
        // signed integer.
        static int get_encoded_length(boost::int16_t value) { return 2; }
        // Encodes a 16-bit signed integer into the underlying buffer.
        void write(boost::int16_t value)
        {
            buffer_.ensure_capacity(2);
            buffer_.put_byte((byte_t)(value >> 8));
            buffer_.put_byte((byte_t)value);
        }

        // Gets the number of bytes required to encode the specified 32-bit
        // signed integer.
        static int get_encoded_length(boost::int32_t value)
        {
            return get_encoded_length_variable(
                (boost::uint32_t)((value << 1) ^ (value >> 31)));
        }
        // Encodes a 32-bit signed integer into the underlying buffer.
        void write(boost::int32_t value)
        {
            // Zigzag encoding
            write_variable((boost::uint32_t)((value << 1) ^ (value >> 31)));
        }

        // Gets the number of bytes required to encode the specified 32-bit
        // non-negative integer.
        static int get_encoded_length_nonnegative(boost::int32_t value)
        {
            value &= 0x7fffffff;
            return get_encoded_length_variable((boost::uint32_t)value);
        }
        // Encodes a 32-bit non-negative integer into the underlying buffer.
        void write_nonnegative(boost::int32_t value)
        {
            value &= 0x7fffffff;
            write_variable((boost::uint32_t)value);
        }

        // Gets the number of bytes required to encode the specified 64-bit
        // signed integer.
        static int get_encoded_length(boost::int64_t value)
        {
            return get_encoded_length_variable(
                (boost::uint64_t)((value << 1) ^ (value >> 63)));
        }
        // Encodes a 64-bit signed integer into the underlying buffer.
        void write(boost::int64_t value)
        {
            // Zigzag encoding
            write_variable((boost::uint64_t)((value << 1) ^ (value >> 63)));
        }

        // Gets the number of bytes required to encode the specified 32-bit
        // floating-point number.
        static int get_encoded_length(float value) { return 4; }
        // Encodes a 32-bit floating-point number into the underlying buffer.
        void write(float value)
        {
            write_fixed_big_endian(*reinterpret_cast<boost::int32_t*>(&value));
        }

        // Gets the number of bytes required to encode the specified 64-bit
        // floating-point number.
        static int get_encoded_length(double value) { return 8; }
        // Encodes a 64-bit floating-point number into the underlying buffer.
        void write(double value)
        {
            write_fixed_big_endian(*reinterpret_cast<boost::int64_t*>(&value));
        }

        // Gets the number of bytes required to encode the specified text
        // string.
        static int get_encoded_length(const std::string& value)
        {
            boost::int32_t length = (boost::int32_t)value.length();
            return get_encoded_length_nonnegative(length) + length;
        }
        // Encodes a text string into the underlying buffer.
        void write(const std::string& value);

        // Gets the number of bytes required to encode the specified datetime
        // value.
        static int get_encoded_length(const boost::posix_time::ptime& value)
        {
            return 8;
        }
        // Encodes a datetime value into the underlying buffer.
        void write(const boost::posix_time::ptime& value)
        {
            boost::posix_time::ptime unix_epoch(boost::gregorian::date(1970, 1, 1));
            boost::int64_t msecs = (value - unix_epoch).total_milliseconds();
            write_fixed_big_endian(msecs);
        }

        // Overloaded write for composite types

        // Gets the number of bytes required to encode the specified array of
        // bytes.
        static int get_encoded_length(byte_t* buf)
        {
            return get_encoded_length(buf, 0, (int)(sizeof(buf) / sizeof(byte_t)));
        }
        // Encodes an array of bytes into the underlying buffer.
        void write(byte_t* buf)
        {
            write(buf, 0, (int)(sizeof(buf) / sizeof(byte_t)));
        }
        static int get_encoded_length(byte_t* buf, int offset, int count)
        {
            boost::int32_t length = (buf == 0 ? 0 : count);
            return get_encoded_length_nonnegative(length) + length;
        }
        void write(byte_t* buf, int offset, int count)
        {
            boost::int32_t length = (buf == 0 ? 0 : count);
            write_nonnegative(length);
            buffer_.write(buf, offset, count);
        }

        // Gets the number of bytes required to encode the specified ordered
        // list of Cell-derived objects.
        template<typename T>
        static int get_encoded_length(const std::vector<T>& value)
        {
            boost::int32_t count = (boost::int32_t)value.size();
            int length = get_encoded_length_nonnegative(count);
            for (int i = 0; i < count; ++i)
            {
                length += get_encoded_length(value[i]);
            }
            return length;
        }
        // Encodes an ordered list of Cell-derived objects into the underlying
        // stream.
        template<typename T>
        void write(const std::vector<T>& value)
        {
            boost::int32_t count = (boost::int32_t)value.size();
            write_nonnegative(count);
            for (int i = 0; i < count; ++i)
            {
                write(value[i]);
            }
        }

        // Gets the number of bytes required to encode the specified
        // Cell-derived object.
        template<class T>  // T : cell
        static int get_encoded_length(boost::intrusive_ptr<T> value)
        {
            boost::int32_t length = value ? value->_get_encoded_length() : 0;
            return get_encoded_length_nonnegative(length) + length;
        }
        template<class T>  // T : cell
        static int get_encoded_length(T** value)
        {
            boost::int32_t length = *value == 0 ? 0 : (*value)->get_encoded_length();
            return get_encoded_length_nonnegative(length) + length;
        }
        // Encodes a Cell-derived objects into the underlying buffer.
        template<class T>  // T : cell
        void write(boost::intrusive_ptr<T> value)
        {
            boost::int32_t length = value ? value->_get_encoded_length() : 0;
            write_nonnegative(length);
            if (value)
            {
                value->_serialize(*this);;
            }
        }
        template<class T>  // T : cell
        void write(T** value)
        {
            bool is_null = (*value == 0);
            boost::int32_t length = is_null ? 0 : (*value)->get_encoded_length();
            write_nonnegative(length);
            if (!is_null)
            {
                (*value)->serialize(*this);;
            }
        }

    private:
        // Wrtie helper methods

        // Encodes a 32-bit signed integer into the underlying buffer,
        // by fixed-width big-endian byte order.
        void write_fixed_big_endian(boost::int32_t value)
        {
            buffer_.ensure_capacity(4);
            buffer_.put_byte((byte_t)(value >> 24));
            buffer_.put_byte((byte_t)(value >> 16));
            buffer_.put_byte((byte_t)(value >> 8));
            buffer_.put_byte((byte_t)value);
        }

        // Encodes a 64-bit signed integer into the underlying buffer,
        // by fixed-width big-endian byte order.
        void write_fixed_big_endian(boost::int64_t value)
        {
            buffer_.ensure_capacity(8);
            buffer_.put_byte((byte_t)(value >> 56));
            buffer_.put_byte((byte_t)(value >> 48));
            buffer_.put_byte((byte_t)(value >> 40));
            buffer_.put_byte((byte_t)(value >> 32));
            buffer_.put_byte((byte_t)(value >> 24));
            buffer_.put_byte((byte_t)(value >> 16));
            buffer_.put_byte((byte_t)(value >> 8));
            buffer_.put_byte((byte_t)value);
        }

        // Encodes a 32-bit signed integer into the underlying buffer,
        // by fixed-width little-endian byte order.
        void write_fixed_little_endian(boost::int32_t value)
        {
            buffer_.ensure_capacity(4);
            buffer_.put_byte((byte_t)value);
            buffer_.put_byte((byte_t)(value >> 8));
            buffer_.put_byte((byte_t)(value >> 16));
            buffer_.put_byte((byte_t)(value >> 24));
        }

        // Encodes a 64-bit signed integer into the underlying buffer,
        // by fixed-width little-endian byte order.
        void write_fixed_little_endian(boost::int64_t value)
        {
            buffer_.ensure_capacity(8);
            buffer_.put_byte((byte_t)value);
            buffer_.put_byte((byte_t)(value >> 8));
            buffer_.put_byte((byte_t)(value >> 16));
            buffer_.put_byte((byte_t)(value >> 24));
            buffer_.put_byte((byte_t)(value >> 32));
            buffer_.put_byte((byte_t)(value >> 40));
            buffer_.put_byte((byte_t)(value >> 48));
            buffer_.put_byte((byte_t)(value >> 56));
        }

    public:
        // Gets the number of bytes required to encode the specified 32-bit
        // unsigned integer with unsigned LEB128 encoding.
        static int get_encoded_length_variable(boost::uint32_t value)
        {
            if ((value & 0xffffff80) == 0) { return 1; }
            if ((value & 0xffffc000) == 0) { return 2; }
            if ((value & 0xffe00000) == 0) { return 3; }
            if ((value & 0xf0000000) == 0) { return 4; }
            return 5;
        }
        // Encodes a 32-bit unsigned integer into the underlying buffer,
        // with unsigned LEB128 encoding.
        void write_variable(boost::uint32_t value)
        {
            do
            {
                buffer_.ensure_capacity(1);
                byte_t b = (byte_t)(value & 0x7f);
                value >>= 7;
                if (value != 0)
                {
                    b |= 0x80;
                }
                buffer_.put_byte(b);
            } while (value != 0);
        }

        static int write_variable(byte_t* buffer, boost::uint32_t value)
        {
            int i = 0;
            do
            {
                byte_t b = (byte_t)(value & 0x7f);
                value >>= 7;
                if (value != 0)
                {
                    b |= 0x80;
                }
                buffer[i++] = b;
            } while (value != 0);
            return i;
        }

        // Gets the number of bytes required to encode the specified 64-bit
        // unsigned integer with unsigned LEB128 encoding.
        static int get_encoded_length_variable(boost::uint64_t value)
        {
            if ((value & 0xffffffffffffff80L) == 0) { return 1; }
            if ((value & 0xffffffffffffc000L) == 0) { return 2; }
            if ((value & 0xffffffffffe00000L) == 0) { return 3; }
            if ((value & 0xfffffffff0000000L) == 0) { return 4; }
            if ((value & 0xfffffff800000000L) == 0) { return 5; }
            if ((value & 0xfffffc0000000000L) == 0) { return 6; }
            if ((value & 0xfffe000000000000L) == 0) { return 7; }
            if ((value & 0xff00000000000000L) == 0) { return 8; }
            if ((value & 0x8000000000000000L) == 0) { return 9; }
            return 10;
        }
        // Encodes a 64-bit unsigned integer into the underlying buffer,
        // with unsigned LEB128 encoding.
        void write_variable(boost::uint64_t value)
        {
            do
            {
                buffer_.ensure_capacity(1);
                byte_t b = (byte_t)(value & 0x7f);
                value >>= 7;
                if (value != 0)
                {
                    b |= 0x80;
                }
                buffer_.put_byte(b);
            } while (value != 0);
        }

    private:
        buffer& buffer_;  // underlying buffer
    };
}

#endif  // X2BOOST_SERIALIZER_HPP_
