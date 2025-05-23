#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "QAM.hpp"
#include <random>


namespace qam {


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);


std::vector<bool> generate_bits(size_t n, double a) {

    std::vector<bool> res(n);

    for (size_t i = 0; i < n; ++i)
        res[i] = (dis(gen) < a);

    return res;
}



void testing(size_t n, double a, Modul mod) {
    QAM q(mod);

    std::vector<bool> bits = generate_bits(n, a);

    auto signal = q.encode(bits);

    auto decode_signal = q.decode(signal);
    for (size_t i = 0; i < n; ++i) {
        CHECK(decode_signal[i] == bits[i]);
    }
}



TEST_CASE("Testing Modul without noise") {

    std::vector<Modul> modules = { Modul::QPSK, Modul::QAM16, Modul::QAM64};

    SUBCASE("small test") {
        int n = 12;
        for (auto mod : modules) {
            testing(n, 0.2, mod);
            testing(n, 0.7, mod);
            testing(n, 1, mod);
        }
    }
    SUBCASE("big test") {
        for (auto mod : modules) {
            testing(3000, 0.2, mod);
            testing(3000, 0.7, mod);
            testing(3000, 1, mod);
        }
    }
}



}