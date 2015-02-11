#pragma once
#include "stdafx.h"

using sf::Vector2f;

class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth, gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth, terrDepth;

	typedef GLfloat vector3[3];
	typedef GLfloat vector2[2];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk

	vector3 *vertices;
	vector3 *normals;
	vector3 *colors;
	vector2 *texCoords;

	std::vector<sf::Vector2f>texCo;
	
	bool loadImage();

	Vector2f normalisePos(Vector2f pos, Vector2f max);
	float normalisePos(float pos, float max); 
	float normaliseGridPos(float pos, float max);

	float heightMapLookup(Vector2f pos);
	float heightMapLookup(float x, float y);

	float getHeight(float x, float y);

	void setVert(int, float, float, float);
	void setNorm(int, float, float, float);
	void setCol(int, float, float, float);
	void setColRand(int, float, float, float);
	void setTex(int, float, float);
	void setTexFromVert(int, int);
	GLfloat* SFtoGL(sf::Vector3f);
	sf::Vector3f GLtoSF(vector3);

	vector<sf::Image> heightMaps;
	vector<sf::Image>::iterator currentHeightMap;
	bool crementIter(int direction);

	enum process { AVERAGE = 0, ADD = 1, EXTADD, ENUMCOUNT };
	int currentProcess;

	float colorProcess(sf::Color c);
	float heightProcess(float h);

	int hAdjust;

	bool drawSolid = true;
	bool drawNormals = false;

public:
	Terrain(void);
	~Terrain(void);

	string index;

	void LoadImages(string filepath);

	void nextMap();
	void prevMap();

	void checkInputKB(sf::Keyboard k);

	void calculateNormal(sf::Vector3f a, sf::Vector3f b, int index);
	sf::Vector3f calculateAvergeNormalOf3Normals(sf::Vector3f a, sf::Vector3f b, sf::Vector3f c);


	void Init();
	void Draw();
};

