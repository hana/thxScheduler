# Scheduler

Schedule function and invoke later.

## example
```cpp
#include "thxScheduler.hpp"

int main() {    
    thx::scheduler sch;
        
    sch.delay(std::chrono::seconds(1), []() {
        std::cout << "1" << std::endl;
    });

    sch.delay(std::chrono::seconds(2), []() {
        std::cout << "2" << std::endl;
    });

    sch.delay(std::chrono::seconds(3), []() {
        std::cout << "3" << std::endl;
    });

    const auto tp = sch.now() + std::chrono::seconds(4);

    sch.add(tp, []() {
        std::cout << "4" << std::endl;
    });

    while (true) {
        sch();
    }

    return 0;
}
```