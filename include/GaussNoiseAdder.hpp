#pragma once
#include <stdexcept>
#include <complex>
#include <vector>
#include <random>


namespace gauss_noise {

namespace {

}

using complex = std::complex<double>;
using vector_complex = std::vector<complex>;

struct GaussNoiseAdder {
public:
    GaussNoiseAdder(double dev) : dev(dev), gen(std::random_device{}() ), dis(0.0, dev) {
        if (dev < 0)
            throw std::runtime_error("noise must be > zero");
    }

    vector_complex add_noise(const vector_complex& _data) {
        vector_complex data_with_noise = _data;

        for (auto& c : data_with_noise)
            c += complex( dis(gen), dis(gen) );

        return data_with_noise;
    }

private:
    double dev;
    std::mt19937 gen;
    std::normal_distribution<double> dis;
};

}