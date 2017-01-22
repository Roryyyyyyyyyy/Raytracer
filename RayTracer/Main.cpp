
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <algorithm>
#include "shape.h"
#include <list>
#include <vector>
#include "objloader.h"
#include <string>
#include "LightSource.h"
#include <ctime>
#include <cstdlib>
#include "BoundingBox.h"
#include <SDL.h>
#include <stdio.h>

using namespace std;
using namespace glm;

int height = 480;
int width = 640;

bool softShadows = false;
bool showTimes = false;

//int done = 0;
SDL_Event event = { 0 };

int raycount = 0;

clock_t beginClock;

LightSource light(vec3(0, 9, 0), 10.0, vec3(1.0, 1.0, 1.0), 8.0);

float angle = 60;

float currentIntersectDist = 100000.0;
float closestIntersect = 100000.0;

vec3 ambientIntensity = vec3(0.1, 0.1, 0.1);
vec3 lightSource = vec3(0.0, 20.0, 0.0);
vec3 lightIntensity = vec3(1.0, 1.0, 1.0);
vec3 specularColour = vec3(0.7, 0.7, 0.7);

vec3 origin = vec3(0.0, 0.0, 0.0);

Shape* redSphere = new Sphere(vec3(0.0, 0.0, -20.0), vec3(1.0, 0.32, 0.36), 4.0, 128.0);
BoundingBox redSphereBounds(vec3(-4, -4, -24), vec3(4, 4, -16));
Shape* yellowSphere = new Sphere(vec3(2.5, -1.0, -15.0), vec3(0.9, 0.76, 0.46), 2.0, 128.0);
BoundingBox yellowSphereBounds(vec3(0.5, -3, -17), vec3(4.5, 1, -13));
Shape* blueSphere= new Sphere(vec3(3.0, 0.0, -25.0), vec3(0.65, 0.77, 0.97), 3.0, 128.0);
BoundingBox blueSphereBounds(vec3(0.0, -3, -28), vec3(5.0, 3.0, -22));
Shape* greySphere = new Sphere(vec3(-2.5, 0.0, -15.0), vec3(0.9, 0.9, 0.9), 3.0, 128.0);
BoundingBox greySphereBounds(vec3(-5.5, -3.0, -18.0), vec3(0.5, 3, -12));
Shape* blackPlane = new Plane(vec3(0.0, -2.5, -30.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.9, 0.0), 40.0);
BoundingBox blackPlaneBounds(vec3(-10000, -100, -50000), vec3(100000, -3.5, -1));
Shape* greenTriangle = new Triangle(vec3(0.0, 1.0, -2.0),vec3(0.0, 0.6, 1.0), vec3(-1.9, -1, -2), vec3(-0.4, -0.4, 1.0), vec3(1.6, -0.5, -2.0), vec3(0.4, -0.4, 1.0), vec3(0.5, 0.5, 0.0), 100.0);
Shape* otherPlane = new Plane(vec3(0.0, -1, -30.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.9, 0.0), 0.0);
BoundingBox cubeBounds(vec3(-5, -1, -13), vec3(-3, 1, -9));

vector <vec3> verts;
vector <vec3> normals;

vector<Shape*> shapeList;
vector<BoundingBox> boundsList;

bool verLine(SDL_Surface* surface, int x, int y, int colr, int colg, int colb)
{
	if (y < 0 || y >= height)
	{
		return 0;
	}
	else if (x < 0 || x >= width)
	{
		return 0;
	}
	else
	{
		Uint32 colorSDL = SDL_MapRGB(surface->format, colr, colg, colb);
		Uint32* bufp;

		bufp = (Uint32*)surface->pixels + y * surface->pitch / 4 + x;
		*bufp = colorSDL;
		bufp += surface->pitch / 4;

		return 1;
	}	
}


bool done(bool quit_if_esc, bool delay) //delay makes CPU have some free time, use once per frame to avoid 100% usage of a CPU core
{
	if (delay) SDL_Delay(5); //so it consumes less processing power
	int done = 0;
	if (!SDL_PollEvent(&event)) return 0;
	// read keys
	// User requests quit
	if (event.type == SDL_QUIT)
	{
		done = 1;
	}
	//User presses a key
	else if (event.type == SDL_KEYDOWN)
	{
		//Select surfaces based on key press
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			done = 1;
			break;

		default:
			break;
		}
	}

	return done;
}

