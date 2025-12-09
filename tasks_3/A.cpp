#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

using std::pair;
using std::string;
using std::vector;

class Shape {
public:
  virtual double Area() const = 0;
  virtual double Perimeter() const = 0;
  virtual ~Shape() {};
};

class Triangle : public Shape {
private:
  double a[3];

public:
  Triangle(const vector<double> &t) {
    if (t.size() != 3) {
      throw std::exception();
    }
    for (int i = 0; i < 3; i++) {
      a[i] = t[i];
    }
  }
  double Area() const {
    double p = Perimeter() / 2.0;
    return sqrt(p) * sqrt(p - a[0]) * sqrt(p - a[1]) * sqrt(p - a[2]);
  }
  double Perimeter() const { return a[0] + a[1] + a[2]; }
};

class Circle : public Shape {
private:
  double r;

public:
  Circle(const vector<double> &t) {
    if (t.size() != 1) {
      throw std::exception();
    }
    r = t[0];
  };
  double Area() const { return M_PI * r * r; }
  double Perimeter() const { return 2 * M_PI * r; }
};

class Rectangle : public Shape {
private:
  double a, b;

public:
  Rectangle(const vector<double>& t) {
    if (t.size() != 2) {
      throw std::exception();
    }
    a = t[0];
    b = t[1];
  };
  double Area() const { return a * b; }
  double Perimeter() const { return 2 * (a + b); }
};

class Square : public Shape {
private:
  double a;

public:
  Square(const vector<double> &t) {
    if (t.size() != 1) {
      throw std::exception();
    }
    a = t[0];
  };
  double Area() const { return a * a; }
  double Perimeter() const { return 4 * a; }
};

vector<Shape *> CreateShapes(const vector<pair<string, vector<double>>> &get) {
  vector<Shape *> res;
  for (size_t i = 0; i < get.size(); i++) {
    if (get[i].first == "Triangle") {
      res.push_back(new Triangle(get[i].second));
    } else if (get[i].first == "Circle") {
      res.push_back(new Circle(get[i].second));
    } else if (get[i].first == "Rectangle") {
      res.push_back(new Rectangle(get[i].second));
    } else if (get[i].first == "Square") {
      res.push_back(new Square(get[i].second));
    }
  }
  return res;
}
double CalcArea(const vector<Shape *> &shapes) {
  double area = 0;
  for (size_t i = 0; i < shapes.size(); i++) {
    area += shapes[i]->Area();
  }
  return area;
}
double CalcPerimeter(const vector<Shape *> &shapes) {
  double perimeter = 0;
  for (size_t i = 0; i < shapes.size(); i++) {
    perimeter += shapes[i]->Perimeter();
  }
  return perimeter;
}
void DestructShapes(vector<Shape *> &&shapes) {
  for (size_t i = 0; i < shapes.size(); i++) {
    delete shapes[i];
  }
}
