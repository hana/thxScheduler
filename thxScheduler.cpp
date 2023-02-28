#include "thxScheduler.hpp"

using namespace thx;

void IRAM_ATTR Scheduler::operator()() {
    if(calender.empty()) return;
    
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