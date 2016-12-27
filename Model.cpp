#include "Model.h"

Model::Model(std::string path) {
	std::ifstream infile(path);
	std::string line;

	while (getline(infile, line)) {
		if (line.find("element vertex") != std::string::npos) {
			numVertices = atoi(line.substr(std::string("element vertex").length()).c_str());
		}
		if (line.find("element face") != std::string::npos) {
			numFaces = atoi(line.substr(std::string("element face").length()).c_str());
		}
		if (line.find("end_header") != std::string::npos) {
			break;
		}
	}

	for (int i = 0; i < numVertices; i++) {
		getline(infile, line);

		double x, y, z;

		x = stod(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ") + 1);

		y = stod(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ") + 1);

		z = stod(line);

		vertices.push_back(new Vector(x, y, z));
	}

	for (int i = 0; i < numFaces; i++) {
		Face *f = new Face;

		getline(infile, line);

		line = line.substr(line.find(" ") + 1);
		f->vertices.push_back(vertices.at(atoi(line.substr(0, line.find(" ")).c_str())));

		line = line.substr(line.find(" ") + 1);
		f->vertices.push_back(vertices.at(atoi(line.substr(0, line.find(" ")).c_str())));

		line = line.substr(line.find(" ") + 1);
		f->vertices.push_back(vertices.at(atoi(line.c_str())));

		faces.push_back(f);
	}
}

Model::Model(double _x, double _y, double _z) {
	x = _x, y = _y, z = _z;
	numFaces = 0;
	numVertices = 0;
}