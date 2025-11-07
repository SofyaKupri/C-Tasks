#include <iostream>
#include <string>

class Rational {
private:
    int n_;
    int d_;
    int GCD(int a, int b) {
        int tmp;
        while (a) {
            tmp = a;
            a = b % a;
            b = tmp;
        }
        return b;
    }
    void ToRight() {
        if (d_ < 0) {
            n_ = -n_;
            d_ = -d_;
        }
        int gcd = GCD(abs(n_), d_);
        n_ /= gcd;
        d_ /= gcd;
    }
public:
// constructors
    Rational() {
        n_ = 0, d_ = 1;
    }
    Rational(int n, int d = 1) {
        n_ = n;
        d_ = d;
        ToRight();
    }

// conversion operator; explicit(?)
    explicit operator double() const {
        return 1.0 * n_ / d_;
    }

// arithmetic operators
    Rational& operator+=(const Rational& rhs) {
        n_ = n_ * rhs.d_ + rhs.n_ * d_;
        d_ = d_ * rhs.d_;
        ToRight();
        return *this;
    }
    Rational& operator-=(const Rational& rhs) {
        n_ = n_ * rhs.d_ - rhs.n_ * d_;
        d_ = d_ * rhs.d_;
        ToRight();
        return *this;
    }
    Rational& operator*=(const Rational& rhs) {
        n_ = n_ * rhs.n_;
        d_ = d_ * rhs.d_;
        ToRight();
        return *this;
    }
    Rational& operator/=(const Rational& rhs) {
        int n = n_ * rhs.d_;
        d_ = d_ * rhs.n_;
        n_ = n;
        ToRight();
        return *this;
    }
    Rational operator++() {
        n_ = n_ + d_;
        return *this;
    }
    Rational operator++(int) {
        n_ = n_ + d_;
        return Rational(n_ - d_, d_);
    }
    Rational operator--() {
        n_ = n_ - d_;
        return *this;
    }
    Rational operator--(int) {
        n_ = n_ - d_;
        return Rational(n_ + d_, d_);
    }

// getters
    int Numerator() const {
        return n_;
    }
    int Denominator() const {
        return d_;
    }
};

// stand-alone arithmetic operators
Rational operator+(Rational lhs) {
    return lhs;
}
Rational operator+(Rational lhs, const Rational& rhs) {
    return lhs += rhs;
}
Rational operator-(Rational lhs, const Rational& rhs) {
    return lhs -= rhs;
}
Rational operator*(Rational lhs, const Rational& rhs) {
    return lhs *= rhs;
}
Rational operator/(Rational lhs, const Rational& rhs) {
    return lhs /= rhs;
}
Rational operator-(Rational lhs) {
    return Rational(-lhs.Numerator(), lhs.Denominator());
}

// stand-alone comparison operators
bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() == rhs.Numerator() && lhs.Denominator() == rhs.Denominator();
}
bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}
bool operator<(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() * rhs.Denominator() < lhs.Denominator() * rhs.Numerator();
}
bool operator<=(const Rational& lhs, const Rational& rhs) {
    return lhs < rhs || lhs == rhs;
}
bool operator>=(const Rational& lhs, const Rational& rhs) {
    return !(lhs < rhs);
}
bool operator>(const Rational& lhs, const Rational& rhs) {
    return !(lhs <= rhs);
}

// stream operators
// cout << Rational(1, 2);
// 1/2
std::ostream& operator<<(std::ostream& out, const Rational& r) {
    out << r.Numerator() << '/' << r.Denominator();
    return out;
}

// Rational r;
// cin >> r;
std::istream& operator>>(std::istream& in, Rational& r) {
    std::ios::fmtflags old_flags = in.flags();
    in.unsetf(std::ios::skipws);
    int n;
    int d;
    char c;
    if (!(in >> n)) {
        in.setstate(std::ios::badbit);
        in.flags(old_flags);
        return in;
    }
    if (!(in >> c) || c != '/') {
        in.setstate(std::ios::badbit);
        in.flags(old_flags);
        return in;
    }
    if (!(in >> d)) {
        in.setstate(std::ios::badbit);
        in.flags(old_flags);
        return in;
    }
    in.flags(old_flags);
    if (d == 0) {
        in.setstate(std::ios::badbit);
        return in;
    }
    r = Rational(n, d);
    return in;
}