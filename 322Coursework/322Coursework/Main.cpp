#include <iostream>
#include <glm/glm/glm.hpp>
#include <SDL2/include/SDL.h>
#include <fstream>
#include <vector>
#include "Shape.h"
#include "AABox.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "objloader.h"
#include "Light.h"
#include<chrono>
#include<thread>
#include<mutex>

using namespace std;
using namespace glm;

const int width = 640.0;
const int height = 480.0;

const float divWidth = 1.0f/width;
const float divHeight = 1.0f/height;

vec3 image[width][height];
float IAR = width / height;
float fovNum = 90.0f;
float fov = radians(fovNum);
vec3 rayOrigin(0.2f, 0, 0);
vec3 ia(0.5f, 0.5f, 0.5f);
vec3 intensity(0.8f, 0.8f, 0.8f);
vector<Shape*> objects;
vector<AABox*> boxes;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

mutex m;

//toggles
bool mThread = false;
bool shadows = true;

#undef main;

SDL_Event event;

void loadComplexObj(const char* fileName, vector<AABox*>& boxes, float xOffset, float yOffset, float zOffset) {
	vector<vec3> vertices;
	vector<vec3> normals;

	vec3 smallest = vec3(INFINITY);
	vec3 largest = vec3(-INFINITY);

	AABox* newBox = new AABox;

	
	bool res = loadOBJ(fileName, vertices, normals);
	for (int i = 0; i < vertices.size(); i += 3)
	{
		Shape* newTri = new Triangle(
			vec3(vertices[i].x + xOffset, vertices[i].y + yOffset, vertices[i].z + zOffset),
			vec3(vertices[i + 1].x + xOffset, vertices[i + 1].y + yOffset, vertices[i + 1].z + zOffset),
			vec3(vertices[i + 2].x + xOffset, vertices[i + 2].y + yOffset, vertices[i + 2].z + zOffset),
			normals[i], 
			normals[i + 1], 
			normals[i + 2],
			vec3(0.5f, 0.5f, 0.5f), 0);

		newBox->meshShapes.push_back(newTri);
	}

	for (int i = 0; i < vertices.size(); i++) {
		smallest = vec3(min(vertices[i].x+xOffset, smallest.x), min(vertices[i].y+yOffset, smallest.y), min(vertices[i].z+zOffset, smallest.z));
		largest = vec3(max(vertices[i].x+xOffset, largest.x), max(vertices[i].y+yOffset, largest.y), max(vertices[i].z+zOffset, largest.z));
	}

	newBox->pMax = largest;
	newBox->pMin = smallest;
	boxes.push_back(newBox);
}

void calcSphereMinMax(AABox* box, vec3 centre, float radius) {
	box->pMax = vec3(centre.x + radius, centre.y + radius, centre.z + radius);
	box->pMin = vec3(centre.x - radius,centre.y - radius,centre.z - radius);
}

