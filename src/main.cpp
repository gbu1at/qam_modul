#include "QAM.hpp"
#include "GaussNoiseAdder.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <fstream>


namespace {

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);


std::vector<bool> generate_bits(size_t n, double a) {

    std::vector<bool> res(n);

    for (size_t i = 0; i < n; ++i)
        res[i] = (dis(gen) < a);

    return res;
}

}



int main(int argc, char* argv[]) {
    if (argc < 3)
        throw std::invalid_argument("Argument error. Program must have deviation and module argument\n");

    double dev = std::stod(argv[1]);

    // std::cout << "deviation error: " << dev << '\n';

    std::string smod = argv[2];

    qam::Modul mod;

    if (smod == "QPSK")
        mod = qam::Modul::QPSK;
    else if (smod == "QAM16")
        mod = qam::Modul::QAM16;
    else if (smod == "QAM64")
        mod = qam::Modul::QAM64;
    else
        throw std::invalid_argument("Argument error. Module argument is not correct\n");

    qam::QAM q(mod);

    int N = 1200000;

    std::vector<bool> bits = generate_bits(N, 0.5);

    // std::cout << "bits\n";
    // for (auto x : bits)
    //     std::cout << x << ", ";
    // std::cout << "\n\n";

    std::vector<std::complex<double>> signal = q.encode(bits);

    // std::cout << "signal\n";
    // for (auto x : signal)
    //     std::cout << x << ", ";
    // std::cout << "\n\n";
    
    gauss_noise::GaussNoiseAdder g(dev);

    std::vector<std::complex<double>> noise_signal = g.add_noise(signal);

    // std::cout << "noise_signal\n";
    // for (auto x : noise_signal)
    //     std::cout << x << ", ";
    // std::cout << "\n\n";

    std::vector<bool> decode_noise_signal = q.decode(noise_signal);

    // std::cout << "decode_noise_signal\n";
    // for (auto x : decode_noise_signal)
    //     std::cout << x << ", ";
    // std::cout << "\n\n";

    int diff = 0;

    for (int i = 0; i < N; ++i) {
        diff += (bits[i] != decode_noise_signal[i]);

        if (bits[i] != decode_noise_signal[i]) {
            // std::cout << 
        }
    }

    double per_error = diff / (double) N;

    if (argc == 4) {
        std::string filename = argv[3];
        std::ofstream out(filename);
        out << per_error << '\n';
    } else {
        std::cout << "percent error\n";
        std::cout << per_error << '\n';
    }
}