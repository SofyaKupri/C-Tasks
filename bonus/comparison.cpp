#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <array>

class BCD {
public:
  BCD(int integral = 0, int fractional = 0) {
    for (int i = 0; i < n_; i++) {
      vint_[i] = 0;
    }
    if (integral < 0) {
      sign_ = -1;
      integral = -integral;
    }
    if (integral != 0) {
      for (int i = 0; i < n_; i++) {
        vint_[i] = integral % 10;
        integral /= 10;
      }
    }
    if (fractional != 0) {
      while (fractional > 0) {
        vfrac_.push_back(fractional % 10);
        fractional /= 10;
      }
      std::reverse(vfrac_.begin(), vfrac_.end());
    }
    normalize();
  }

  explicit BCD(const std::string &str) {
    for (int i = 0; i < n_; i++) {
      vint_[i] = 0;
    }
    size_t i = 0;
    if (str[0] == '+') {
      sign_ = 1;
      i++;
    } else if (str[0] == '-') {
      sign_ = -1;
      i++;
    } else {
      sign_ = 1;
    }
    std::string sint = "", sfrac = "";
    for (; i < str.size(); i++) {
      if (str[i] == '.') {
        i++;
        break;
      }
      sint += str[i];
    }
    for (; i < str.size(); i++) {
      sfrac += str[i];
    }
    for (size_t k = sint.size(); k > 0; k--) {
      vint_[sint.size() - k] = sint[k - 1] - '0';
    }
    vfrac_.resize(sfrac.size());
    for (size_t k = 0; k < sfrac.size(); k++) {
      vfrac_[k] = sfrac[k] - '0';
    }
    normalize();
  }
  int getInteger() const {
    int64_t st = 1;
    int res = 0;
    for (int i = 0; i < n_; i++) {
      res += st * vint_[i];
      st *= 10;
    }
    res *= sign_;
    return res;
  }
  int getSign() const { return sign_; }
  bool isZero() const {
    for (int i = 0; i < n_; i++) {
      if (vint_[i] != 0) {
        return false;
      }
    }
    return vfrac_.empty();
  }

  BCD(BCD &&other) {
    std::swap(sign_, other. sign_);
    std::swap(vint_, other.vint_);
    std::swap(vfrac_, other. vfrac_);
    normalize();
  }
  BCD &operator=(BCD &&other) {
    sign_ = other.sign_;
    for (int i = 0; i < n_; i++) {
      vint_[i] = other.vint_[i];
    }
    vfrac_.resize(0);
    vfrac_.resize(other.vfrac_.size());
    for (size_t i = 0; i < other.vfrac_.size(); i++) {
      vfrac_[i] = other.vfrac_[i];
    }
    normalize();
    return (*this);
  }
  BCD(const BCD &other) = default;
  BCD &operator=(const BCD &other) = default;
  ~BCD() = default;

  // операторы
  BCD &operator-=(const BCD &rhs) {
    BCD tmp = rhs;
    tmp.sign_ = -tmp.sign_;
    (*this) += tmp;
    return *this;
  }
  BCD &operator+=(const BCD &rhs) {
    if (sign_ == rhs.sign_) {
      (*this) = addAbs(*this, rhs);
      sign_ = rhs.sign_;
    } else if (this->sign_ == -1) {
      if (compareAbs(*this, rhs) == 1) {
        (*this) = subAbs(*this, rhs);
        sign_ = -1;
      } else {
        (*this) = subAbs(*this, rhs);
        sign_ = 1;
      }
    } else {
      if (compareAbs(*this, rhs) == -1) {
        (*this) = subAbs(*this, rhs);
        sign_ = -1;
      } else {
        (*this) = subAbs(*this, rhs);
        sign_ = 1;
      }
    }
    normalize();
    return *this;
  }

private:
  int n_ = 10001;
  signed char sign_ = 1;
  std::array<uint8_t, 10001> vint_;
  std::vector<uint8_t> vfrac_;

