#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

class Node {
public:
  virtual double Eval() const = 0;
  virtual ~Node() = default;
};

class Number : public Node {
public:
  explicit Number(double value) : value_(value) {}
  double Eval() const override { return value_; }

private:
  double value_;
};

int IsUnary(const string &op) {
  if (op == "sin") {
    return 0;
  }
  if (op == "cos") {
    return 1;
  }
  if (op == "tg") {
    return 2;
  }
  if (op == "ctg") {
    return 3;
  }
  if (op == "exp") {
    return 4;
  }
  if (op == "log10") {
    return 5;
  }
  if (op == "sqrt") {
    return 6;
  }
  return -1;
}

class UnaryOperator : public Node {
public:
  UnaryOperator(const shared_ptr<Node> &a, const int &op)
      : a_(std::move(a)), op_(op) {}
  double Eval() const override {
    double a = a_->Eval();
    switch (op_) {
    case 0:
      return sin(a);
    case 1:
      return cos(a);
    case 2:
      return tan(a);
    case 3:
      return 1.0 / tan(a);
    case 4:
      return exp(a);
    case 5:
      return log10(a);
    case 6:
      return sqrt(a);
    default:
      return a;
    }
  }

private:
  shared_ptr<Node> a_;
  int op_;
};

int IsBinary(const string &op) {
  if (op == "+") {
    return 0;
  }
  if (op == "-") {
    return 1;
  }
  if (op == "*") {
    return 2;
  }
  if (op == "/") {
    return 3;
  }
  if (op == "atan2") {
    return 4;
  }
  if (op == "pow") {
    return 5;
  }
  return -1;
}

class BinaryOperator : public Node {
public:
  BinaryOperator(const shared_ptr<Node> &a, const shared_ptr<Node> &b,
                 const int &op)
      : a_(std::move(a)), b_(std::move(b)), op_(op) {}
  double Eval() const override {
    double a = a_->Eval();
    double b = b_->Eval();
    switch (op_) {
    case 0:
      return a + b;
    case 1:
      return a - b;
    case 2:
      return a * b;
    case 3:
      return a / b;
    case 4:
      return atan2(a, b);
    case 5:
      return pow(a, b);
    default:
      return a;
    }
  }

private:
  shared_ptr<Node> a_, b_;
  int op_;
};

class TripleOperator : public Node {
public:
  TripleOperator(const shared_ptr<Node> &a, const shared_ptr<Node> &b,
                 const shared_ptr<Node> &c)
      : a_(std::move(a)), b_(std::move(b)), c_(std::move(c)) {}
  double Eval() const override {
    double x = a_->Eval(), y = b_->Eval(), z = c_->Eval();
    if ((x >= y && y >= z) || (z >= y && y >= x))
      return y;
    if ((y >= x && x >= z) || (z >= x && x >= y))
      return x;
    return z;
  }

private:
  shared_ptr<Node> a_, b_, c_;
};

double EvaluateExpression(const std::string &expr) {
  std::istringstream iss(expr);
  vector<shared_ptr<Node>> st;
  string cur;
  while (iss >> cur) {
    if ((cur[0] >= '0' && cur[0] <= '9') ||
        (cur.size() > 1 && cur[0] == '-' && (cur[1] >= '0' && cur[1] <= '9'))) {
      st.push_back(std::make_shared<Number>(std::stod(cur)));
    } else if (int op = IsUnary(cur); op != -1) {
      auto a = st.back();
      st.pop_back();
      st.push_back(make_shared<UnaryOperator>(a, op));
    } else if (int op = IsBinary(cur); op != -1) {
      auto b = st.back();
      st.pop_back();
      auto a = st.back();
      st.pop_back();
      st.push_back(make_shared<BinaryOperator>(a, b, op));
    } else {
      auto c = st.back();
      st.pop_back();
      auto b = st.back();
      st.pop_back();
      auto a = st.back();
      st.pop_back();
      st.push_back(make_shared<TripleOperator>(a, b, c));
    }
  }
  return st.back()->Eval();
}
