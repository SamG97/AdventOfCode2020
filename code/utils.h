#include <chrono>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

class comma_numpunct : public std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const { return ','; }
    virtual std::string do_grouping() const { return "\03"; }
};

template <typename Fn, Fn fn, typename... Args>
typename std::result_of<Fn(Args...)>::type wrapper(Args... args) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = fn(std::forward<Args>(args)...);
    auto stop = std::chrono::high_resolution_clock::now();
    int duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count();

    std::locale comma_locale{std::locale(), new comma_numpunct()};
    std::cout.imbue(comma_locale);
    std::cout << "Function ran in " << duration << " nanoseconds\n";
    std::cout.imbue(std::locale());
    return result;
}
// Timing utility; can wrap a function with timit to give the execution time
// of that function: e.g. timeit(foo)(bar) will time the execution of foo(bar)
#define timeit(FUNC) wrapper<decltype(&FUNC), &FUNC>

std::vector<std::string> split(const std::string& str, const char delim);
