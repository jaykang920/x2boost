// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#ifndef X2BOOST_HUB_HPP_
#define X2BOOST_HUB_HPP_

#ifndef X2BOOST_PRE_HPP_
#include "x2boost/pre.hpp"
#endif

#include <boost/thread/shared_mutex.hpp>
#include <boost/unordered_map.hpp>

namespace x2boost
{
    /// Represents the singleton event distribution bus.
    class X2BOOST_API hub : private boost::noncopyable
    {
    public:
        /// Attaches the specified flow to the hub.
        hub& attach(flow_ptr flow);
        /// Detaches the specified flow from the hub.
        hub& detach(flow_ptr flow);
        /// Detaches all the attached flows;
        void detach_all();

        /// Posts up the specified event to the hub.
        static void post(event_ptr e)
        {
            instance().feed(e);
        }

        /// Starts all the flows attached to the hub.
        static void startup()
        {
            instance().start_attached_flows();
        }
        /// Stops all the flows attached to the hub.
        static void shutdown()
        {
            instance().stop_attached_flows();
        }

        /// Makes the given attached flow subscribe to the specifieid channel.
        void subscribe(flow_ptr flow, const char* channel);
        /// Makes the given attacehd flow unsubscribe from the specified channel.
        void unsubscribe(flow_ptr flow, const char* channel);

        /// Gets the singleton instance of the hub.
        static hub& instance() { return instance_; }

        /// Represents the attached flows for cleanup convenience.
        class flows : private boost::noncopyable
        {
        public:
            ~flows() { shutdown(); }

            void startup() { hub::startup(); }
            void shutdown() { hub::shutdown(); }
        };

    private:
        typedef std::vector<flow_ptr> list_type;
        typedef boost::unordered_map<const char*, list_type> map_type;

        // Private constructor to prevent explicit instantiation
        hub() {}

        void feed(event_ptr e) const;
        void start_attached_flows() const;
        void stop_attached_flows() const;
        void subscribe_internal(flow_ptr flow, const char* channel);
        void unsubscribe_internal(flow_ptr flow);
        void unsubscribe_internal(flow_ptr flow, const char* channel);

        static hub instance_;

        // List of all the flows attached to the hub
        list_type flows_;
        // Explicit (named) channel subscription map
        map_type subscriptions_;

        mutable boost::shared_mutex mutex_;
    };
}

#endif  // X2BOOST_HUB_HPP_
