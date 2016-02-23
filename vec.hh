#ifndef VEC_HH
#define VEC_HH

#include <cmath>

#include "bitmap.hh"

class vec {
public:
  // Create a vector
  vec(float x, float y, float z) : _x(x), _y(y), _z(z) {}
  
  vec() : _x(0), _y(0), _z(0) {}
  
  // Getters for x, y, and z
  float x() { return _x; }
  float y() { return _y; }
  float z() { return _z; }
  
  // Add another vector to this one and return the result
  vec operator+(const vec& other) {
    return vec(_x + other._x, _y + other._y, _z + other._z);
  }
  
  // Add another vector to this one and update in place
  vec& operator+=(const vec& other) {
    _x += other._x;
    _y += other._y;
    _z += other._z;
    return *this;
  }
  
  // Negate this vector
  vec operator-() {
    return vec(-_x, -_y, -_z);
  }
  
  // Subtract another vector from this one and return the result
  vec operator-(const vec& other) {
    return vec(_x-other._x, _y-other._y, _z-other._z);
  }
  
  // Subtract another vector from this one and update in place
  vec& operator-=(const vec& other) {
    _x -= other._x;
    _y -= other._y;
    _z -= other._z;
    return *this;
  }
  
  // Multiply this vector by a scalar and return the result
  vec operator*(float scalar) {
    return vec(_x*scalar, _y*scalar, _z*scalar);
  }
  
  // Multiply this vector by a scalar and update in place
  vec& operator*=(float scalar) {
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
    return *this;
  }
  
  // Divide this vector by a scalar and return the result
  vec operator/(float scalar) {
    return vec(_x/scalar, _y/scalar, _z/scalar);
  }
  
  // Divide this vector by a scalar and update in place
  vec& operator/=(float scalar) {
    _x /= scalar;
    _y /= scalar;
    _z /= scalar;
    return *this;
  }
  
  // Compute the dot product of this vector with another vector
  float dot(const vec& other) {
    return _x*other._x + _y*other._y + _z*other._z;
  }
  
  // Compute the cross product of this vector with another vector
  vec cross(const vec& other) {
    return vec(_y * other._z - _z * other._y,
               _z * other._x - _x * other._z,
               _x * other._y - _y * other._x);
  }
  
  // Compute the hadamard, or component-wise product with another vector
  vec hadamard(const vec& other) {
    return vec(_x * other._x, _y * other._y, _z * other._z);
  }
  
  // Compute the magnitude of this vector
  float magnitude() {
    return sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
  }
  
  // Compute a normalized version of this vector
  vec normalized() {
    return (*this) / this->magnitude();
  }
  
  // Compute a new vector rotated around the x axis
  vec xrotated(float radians) {
    return vec(_x,
               cos(radians) * _y - sin(radians) * _z,
               sin(radians) * _y + cos(radians) * _z);
  }
  
  // Compute a new vector rotated around the y axis
  vec yrotated(float radians) {
    return vec(cos(radians) * _x + sin(radians) * _z,
               _y,
               -sin(radians) * _x + cos(radians) * _z);
  }
  
  // Compute a new vector rotated around the z axis
  vec zrotated(float radians) {
    return vec(cos(radians) * _x - sin(radians) * _y,
               sin(radians) * _x + cos(radians) * _y,
               _z);
  }
  
  // Compute a new vector rotated around another vector
  // Source: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
  vec rotated_around(vec& k, float theta) {
    vec& v = *this;
    return v * cos(theta) + k.cross(v) * sin(theta) + k * k.dot(v) * (1 - cos(theta));
  }
  
private:
  float _x;
  float _y;
  float _z;
};

#endif
