#ifndef TIMER_HPP
#define TIMER_HPP
#include <atomic>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>

class IntervalThread
{
    using interval_signal = boost::signals2::signal<void(void)>;

public:
    using interval_slot_t = interval_signal::slot_type;

    IntervalThread(const int interval_ms = 60, bool _is_active_ = false, bool _is_periodic_ = false)
      : _interval_ms(interval_ms),
        _is_active(_is_active_),
        _is_periodic(_is_periodic_),
        _signal_expired(new interval_signal()) {};

    inline ~IntervalThread(void) { stop(); };

    boost::signals2::connection connect(const interval_slot_t &subscriber)
    {
        // thread-safe: signals2 obtains a mutex on connect()
        return _signal_expired->connect(subscriber);
    };

    void start(void)
    {
        if (is_active())
            return; // Already executed.
        if (get_interval_ms() <= 0)
            return;

        boost::lock_guard<boost::mutex> lock(_timer_thread_guard);
        _timer_thread.interrupt();
        _timer_thread.join();

        _timer_thread = boost::thread(timer_worker(),
                static_cast<int>(get_interval_ms()),
                static_cast<bool>(is_periodic()),
                _signal_expired);
        _is_active = true;
    };

    void reset(void)
    {
        if (is_active())
            stop();
        start();
    }

    void stop(void)
    {
        if (!is_active())
            return; // Already executed.

        boost::lock_guard<boost::mutex> lock(_timer_thread_guard);
        _timer_thread.interrupt();
        _timer_thread.join();
        _is_active = false;
    };

    inline bool is_active(void) const { return _is_active; };

    inline int get_interval_ms(void) const { return _interval_ms; };

    void set_interval_ms(const int interval_ms)
    {
        if (interval_ms <= 0 || get_interval_ms() == interval_ms)
            return;

        // Cache timer activity state.
        const bool was_active = is_active();
        // Initialize timer with new interval.
        if (was_active)
            stop();
        _interval_ms = interval_ms;
        if (was_active)
            start();
    };

    inline bool is_periodic(void) const { return _is_periodic; }
    inline void set_periodic(const bool is_periodic = true) { _is_periodic = is_periodic; }

private:
    // The timer worker for the interval thread.
    struct timer_worker {
        void operator()(const int interval_ms, const bool is_periodic, boost::shared_ptr<interval_signal> signal_expired)
        {
            boost::posix_time::milliseconds duration(interval_ms);
            try {
                do {
                    boost::this_thread::sleep<boost::posix_time::milliseconds>(duration);
                    {
                        boost::this_thread::disable_interruption di;
                        signal_expired->operator()();
                    }
                } while (is_periodic);
            } catch (const boost::thread_interrupted &) {
                // IntervalThread start(), stop() and reset() throws boost::this_thread::interrupt,
                // which is expected since this thread is interrupted. No action neccessary.
            }
        };
    };

    std::atomic<int> _interval_ms;  // Interval, in ms
    std::atomic<bool> _is_active;   // Is the timed interval active?
    std::atomic<bool> _is_periodic; // Is the timer periodic?

    boost::mutex _timer_thread_guard;
    boost::thread _timer_thread;

    // The signal to call on interval expiration.
    boost::shared_ptr<interval_signal> _signal_expired;
};
#endif // TIMER_HPP
