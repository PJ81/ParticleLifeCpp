//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#pragma once

#include "../inc/stdafx.h"

//--------------------------------------------------------------------------------------------------
class cVector {

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ctor
public:
	cVector();
	cVector(float _x, float _y);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ public variables
	float x, y;  // Class variables to store the x and y coordinates

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ public functions
	float angle(const cVector& o) const;
	float cross(const cVector& o) const;
	float distance(const cVector& o) const;
	float distanceSqr(const cVector& o) const;
	float dot(const cVector& o) const;
	bool isZero() const;
	float length() const;
	float lengthSqr() const;
	void normalize();

	bool operator !=(const cVector& o) const;
	cVector operator *(float a) const;
	cVector& operator *=(float a);
	cVector operator +(const cVector& o) const;
	void operator +=(const cVector& o);
	cVector operator -(const cVector& o) const;
	void operator -=(const cVector& o);
	cVector operator /(float a) const;
	cVector& operator /=(float a);
	cVector& operator =(const cVector& o);
	bool operator ==(const cVector& o) const;

	void rotate(float a);
	void rotate(float cosA, float sinA);
	void set(float _x, float _y);
	void zero();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ private variables
private:
	static constexpr float epsilon = 1e-6f; // Small value to handle floating point precision issues
};
//--------------------------------------------------------------------------------------------------
// Default constructor that initializes the vector to (0, 0)
inline cVector::cVector() {
	x = y = 0;
}
//--------------------------------------------------------------------------------------------------
// Constructor that initializes the vector with provided values
inline cVector::cVector(float _x, float _y) {
	x = _x;
	y = _y;
}
//--------------------------------------------------------------------------------------------------
// Function to set the vector to specified values
inline void cVector::set(float _x, float _y) {
	x = _x;
	y = _y;
}
//--------------------------------------------------------------------------------------------------
// Function to normalize the vector (make it unit length)
inline void cVector::normalize() {
	float l = length();
	if (l > epsilon) {
		x /= l;
		y /= l;
	}
	else {
		x = 0;
		y = 0;
	}
}
//--------------------------------------------------------------------------------------------------
// Assignment operator
inline cVector& cVector::operator =(const cVector& o) {
	if (this != &o) {
		x = o.x;
		y = o.y;
	}
	return *this;
}
//--------------------------------------------------------------------------------------------------
// Equality operator
inline bool cVector::operator ==(const cVector& o) const {
	return std::fabs(o.x - x) < epsilon && std::fabs(o.y - y) < epsilon;
}
//--------------------------------------------------------------------------------------------------
// Inequality operator
inline bool cVector::operator !=(const cVector& o) const {
	return std::fabs(o.x - x) > epsilon || std::fabs(o.y - y) > epsilon;
}
//--------------------------------------------------------------------------------------------------
// Subtraction operator
inline cVector cVector::operator -(const cVector& o) const {
	return cVector(x - o.x, y - o.y);
}
//--------------------------------------------------------------------------------------------------
// Addition operator
inline cVector cVector::operator +(const cVector& o) const {
	return cVector(x + o.x, y + o.y);
}
//--------------------------------------------------------------------------------------------------
// Dot product function
inline float cVector::dot(const cVector& o) const {
	return (x * o.x + y * o.y);
}
//--------------------------------------------------------------------------------------------------
// Angle between two vectors (in radians)
inline float cVector::angle(const cVector& o) const {
	return acosf(dot(o) / (length() * o.length()));
}
//--------------------------------------------------------------------------------------------------
// Multiplication by a scalar
inline cVector cVector::operator *(float a) const {
	return cVector(x * a, y * a);
}
//--------------------------------------------------------------------------------------------------
// Division by a scalar
inline cVector cVector::operator /(float a) const {
	if (std::fabs(a) < epsilon) {
		throw std::invalid_argument("Division by zero");
	}
	return cVector(x / a, y / a);
}
//--------------------------------------------------------------------------------------------------
// Compound multiplication by a scalar
inline cVector& cVector::operator *=(float a) {
	x *= a;
	y *= a;
	return *this;
}
//--------------------------------------------------------------------------------------------------
// Compound division by a scalar
inline cVector& cVector::operator /=(float a) {
	if (std::fabs(a) < epsilon) {
		throw std::invalid_argument("Division by zero");
	}
	x /= a;
	y /= a;
	return *this;
}
//--------------------------------------------------------------------------------------------------
// Compound subtraction of another vector
inline void cVector::operator -=(const cVector& o) {
	x -= o.x;
	y -= o.y;
}
//--------------------------------------------------------------------------------------------------
// Compound addition of another vector
inline void cVector::operator +=(const cVector& o) {
	x += o.x;
	y += o.y;
}
//--------------------------------------------------------------------------------------------------
// Function to calculate the length (magnitude) of the vector
inline float cVector::length() const {
	return sqrt(x * x + y * y);
}
//--------------------------------------------------------------------------------------------------
// Function to calculate the squared length (magnitude) of the vector
inline float cVector::lengthSqr() const {
	return x * x + y * y;
}
//--------------------------------------------------------------------------------------------------
// Function to calculate the distance between two vectors
inline float cVector::distance(const cVector& o) const {
	return (*this - o).length();
}
//--------------------------------------------------------------------------------------------------
// Function to calculate the squared distance between two vectors
inline float cVector::distanceSqr(const cVector& o) const {
	return (*this - o).lengthSqr();
}
//--------------------------------------------------------------------------------------------------
// Function to rotate the vector by an angle (in radians)
inline void cVector::rotate(float a) {
	float c = cosf(a), s = sinf(a);
	float X = x * c - y * s,
		Y = x * s + y * c;
	x = X;
	y = Y;
}
//--------------------------------------------------------------------------------------------------
// Function to rotate the vector using precomputed sine and cosine values
inline void cVector::rotate(float cosA, float sinA) {
	float X = x * cosA - y * sinA,
		Y = x * sinA + y * cosA;
	x = X;
	y = Y;
}
//--------------------------------------------------------------------------------------------------
// Function to calculate the cross product with another vector (2D cross product is a scalar)
inline float cVector::cross(const cVector& o) const {
	return x * o.y - y * o.x;
}
//--------------------------------------------------------------------------------------------------
// Function to check if the vector is zero
inline bool cVector::isZero() const {
	return (x == 0 && y == 0);
}
//--------------------------------------------------------------------------------------------------
// Function to set the vector to zero
inline void cVector::zero() {
	x = 0;
	y = 0;
}
//--------------------------------------------------------------------------------------------------