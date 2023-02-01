# Scheduler

Schedule function and invoke later.

## example
```cpp
#include "thxScheduler.hpp"

int main() {    
    auto& sch = thx::scheduler::get_instance();

    std::cout << "hello" << std::endl;

    sch.schedule(std::chrono::seconds(1), []() {   // specify duration until the event invocation
        std::cout << "1" << std::endl;
    });

    sch.schedule(std::chrono::seconds(2), []() {
        std::cout << "2" << std::endl;
    });

    sch.schedule(std::chrono::seconds(3), []() {
        std::cout << "3" << std::endl;
    });

    const auto tp = sch.now() + std::chrono::seconds(4);

    sch.schedule(tp, []() { // specify specific time point.
        std::cout << "4" << std::endl;
    });

    while (sch.size()) {
        sch();
    }

    return 0;
}
```