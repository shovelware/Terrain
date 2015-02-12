#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain(void) : index("index.txt"), currentProcess(AVERAGE), hAdjust(0.1)
{
	gridWidth=25; //squares in grid
	gridDepth=25;

	terrWidth=25; //size of terrain in world units
	terrDepth=25;

	vertices=NULL;
	normals = NULL;
	avergeNormals = NULL;
	colors=NULL;	
	texCoords = NULL;
	//num squares in grid will be width*height,  two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth * gridWidth * 2 * 3;

	 //Avg. of width and depth
	 hAdjust = (gridWidth + gridDepth) / 2;
}

Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
	delete [] texCoords;
	delete [] normals;
	delete [] avergeNormals;
}

//interpolate between two values
float lerp(float start,  float end,  float t){
	return start + (end-start) * t;
}

//Set vertex given an index and xyz
void Terrain::setVert(int index, float x, float y, float z)
{
	vertices[index][0] = x;
	vertices[index][1] = y;
	vertices[index][2] = z;
};

//Sets colour given an index and rgb (Safe for >255 values)
void Terrain::setCol(int index, float r, float g, float b)
{
	colors[index][0] = r <= 255 ? r : 255;
	colors[index][1] = g <= 255 ? g : 255;
	colors[index][2] = b <= 255 ? b : 255;
}

void Terrain::setColRand(int index, float minR, float minG, float minB)
{
	//[num] = rand() % [range + 1] + [min];
	colors[index][0] = rand() % 256 + minR;
	colors[index][1] = rand() % 256 + minG;
	colors[index][2] = rand() % 256 + minB;
}

//Set normal given an index (Safe for > 1 values)
void Terrain::setNorm(int index, float x, float y, float z)
{
	normals[index][0] = (0 < x && x < 1) ? x : 0;
	normals[index][1] = (0 < y && y < 1) ? y : 0;
	normals[index][2] = (0 < z && z < 1) ? z : 0;
}

//Wrapper function for vector2f support (max is maximum x,  maximum y)
Vector2f Terrain::normalisePos(Vector2f pos,  Vector2f max)
{
	Vector2f newPos;
	newPos.x = normalisePos(pos.x,  max.x);
	newPos.y = normalisePos(pos.y,  max.y);

	return newPos;
}

//Normalises a position given the current one and max
float Terrain::normalisePos(float pos,  float max)
{
	//float newX = targLeft + (targWidth / srcWidth) * x;
	return 0 + (1 / max) * pos;

	//(pos + start) / (end - start);
}

float Terrain::normaliseGridPos(float x, float y)
{
	float normX = normalisePos(x + 25, 50);
	float normY = normalisePos(y + 25, 50);

	return normX, normY;
}

//Wrapper function for vector2f support
float Terrain::heightMapLookup(Vector2f pos)
{
	return heightMapLookup(pos.x,  pos.y);
}

//Grab data from the current map (x,  y should be in normalised coords [0 -> 1])
float Terrain::heightMapLookup(float x,  float y)
{
	float height = 0;

	//Return flat if:
		//Heightmap is broke
		//Invalid coords
	if (!(currentHeightMap._Ptr == NULL 
		|| x < 0 || x > 1
		|| y < 0 || y > 1))
	{
		sf::Image &hm = (*currentHeightMap);

		unsigned int mapWidth = hm.getSize().x;
		unsigned int mapHeight = hm.getSize().y;

		sf::Color col = hm.getPixel(mapWidth - (x * mapWidth), mapHeight - (y * mapHeight));

		//Do some stuff with the color for calculations
		height = (heightProcess(colorProcess(col)));
	}

	else cout << "Invalid heightMapLookup" << endl;

	return height;
}

//Take the color and turn it into a float
//Has a multitude of options controlled by the currentProcess var
float Terrain::colorProcess(sf::Color c)
{
	switch (currentProcess)
	{
		//Average of rgb values, works for greyscale too.
	case AVERAGE:
		return (c.r + c.g + c.b) / 3;
		break;

		//Addition of rgb values, straight up.
	case ADD:
		return c.r + c.g + c.b;
		break;

		//Extended range addition, for more values
	case EXTADD:
		return c.r * 255 * 255 + c.g * 255 + c.b;
		break;
	
		//Defaults to average
	default:
		return (c.r + c.g + c.b) / 3;
		break;
	}
}

//Make the height not so awfully big
//Use the same switch as colorProcess, controlled by currentProcess as well
float Terrain::heightProcess(float h)
{
	switch (currentProcess)
	{
		//Average of rgb values, works for greyscale too.
	case AVERAGE:
		return h / hAdjust;// / (h / hAdjust);
		break;

		//Addition of rgb values, straight up.
	case ADD:
		return h;
		break;

		//Extended range addition, for more values
	case EXTADD:
		return h;
		break;

		//Defaults to average
	default:
		return h;
		break;
	}
}

