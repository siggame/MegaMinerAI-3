//Ben Murrell
#include "Engine.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <SDL_thread.h>

#include <algorithm>
#include <math.h>
#include <sstream>
using std::ostringstream;
using std::max;

#include "Config.h"

int killThread;

Engine* Engine::instance()
{
    static Engine* inst = new Engine();
    return inst;
}

Engine::Engine()
{
	m_zoom = (float)atof( Config::instance()->get("defaultZoom").c_str() );
	m_centerX = 0.0f;
	m_centerY = 0.0f;
	
	m_showGrid = true;
	m_humanView = false;
	m_zombieView = false;

	m_width = atoi( Config::instance()->get("screenWidth").c_str() );
	m_height = atoi( Config::instance()->get("screenHeight").c_str() );

	m_exitAtEnd = atoi( Config::instance()->get("exitAtEnd").c_str() ) != 0;
	m_exitDelay = atoi( Config::instance()->get("exitDelay").c_str() );

	m_skipForward = false;
	m_skipBackward = false;
	m_paused = false;

	m_white[0] = 1.0; m_white[1] = 1.0; m_white[2] = 1.0;
	m_black[0] = 0.0; m_black[1] = 0.0; m_black[2] = 0.0;

	//Game is initially null
	m_game = 0;

	m_curState = -1;
	m_atEnd = 0;
	t_beginState = 0;

	//Display list param
	m_L_hexCircleRadius = 1;
}

Engine::~Engine()
{
	for(map<string, GLuint>::iterator cursor = m_displayLists.begin(); cursor != m_displayLists.end(); cursor++)
	{
		glDeleteLists(cursor->second, 1);
	}
}

GLuint Engine::registerList(string list)
{
	if(m_displayLists.count(list))
		return -1;

	m_displayLists[list] = glGenLists(1);

	return m_displayLists[list];
}

GLuint Engine::recallList(string list)
{
	if(m_displayLists.count(list) == 0)
		return -1;

	return m_displayLists[list];
}


void Engine::deleteList(string list)
{
	glDeleteLists(m_displayLists[list],1);
	m_displayLists.erase(list);
}


#include <iostream>
using std::cout;
using std::cin;
using std::endl;
void Engine::init()
{
	//Initialize the SDL
	if(Config::instance()->fullscreen())
	{
		cout << "Fullscreen mode enabled - defaulting to desktop resolution" << endl;
		SDL_SetVideoMode(0, 0, 0, SDL_OPENGL | SDL_FULLSCREEN);
	}
	else
	{
		SDL_SetVideoMode(m_width, m_height, 0, SDL_OPENGL | SDL_RESIZABLE);
	}

	//Color init
	glClearColor(1.0, 1.0, 1.0, 1.0); //White background
	glColor3ub(255, 165, 0); //Draw in orange

	//Depth init - really only need this for 3d
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);


	//Allow key repeating
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 40);
	
	//Verify we got our width & height
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	m_width = videoInfo->current_w;
	m_height = videoInfo->current_h;

	//Init viewing - fit the size of image the camera captures to our window size
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, m_width, m_height);
	glLoadIdentity();
	
	glOrtho(m_centerX - m_zoom*(m_width/320.0f), m_centerX + m_zoom*(m_width/320.0f), m_centerY - m_zoom*(m_height/320.0f), m_centerY + m_zoom*(m_height/320.0f), 0.0, 1.0); //Set up our coordinate system
	glMatrixMode(GL_MODELVIEW); //Switch back to manipulating model/object matrix

	//Give us a title
	SDL_WM_SetCaption("Initializing...", 0);

	//Show the OpenGL info of the machine we're one
	if(Config::instance()->get("glInfo") != "")
	{
		cout << "OpenGL Info: " << endl;
		cout << "GL_RENDERER   = " << (char *) glGetString(GL_RENDERER) << endl;
		cout << "GL_VERSION    = " << (char *) glGetString(GL_VERSION) << endl;
		cout << "GL_VENDOR     = " << (char *) glGetString(GL_VENDOR) << endl;
		cout << "GL_EXTENSIONS = " << (char *) glGetString(GL_EXTENSIONS) << endl;
		cout << endl;
	}
	
	//Create our display lists
	createCircleList();
	createDirectedCircleList();
	createHexList();
	createHexCircleList();
	createCrosshairList();
	createFilledHexList();

	//Grab values from the config
	m_animTime = Config::instance()->animTime();
}

