#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain(void) : index("index.txt"), currentProcess(ADD), hAdjust(0.1)
{
	gridWidth = 50; //squares in grid
	gridDepth = 50;

	//50*50 makes heightmaps work, do not change!
	terrWidth = 50; //size of terrain in world units
	terrDepth = 50;

	vertices = NULL;
	normals = NULL;
	avergeNormals = NULL;
	colors = NULL;	
	texCoords = NULL;

	//num squares in grid will be width*height,  two triangles per square
	//3 verts per triangle
	 numVerts = gridDepth * gridWidth * 2 * 3;
	 std::cout << "Num verts: " << numVerts << std::endl;

	 //Avg. of width and depth
	 hAdjust = (gridWidth + gridDepth) / 2;

	 hMax = heightProcess(colorProcess(sf::Color::White));
}

Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] normals;
	delete [] colors;
	delete [] texCoords;
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

//Set normal given an index (Likes 0 < value < 1 but can deal with others)
void Terrain::setNorm(int index, float x, float y, float z)
{
	normals[index][0] = (0 < x && x < 1) ? x : 0;
	normals[index][1] = (0 < y && y < 1) ? y : 0;
	normals[index][2] = (0 < z && z < 1) ? z : 0;
}

//Sets colour given an index and rgb (Safe for >255 values)
void Terrain::setCol(int index, float r, float g, float b)
{
	colors[index][0] = r <= 255 ? r : 255;
	colors[index][1] = g <= 255 ? g : 255;
	colors[index][2] = b <= 255 ? b : 255;
}

//Sets a texture coordinate (Likes 0 < value < 1 but can deal with others)
void Terrain::setTex(int index, float u, float v)
{
	texCoords[index][0] = (0 < u && u < 1) ? u : 0;
	texCoords[index][1] = (0 < v && v < 1) ? v : 0;
}

//Sets a texture coordinate based on the normalised x and z of a vertex
void Terrain::setTexFromVert(int texIndex, int vertIndex)
{
	float u = normalisePos(vertices[vertIndex][0], terrWidth);
	float v = normalisePos(vertices[vertIndex][2], terrDepth);
	setTex(texIndex, u, v);
}

//Convert a GL vector3 to sf::Vector3f
sf::Vector3f Terrain::GLtoSF(vector3 vector)
{
	return sf::Vector3f(vector[0], vector[1], vector[2]);
}

//Convert a sf::Vector3f to GL vector3
GLfloat* Terrain::SFtoGL(sf::Vector3f vector)
{
	vector3 ret;
	ret[0] = vector.x;
	ret[1] = vector.y;
	ret[2] = vector.z;

	return ret;
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
		sf::Image &hm = (currentHeightMap->first);

		unsigned int mapWidth = hm.getSize().x;
		unsigned int mapHeight = hm.getSize().y;

		sf::Color col = hm.getPixel(x * mapWidth, y * mapHeight);

		//Do some stuff with the color for calculations
		height = (heightProcess(colorProcess(col)));
	}

	else cout << "Invalid heightMapLookup: (" << x << ", " << y << ")" << endl;

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
		return h / hAdjust;
		break;

		//Addition of rgb values, straight up.
	case ADD:
		return h / hAdjust;
		break;

		//Extended range addition, for more values
	case EXTADD:
		return h / hAdjust;
		break;

		//Defaults to average
	default:
		return h / hAdjust;
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
void Terrain::loadImages(string indexfile)
{
	std::cout << "Loading images:" << endl;
	string c;
	std::ifstream myfile;

	sf::Image temp;
	
	myfile.open(mapbasepath + indexfile);
	while (std::getline(myfile, c)) 
	{
		if (c != "")
		{
			//Output file comments
			if (c[0] == '/' && c[1] == '/')
			{
				cout << endl << '\t' << c << endl;
				continue;
			}

			//Load map
			if (!temp.loadFromFile(mapbasepath + c + ".png"))
			{
				heightMaps.push_back(make_pair(temp, c));
				continue;
			}

			std::cout << "\t" << c << endl;
			heightMaps.push_back(make_pair(temp, c));
		}
	}
	myfile.close();

	currentHeightMap = heightMaps.begin();
}

