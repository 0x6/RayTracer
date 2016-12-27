#include "Vector.h"

Vector::Vector() {
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double _x, double _y, double _z) {
	x = _x;
	y = _y;
	z = _z;
}

//Subtraction
Vector Vector::operator-(const Vector& v) {
	return Vector(x - v.x, y - v.y, z - v.z);
}

//Addition
Vector Vector::operator+(const Vector& v) {
	return Vector(x + v.x, y + v.y, z + v.z);
}

//+=
void Vector::operator+=(const Vector& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

//Multiplication with int
Vector Vector::operator*(const double i) {
	return Vector(x*i, y*i, z*i);
}

//Cross Product
Vector Vector::operator*(const Vector& v) {
	return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

//Element product
Vector Vector::operator&(const Vector& v) {
	return Vector(x * v.x, y * v.y, z * v.z);
}

//Dot Product
double Vector::operator%(const Vector& v) {
	return x*v.x + y*v.y + z*v.z;
}

bool Vector::operator==(const Vector& v) {
	if (x != v.x)
		return false;
	if (y != v.y)
		return false;
	if (z != v.z)
		return false;

	return true;
}

bool Vector::operator!=(const Vector& v) {
	if (x != v.x)
		return true;
	if (y != v.y)
		return true;
	if (z != v.z)
		return true;

	return false;
}

void Vector::normalize() {
	double magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}

double Vector::magnitude() {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void Vector::print() {
	std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
}