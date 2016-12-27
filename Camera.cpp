#include "Camera.h"

Camera::Camera(std::string path){
	std::ifstream infile(path);
	std::string line;
	int x, y, z, extra;

	//Get focalPoint
	getline(infile, line);
	line = line.substr(line.find(" ") + 1);
	x = atoi(line.substr(0, line.find(" ")).c_str());

	line = line.substr(line.find(" ") + 1);
	y = atoi(line.substr(0, line.find(" ")).c_str());

	z = atoi(line.substr(line.find(" ") + 1).c_str());

	focalPoint = new Vector(x, y, z);

	//Get lookAtPoint
	getline(infile, line);
	line = line.substr(line.find(" ") + 1);
	x = atoi(line.substr(0, line.find(" ")).c_str());

	line = line.substr(line.find(" ") + 1);
	y = atoi(line.substr(0, line.find(" ")).c_str());

	z = atoi(line.substr(line.find(" ") + 1).c_str());

	lookAtPoint = new Vector(x, y, z);

	//Get upVector
	getline(infile, line);
	line = line.substr(line.find(" ") + 1);
	x = atoi(line.substr(0, line.find(" ")).c_str());

	line = line.substr(line.find(" ") + 1);
	y = atoi(line.substr(0, line.find(" ")).c_str());

	z = atoi(line.substr(line.find(" ") + 1).c_str());

	upVector = new Vector(x, y, z);

	//Get d
	getline(infile, line);
	d = atoi(line.substr(line.find(" ") + 1).c_str());

	//Get bounds
	getline(infile, line);
	line = line.substr(line.find(" ") + 1);
	x = atoi(line.substr(0, line.find(" ")).c_str());

	line = line.substr(line.find(" ") + 1);
	y = atoi(line.substr(0, line.find(" ")).c_str());

	line = line.substr(line.find(" ") + 1);
	z = atoi(line.substr(0, line.find(" ")).c_str());

	extra = atoi(line.substr(line.find(" ") + 1).c_str());

 	bounds = new int[4] { x, y, z, extra };

	//Get res
	getline(infile, line);
	line = line.substr(line.find(" ") + 1);
	x = atoi(line.substr(0, line.find(" ")).c_str());
	y = atoi(line.substr(line.find(" ") + 1).c_str());

	resX = x;
	resY = y;
}

void Camera::print() {
	std::cout << "Focal point: " << focalPoint->x << " " << focalPoint->y << " " << focalPoint->z << std::endl;
	std::cout << "Look at point: " << lookAtPoint->x << " " << lookAtPoint->y << " " << lookAtPoint->z << std::endl;
	std::cout << "Up vector: " << upVector->x << " " << upVector->y << " " << upVector->z << std::endl;
	std::cout << "d: " << d << std::endl;
	std::cout << "Bounds: " << bounds[0] << " " << bounds[1] << " " << bounds[2] << " " << bounds[3] << std::endl;
	std::cout << "Res: " << resX << " " << resY << std::endl;
}