/* KEN'S METHOD
=======
	if (!(currentHeightMap._Ptr == NULL || x > 1 || y > 1))
	{
		sf::Image &hm = (*currentHeightMap);

		unsigned int width = hm.getSize().x;
		unsigned int height = hm.getSize().y;

		sf::Color col = hm.getPixel(x * width, y * height);



	}

	return height;
}

>>>>>>> 064425f06a642e570f49f80bf8617483308336c3
//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(float x,  float y){

	//for the sample we will calculate height based on distance form origin
	float dist= sqrt(x * x + y * y);

	//center will be the highest point
	dist = 30 - dist;
	//put a nice curve in it
	dist *= dist;
	dist *= dist;
	//whoah,  way to high,  make it smaller
	dist /= 50000;

	return dist;
}
<<<<<<< HEAD
*/

//getHeight takes OpenGL coords [-1 -> 1]
float Terrain::getHeight(float x, float y){

	float normX = normalisePos(x + 25, 50);
	float normY = normalisePos(y + 25, 50);

	return heightMapLookup(normX, normY);
}

//Positive goes right, negative goes left, will loop around if you go beyond the edge.
//Returns true if we actually moved, else false
bool Terrain::crementIter(int direction)
{
	bool moved = false;

	//Back
	if (direction < 0)
	{
		//If we hit the end, loop around
		if (currentHeightMap == heightMaps.begin())
		{
			currentHeightMap = --heightMaps.end();
		}

		//Move backwards
		else currentHeightMap--;
		
		moved = true;
	}

	//Forward
	if (direction > 0)
	{
		//Move forward
		currentHeightMap++;

		//If we hit the end, loop around
		if (currentHeightMap == heightMaps.end())
		{
			currentHeightMap = heightMaps.begin();
		}

		moved = true;
	}

	return moved;
}

//PUBLIC FUNCTIONS BELOW//

void Terrain::LoadImages(string indexfile)
{
	std::cout << "Loading images" << endl;
	string c;
	std::ifstream myfile;

	sf::Image temp;
	
	myfile.open(basepath + indexfile);
	while (myfile >> c) 
	{
		if (!temp.loadFromFile(basepath + c))
		{
			heightMaps.push_back(temp);
			continue;
		}
		std::cout << "\t" << "Loaded: " << c << endl;
		heightMaps.push_back(temp);
	}
	myfile.close();

	currentHeightMap = heightMaps.begin();
}

//Moves to the next map and reloads
void Terrain::nextMap()
{
	//If we've actually changed
	if (crementIter(1))
	{
		Init();
	}
}

//Moves to the previous map and reloads
void Terrain::prevMap()
{
	if (crementIter(-1))
	{
		Init();
	}
}

void Terrain::checkInputKB(sf::Keyboard k)
{
	static bool kComma;
	static bool kPeriod;
	static bool kI;

	//< : Previous Map
	if (k.isKeyPressed(k.Comma))
	{
		if (!kComma)
			prevMap();

		kComma = true;
	}

	else kComma = false;

	//> : Next Map
	if (k.isKeyPressed(k.Period))
	{
		if (!kPeriod)
			nextMap();

		kPeriod = true;
	}

	else kPeriod = false;


	//I : Toggle wireframe
	if (k.isKeyPressed(k.I))
	{
		if (!kI)
			solid = !solid;

		kI = true;
	}

	else kI = false;
}

void Terrain::calculateNormal(sf::Vector3f a, sf::Vector3f b, int index){
	normals[index][0] = (a.y * b.z) - (a.z * b.y);
	normals[index][1] = (a.z * b.x) - (a.x * b.z);
	normals[index][2] = (a.x * b.y) - (a.y * b.x);
}

void Terrain::calculateAvergeNormalOf3Normals(sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, int index){
	avergeNormals[index][0] = (a.x + b.x + c.x) / 3.0f;
	avergeNormals[index][1] = (a.y + b.y + c.y) / 3.0f;
	avergeNormals[index][2] = (a.z + b.z + c.z) / 3.0f;
}