void Engine::handleKeys(SDL_Event* event)
{
	//Key Down
	if(event->key.type == SDL_KEYDOWN)
	{
		switch(event->key.keysym.sym)
		{
			//Exit!
			case SDLK_ESCAPE:
				exit(0);
			break;

			//Show/hide the grid
			case SDLK_h:
				m_humanView = !m_humanView;
				m_zombieView = false;
				//m_showGrid = !m_showGrid;
				break;

			case SDLK_z:
				m_zombieView = !m_zombieView;
				m_humanView = false;
				break;
			
			//Skip forward a state
			case SDLK_RIGHT:
				m_skipForward = true;
				break;

			//Skip back a state
			case SDLK_LEFT:
				m_skipBackward = true;
				break;

			//zoom in
            case SDLK_EQUALS:
                m_zoom -= 5;
				if(m_zoom < 5)
				{
					m_zoom = 5;
				}
                break;

			//zoom out
            case SDLK_MINUS:
                m_zoom += 5;
                break;

			//pause
			case SDLK_SPACE:
				m_paused = !m_paused;
				break;
		}
	}

}

void Engine::handleMouseButtons(SDL_Event* event)
{
	switch(event->button.button)
	{
		//Zoom in
		case SDL_BUTTON_WHEELUP:
				m_zoom -= 2;
				if(m_zoom < 5)
				{
					m_zoom = 5;
				}
			break;

		//Zoom out
		case SDL_BUTTON_WHEELDOWN:
			m_zoom += 2;
			break;
	}
}


void Engine::handleMouseMotion(SDL_Event* event)
{
	//Moving mouse & button 1 is down - pan
	if(event->motion.state&SDL_BUTTON(1))
	{
		m_centerX -= (event->motion.xrel/(float)m_width)*m_zoom*(m_width/320.0f)*2;
		m_centerY += (event->motion.yrel/(float)m_height)*m_zoom*(m_height/320.0f)*2;
	}
}

void Engine::handleEvents()
{
	SDL_Event event;

	//Handle all inbound events
	while(SDL_PollEvent(&event) > 0)
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				handleKeys(&event);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				handleMouseButtons(&event);
				break;

			case SDL_MOUSEMOTION:
				handleMouseMotion(&event);
				break;

			case SDL_QUIT:
				exit(0);
				break;

			case SDL_VIDEOEXPOSE:
				break;

			case SDL_VIDEORESIZE:
				windowResized(event.resize.w, event.resize.h);
				break;
		}
	}
}

void Engine::windowResized(int width, int height)
{
	m_width = width;
	m_height = height;
}

int readThread(void* unused)
{
    static Engine* engine = Engine::instance();

    while(killThread == 0)
    {
        engine->pollGameSource();
        //SDL_Delay(10);
    }
    return 0;
}

void Engine::pollGameSource()
{
    if(m_game != 0)
        m_game->pollSource();
}