void CreateSpheres()
{
	shapeList.push_back(yellowSphere);
	boundsList.push_back(yellowSphereBounds);
	shapeList.push_back(blueSphere);
	boundsList.push_back(blueSphereBounds);
	shapeList.push_back(greySphere);
	boundsList.push_back(greySphereBounds);
	shapeList.push_back(redSphere);
	boundsList.push_back(redSphereBounds);
	shapeList.push_back(blackPlane);
	boundsList.push_back(blackPlaneBounds);
	//shapeList.push_back(greenTriangle);
	//shapeList.push_back(otherPlane);
}

float RandomF(float highest, float lowest)
{
	srand(time(NULL));
	float jit = (rand() / (float)RAND_MAX * highest) + lowest;
	return jit;
}


void CreateObj()
{
	shapeList.push_back(blackPlane);
	boundsList.push_back(blackPlaneBounds);

	bool res = loadOBJ("face.obj", verts, normals);

	for (int i = 0; i < size(verts); i++)
	{
		verts[i].z += -10.0;
		verts[i].x += -3.0;
		verts[i].y += 0.0;
	}

	for (int i = 0; i < size(verts); i = i+3)
	{
		shapeList.push_back(new Triangle(verts[i], normals[i], verts[i + 1], normals[i + 1], verts[i + 2], normals[i + 2], vec3(0.5, 0.5, 0.0), 100));
		boundsList.push_back(cubeBounds);
	}

	

}



void step1(SDL_Surface* surface)
{
	beginClock = clock();

	vec3 **a = new vec3*[width];
	for (int i = 0; i < width; i++)
	{
		a[i] = new vec3[height];
	}

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//normalise pixel coord
			float aspectRatio = (float)width / (float)height;

			float xNormalised = (i + 0.5) / (float)width;
			float yNormalized = (j + 0.5) / (float)height;

			float xRemapped = ((2 * xNormalised) - 1) * aspectRatio;
			float yRemapped = 1 - 2 * yNormalized;

			float pixelCamX = xRemapped * tan(radians(angle) / 2.0);
			float pixelCamy = yRemapped * tan(radians(angle) / 2.0);

			vec3 pCamSpace = vec3(pixelCamX, pixelCamy, -1);

			vec3 rayDir = normalize(pCamSpace - origin);



			closestIntersect = 100000.0;
			int hitIndex = -1;

			//cout << "Boundslist: " << size(boundsList) << endl;
			for (int k = 0; k < size(boundsList); k++)
			{
				raycount++;
				if (boundsList[k].Intersects(rayDir, origin))
				{
					raycount++;
					if (shapeList[k]->Intersects(origin, rayDir))
					{
						if (shapeList[k]->t < closestIntersect)
						{
							closestIntersect = shapeList[k]->t;
							hitIndex = k;
						}
					}
				}
			}

			if (hitIndex != -1)
			{
				vec3 currentCol = shapeList[hitIndex]->colour;
				//apply diffuse light

				vec3 intersectPoint = origin + (closestIntersect * rayDir);
				vec3 l = normalize(lightSource - intersectPoint);
				vec3 norm = shapeList[hitIndex]->CalcNormal(intersectPoint);

				vec3 diffuse = currentCol * light.intensity * glm::max(0.0f, dot(l, norm));

				//apply specular light
				
				vec3 r = norm * glm::max(0.0f, dot(l, norm));
				r = r * (float)2.0;
				r = r - l;
				r = normalize(r);

				currentCol = specularColour * light.intensity;
				vec3 v = normalize(origin - intersectPoint);
				double rv = dot(r, v);
				float maxrv = glm::max(0.0, rv);
				maxrv = pow(maxrv, shapeList[hitIndex]->shinyness);
				vec3 specular = currentCol * maxrv;



				//shadows

				bool shadow = false;

				float hits = 0;

				if (softShadows)
				{
					for (int l = 0; l < size(boundsList); l++)
					{
						if (boundsList[l].Intersects(normalize(light.centrePoint - intersectPoint), intersectPoint))
						{
							if (l != hitIndex)
							{
								for (float k = 0; k < light.samples; k = k + light.sampleSize)
								{
									for (float ii = 0; ii < light.samples; ii = ii + light.sampleSize)
									{
										float rand = RandomF(light.sampleSize, light.sampleSize * -1);
										raycount++;
										if (shapeList[l]->Intersects(intersectPoint, normalize(vec3(light.startpoint.x + (light.sampleSize*(k + rand)), light.centrePoint.y, light.startpoint.z - (light.sampleSize*(ii + rand))))))
										{
											hits += 1;
										}
									}
								}

							}
						}
					}
					float avg = hits / (light.samples*light.samples);

					a[i][j] = (diffuse + specular);

					if (avg == 0)
					{
						//currentCol = diffuse + specular;
						//verLine(surface, i, j, std::min((float)1, a[i][j].x) * 255, std::min((float)1, a[i][j].y) * 255, std::min((float)1, a[i][j].z) * 255);
						a[i][j] = diffuse + specular;
					}
					else
					{
						a[i][j] = a[i][j] * vec3(1 - avg, 1 - avg, 1 - avg);

						//currentCol = (diffuse + specular) * vec3(1 - avg, 1 - avg, 1 - avg);
						//verLine(surface, i, j, std::min((float)1, a[i][j].x) * 255, std::min((float)1, a[i][j].y) * 255, std::min((float)1, a[i][j].z) * 255);
					}

				}
				else //Hard Shadows
				{
					for (int l = 0; l < size(shapeList); l++)
					{
						if (l != hitIndex)
						{
							if (boundsList[l].Intersects(normalize(light.centrePoint - intersectPoint), intersectPoint))
							{
								if (shapeList[l]->Intersects(intersectPoint, normalize(light.centrePoint - intersectPoint)))
								{
									shadow = true;
								}
							}
						}

					}

					if (!shadow)
					{
						a[i][j] = diffuse + specular;
					}
					else
					{
						a[i][j] = shapeList[hitIndex]->colour * ambientIntensity;
					}


				}
			}
			else
			{
				//verLine(surface, i, j, std::min((float)1, a[i][j].x) * 255, std::min((float)1, a[i][j].y) * 255, std::min((float)1, a[i][j].z) * 255);
				a[i][j] = vec3(1, 1, 1);
			}



			verLine(surface, i, j, std::min((float)1, a[i][j].x) * 255, std::min((float)1, a[i][j].y) * 255, std::min((float)1, a[i][j].z) * 255);
			
			
		}

		
	}



	
	if (showTimes)
	{
		cout << "Rays: " << raycount << endl;
		cout << "Time Taken: " << float(clock() - beginClock) / CLOCKS_PER_SEC << " Seconds" << endl;
	}
}




