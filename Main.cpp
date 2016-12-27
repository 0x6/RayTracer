#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "Model.h"
#include "Vector.h"
#include "Camera.h"
#include "Scene.h"
using namespace std;

struct Ray {
	Vector origin;
	Vector direction;
};

struct Pixel {
	double distance;
	Material* material;
	Vector surfaceNormal;
	Vector color;
	Vector intersectPoint;
};

Pixel triangleIntersect(Ray ray, Model* model, Camera* camera) {
	double width = camera->resX, height = camera->resY;
	double lx = ray.origin.x, ly = ray.origin.y, lz = ray.origin.z;
	double dx = ray.direction.x, dy = ray.direction.y, dz = ray.direction.z;

	Pixel finalPixel{ INFINITY, NULL, Vector(0, 0, 0), Vector(0, 0, 0) };

	for (vector<Face *>::iterator it = model->faces.begin(); it != model->faces.end(); it++) {
		double denom, numer, beta, gamma, distance;

		Vector* vectA = (**it).vertices.at(0);
		double ax = vectA->x, ay = vectA->y, az = vectA->z;

		Vector* vectB = (**it).vertices.at(1);
		double bx = vectB->x, by = vectB->y, bz = vectB->z;

		Vector* vectC = (**it).vertices.at(2);
		double cx = vectC->x, cy = vectC->y, cz = vectC->z;

		denom = ((az - cz) * dy - (ay - cy) * dz) * (ax - bx) - ((az - cz) * dx - (ax - cx) * dz) * (ay - by) + ((ay - cy) * dx - (ax - cx) * dy) * (az - bz);
		if (denom == 0) continue;
		numer = ((az - cz) * dy - (ay - cy) * dz) * (ax - lx) - ((az - cz) * dx - (ax - cx) * dz) * (ay - ly) + ((ay - cy) * dx - (ax - cx) * dy) * (az - lz);
		beta = numer / denom;
		if (beta < 0) continue;

		denom = ((az - cz) * dy - (ay - cy) * dz) * (ax - bx) - ((az - cz) * dx - (ax - cx) * dz) * (ay - by) + ((ay - cy) * dx - (ax - cx) * dy) * (az - bz);
		if (denom == 0) continue;
		numer = ((az - lz) * dy - (ay - ly) * dz) * (ax - bx) - ((az - lz) * dx - (ax - lx) * dz) * (ay - by) + ((ay - ly) * dx - (ax - lx) * dy) * (az - bz);
		gamma = numer / denom;
		if (gamma < 0) continue;
		if (gamma + beta > 1) continue;

		denom = ((az - cz) * dy - (ay - cy) * dz) * (ax - bx) - ((az - cz) * dx - (ax - cx) * dz) * (ay - by) + ((ay - cy) * dx - (ax - cx) * dy) * (az - bz);
		if (denom == 0) continue;
		numer = ((ay - ly) * (az - cz) - (ay - cy) * (az - lz)) * (ax - bx) - ((ax - lx) * (az - cz) - (ax - cx) * (az - lz)) * (ay - by) + ((ax - lx) * (ay - cy) - (ax - cx) * (ay - ly)) * (az - bz);
		distance = numer / denom;
		if (distance <= 0) continue;

		if (distance < finalPixel.distance) {
			finalPixel.distance = distance;
			finalPixel.material = (**it).material;

			Vector U = *vectB - *vectA;
			Vector V = *vectC - *vectA;

			Vector N =  U * V;
			N.normalize();

			finalPixel.surfaceNormal = N;
			finalPixel.intersectPoint = Vector(ray.origin + (ray.direction * distance));
		}
	}

	return finalPixel;
}

Pixel sphereIntersect(Ray ray, Sphere sphere) {
	Pixel pixel{ INFINITY, NULL, Vector(0, 0, 0), Vector(0, 0, 0), Vector(0, 0, 0) };

	Vector Tv = sphere.pos - ray.origin;

	double v = Tv % ray.direction;

	double csq = Tv % Tv;
	
	double disc = pow(sphere.radius, 2) - (csq - pow(v, 2)); 
	if (disc > 0) {
		double tval = v - sqrt(disc);
		if(tval > 0){
			pixel.distance = tval;
			pixel.intersectPoint = Vector(ray.origin + (ray.direction * tval));
			pixel.material = sphere.material;
			pixel.surfaceNormal = Vector(pixel.intersectPoint - sphere.pos);
			pixel.surfaceNormal.normalize();
		}
	}

	return pixel;
}


