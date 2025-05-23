#include "QAM.hpp"
#include <complex>
#include <vector>
#include <cassert>
#include <iostream>
#include <map>


namespace qam {

vector_complex QAM::encode(const vector_bits& bits) const {
    

    if (bits.size() % D != 0)
        throw std::runtime_error("bits size error\n");

    size_t blocks = bits.size() / D;

    vector_complex res;
    res.reserve(blocks);

    for (size_t block = 0; block < blocks; ++block) {
        size_t code_value = 0;
        for (size_t j = 0; j < D; ++j)
            code_value |= (bits[block * D + j]) << j;

        res.push_back(_btoc.at(code_value));
    }
    return res;
}

vector_bits QAM::decode(const vector_complex& signals) const {        
    vector_bits res;
    res.reserve(signals.size() * D);

    for (size_t i = 0; i < signals.size(); ++i) {
        complex nearest_complex = _get_nearest_complex_node(signals[i]);


        // std::cout << "nearest_complex: " << nearest_complex << '\n';

        if (_ctob.find(nearest_complex) == _ctob.end()) {
            std::cout << "error" << '\n';
            exit(1);
        }

        size_t code_value = _ctob.at(nearest_complex);

        for (size_t i = 0; i < D; ++i)
            res.push_back((code_value >> i) & 1);
    }
    return res;
}



void QAM::_init_params() {
    switch (_mod)
    {
    case (Modul::QPSK):
        D = 2;
        qside = 2;
        norm = sqrtl(2);
        break;
    case (Modul::QAM16):
        D = 4;
        qside = 4;
        norm = sqrt(10);
        break;
    case (Modul::QAM64):
        D = 6;
        qside = 8;
        norm = sqrt(42);
        break;
    default:
        assert(false);
    }
}

void QAM::_init() {
    _init_params();
    _init_btoc_ctob();
}


void QAM::_init_btoc_ctob() {
    int mask = 0;
    for (int re = -qside + 1; re < (int)qside; re += 2)
        for (int im = -qside + 1; im < (int)qside; im += 2) {
            complex c = complex( { re / norm, im / norm } );
            _btoc[mask] = c;
            _ctob[c] = mask;

            mask++;
        }
}


complex QAM::_get_nearest_complex_node(const complex& _x) const {
    complex x = _x * norm;

    // std::cout << "_get_nearest_complex_node\n";

    // std::cout << x << '\n';

    int sign_re = x.real() > 0 ? 1 : -1;
    int sign_im = x.imag() > 0 ? 1 : -1;

    int re = x.real();
    int im = x.imag();

    // std::cout << re << " " << im << '\n';

    if (abs(im) >= qside - 1) {
        im = (qside - 1) * sign_im;
        sign_im *= -1;
    }
    if (abs(re) >= qside - 1) {
        re = (qside - 1) * sign_re;
        sign_re *= -1;
    }

    re -= (re % 2 == 0) * sign_re;
    im -= (im % 2 == 0) * sign_im;

    // std::cout << re << " " << im << '\n';

    // std::cout << "!\n";

    std::vector<int> possible_re = { re, re + sign_re * 2 };
    std::vector<int> possible_im = { im, im + sign_im * 2 };

    complex near_x = { (double)possible_re[0], (double)possible_im[0] };

    for (int pre : possible_re)
        for (int pim : possible_im) {
            // std::cout << pre << " " << pim << '\n';
            if (abs(near_x - x) > abs(complex(pre, pim) - x))
                near_x = complex(pre, pim);
        }

    return near_x / norm;
}



}