// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_DESERIALIZER_HPP_
#define X2BOOST_DESERIALIZER_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include "x2boost/buffer.hpp"
#include "x2boost/event_factory.hpp"

namespace x2boost
{
    /// Binary wire format deserializer.
    class X2BOOST_API deserializer
    {
    public:
        /// Constructs a new deserializer object that works on the specified
        /// buffer.
        deserializer(buffer& buffer) : buffer_(buffer) {}

        // Overloaded read for primitive types

        // Decodes a boolean value out of the underlying buffer.
        void read(bool& value)
        {
            buffer_.check_length_to_read(1);
            value = (buffer_.get_byte() != 0);
        }

        // Decodes a single byte out of the underlying buffer.
        void read(byte_t& value)
        {
            buffer_.check_length_to_read(1);
            value = buffer_.get_byte();
        }

        // Decodes an 8-bit signed integer out of the underlying buffer.
        void read(boost::int8_t& value)
        {
            buffer_.check_length_to_read(1);
            value = (boost::int8_t)buffer_.get_byte();
        }

        // Decodes a 16-bit signed integer out of the underlying buffer.
        void read(boost::int16_t& value)
        {
            buffer_.check_length_to_read(2);
            value = (boost::int16_t)buffer_.get_byte();
            value = (boost::int16_t)((value << 8) | buffer_.get_byte());
        }

        // Decodes a 32-bit signed integer out of the underlying buffer.
        int read(boost::int32_t& value)
        {
            // Zigzag decoding
            boost::uint32_t u;
            int bytes = read_variable(u);
            value = (boost::int32_t)(u >> 1) ^ -((boost::int32_t)u & 1);
            return bytes;
        }

        // Decodes a 32-bit non-negative integer out of the underlying buffer.
        int read_nonnegative(boost::int32_t& value)
        {
            boost::uint32_t u;
            int bytes = read_variable(u);
            u &= 0x7fffffffU;
            value = (boost::int32_t)u;
            return bytes;
        }

        // Decodes a 64-bit signed integer out of the underlying buffer.
        int read(boost::int64_t& value)
        {
            // Zigzag decoding
            boost::uint64_t u;
            int bytes = read_variable(u);
            value = (boost::int64_t)(u >> 1) ^ -((boost::int64_t)u & 1);
            return bytes;
        }

        // Decodes a 32-bit floating-point number out of the underlying buffer.
        void read(float& value)
        {
            boost::int32_t i;
            read_fixed_big_endian(i);
            value = *reinterpret_cast<float*>(&i);
        }

        // Decodes a 64-bit floating-point number out of the underlying buffer.
        void read(double& value)
        {
            boost::int64_t l;
            read_fixed_big_endian(l);
            value = *reinterpret_cast<double*>(&l);
        }

        // Decodes a text string out of the underlying buffer.
        void read(std::string& value);

        // Decodes a datetime value out of the underlying buffer.
        void read(boost::posix_time::ptime& value)
        {
            boost::int64_t msecs;
            read_fixed_big_endian(msecs);
            boost::posix_time::ptime unix_epoch(boost::gregorian::date(1970, 1, 1));
            value = unix_epoch + boost::posix_time::milliseconds(msecs);
        }

        // Overloaded read for composite types

        // Decodes an array of bytes out of the underlying buffer.
        void read(byte_t*& value)
        {
            boost::int32_t length;
            read_nonnegative(length);
            buffer_.check_length_to_read(length);
            value = new byte_t[length];
            buffer_.read(value, 0, length);
        }

        // Decodes an ordered list of Cell-derived objects out of the
        // underlying buffer.
        template<typename T>
        void read(std::vector<T>& value)
        {
            boost::int32_t count;
            read_nonnegative(count);
            for (int i = 0; i < count; ++i)
            {
                T element;
                read(element);
                value.push_back(element);
            }
        }