void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices=new vector3[numVerts];
	delete [] colors;
	colors=new vector3[numVerts];
	delete[] texCoords;
	texCoords = new vector3[numVerts];
	delete[] normals;
	normals = new vector3[numVerts];
	delete[] avergeNormals;
	avergeNormals = new vector3[numVerts / 3];

	//If we're empty try to load
	if (heightMaps.empty())
	{
		LoadImages(index);
	}

	float xT = 0;
	float yT = 0;
	//interpolate along the edges to generate interior points
	for (int index = 0, int i = 0; i < gridWidth - 1; i++) //iterate left to right
	{
		for(int j = 0; j < gridDepth - 1; j++)//iterate front to back
		{
			int sqNum = (j + i * gridDepth);
			int vertexNum = sqNum * 3 * 2; //6 vertices per square (2 tris)

			float front = lerp(-terrDepth / 2, terrDepth / 2, (float)j / gridDepth);
			float back  = lerp(-terrDepth / 2, terrDepth / 2, (float)(j + 1) / gridDepth);

			float left = lerp(-terrWidth / 2, terrWidth / 2, (float)i / gridWidth);
			float right = lerp(-terrWidth / 2, terrWidth / 2, (float)(i + 1) / gridWidth);
			
			/*
			back  + ----- + 	looking from above,  the grid is made up of regular squares
			       |tri2/|	'left & 'right' are the x cooded of the edges of the square
				   |   / |	'back' & 'front' are the y coords of the square
				   |  /  |	each square is made of two triangles (1 & 2)
				   | /   |	
				   |/tri1|
			front + ----- + 
			     left   right
				 */

			//Order: Vertex, Normal, Color, Texture

			//tri1
			//side a
			setVert(vertexNum, left, getHeight(left, front), front);
			setCol(vertexNum, 255, 0, 0);
			calculateNormal(sf::Vector3f(vertices[vertexNum][0], vertices[vertexNum][1], vertices[vertexNum][2]),
				sf::Vector3f(vertices[vertexNum + 1][0], vertices[vertexNum + 1][1], vertices[vertexNum+1][2]), vertexNum);
			vertexNum++;
			//side b
			setVert(vertexNum, right, getHeight(right, front), front);
			setCol(vertexNum, 0, 255, 0);
			calculateNormal(sf::Vector3f(vertices[vertexNum][0], vertices[vertexNum][1], vertices[vertexNum][2]),
				sf::Vector3f(vertices[vertexNum + 1][0], vertices[vertexNum + 1][1], vertices[vertexNum+1][2]), vertexNum);
			vertexNum++;
			//side c
			setVert(vertexNum, right, getHeight(right, back), back);
			setCol(vertexNum, 0, 0, 255);
			calculateNormal(sf::Vector3f(vertices[vertexNum][0], vertices[vertexNum][1], vertices[vertexNum][2]),
				sf::Vector3f(vertices[vertexNum + 1][0], vertices[vertexNum + 1][1], vertices[vertexNum+1][2]), vertexNum);
			vertexNum++;
			//calculate normals for triangle
			calculateAvergeNormalOf3Normals(sf::Vector3f(normals[vertexNum - 3][0], normals[vertexNum][1], normals[vertexNum][2]),
				sf::Vector3f(normals[vertexNum + 1][0], normals[vertexNum + 1][1], normals[vertexNum + 1][2]),
				sf::Vector3f(normals[vertexNum + 2][0], normals[vertexNum + 2][1], normals[ivertexNum + 2][2]),
				index);

			//declare a degenerate triangle
			//TODO: fix this to draw the correct triangle

			//tri2
			//side a
			setVert(vertexNum, right, getHeight(right, back), back);
			setCol(vertexNum, 0, 255, 255);
			calculateNormal(sf::Vector3f(vertices[vertexNum][0], vertices[vertexNum][1], vertices[vertexNum][2]),
				sf::Vector3f(vertices[vertexNum + 1][0], vertices[vertexNum + 1][1], vertices[vertexNum+1][2]), vertexNum);
			vertexNum++;
			//side b
			setVert(vertexNum, left, getHeight(left, back), back);
			setCol(vertexNum, 255, 0, 255);
			calculateNormal(sf::Vector3f(vertices[vertexNum][0], vertices[vertexNum][1], vertices[vertexNum][2]),
				sf::Vector3f(vertices[vertexNum + 1][0], vertices[vertexNum + 1][1], vertices[vertexNum+1][2]), vertexNum);
			vertexNum++;
			//side c
			setVert(vertexNum, left, getHeight(left, front), front);
			setCol(vertexNum, 255, 255, 0);
			calculateNormal(sf::Vector3f(vertices[vertexNum][0], vertices[vertexNum][1], vertices[vertexNum][2]),
				sf::Vector3f(vertices[vertexNum + 1][0], vertices[vertexNum + 1][1], vertices[vertexNum+1][2]), vertexNum);
			vertexNum++;
		}
	}
	for (int i = 0; i < numVerts / 3; i++){
		calculateAvergeNormalOf3Normals(sf::Vector3f(normals[i][0], normals[i][1], normals[i][2]),
			sf::Vector3f(normals[i+1][0], normals[i+1][1], normals[i+1][2]),
			sf::Vector3f(normals[i+2][0], normals[i+2][1], normals[i+2][2]),
			i);
	}


	std::cout << "num verts: " << numVerts << std::endl;
}

void Terrain::Draw(){
	if (solid)
		glBegin(GL_TRIANGLES);
	
	else glBegin(GL_LINES);

	for(int i = 0 ; i < numVerts ; ++i)
	{
			glColor3fv(colors[i]);
			glVertex3fv(vertices[i]);
			glTexCoord2d(vertices[i][0]/gridWidth, vertices[i][2]/gridDepth);//this will do some of the grid(technically all). lerp fucks it up(apparently)
			//lerp throws the position off. somehow need vertices before lerp
	}

	glEnd();
}