void Engine::tick()
{
	handleEvents();
	if(m_zombieView || m_humanView)
	{
		glClearColor(m_black[0], m_black[1], m_black[2], 1.0);
	}
	else
	{
		glClearColor(m_white[0], m_white[1], m_white[2], 1.0);
	}

    //Dirty
    if(killThread == 3)
    {
        SDL_WaitThread(m_readThread, 0);
        throw("Die");
    }

	//For the title bar
	ostringstream sout;

	//Move camera
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, m_width, m_height);
	glLoadIdentity();
	glOrtho(m_centerX - m_zoom*(m_width/320.0f), m_centerX + m_zoom*(m_width/320.0f), m_centerY - m_zoom*(m_height/320.0f), m_centerY + m_zoom*(m_height/320.0f), 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW); //Switch back to manipulating model/object matrix


	//Clear out the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Clear out any old transformations
	glLoadIdentity();

	//If we don't have a game, draw a little grid and bail out
	if(m_game == 0)
	{
		glColor3ub(255, 165, 0); //Draw in orange
		drawHexCircle(3);
		//Flush all of the GL commands we have entered through the pipeline
		glFlush();

		//Swap the buffers (using 2 buffers - we draw on a hidden one and swap it for the visible one when we're done
		SDL_GL_SwapBuffers();
		return;
	}

	//m_game->pollSource();

	//If the game has no states, do the same thing as above
	if(m_game->size() == 0)
	{
		glColor3ub(255, 165, 0); //Draw in orange
		drawHexCircle(3);
		//Flush all of the GL commands we have entered through the pipeline
		glFlush();

		//Swap the buffers (using 2 buffers - we draw on a hidden one and swap it for the visible one when we're done
		SDL_GL_SwapBuffers();
		return;
	}

	//Draw the grid if the user wants us to
	if(m_showGrid)
	{
		glColor3ub(255, 165, 0); //Draw in orange
		drawHexGrid(m_game->minCoord().x(), m_game->maxCoord().x(), m_game->minCoord().y(), m_game->maxCoord().y());
	}

	//How much time went by since we were last called?
	unsigned int curTime = SDL_GetTicks();
	unsigned int deltaT = curTime - t_beginState;

	int dispState = (m_curState >= m_game->size() ? m_curState-1: m_curState);

	//Figure out if it's time to move to a new state
	if((deltaT > m_animTime && !m_atEnd) || (m_skipForward || m_skipBackward))
	{
		t_beginState = curTime;
		deltaT = 0;
		
		if(m_skipForward)
		{
			int curTurn = (*m_game)[m_curState]->curTurn();

			while(m_curState < m_game->size() && (*m_game)[m_curState]->curTurn() == curTurn)
			{
				m_curState++;
			}

			m_skipForward = false;
		}
		else if(m_skipBackward)
		{
			int curTurn = (*m_game)[m_curState]->curTurn();

			while(m_curState > 0 && (*m_game)[m_curState]->curTurn() == curTurn)
			{
				m_curState--;
			}

			m_skipBackward = false;
		}
		else if(m_paused)
		{
			sout << "PAUSED -- ";
			t_beginState = SDL_GetTicks();
			deltaT = 0;
		}
		else
		{
			m_curState++;
		}
		
		sout.clear();
		int dispState = (m_curState >= m_game->size() ? m_curState-1: m_curState);
		sout << "Turn #" << (*m_game)[dispState]->curTurn() << "  --  [[" << m_game->zombieName() << "]] Zombies: " << (*m_game)[dispState]->numZombies() << "/" << (*m_game)[dispState]->zombieCap() << ", " << (*m_game)[dispState]->zombiesReady() << " ready  --  [[" << m_game->humanName() << "]] Total Human Health: " << (*m_game)[dispState]->humanHealth();
        SDL_WM_SetCaption(sout.str().c_str(), 0);

		cout << "State " << m_curState << " / " << m_game->size() << " (#" << (*m_game)[dispState]->curTurn() << ")" << endl;
	}

	//If we're paused, we want to show the paused text up top immediately
	else if(m_paused)
	{
		sout << "PAUSED --   ";
		t_beginState = SDL_GetTicks();
		deltaT = 0;
	
		sout << "Turn #" << (*m_game)[dispState]->curTurn() << "  --  [[" << m_game->zombieName() << "]] Zombies: " << (*m_game)[dispState]->numZombies() << "/" << (*m_game)[dispState]->zombieCap() << ", " << (*m_game)[dispState]->zombiesReady() << " ready  --  [[" << m_game->humanName() << "]] Total Human Health: " << (*m_game)[dispState]->humanHealth();
		SDL_WM_SetCaption(sout.str().c_str(), 0);
	}

	//If we're not paused, we want to remove the paused text up top immediately
	else if(dispState > 0)
	{
		sout << "Turn #" << (*m_game)[dispState]->curTurn() << "  --  [[" << m_game->zombieName() << "]] Zombies: " << (*m_game)[dispState]->numZombies() << "/" << (*m_game)[dispState]->zombieCap() << ", " << (*m_game)[dispState]->zombiesReady() << " ready  --  [[" << m_game->humanName() << "]] Total Human Health: " << (*m_game)[dispState]->humanHealth();
		SDL_WM_SetCaption(sout.str().c_str(), 0);
	}


	//Hit end of game?
	if(m_curState >= m_game->size())
	{
		m_curState = m_game->size()-1;
		m_atEnd = true;
	}
	else if(m_curState < m_game->size()-1)
	{
		m_atEnd = false;
	}

	//If we're at the last state, we don't want to keep animating into it, we want to hold it at the end
	if(m_atEnd)
	{
		//Exit after exitDelay seconds if we are exiting at the end
		if(deltaT > (m_exitDelay*1000) && m_exitAtEnd)
		{
			cout << "End of game, " << m_exitDelay << " seconds have passed. Exiting." << endl;
			exit(0);
		}

		deltaT = m_animTime;
	}

	//Somehow got before beginning of game - clamp to 0
	if(m_curState < 0)
	{
		m_curState = 0;
	}

	//Draw the state
	drawState(m_curState, static_cast<float>(deltaT/(float)m_animTime));

	//Draw a red crosshair at 0,0 for a reference
	glColor3f(1, 0, 0);
	drawCrosshair(0.0f, 0.0f, 2.0f);
	
	//Flush all of the GL commands we have entered through the pipeline
	glFlush();

	//Swap the buffers (using 2 buffers - we draw on a hidden one and swap it for the visible one when we're done
	SDL_GL_SwapBuffers();
    
    t_lastCalled = curTime;
}


