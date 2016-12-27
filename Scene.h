#pragma once
#include<string>
#include<fstream>
#include<vector>
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<map>
#include "Model.h"
#include "Camera.h"

struct Light {
	Vector* pos;
	double w;
	Vector* color;
};

struct Sphere {
	Vector pos;
	double radius;
	Material* material;
};

class Scene {
public:
	Scene(std::string path);

	Camera* camera;
	std::vector<Sphere *> spheres;
	std::vector<Light *> lights;
	std::vector<Model *> models;
	Vector* ambient;
private:
};