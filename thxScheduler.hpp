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

namespace thx {
    class scheduler {
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
            virtual void operator()() override {
                func();
            }          
            virtual ~event(){};  
        };

    private:
        // using Calender = std::map<clock::time_point, std::function<void(void)>>;
        using Calender = std::unordered_set<std::unique_ptr<base>>;
        Calender calender;

    public:
        template<typename Duration, typename F>
        void delay(const std::chrono::duration<Duration>& delay_time, F&& func) {
            const auto now = clock::now();
            const auto scheduled_time = now + delay_time;

            auto p = std::make_unique<event<F>>(std::forward<F>(func));
            p->schedule = scheduled_time;
                    
            calender.emplace(std::move(p));
        };

        template<typename F>
        void add(clock::time_point time, F&& func) {            
            auto p = std::make_unique<event<F>>(std::forward<F>(func));
            p->schedule = time;
            calender.emplace(std::move(p));
        }

        void operator()() {
            const auto now = clock::now();
            std::deque<Calender::iterator> remove_list;

            for(auto itr = calender.begin(), end = calender.end(); itr != end; ++itr) {
                auto& event = *itr->get();
                
                if (event.schedule < now) {
                    event();
                    remove_list.emplace_back(itr);
                }
            }

            for(const auto& elm : remove_list) {
                calender.erase(elm);
            }
        }

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
}
