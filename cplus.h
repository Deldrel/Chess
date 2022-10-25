#ifndef CPLUS_CPLUS_H
#define CPLUS_CPLUS_H

#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <cmath>
#include <chrono>
#include <functional>

template<typename Template>
inline void print(Template t) {
    std::cout << t;
}

template<typename Template>
inline void print(const std::vector<Template> &vector) {
    std::string string;
    string.reserve(vector.size());
    for (Template i : vector) string += std::to_string(i) + " ";
    std::cout << string << std::endl;
}

template<typename Template>
inline void println(Template t) {
    std::cout << t << std::endl;
}

template<typename Template>
inline void println(const std::vector<Template> &vector) {
    for (Template i : vector) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

inline int random(int min, int max) {
    static std::mt19937 randGen(time(nullptr));
    return std::uniform_int_distribution<>(min, max)(randGen);
}

inline float map(float n, float min1, float max1, float min2, float max2) {
    return min2 + (max2 - min2) * ((n - min1) / (max1 - min1));
}

inline bool inbound(int x, int y, int upperLeftX, int upperLeftY, int downRightX, int downRightY) {
    return x >= upperLeftX && x < downRightX && y >= upperLeftY && y < downRightY;
}

inline void benchmark() {
    auto t0 = std::chrono::high_resolution_clock::now();
    //code to test
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> us_time = t1 - t0;
    std::cout << "\n" << us_time.count() << "us\n";
}

class vector2f {

public:

    float x, y;

    inline explicit vector2f(float _x = 0, float _y = 0) {
        x = _x;
        y = _y;
    }

    inline ~vector2f() = default;

    inline void add(float _x, float _y) {
        x += _x;
        y += _y;
    }

    inline void add(const vector2f &v) {
        x += v.x;
        y += v.y;
    }

    inline void sub(float _x, float _y) {
        x -= _x;
        y -= _y;
    }

    inline void sub(const vector2f &v) {
        x -= v.x;
        y -= v.y;
    }

    inline void mult(float m) {
        x *= m;
        y *= m;
    }

    inline void div(float d) {
        x /= d;
        y /= d;
    }

    inline float mag() const {
        return (float) sqrt(double(x * x + y * y));
    }

    inline void setMag(float m) {
        this->normalize();
        this->mult(m);
    }

    inline void normalize() {
        float m = this->mag();
        if (m != 0) this->div(m);
        else
            std::cerr << "Error cp::vector2.normalize() : division by zero caused by vector2f magnitude"
                      << std::endl;
    }

    inline void randomize(int min1, int max1, int min2, int max2) {
        x = (float) random(min1, max1);
        y = (float) random(min2, max2);
    }

    inline void set(float _x, float _y) {
        x = _x;
        y = _y;
    }

    inline void print() const {
        std::cout << x << " " << y << std::endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const vector2f &f) {
        os << "x: " << f.x << " y: " << f.y;
        return os;
    }
};

#endif