void Engine::drawState(int stateNum, float percentThere)
{
	//Clamp the percent between 0 and 1
	if(percentThere > 1.0f)
	{
		percentThere = 1.0f;
	}

	if(percentThere < 0.0f)
	{
		percentThere = 0.0f;
	}

	//Clamp the stateNum between 0 and the number of states available
	//TODO: Should be able to skip intermediate states, but these indices index intermediate states
	//Options: Percent is percentage from state i-1 to state i, intermediate steps not considered
	if(stateNum >= m_game->size())
	{
		stateNum = m_game->size()-1;
	}

	if(stateNum < 0)
	{
		stateNum = 0;
	}


	//Draw the game state (Draw each object in the state)
	map<int, GameObject*>& objects = (*m_game)[stateNum]->map().objects();
	for(map<int, GameObject*>::iterator cursor = objects.begin(); cursor != objects.end(); cursor++)
	{
		GLfloat center[] = {0.0f, 0.0f};
		GLfloat direction = 0.0f;

		int objectID = cursor->first;
		GameObject* curObject = cursor->second;
		GameCoordinate& curLoc = curObject->location();

		//No interpolation for first state
		if(stateNum == 0)
		{
			Engine::hexToRect((float)curLoc.x(),(float)curLoc.y(),center);
			direction = Engine::orientationToDegrees((float)curLoc.heading());
		}

		//Interpolate
		else
		{
			float interpolatedX = 0;
			float interpolatedY = 0;
			float interpolatedOrientation = 0;

			GameState* prevState = (*m_game)[stateNum-1];

			//Interpolate
			if(prevState->map().objects().count(objectID) > 0 && Config::instance()->animate())
			{
				GameObject* prevObject = prevState->map().objects()[objectID];
				GameCoordinate& prevLoc = prevObject->location();

				interpolatedX = static_cast<float>(curLoc.x()) - (curLoc.x() - prevLoc.x()) * (1.0f - percentThere);
				interpolatedY = static_cast<float>(curLoc.y()) - (curLoc.y() - prevLoc.y()) * (1.0f - percentThere);
				
				interpolatedOrientation = interpolateOrientation(prevLoc.heading(), curLoc.heading(), percentThere);

			}

			//No interpolation for new units
			else
			{
				interpolatedX = static_cast<float>(curLoc.x());
				interpolatedY = static_cast<float>(curLoc.y());
				interpolatedOrientation = static_cast<float>(curLoc.heading());	
			}

			Engine::hexToRect(interpolatedX, interpolatedY, center);
			direction = Engine::orientationToDegrees(interpolatedOrientation);
			
		}

		//Save the current transformations
		glPushMatrix();

		//Put the object where we want it
		glTranslatef(center[0], center[1], 0);
		glRotatef(direction, 0, 0, 1);

		//Draw it
		cursor->second->draw2d( percentThere );
		
		//Restore the previous transformations
		glPopMatrix();
	}
}