void addObjects() {

	float rad = 4;
	Shape* red = new Sphere(vec3(0, 0, -20), rad, vec3(1, 0.32f, 0.36f), 128); 
	AABox* b1 = new AABox;
	calcSphereMinMax(b1, red->centre, rad);
	b1->meshShapes.push_back(red);
	boxes.push_back(b1);

	rad = 3;
	Shape* lightGrey = new Sphere(vec3(-5.5f, 0, -15), rad, vec3(0.9f, 0.9f, 0.9f), 128);
	AABox* b2 = new AABox;
	calcSphereMinMax(b2, lightGrey->centre, rad);
	b2->meshShapes.push_back(lightGrey);
	boxes.push_back(b2);
	
	rad = 2;
	Shape* yellow = new Sphere(vec3(5, -1, -15), 2, vec3(0.9f, 0.76f, 0.46f),128);
	AABox* b3 = new AABox;
	calcSphereMinMax(b3, yellow->centre, rad);
	b3->meshShapes.push_back(yellow);
	boxes.push_back(b3);

	Shape* lightBlue = new Sphere(vec3(5, 0, -25), 2, vec3(0.65f, 0.77f, 0.97f),128);
	AABox* b4 = new AABox;
	calcSphereMinMax(b4, lightBlue->centre, rad);
	b4->meshShapes.push_back(lightBlue);
	boxes.push_back(b4);

	Shape* darkGrey = new Plane(vec3(0, -5, 0), vec3(0, 1, 0), vec3(0.2f, 0.2f, 0.2f),0);
	AABox* b5 = new AABox;
	b5->pMax = vec3(INFINITY, 0.1, INFINITY);
	b5->pMin = -b5->pMax;
	b5->meshShapes.push_back(darkGrey);
	boxes.push_back(b5);

	Shape* trongle = new Triangle(vec3(0, 1, -2), vec3(-1.9f, -1, -2), vec3(1.6f, -0.5f, -2), vec3(0.0f, 0.6f, 1.0f), vec3(-0.4f, -0.4f, 1.0), vec3(0.4, -0.4, 1.0), vec3(0.5, 0.5, 0.0), 100);

	loadComplexObj("teapot_smooth.obj", boxes, -5,1,-10);
	loadComplexObj("teapot_simple.obj", boxes, 10, 2, -15);
}

bool initSDL(SDL_Window*& window, SDL_Surface*& screenSurface)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		//create the window
		window = SDL_CreateWindow("SDL Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			std::cout << "SDL Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			return false;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			return true;
		}
	}
};

void closeSDL(SDL_Window*& window)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
};

Uint32 convertColour(vec3 colour)
{
	int tt;
	Uint8 r, g, b;

	tt = (int)(colour.r * 255);
	if (tt <= 255) r = tt; else r = 255;
	tt = (int)(colour.g * 255);
	if (tt <= 255) g = tt; else g = 255;
	tt = (int)(colour.b * 255);
	if (tt <= 255) b = tt; else b = 255;

	Uint32 rgb;

	//check which order SDL is storing bytes
	rgb = (r << 16) + (g << 8) + b;

	return rgb;
};

void PutPixel32_nolock(SDL_Surface*& surface, int x, int y, Uint32 colour)
{
	Uint8* pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = colour;
};

void FinalRender() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			PutPixel32_nolock(screenSurface, x, y, convertColour(image[x][y]));
		}
	}
	std::cout << endl;
	SDL_UpdateWindowSurface(window);
}

