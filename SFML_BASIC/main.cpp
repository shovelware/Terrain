//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <iostream>
 
#include "Terrain.h"
#include "Camera.h"

const sf::Mouse mouse;
const sf::Keyboard keyboard;

int main() 
{ 
    // Create the main window 
    int width=600,height=600;

	//sf::RenderWindow window(sf::VideoMode(width, height, 32), "SFML OpenGL", 7U, sf::ContextSettings(0U, 0U, 8U, 2U, 0U)); //ANTIALIASING
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "SFML OpenGL");
	sf::ContextSettings settings = window.getSettings();
	std::cout << "OpenGL version: " << settings.majorVersion << "." << settings.minorVersion << std::endl << std::endl;

    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 

	//load the texture maps
	
	sf::Texture grassTexture;
	sf::Texture snowyRocksTexture;
	sf::Texture waterTexture;

	if (grassTexture.loadFromFile("asset/grassMap.png")){ cout << "grass texture loaded successfully" << endl; }
	else cout << "Failed to load grass texture" << endl;
	if (snowyRocksTexture.loadFromFile("asset/snowyRocksMap.png")){ cout << "snowyRocks texture loaded successfully" << endl; }
	else cout << "Failed to load snowyRocks texture" << endl;
	if (waterTexture.loadFromFile("asset/seaMap.png")){ cout << "water texture loaded successfully" << endl; }
	else cout << "Failed to load water texture" << endl;
	cout << endl;

	aiVector3D position(0, 20, -30);
	aiVector3D zaxis(0, 0, 1); //-z to turn camera around
	Camera camera;
    camera.Init(position, zaxis); //create a camera
      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.6f, 0.4f, 0.1f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
   
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
     
    //set up a 3D Perspective View volume
    //gluPerspective(110.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar 
	//possibly how we do an orthographics perspective?
	//glOrtho(-200, 200, -200, 200, -200, 200); //left,right,bottom,top,nearVal,farVal

	glEnable(GL_TEXTURE_2D);//enable the use of textures

	//load the shaders
	sf::Shader plain;
	sf::Shader colmod;
	sf::Shader norm;
	sf::Shader texmap;
	sf::Shader texture;
	sf::Shader colmodplus;

	if (!plain.loadFromFile("shader/plain.vert", "shader/plain.frag")) { exit(1); }
	if (!colmod.loadFromFile("shader/colmod.vert", "shader/colmod.frag")) { exit(1); }
	if (!norm.loadFromFile("shader/norm.vert", "shader/norm.frag")) { exit(1); }
	if (!texmap.loadFromFile("shader/texmap.vert", "shader/texmap.frag")) { exit(1); }
	if (!texture.loadFromFile("shader/texture.vert", "shader/texture.frag")) { exit(1); }
	if (!colmodplus.loadFromFile("shader/colmodplus.vert", "shader/colmodplus.frag")) { exit(1); }

	//Create our Terrain
	Terrain terrain;
	terrain.Init();

	bool perspective = true;
	bool ortho = false;

    // Start game loop 
    while (window.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (window.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                window.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                window.close(); 
        }

		if (keyboard.isKeyPressed(keyboard.O))
		{
			ortho = true;
			perspective = false;
		}

		if (keyboard.isKeyPressed(keyboard.P))
		{
			ortho = false;
			perspective = true;
		}

		if (ortho == false && perspective == true)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();


			gluPerspective(110.f, (float)width / height, 1.f, 300.0f);//fov, aspect, zNear, zFar 
		}

		else if (ortho == true && perspective == false)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glOrtho(-200, 200, -200, 200, -200, 200); //left,right,bottom,top,nearVal,farVal
		}

		//UPDATE
		//update the camera
		camera.CheckInputKB(keyboard);
		terrain.checkInputKB(keyboard);

		//DRAW
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   
        // Apply some transformations 
        //initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 

		//get the viewing transform from the camera
		camera.ViewingTransform();

		//make the world spin
		//TODO:probably should remove this in final
		//static float ang=0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		
		//draw the world
		sf::Texture::bind(&waterTexture);
		//terrain.DrawTerrainTextured(&texture);
		terrain.DrawTerrain(&colmodplus);
		terrain.DrawNormals(&plain);

        // Finally, display rendered frame on screen 
        window.display(); 
    }
    return EXIT_SUCCESS; 
}
