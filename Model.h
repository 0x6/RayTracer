#pragma once
#include "Vector.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

struct Material {
	std::string id;
	Vector ka, kd, ks;
};

class Face {
public:
	std::vector<Vector *> vertices;
	Material* material;
};

class Model {
public:
	Model(std::string path);
	Model(double x, double y, double z);
	
	double x, y, z;
	int numFaces, numVertices;
	std::vector<Vector *> vertices;
	std::vector<Face *> faces;
	std::map<std::string, Material *> materials;
};