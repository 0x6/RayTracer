#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Vector.h"

class Camera
{
public:
	Camera(std::string path);
	void print();

	Vector* focalPoint;
	Vector* lookAtPoint;
	Vector* upVector;

	int d;
	int resX, resY;
	int* bounds;
};