  void normalize() {
    while (!vfrac_.empty() && vfrac_.back() == 0) {
      vfrac_.pop_back();
    }
  }
  BCD addAbs(const BCD &a, const BCD &b) {
    BCD res;
    int f = 0, cur;
    res.vfrac_.resize(std::max(a.vfrac_.size(), b.vfrac_.size()));
    for (int i = std::max(a.vfrac_.size(), b.vfrac_.size()) - 1; i >= 0; i--) {
      cur = f;
      f = 0;
      if (i < (int)a.vfrac_.size()) {
        cur += (int)a.vfrac_[i];
      }
      if (i < (int)b.vfrac_.size()) {
        cur += (int)b.vfrac_[i];
      }
      res.vfrac_[i] = cur % 10;
      if (cur >= 10) {
        f = 1;
      }
    }
    for (int i = 0; i < n_; i++) {
      cur = f + (int)a.vint_[i] + (int)b.vint_[i];
      f = 0;
      res.vint_[i] = cur % 10;
      if (cur > 10) {
        f = 1;
      }
    }
    res.normalize();
    return res;
  }
  BCD subAbs(const BCD &a, const BCD &b) {
    BCD x, y;
    if (compareAbs(a, b) == -1) {
      x = b;
      y = a;
    } else {
      x = a;
      y = b;
    }
    BCD res;
    int f = 0, cur = 0;
    res.vfrac_.resize(std::max(x.vfrac_.size(), y.vfrac_.size()));
    for (int i = std::max(x.vfrac_.size(), y.vfrac_.size()) - 1; i >= 0; i--) {
      cur = -f;
      f = 0;
      if (i < (int)x.vfrac_.size()) {
        cur += (int)x.vfrac_[i];
      }
      if (i < (int)y.vfrac_.size()) {
        cur -= (int)y.vfrac_[i];
      }
      if (cur < 0) {
        f = 1;
        cur += 10;
      }
      res.vfrac_[i] = cur;
    }
    for (int i = 0; i < n_; i++) {
      cur = -f + (int)x.vint_[i] - (int)y.vint_[i];
      f = 0;
      if (cur < 0) {
        f = 1;
        cur += 10;
      }
      res.vint_[i] = cur;
    }
    res.normalize();
    return res;
  }
  int compareAbs(const BCD &a, const BCD &b) const {
    for (int i = n_ - 1; i >= 0; i--) {
      if (a.vint_[i] < b.vint_[i]) {
        return -1;
      } else if (a.vint_[i] > b.vint_[i]) {
        return 1;
      }
    }
    for (int i = 0; i < (int)std::min(a.vfrac_.size(), b.vfrac_.size()); i++) {
      if (a.vfrac_[i] < b.vfrac_[i]) {
        return -1;
      } else if (a.vfrac_[i] > b.vfrac_[i]) {
        return 1;
      }
    }
    if (a.vfrac_.size() < b.vfrac_.size()) {
      return -1;
    } else if (a.vfrac_.size() == b.vfrac_.size()) {
      return 0;
    } else {
      return 1;
    }
  }
};

BCD operator-(BCD lhs, const BCD &rhs) {
  lhs -= rhs;
  return lhs;
}
bool operator==(const BCD &lhs, const BCD &rhs) { return (lhs - rhs).isZero(); }
bool operator!=(const BCD &lhs, const BCD &rhs) { return !(lhs == rhs); }
bool operator<(const BCD &lhs, const BCD &rhs) {
  return lhs != rhs && (lhs - rhs).getSign() == -1;
}

int main() {
  std::string a, b;
  std::cin >> a >> b;
  BCD num1(a), num2(b);
  if (num1 < num2) {
    std::cout << "<\n";
  } else if (num1 == num2) {
    std::cout << "=\n";
  } else {
    std::cout << ">\n";
  }
}