void Engine::setGame(Game* game)
{
	if(m_game != 0)
	{
        if(killThread == 0)
        {
            killThread = 1;
            SDL_WaitThread(m_readThread, 0);
        }
		delete m_game;
	}
	m_game = game;

   	m_curState = -1;
	m_atEnd = 0;
	t_beginState = 0;

    killThread = 0;
    m_readThread = SDL_CreateThread(readThread, 0);
}

void Engine::stopPolling()
{
    killThread = 3;
}



//Draw a hexagon given the center in rect. coordinates
void Engine::drawHex(GLfloat* center, const GLfloat& raySize)
{
	//Save the current transformation matrix
	glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the hex is supposed to be
	glScalef(raySize, raySize, 0.0f);
	
	//Using display lists is much more efficient - less cpu -> gpu talk
	glCallList(recallList("hex"));

	//Restore the transformation matrix
	glPopMatrix();
}


void Engine::drawFilledHex(GLfloat* center, const GLfloat& raySize)
{
	//Save the current transformation matrix
	glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the hex is supposed to be
	glScalef(raySize, raySize, 0.0f);
	
	//Using display lists is much more efficient - less cpu -> gpu talk
	glCallList(recallList("filledHex"));

	//Restore the transformation matrix
	glPopMatrix();
}

//Draw a hexagon given its hex coordinates
void Engine::drawHex(int hexX, int hexY)
{
	//Calculate the center of the hex in rect. coordinates
	GLfloat center[] = {0.0f, 0.0f};
	Engine::hexToRect(static_cast<float>(hexX), static_cast<float>(hexY), center);

	drawHex(center, 3.0);
}


//Draw a hex grid
void Engine::drawHexGrid(int minX, int maxX, int minY, int maxY)
{
	for(int i=minX; i <= maxX; ++i)
	{
		for(int j=minY; j <= maxY; ++j)
		{	
			drawHex(i,j);
		}
	}
}


//Draw a filled hex circle
void Engine::drawHexCircle(int radius)
{
	if(m_L_hexCircleRadius != radius)
	{
		m_L_hexCircleRadius = radius;
		deleteList("hexCircle");
		createHexCircleList();
	}

	glCallList(recallList("hexCircle"));
}


void Engine::drawDirectedCircle(GLfloat* center, GLfloat radius, GLfloat direction)
{
	//Save the current transformation matrix
	glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the circle is supposed to be
	glRotatef(direction, 0,0,1); //Rotate about the z axis
	glScalef(radius, radius, 0.0f);
	
	//Using display lists is much more efficient - less cpu -> gpu talk
	glCallList(recallList("zombie"));

	//Restore the transformation matrix
	glPopMatrix();
}

void Engine::drawCircle(GLfloat* center, GLfloat radius)
{
	//Save the current transformation matrix
	glPushMatrix();
	glTranslatef(center[0], center[1], 0); //Translate out to where the circle is supposed to be
	glScalef(radius, radius, 0.0f);
	
	//Using display lists is much more efficient - less cpu -> gpu talk
	glCallList(recallList("circle"));

	//Restore the transformation matrix
	glPopMatrix();
}

void Engine::drawCrosshair(const GLfloat& x, const GLfloat& y, const GLfloat& size)
{
	glPushMatrix();
	glTranslatef(x,y,0.0f);
	glScalef(size, size, 0.0f);

	glCallList(recallList("crosshair"));

	glPopMatrix();
}


//Convert a hex coordinate into a rect coordinate
void Engine::hexToRect(float hexX, float hexY, GLfloat* rect)
{
	float d = 2 * (float)sin(PI_OVER_3) * 3.0f;

	//Calculate the center of the hex in rect. coordinates
	rect[0] = d*cos(PI_OVER_6)*hexY + d*cos(PI_OVER_6)*hexX;
	rect[1] = d*sin(PI_OVER_6)*hexY - d*sin(PI_OVER_6)*hexX;
}

float Engine::orientationToDegrees(float hexOrientation)
{
	return (float)hexOrientation*60 - 30.0f;
}


float Engine::interpolateOrientation(int current, int desired, const float& percentThere)
{
	float difference;
	
	if(current > 3 || desired > 3)
	{
		difference = (float)(abs(desired-3)- abs(current-3));
	}
	else
	{
		difference = (float)(desired-current);
	}

	return (float)current + difference*percentThere;
}