        // Decodes a Cell-derived objects out of the underlying buffer.
        template<class T>  // T : cell
        void read(boost::intrusive_ptr<T> value)
        {
            value.reset();
            int length;
            read_nonnegative(length);
            if (length == 0) { return; }

            int marker = buffer_.pos() + length;

            value.reset(new T());
            value->deserialize(*this);

            if (buffer_.pos() != marker)
            {
                buffer_.pos(marker);
            }
        }
        template<class T>  // T : cell
        void read(T** value)
        {
            *value = NULL;
            int length;
            read_nonnegative(length);
            if (length == 0) { return; }

            int marker = buffer_.pos() + length;

            *value = new T();
            (*value)->deserialize(*this);

            if (buffer_.pos() != marker)
            {
                buffer_.pos(marker);
            }
        }

    private:
        // Read helper methods

        // Decodes a 32-bit signed integer out of the underlying buffer,
        // by fixed-width big-endian byte order.
        void read_fixed_big_endian(boost::int32_t& value)
        {
            buffer_.check_length_to_read(4);
            value = buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
        }

        // Decodes a 64-bit signed integer out of the underlying buffer,
        // by fixed-width big-endian byte order.
        void read_fixed_big_endian(boost::int64_t& value)
        {
            buffer_.check_length_to_read(8);
            value = buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
            value = (value << 8) | buffer_.get_byte();
        }

        // Decodes a 32-bit signed integer out of the underlying buffer,
        // by fixed-width little-endian byte order.
        void read_fixed_little_endian(boost::int32_t& value)
        {
            buffer_.check_length_to_read(4);
            value = (boost::int32_t)buffer_.get_byte();
            value |= (boost::int32_t)buffer_.get_byte() << 8;
            value |= (boost::int32_t)buffer_.get_byte() << 16;
            value |= (boost::int32_t)buffer_.get_byte() << 24;
        }

        // Decodes a 64-bit signed integer out of the underlying buffer,
        // by fixed-width little-endian byte order.
        void read_fixed_little_endian(boost::int64_t& value)
        {
            buffer_.check_length_to_read(8);
            value = buffer_.get_byte();
            value |= (boost::int64_t)buffer_.get_byte() << 8;
            value |= (boost::int64_t)buffer_.get_byte() << 16;
            value |= (boost::int64_t)buffer_.get_byte() << 24;
            value |= (boost::int64_t)buffer_.get_byte() << 32;
            value |= (boost::int64_t)buffer_.get_byte() << 40;
            value |= (boost::int64_t)buffer_.get_byte() << 48;
            value |= (boost::int64_t)buffer_.get_byte() << 56;
        }

    public:
        // Decodes a 32-bit unsigned integer out of the underlying buffer,
        // with unsigned LEB128 decoding.
        int read_variable(boost::uint32_t& value)
        {
            return read_variable_internal(buffer_, value);
        }

        static int read_variable_internal(buffer& buf, boost::uint32_t& value)
        {
            // Unsigned LEB128 decoding
            value = 0U;
            int i, shift = 0;
            for (i = 0; i < 5; ++i)
            {
                buf.check_length_to_read(1);
                byte_t b = buf.get_byte();
                value |= (((boost::uint32_t)b & 0x7fU) << shift);
                if ((b & 0x80) == 0)
                {
                    break;
                }
                shift += 7;
            }
            return (i < 5 ? (i + 1) : 5);
        }

        // Decodes a 64-bit unsigned integer out of the underlying buffer,
        // with unsigned LEB128 decoding.
        int read_variable(boost::uint64_t& value)
        {
            // Unsigned LEB128 decoding
            value = 0UL;
            int i, shift = 0;
            for (i = 0; i < 10; ++i)
            {
                buffer_.check_length_to_read(1);
                byte_t b = buffer_.get_byte();
                value |= (((boost::uint64_t)b & 0x7fU) << shift);
                if ((b & 0x80) == 0)
                {
                    break;
                }
                shift += 7;
            }
            return (i < 10 ? (i + 1) : 10);
        }

    private:
        buffer& buffer_;  // underlying buffer
    };
}

#endif  // X2BOOST_DESERIALIZER_HPP_
