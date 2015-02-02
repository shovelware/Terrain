#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class Camera{
    static aiVector3D zero,yaxis,zaxis;
public:
    aiVector3D position;
    aiVector3D forward;
    aiVector3D up;
	aiVector3D right;
 
    float forwardSpeed;
    float roationSpeed;
	bool keyPressed;
     
    Camera():forwardSpeed(0.5f),roationSpeed(0.1f){}
 
    void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis){
        position=p;
        forward=f;
        up=u;
		keyPressed = false;
		right = aiVector3D(1, 0, 0);
    }
 
	void CheckInputKB(sf::Keyboard k)
	{
		//W : Move forward
		if (k.isKeyPressed(k.W))// && keyPressed == false)
		{
			MoveForwardBack(+0.1);
			keyPressed = true;
		}

		//A : Move left
		if (k.isKeyPressed(k.A))// && keyPressed == false)
		{
			MoveLeftRight(-0.1);
			keyPressed = true;
		}

		//S : Move backward
		if (k.isKeyPressed(k.S))// && keyPressed == false)
		{
			MoveForwardBack(-0.1);
			keyPressed = true;
		}

		//D : Move right
		if (k.isKeyPressed(k.D))// && keyPressed == false)
		{
			MoveLeftRight(+0.1);
			keyPressed = true;
		}

		//Q : Move up
		if (k.isKeyPressed(k.Q))// && keyPressed == false)
		{
			MoveUpDown(+0.1);
			keyPressed = true;
		}

		//E : Move down
		if (k.isKeyPressed(k.E))// && keyPressed == false)
		{
			MoveUpDown(-0.1);
			keyPressed = true;
		}

		//Up : Look up
		if (k.isKeyPressed(k.Up))// && keyPressed == false)
		{
			TurnUpDown(+0.01);
			keyPressed = true;
		}

		//Left : Look left
		if (k.isKeyPressed(k.Left))// && keyPressed == false)
		{
			TurnLeftRight(-0.01);
			keyPressed = true;
		}

		//Down : Look down
		if (k.isKeyPressed(k.Down))// && keyPressed == false)
		{
			TurnUpDown(-0.01);
			keyPressed = true;
		}

		//Right : Look right
		if (k.isKeyPressed(k.Right))// && keyPressed == false)
		{
			TurnLeftRight(+0.01);
			keyPressed = true;
		}

		//else keyPressed = false;


	}

	void MoveLeftRight(float dir){ //Dir=+1=>Right, dir=-1=> Left
		//TODO
		position += (right * forwardSpeed*(dir));//need to fix this(forward or make a right?)
	}

	void MoveUpDown(float dir){ //Dir=+1=>Right, dir=-1=> Left
		//TODO
		position.y += (up.y*(forwardSpeed*(dir)));
	}

	void MoveForwardBack(float dir){ //Dir=+1=>Forward, dir=-1=> Back

		position += (forward*(forwardSpeed*(dir)));
	}

	void TurnLeftRight(float dir){ //Dir=+1=>Right, dir=-1=> Left
		//TODO
		forward.x += (dir);//and this only rotates so far. need to fix this as well
	}

	void TurnUpDown(float dir){ //Dir=+1=>Up, dir=-1=> Down
		//TODO
		forward.y += (dir);
	}
 
    void ViewingTransform(){
        gluLookAt(	position.x,position.y,position.z,// camera position
			position.x+forward.x, position.y+forward.y, position.z+forward.z, //look at this point //TODO: BUG here!! what is it ??
					0,1,0); //camera up
    }
 
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);