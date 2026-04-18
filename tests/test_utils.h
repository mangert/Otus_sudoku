#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

class TestRunner {
public:
    void addTest(const std::string& name, std::function<bool()> test) {
        tests_.push_back({name, test});
    }
    
    bool runAll() {
        int passed = 0;
        int failed = 0;
        
        for (const auto& [name, test] : tests_) {
            std::cout << "Running: " << name << " ... ";
            try {
                if (test()) {
                    std::cout << "OK\n";
                    ++passed;
                } else {
                    std::cout << "FAILED\n";
                    ++failed;
                }
            } catch (const std::exception& e) {
                std::cout << "EXCEPTION: " << e.what() << "\n";
                ++failed;
            } catch (...) {
                std::cout << "UNKNOWN EXCEPTION\n";
                ++failed;
            }
        }
        
        std::cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===\n";
        return failed == 0;
    }
    
private:
    struct Test {
        std::string name;
        std::function<bool()> func;
    };
    std::vector<Test> tests_;
};

#define ASSERT_TRUE(expr) if (!(expr)) { std::cerr << "Assertion failed: " #expr << " at " << __FILE__ << ":" << __LINE__ << "\n"; return false; }
#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))