void rayTrace(Ray ray, Vector *accum, Vector attenuation, int level, Scene* scene) {
	Pixel spherePixel{ INFINITY, NULL, Vector({ 0, 0, 0 }), Vector({ 0, 0, 0 }) };
	Pixel trianglePixel{ INFINITY, NULL, Vector({ 0, 0, 0 }), Vector({ 0, 0, 0 }) };
	Pixel finalPixel{ INFINITY, NULL, Vector({ 0, 0, 0 }), Vector({ 0, 0, 0 }) };
	Vector color(0, 0, 0);

	for (vector<Model *>::iterator modelIt = scene->models.begin(); modelIt != scene->models.end(); modelIt++) {
		Pixel temp = triangleIntersect(ray, *modelIt, scene->camera);
		trianglePixel = trianglePixel.distance < temp.distance ? trianglePixel : temp;
	}

	for (vector<Sphere *>::iterator sphereIt = scene->spheres.begin(); sphereIt != scene->spheres.end(); sphereIt++) {
		Pixel temp = sphereIntersect(ray, **sphereIt);
		spherePixel = spherePixel.distance < temp.distance ? spherePixel : temp;
	}

	finalPixel = spherePixel.distance < trianglePixel.distance ? spherePixel : trianglePixel;

	if (finalPixel.distance != INFINITY) {
		Vector normal = finalPixel.surfaceNormal;
		Material material = *finalPixel.material;
		Vector intersect = finalPixel.intersectPoint;

		//Ambient
		color.x += scene->ambient->x * material.ka.x;
		color.y += scene->ambient->y * material.ka.y;
		color.z += scene->ambient->z * material.ka.z;

		for (vector<Light* >::iterator lightIt = scene->lights.begin(); lightIt != scene->lights.end(); lightIt++) {
			Vector toL = *(*lightIt)->pos - intersect;
			toL.normalize(); 
			
			double NdotL = normal % toL;
			
			if (NdotL > 0) {
				//Diffuse
				color.x += material.kd.x * (*lightIt)->color->x * NdotL;
				color.y += material.kd.y * (*lightIt)->color->y * NdotL;
				color.z += material.kd.z * (*lightIt)->color->z * NdotL;

				Vector toC = ray.origin - intersect;
				toC.normalize();

				Vector spR = (normal * 2 * NdotL) - toL;
				spR.normalize();

				double exp = pow(toC % spR, 16);

				//Specular
				color.x += material.kd.x * (*lightIt)->color->x * exp;
				color.y += material.kd.y * (*lightIt)->color->y * exp;
				color.z += material.kd.z * (*lightIt)->color->z * exp;
			}
		}

		accum->x += attenuation.x * color.x;
		accum->y += attenuation.y * color.y;
		accum->z += attenuation.z * color.z;

		if (level > 0) {
			Vector Uinv = ray.direction * -1;
			Vector refR = (normal * (2 * (normal % Uinv))) - Uinv;
			refR.normalize();
			Ray nextRay{ intersect, refR };
	
			rayTrace(nextRay, accum, attenuation * .5, (level - 1), scene);
		}
	}
}

int main(int argc, char* argv[]) {
	Scene scene(argv[1]);

	string outpath = argv[1];
	outpath = outpath.substr(0, outpath.find('.')) + ".ppm";
	ofstream outfile(outpath);
	outfile << "P3\n";
	outfile << scene.camera->resX << " " << scene.camera->resY << " 255\n";

	double ex = scene.camera->focalPoint->x, ey = scene.camera->focalPoint->y, ez = scene.camera->focalPoint->z;
	double lx = scene.camera->lookAtPoint->x, ly = scene.camera->lookAtPoint->y, lz = scene.camera->lookAtPoint->z;
	double upx = scene.camera->upVector->x, upy = scene.camera->upVector->y, upz = scene.camera->upVector->z;

	double near = -1 * scene.camera->d;
	double bottom = scene.camera->bounds[1], left = scene.camera->bounds[0], top = scene.camera->bounds[3], right = scene.camera->bounds[2];
	double width = scene.camera->resX, height = scene.camera->resY;

	Vector EV(ex, ey, ez);
	Vector LV(lx, ly, lz);
	Vector UP(upx, upy, upz);

	Vector WV = EV - LV;
	WV.normalize();
	Vector UV = UP*WV;
	UV.normalize();
	Vector VV = WV*UV;

	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			double px = (((double)(i) / (width - 1))*(right - left)) + left;
			double py = (((double)(j) / (height - 1))*(top - bottom)) + bottom;

			Vector pixpt = EV + (WV * near) + (UV * px) + (VV * py);
			Vector ray = pixpt - EV;
			ray.normalize();

			Ray currentRay { Vector(pixpt), Vector(ray) };

			Vector color(0, 0, 0);

			if (outpath.find("masterwork") != string::npos)
				rayTrace(currentRay, &color, Vector(.9, .9, .9), 6, &scene);
			else {
				rayTrace(currentRay, &color, Vector(.9, .9, .9), 0, &scene);
			}

			int r = (int)(color.x * 255) > 255 ? 255 : (int)(color.x * 255);
			int g = (int)(color.y * 255) > 255 ? 255 : (int)(color.y * 255);
			int b = (int)(color.z * 255) > 255 ? 255 : (int)(color.z * 255);

			outfile << r << " " << g << " " << b << " ";
		}
	}

	outfile.close();

	system("pause");
	return 0;
}