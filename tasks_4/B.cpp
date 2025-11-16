#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

class BCD {
public:
  // конструктор для создания числа `integral.fractional`
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

  // конструктор для создания числа из строки
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
    for (size_t k = sfrac.size(); k > 0; k--) {
      vfrac_[k - 1] = sfrac[k - 1] - '0';
    }
    normalize();
  }
  std::string getString() const {
    if (isZero()) {
      return "0";
    }
    std::string res = "";
    if (sign_ == -1) {
      res += '-';
    }
    int i = n_ - 1;
    for (; i >= 0 && vint_[i] == 0; i--)
      ;
    if (i == -1) {
      res += '0';
    } else {
      for (; i >= 0; i--) {
        res += ('0' + vint_[i]);
      }
    }
    if (!vfrac_.empty()) {
      res += '.';
      for (size_t j = 0; j < vfrac_.size(); j++) {
        res += ('0' + vfrac_[j]);
      }
    }
    return res;
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
  std::vector<uint8_t> getVInt() const {
    std::vector<uint8_t> res;
    int i = n_ - 1;
    for (; i >= 0 && vint_[i] == 0; i--)
      ;
    for (; i >= 0; i--) {
      res.push_back(vint_[i]);
    }
    if (res.empty()) {
      res.push_back(0);
    }
    return res;
  }
  std::vector<uint8_t> getVFrac() const { return vfrac_; }
  bool isZero() const {
    for (int i = 0; i < n_; i++) {
      if (vint_[i] != 0) {
        return false;
      }
    }
    return vfrac_.empty();
  }

  BCD(BCD &&other) {
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
  BCD &operator*=(const BCD &rhs) {
    std::vector<int> a, b;
    int i = n_ - 1;
    for (; i >= 0 && vint_[i] == 0; i--)
      ;
    for (; i >= 0; i--) {
      a.push_back(vint_[i]);
    }
    for (auto c : vfrac_) {
      a.push_back(c);
    }
    i = n_ - 1;
    for (; i >= 0 && rhs.vint_[i] == 0; i--)
      ;
    for (; i >= 0; i--) {
      b.push_back(rhs.vint_[i]);
    }
    for (auto c : rhs.vfrac_) {
      b.push_back(c);
    }
    int n = a.size(), m = b.size();
    std::vector<int> prod(n + m);
    for (int i = n - 1; i >= 0; i--) {
      for (int j = m - 1; j >= 0; j--) {
        prod[i + j + 1] += a[i] * b[j];
      }
    }
    for (int i = n + m - 1; i > 0; i--) {
      int f = prod[i] / 10;
      prod[i] %= 10;
      prod[i - 1] += f;
    }
    BCD res;
    res.sign_ = this->sign_ * rhs.sign_;
    int int_size = prod.size() - vfrac_.size() - rhs.vfrac_.size();
    int frac_size = vfrac_.size() + rhs.vfrac_.size();
    res.vfrac_.resize(frac_size);
    for (int i = 0; i < frac_size; i++) {
      res.vfrac_[i] = (uint8_t)prod[int_size + i];
    }
    for (int i = 0; i < int_size; i++) {
      res.vint_[i] = prod[int_size - i - 1];
    }
    res.normalize();
    (*this) = res;
    return *this;
  }
  BCD &operator*=(int multiplier) {
    BCD tmp(multiplier);
    (*this) *= tmp;
    return (*this);
  }

private:
  int n_ = 10;
  int sign_ = 1;
  uint8_t vint_[10];
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

// арифметика
BCD operator+(BCD lhs, const BCD &rhs) {
  lhs += rhs;
  return lhs;
}
BCD operator+(const BCD &bcd) { return bcd; }
BCD operator-(BCD lhs, const BCD &rhs) {
  lhs -= rhs;
  return lhs;
}
BCD operator-(const BCD &bcd) { return BCD(0, 0) - bcd; }
BCD operator*(BCD lhs, const BCD &rhs) {
  lhs *= rhs;
  return lhs;
}
BCD operator*(BCD lhs, int d) {
  lhs *= d;
  return lhs;
}
BCD operator*(int d, BCD lhs) { return lhs * d; }

// операторы сравнения
bool operator==(const BCD &lhs, const BCD &rhs) { return (lhs - rhs).isZero(); }
bool operator!=(const BCD &lhs, const BCD &rhs) { return !(lhs == rhs); }
bool operator<(const BCD &lhs, const BCD &rhs) {
  return lhs != rhs && (lhs - rhs).getSign() == -1;
}
bool operator<=(const BCD &lhs, const BCD &rhs) {
  return lhs == rhs || (lhs - rhs).getSign() == -1;
}
bool operator>=(const BCD &lhs, const BCD &rhs) { return !(lhs < rhs); }
bool operator>(const BCD &lhs, const BCD &rhs) { return !(lhs <= rhs); }

// оператор вывода
std::ostream &operator<<(std::ostream &out, const BCD &bcd) {
  out << bcd.getString();
  return out;
}

int floor(const BCD &bcd) {
  int Int = bcd.getInteger();
  if (Int >= 0) {
    return Int;
  } else {
    if ((BCD(Int) - bcd) > BCD(0)) {
      return Int - 1;
    } else {
      return Int;
    }
  }
}

int round(const BCD &bcd) {
  int Int = bcd.getInteger();
  if (Int >= 0) {
    if ((BCD(Int + 1) - bcd) > (bcd - BCD(Int))) {
      return Int;
    } else {
      return Int + 1;
    }
  } else {
    if ((bcd - BCD(Int - 1)) > (BCD(Int) - bcd)) {
      return Int;
    } else {
      return Int - 1;
    }
  }
}

int ceil(const BCD &bcd) {
  int Int = bcd.getInteger();
  if (Int >= 0) {
    if ((bcd - BCD(Int)) > BCD(0)) {
      return Int + 1;
    } else {
      return Int;
    }
  } else {
    return Int;
  }
}

BCD CalcReciprocal(int N, size_t accuracy) {
  if (N == 1) {
    return BCD(1);
  }
  int sign = 1;
  if (N < 0) {
    sign = -1;
    N = -N;
  }
  std::vector<uint8_t> frac;
  int rem = 1;
  for (size_t i = 0; i < accuracy + 1; i++) {
    rem *= 10;
    int q = rem / N;
    rem = rem % N;
    frac.push_back((uint8_t)q);
  }
  if (frac[accuracy] >= 5) {
    frac[accuracy - 1]++;
  }
  frac.pop_back();
  std::string s = "";
  if (sign == -1) {
    s += '-';
  }
  s += '0';
  if (!frac.empty()) {
    s += '.';
    for (auto d : frac) {
      s += ('0' + d);
    }
  }
  return BCD(s);
}

BCD divideByInt(const BCD &bcd, int n, size_t accuracy) {
  std::vector<uint8_t> div;
  for (auto d : bcd.getVInt()) {
    div.push_back(d);
  }
  for (auto d : bcd.getVFrac()) {
    div.push_back(d);
  }
  std::vector<int> digits;
  int rem = 0;
  for (size_t i = 0; i < div.size(); i++) {
    rem = rem * 10 + div[i];
    int tmp = rem / n;
    rem %= n;
    digits.push_back(tmp);
  }
  for (size_t k = 0; k < accuracy; k++) {
    rem *= 10;
    int tmp = rem / n;
    rem %= n;
    digits.push_back(tmp);
  }
  BCD res;
  int frac_size = bcd.getVFrac().size() + accuracy;
  int int_size = digits.size() - frac_size;
  std::string num = "";
  if (int_size <= 0) {
    num += "0.";
    for (auto d : digits) {
      num += ('0' + d);
    }
  } else {
    for (int i = 0; i < int_size; i++) {
      num += ('0' + digits[i]);
    }
    num += '.';
    for (int i = int_size; i < (int)digits.size(); i++) {
      num += ('0' + digits[i]);
    }
  }
  res = BCD(num);
  return res;
}

BCD CalcEuler(int N, size_t accuracy) {
  BCD res(0);
  BCD tmp(1);
  res += tmp;
  for (int k = 1; k <= N; k++) {
    tmp = divideByInt(tmp, k, accuracy);
    res += tmp;
  }
  if (res.getVFrac().size() > accuracy) {
    std::string num = res.getString();
    int d = res.getVFrac().size() - accuracy;
    while (d--) {
      num.pop_back();
    }
    res = BCD(num);
  }
  return res;
}