void ThreadRender(int widthThr, int heightThr, int xStart, int yStart, int threadNum) {
	auto start = chrono::system_clock::now();
	for (int x = xStart; x < widthThr; x++) {
		for (int y = yStart; y < heightThr; y++) {

			float xN = (x + 0.5f) * divWidth;
			float yN = (y + 0.5f) * divHeight;
			float xR = (2 * xN - 1) * IAR;
			float yR = 1 - 2 * yN;
			float xC = xR * tan(fov * 0.5f);
			float yC = yR * tan(fov * 0.5f);
			vec3 camSpace(xC, yC, -1);
			vec3 rayDirection = normalize(camSpace - rayOrigin);
			HitInfo info;
			int objectIndex = -1;
			int boxIndex = -1;
			float closest = INFINITY;

			for (int i = 0; i < boxes.size(); i++) {
				if (boxes[i]->Intersection(rayOrigin, rayDirection)) {
					for (int j = 0; j < boxes[i]->meshShapes.size(); j++) {
						if (boxes[i]->meshShapes[j]->Intersection(rayOrigin, rayDirection, info)) {
							if (info.hit == true && info.t < closest) {
								closest = info.t;
								boxIndex = i;
								objectIndex = j;
							}
						}
					}
				}
			}
			if (objectIndex != -1 && boxIndex != -1) {
				image[x][y] = Light::Shading(ia, intensity, rayDirection, boxes[boxIndex]->meshShapes, objectIndex, boxes, boxIndex, rayOrigin, shadows);
			}
			else {
				image[x][y] = vec3(0, 0, 0);
			}
		}
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Thread " << threadNum << " Time taken: " << elapsed_seconds.count() << endl;
}

void Render() {
	if (mThread) {
		std::vector<thread> threads;
		const int threadCount = 32;
		const int widthNum = width / threadCount;
		for (int i = 0; i < threadCount; i++) {
			threads.push_back( thread (ThreadRender, widthNum * (i + 1), height, ((widthNum)*i) , 0, i));
		}

		for (int j = 0; j < threads.size(); j++) {
			threads[j].join();
		}
	}
	else {
		auto start = chrono::system_clock::now();
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {

				float xN = (x + 0.5f) * divWidth;
				float yN = (y + 0.5f) * divHeight;
				float xR = (2 * xN - 1) * IAR;
				float yR = 1 - 2 * yN;
				float xC = xR * tan(fov * 0.5f);
				float yC = yR * tan(fov * 0.5f);
				vec3 camSpace(xC, yC, -1);
				vec3 rayDirection = normalize(camSpace - rayOrigin);
				HitInfo info;
				int objectIndex = -1;
				int boxIndex = -1;
				float closest = INFINITY;

				for (int i = 0; i < boxes.size(); i++) {
					if (boxes[i]->Intersection(rayOrigin, rayDirection)) {
						for (int j = 0; j < boxes[i]->meshShapes.size(); j++) {
							if (boxes[i]->meshShapes[j]->Intersection(rayOrigin, rayDirection, info)) {
								if (info.hit == true && info.t < closest) {
									closest = info.t;
									boxIndex = i;
									objectIndex = j;
								}
							}
						}
					}
				}
				if (objectIndex != -1 && boxIndex != -1) {
					image[x][y] = Light::Shading(ia, intensity, rayDirection, boxes[boxIndex]->meshShapes, objectIndex, boxes, boxIndex, rayOrigin, shadows);
				}
				else {
					image[x][y] = vec3(0, 0, 0);
				}
			}
		}
		auto end = chrono::system_clock::now();
		chrono::duration<double> elapsed_seconds = end - start;
		std::cout <<"Time taken: " << elapsed_seconds.count() << endl;
	}
	FinalRender();
}

int main() {
	addObjects();

	if (!initSDL(window, screenSurface)) return -1;
	Render();
	
	bool quit = false;
	while (!quit)
	{
		//Keyboard Input
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					Light::lightPos.x = Light::lightPos.x - 0.5f;
					Render();
					break;
				case SDLK_RIGHT:
					Light::lightPos.x = Light::lightPos.x + 0.5f;
					Render();
					break;
				case SDLK_UP:
					Light::lightPos.y = Light::lightPos.y + 0.5f;
					Render();
					break;
				case SDLK_DOWN:
					Light::lightPos.y = Light::lightPos.y - 0.5f;
					Render();
					break;
				case SDLK_q:
					Light::lightPos.z = Light::lightPos.z + 0.5f;
					Render();
					break;
				case SDLK_e:
					Light::lightPos.z = Light::lightPos.z - 0.5f;
					Render();
					break;
				case SDLK_i:
					intensity = vec3(intensity.x + 0.1f, intensity.y + 0.1f, intensity.z + 0.1f);
					Render();
					break;
				case SDLK_k:
					intensity = vec3(intensity.x - 0.1f, intensity.y -0.1f, intensity.z - 0.1f);
					Render();
					break;
				case SDLK_t:
					if (mThread) {
						mThread = false;
					}
					else {
						mThread = true;
					}
					Render();
					break;
				case SDLK_s:
					if (shadows) {
						shadows = false;
					}
					else {
						shadows = true;
					}
					Render();
					break;
			}
			}
		}
	}

	closeSDL(window);
	return 0;
}