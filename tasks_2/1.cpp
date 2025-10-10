#include <iostream>

const int cInf = 2e9;

struct Rectangle {
    Rectangle() {
        left_x = -cInf;
        left_y = cInf;
        right_x = cInf;
        right_y = -cInf;
    }
    int left_x;
    int left_y;
    int right_x;
    int right_y;
    void GetIntersection(Rectangle p) {
        left_x = std::max(left_x, p.left_x);
        left_y = std::min(left_y, p.left_y);
        right_x = std::min(right_x, p.right_x);
        right_y = std::max(right_y, p.right_y);
        if (left_x > right_x || left_y < right_y) {
            left_x = 0;
            left_y = 0;
            right_x = 0;
            right_y = 0;
        }
    }
};

std::istream& operator>>(std::istream& in, Rectangle& p) {
    in >> p.left_x >> p.left_y >> p.right_x >> p.right_y;
    return in;
}
std::ostream& operator<<(std::ostream& out, Rectangle& p) {
    out << p.left_x << " " << p.left_y << " ";
    if (p.left_x == p.right_x || p.left_y == p.right_y) {
        out << "0 0";
    } else {
        out << p.right_x << " " << p.right_y;
    }
    return out;
}

int main() {
    int n;
    std::cin >> n;
    Rectangle res;
    while ((n--) > 0) {
        Rectangle cur;
        std::cin >> cur;
        res.GetIntersection(cur);
    }
    std::cout << res << "\n";
}