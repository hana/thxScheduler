/*
 * function scheduler
 * v1.0 
 * Hananosuke Takimoto
 */

#pragma once

#include <chrono>
#include <cstddef>
#include <deque>
#include <functional>
#include <memory>
#include <unordered_set>

#if defined(ESP32)
#include <esp_attr.h>
#else 
#define IRAM_ATTR
#endif

namespace thx {
    class Scheduler {
    public:
        using clock = std::chrono::system_clock;
        struct base {
            clock::time_point schedule;
            virtual ~base() {};
            virtual void operator()() = 0;
        };

        template<typename Func>
        struct event : public base {
            Func func;
            event(Func&& f) : func(f) {};
            virtual void IRAM_ATTR operator()() override {
                func();
            }          
            virtual ~event(){};  
        };

    private:
        using Calender = std::unordered_set<std::unique_ptr<base>>;
        Calender calender;

    public:
        template<typename Duration, typename F>
        void schedule(const Duration& delay_time, F&& func) {
            const auto now = clock::now();
            const auto scheduled_time = now + delay_time;

            auto p = std::make_unique<event<F>>(std::forward<F>(func));
            p->schedule = scheduled_time;
                    
            calender.emplace(std::move(p));
        };

        template<typename F>
        void schedule(clock::time_point time, F&& func) {            
            auto p = std::make_unique<event<F>>(std::forward<F>(func));
            p->schedule = time;
            calender.emplace(std::move(p));
        }

        void IRAM_ATTR operator()();

        auto empty() const {
            return calender.empty();
        }

        auto size() const {
            return calender.size();
        }

        auto now() const {
            return clock::now();
        }

        static auto sec_from_now(const std::size_t sec) {
            return clock::now() + std::chrono::seconds(sec);
        }

        static auto ms_from_now(const std::size_t ms) {
            return clock::now() + std::chrono::milliseconds(ms);
        }

        static auto us_from_now(const std::size_t us) {
            return clock::now() + std::chrono::microseconds(us);
        }
    }; 

    class scheduler {
        public: 
            static auto& get_instance() {
                static Scheduler instance;
                return instance;
            }

        private:
            scheduler() = default;
            ~scheduler() = default;
            scheduler(const scheduler&) = delete;
            scheduler(scheduler&&) = delete;
            scheduler& operator=(const scheduler&) = delete;
            scheduler& operator=(scheduler&&) = delete;
    };
}