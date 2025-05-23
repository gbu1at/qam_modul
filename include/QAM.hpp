#pragma once
#include <complex>
#include <vector>
#include <map>

namespace qam {

using complex = std::complex<double>;
using vector_complex = std::vector<complex>;
using vector_bits = std::vector<bool>;

enum class Modul { QPSK, QAM16, QAM64 };

struct QAM {
public:
    QAM(Modul mod) : _mod(mod) {
        _init();
    }

    vector_complex encode(const vector_bits& bits) const;

    vector_bits decode(const vector_complex& signals) const;

private:
    Modul _mod;
    size_t D;
    size_t qside;
    double norm;

    struct ComplexCompare {
        bool operator()(const complex a, const complex& b) const {
            if (a.real() != b.real())
                return a.real() < b.real();
            return a.imag() < b.imag();
        }
    };

    std::map<size_t, complex> _btoc;

    std::map<complex, size_t, ComplexCompare> _ctob;

    void _init_params();

    void _init();

    void _init_btoc_ctob();

    complex _get_nearest_complex_node(const complex& _x) const;
};

}
