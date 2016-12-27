#pragma once
#include <cmath>
#include <iostream>

class Vector {
public:
	Vector();
	Vector(double x, double y, double z);

	Vector operator-(const Vector& v);
	Vector operator+(const Vector& v);
	Vector operator*(const Vector& v);
	Vector operator*(const double i);
	Vector operator&(const Vector& v);
	double operator%(const Vector& v);
	bool operator==(const Vector& v);
	bool operator!=(const Vector& v);
	void operator+=(const Vector& v);

	void normalize();
	void print();
	double magnitude();

	double x, y, z;
};