int main(int argc, char* args[])
{

	CreateSpheres();
	CreateObj();

	cout << "Space To Turn On Soft Shadows" << endl;
	cout << "Directional Keys change position of camera" << endl;
	cout << "Enter to show ray counts and render times" << endl;
	cout << "Number Keys 1 - 5 for different light intensities" << endl;

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	window = SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

	screenSurface = SDL_GetWindowSurface(window);

	
	
	
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
	step1(screenSurface);
	SDL_UpdateWindowSurface(window);

	while (!done(true, false))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				if (origin.z == 0)
				{
					origin.z = 0.7;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				else if (origin.z < 0)
				{
					origin.z = 0;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				break;

			case SDLK_DOWN:
				if (origin.z == 0)
				{
					origin.z = -0.7;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				if (origin.z > 0)
				{
					origin.z = 0;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				break;
			case SDLK_LEFT:
				if (origin.x == 0)
				{
					origin.x = 0.7;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				else if (origin.x < 0)
				{
					origin.x = 0;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				break;

			case SDLK_RIGHT:
				if (origin.x == 0)
				{
					origin.x = -0.7;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				if (origin.x > 0)
				{
					origin.x = 0;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				break;
			case SDLK_SPACE:
				cout << "Shadows" << softShadows << endl;
				if (softShadows == true)
				{
					softShadows = false;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				else
				{
					softShadows = true;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				
				break;
			case SDLK_RETURN:
				if (!showTimes)
				{
					showTimes = true;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
					step1(screenSurface);
					SDL_UpdateWindowSurface(window);
				}
				break;
			case SDLK_1:
				light.intensity = vec3(0.2,0.2,0.2);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
				step1(screenSurface);
				SDL_UpdateWindowSurface(window);
				break;
			case SDLK_2:
				light.intensity = vec3(0.4, 0.4, 0.4);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
				step1(screenSurface);
				SDL_UpdateWindowSurface(window);
				break;
			case SDLK_3:
				light.intensity = vec3(0.6, 0.6, 0.6);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
				step1(screenSurface);
				SDL_UpdateWindowSurface(window);
				break;
			case SDLK_4:
				light.intensity = vec3(0.8, 0.8, 0.8);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
				step1(screenSurface);
				SDL_UpdateWindowSurface(window);
				break;
			case SDLK_5:
				light.intensity = vec3(1.0, 1.0, 1.0);
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 0x00, 0x00));
				step1(screenSurface);
				SDL_UpdateWindowSurface(window);
				break;

			default:
				break;
			}
		}
		
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;

	

}