//Outputs name of current heightmap
void Terrain::heightMapOutput()
{
	cout << "Current: " << currentHeightMap->second << endl;
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


//Sets the shader variable to be the passed shader
void Terrain::setShader(sf::Shader * inshader)
{
	shader = inshader;
}


void Terrain::checkInputKB(sf::Keyboard k)
{
	static bool kComma;
	static bool kPeriod;
	static bool kI;
	static bool kN;
	static bool kM;

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

	//M : Output Map Name
	if (k.isKeyPressed(k.M))
	{
		if (!kM)
			heightMapOutput();

		kM = true;
	}

	else kM = false;

	//I : Toggle wireframe
	if (k.isKeyPressed(k.I))
	{
		if (!kI)
			drawSolid = !drawSolid;

		kI = true;
	}

	else kI = false;

	//N : Toggle normals
	if (k.isKeyPressed(k.N))
	{
		if (!kN)
			drawNormals = !drawNormals;

		kN = true;
	}

	else kN = false;
}

void Terrain::calculateNormal(sf::Vector3f a, sf::Vector3f b, int index){
	normals[index][0] = (a.y * b.z) - (a.z * b.y);
	normals[index][1] = (a.z * b.x) - (a.x * b.z);
	normals[index][2] = (a.x * b.y) - (a.y * b.x);
	//distance formula
	float len = sqrt(pow(normals[index][0] - 0, 2) + pow(normals[index][1] - 0, 2) + pow(normals[index][2] - 0, 2));
	normals[index][0] /= len;
	normals[index][1] /= len;
	normals[index][2] /= len;
}

void Terrain::calculateAvergeNormalOf3Normals(sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, int index){
	avergeNormals[index][0] = (a.x + b.x + c.x) / 3.0f;
	avergeNormals[index][1] = (a.y + b.y + c.y) / 3.0f;
	avergeNormals[index][2] = (a.z + b.z + c.z) / 3.0f;
}

void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices = new vector3[numVerts];

	delete[] normals;
	normals = new vector3[numVerts];

	delete[] avergeNormals;
	avergeNormals = new vector3[numVerts / 3];

	delete [] colors;
	colors=new vector3[numVerts];

	delete[] texCoords;
	texCoords = new vector2[numVerts];

	//If we're empty try to load
	if (heightMaps.empty())
	{
		loadImages(index);
	}

	float xT = 0;
	float yT = 0;
	//interpolate along the edges to generate interior points
	for (int index = 0, i = 0; i < (gridWidth - 1); i++) //iterate left to right
	{
		for(int j = 0; j < (gridDepth - 1); j++)//iterate front to back
		{
			int sqNum = (j + i * gridDepth);
			int vertexNum = sqNum * 3 * 2; //6 vertices per square (2 tris)

			float front = lerp(-terrDepth / 2, terrDepth / 2, (float)j / gridDepth);
			float back  = lerp(-terrDepth / 2, terrDepth / 2, (float)(j + 1) / gridDepth);

			float left = lerp(-terrWidth / 2, terrWidth / 2, (float)i / gridWidth);
			float right = lerp(-terrWidth / 2, terrWidth / 2, (float)(i + 1) / gridWidth);
			
			/* rotate 90deg CW to be 
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

			setCol(vertexNum, 255, 255, 255);
			setTexFromVert(vertexNum, vertexNum);
			calculateNormal(GLtoSF(vertices[vertexNum]), GLtoSF(vertices[vertexNum + 1]), vertexNum);
			vertexNum++;
			
			setVert(vertexNum, right, getHeight(right, front), front);
			setCol(vertexNum, 255, 255, 255);
			setTexFromVert(vertexNum, vertexNum);
			calculateNormal(GLtoSF(vertices[vertexNum]), GLtoSF(vertices[vertexNum + 1]), vertexNum);
			vertexNum++;
			
			setVert(vertexNum, right, getHeight(right, back), back);
			setCol(vertexNum, 255, 255, 255);
			setTexFromVert(vertexNum, vertexNum);
			calculateNormal(GLtoSF(vertices[vertexNum]), GLtoSF(vertices[vertexNum + 1]), vertexNum);
			vertexNum++;
			//calculate normals for triangle
			calculateAvergeNormalOf3Normals(sf::Vector3f(normals[vertexNum - 3][0], normals[vertexNum][1], normals[vertexNum][2]),
				sf::Vector3f(normals[vertexNum + 1][0], normals[vertexNum + 1][1], normals[vertexNum + 1][2]),
				sf::Vector3f(normals[vertexNum + 2][0], normals[vertexNum + 2][1], normals[vertexNum + 2][2]),
				index);

			//tri2
			//side a
			setVert(vertexNum, right, getHeight(right, back), back);
			setCol(vertexNum, 0, 0, 0);
			setTexFromVert(vertexNum, vertexNum);
			calculateNormal(GLtoSF(vertices[vertexNum]), GLtoSF(vertices[vertexNum + 1]), vertexNum);
			vertexNum++;
			
			setVert(vertexNum, left, getHeight(left, back), back);
			setCol(vertexNum, 0, 0, 0);
			setTexFromVert(vertexNum, vertexNum);			
			calculateNormal(GLtoSF(vertices[vertexNum]), GLtoSF(vertices[vertexNum + 1]), vertexNum);
			vertexNum++;
			
			setVert(vertexNum, left, getHeight(left, front), front);
			setCol(vertexNum, 0, 0, 0);
			setTexFromVert(vertexNum, vertexNum);
			calculateNormal(GLtoSF(vertices[vertexNum]), GLtoSF(vertices[vertexNum + 1]), vertexNum);
			vertexNum++;
		}
	}

	for (int i = 0; i < numVerts / 3; i++){
		calculateAvergeNormalOf3Normals(sf::Vector3f(normals[i][0], normals[i][1], normals[i][2]),
			sf::Vector3f(normals[i+1][0], normals[i+1][1], normals[i+1][2]),
			sf::Vector3f(normals[i+2][0], normals[i+2][1], normals[i+2][2]),
			i);
	}
}

void Terrain::Draw(){
	if (drawSolid)
		glBegin(GL_TRIANGLES);
	
	else glBegin(GL_LINES);
	
	//Once we have access to the shader do this in here
	//shader->setParameter("normals", false);
	//shader->setParameter("maxheight", hMax);

	//Drawing terrain
	for(int i = 0 ; i < numVerts ; ++i)
	{
			glColor3fv(colors[i]);
			glNormal3fv(normals[i]);
			glTexCoord2fv(texCoords[i]);
			//Do color and normal first since position "finishes" a vertex
			glVertex3fv(vertices[i]);
	}
	glEnd();

	//Drawing normals
	if (drawNormals)
	{
		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		for (int i = 0; i < numVerts; ++i)
		{
			glVertex3fv(vertices[i]);
			vector3 vertPlusNorm = { vertices[i][0] + normals[i][0], vertices[i][1] + normals[i][1], vertices[i][2] + normals[i][2] };
			glVertex3fv(vertPlusNorm);
		}
		glEnd();
	}
}