//Creates a display list in OpenGL that will draw a unit circle with a line pointing to 1,0
void Engine::createCircleList()
{
	GLfloat x;
	GLfloat y;
	GLfloat theta;
	int n = 16;

	GLuint list = registerList("circle");
	glNewList(list, GL_COMPILE);

		glBegin(GL_TRIANGLE_FAN);
			for(int i=0; i <= n; ++i)
			{
				theta = (i/static_cast<float>(n))*2.0f*PI;
				x = cos(theta);
				y = sin(theta);

				glVertex2f(x,y);
			}
		glEnd();
	glEndList();
}

void Engine::createDirectedCircleList()
{
	GLuint list = registerList("zombie");
	glNewList(list, GL_COMPILE);

		glCallList(recallList("circle"));
		glBegin(GL_LINES);
			glVertex2f(0,0);
			glVertex2f(1.5f, 0.0f);
		glEnd();
	glEndList();
}

//Hexagon centered at 0,0 with raysize = 1
void Engine::createHexList()
{
	GLuint list = registerList("hex");
	glNewList(list, GL_COMPILE);

		glBegin(GL_LINE_STRIP);
			GLfloat point[] = {0,0};
			GLfloat theta = 0.0f;
			for(int j=0; j <= 6; ++j)
			{
				theta = j*PI_OVER_3;
				point[0] = cos(theta);
				point[1] = sin(theta);
				glVertex2fv(point);
			}
		glEnd();
	glEndList();
}

//Hexagon centered at 0,0 with raysize = 1
void Engine::createFilledHexList()
{
	GLuint list = registerList("filledHex");
	glNewList(list, GL_COMPILE);

		glBegin(GL_TRIANGLE_FAN);
			GLfloat point[] = {0,0};
			GLfloat theta = 0.0f;
			glVertex2fv(point);
			for(int j=0; j <= 6; ++j)
			{
				theta = j*PI_OVER_3;
				point[0] = cos(theta);
				point[1] = sin(theta);
				glVertex2fv(point);
			}
		glEnd();
	glEndList();
}

//Hex "circle" centered at 0,0
void Engine::createHexCircleList()
{
	GLuint list = registerList("hexCircle");
	glNewList(list, GL_COMPILE);
		//There is probably a more elegant way to do this - we're redrawing a lot of overlapping lines
		for(int x=-m_L_hexCircleRadius; x <= m_L_hexCircleRadius; ++x)
		{
			int dx = -x;
			for(int y=-m_L_hexCircleRadius; y <= m_L_hexCircleRadius; ++y)
			{
				int distance;
				int dy =-y;
				
				if(dx*dy > 0)
					distance = abs(dx) + abs(dy);
				else
					distance = max( abs(dx), abs(dy) );
					
				if(distance <= m_L_hexCircleRadius)
				{
					//Calculate the center of the hex in rect. coordinates
					GLfloat center[] = {0.0f, 0.0f};
					Engine::hexToRect(static_cast<float>(x), static_cast<float>(y), center);

					drawFilledHex(center, 3.0);
				}
			}
		}
	glEndList();
}


void Engine::createCrosshairList()
{
	GLuint list = registerList("crosshair");

	glNewList(list, GL_COMPILE);
		glBegin(GL_LINES);
			glVertex2f(1.0, 0.0);
			glVertex2f(-1.0, 0.0);
			glVertex2f(0.0, 1.0f);
			glVertex2f(0.0, -1.0f);
		glEnd();
	glEndList();
}




string Engine::lastError()
{
	string ret = "no error";
	switch(glGetError())
	{
		case GL_NO_ERROR:
			ret = "no error";
			break;
		case GL_INVALID_ENUM:
			ret = "invalid enum";
			break;
		case GL_INVALID_VALUE:
			ret = "invalid value";
			break;
		case GL_INVALID_OPERATION:
			ret = "invalid operation";
			break;
		case GL_STACK_OVERFLOW:
			ret = "stack overflow";
			break;
		case GL_STACK_UNDERFLOW:
			ret = "stack underflow";
			break;
		case GL_OUT_OF_MEMORY:
			ret = "out of memory";
			break;
	}
	return ret;
}


bool Engine::showHumanSight()
{
	return m_humanView;
}

bool Engine::showZombieSight()
{
	return m_zombieView;
}
