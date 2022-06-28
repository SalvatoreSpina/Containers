#include "vector.hpp"
#include "map.hpp"
#include "stack.hpp"
#include "set.hpp"

int main() {

    std::cout << "Ok, first of all we launch the tester..." << std::endl;
    system("make test");
    std::cout << "\nNow that we know everything works, let's launch the speed test..." << std::endl;
    system("make speedtest");
    std::cout << "\nAnd then the leaks..." << std::endl;
    system("make leak");
    std::cout << "\nAnd now I'll clean everything, so you can easily find what you're looking for" << std::endl;
    system("make fclean");
    return 0;
}