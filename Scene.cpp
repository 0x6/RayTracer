#include "Scene.h"

std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the string into a stream.
	std::string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

Sphere* parseSphere(std::string line) {
	std::vector<std::string> frags = split(line, ' ');
	Vector materialVector(std::stod(frags[4].c_str()), std::stod(frags[5].c_str()), std::stod(frags[6].c_str()));

	return new Sphere{
		Vector(std::stod(frags[0].c_str()), std::stod(frags[1].c_str()), std::stod(frags[2].c_str())),
		std::stod(frags[3].c_str()),
		new Material{ std::string("sphere_material"), Vector(.2, .2, .2), materialVector, materialVector }
	};;
}

Vector* parseAmbient(std::string line) {
	std::vector<std::string> frags = split(line, ' ');
	return new Vector(std::stod(frags[0].c_str()), std::stod(frags[1].c_str()), std::stod(frags[2].c_str()));
}

Light* parseLight(std::string line) {
	std::vector<std::string> frags = split(line, ' ');

	return new Light{
		new Vector(std::stod(frags[0].c_str()), std::stod(frags[1].c_str()), std::stod(frags[2].c_str())),
		std::stod(frags[3].c_str()),
		new Vector(std::stod(frags[4].c_str()), std::stod(frags[5].c_str()), std::stod(frags[6].c_str()))
	};;
}

std::map<std::string, Material *> parseMaterial(std::string path) {
	std::string line;
	std::ifstream infile(path);
	std::map<std::string, Material *> materials;

	while (getline(infile, line)) {
		if (line.size() < 1) continue;
		std::vector<std::string> frags = split(line, ' ');

		if (frags[0] == "newmtl") {
			Material *m = new Material();
			m->id = frags[1];
			std::string temp = frags[1];

			getline(infile, line);
			frags = split(line, ' ');
			m->ka = Vector(std::stod(frags[1].c_str()), std::stod(frags[2].c_str()), std::stod(frags[3].c_str()));

			getline(infile, line);
			frags = split(line, ' ');
			m->kd = Vector(std::stod(frags[1].c_str()), std::stod(frags[2].c_str()), std::stod(frags[3].c_str()));

			getline(infile, line);
			frags = split(line, ' ');
			m->ks = Vector(std::stod(frags[1].c_str()), std::stod(frags[2].c_str()), std::stod(frags[3].c_str()));

			materials[m->id] = m;
		}
	}

	return materials;
}

Model* parseModel(std::string line) {
	Material* currentMaterial = new Material{ std::string("default"), Vector(0.2, 0.2, 0.2), Vector(0.2, 0.2, 0.2), Vector(0.2, 0.2, 0.2) };
	std::vector<std::string> args = split(line, ' ');
	Model *m = new Model(std::stod(args[0].c_str()), std::stod(args[1].c_str()), std::stod(args[2].c_str()));

	std::ifstream infile(args[7]);

	while (getline(infile, line)) {
		if (line.size() < 1) continue;
		std::vector<std::string> frags = split(line, ' ');

		if (frags[0] == "mtllib") {
			std::map<std::string, Material *> temp = parseMaterial(frags[1]);
			m->materials.insert(temp.begin(), temp.end());
		}
		if (frags[0] == "usemtl"){
			currentMaterial = m->materials[frags[1]];
		}
		if (frags[0] == "v")
			m->vertices.push_back(new Vector(std::stod(frags[1].c_str()), std::stod(frags[2].c_str()), std::stod(frags[3].c_str())));
		if (frags[0] == "f") {
			Face *f = new Face;

			for (int i = 1; i < frags.size(); i++)
				f->vertices.push_back(m->vertices.at(std::atoi(frags[i].substr(0, frags[i].find("/")).c_str()) - 1));

			f->material = currentMaterial;
			m->faces.push_back(f);
		}
	}

	return m;
}

Scene::Scene(std::string path) {
	std::ifstream infile(path);
	std::ofstream outfile("cfile");
	std::string line;

	for (int i = 0; i < 6; i++) {
		getline(infile, line);
		outfile << line;

		if (i < 5) outfile << "\n";
	}
	outfile.close();
	camera = new Camera("cfile");
	ambient = new Vector(0, 0, 0);

	while (getline(infile, line)) {
		if (line.substr(0, 1) == "#")
			continue;
		if (line.substr(0, line.find(" ")) == "sphere")
			spheres.push_back(parseSphere(line.substr(line.find(" ") + 1)));
		if (line.substr(0, line.find(" ")) == "light")
			lights.push_back(parseLight(line.substr(line.find(" ") + 1)));
		if (line.substr(0, line.find(" ")) == "ambient")
			ambient = parseAmbient(line.substr(line.find(" ") + 1));
		if (line.substr(0, line.find(" ")) == "model")
			models.push_back(parseModel(line.substr(line.find(" ") + 1)));
	}
	infile.close();
}



