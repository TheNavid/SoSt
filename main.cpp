#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "RSDL/src/rsdl.hpp"

using namespace std;

#define GAMEFIELD_WIDTH 800
#define GAEMFIELD_LENGHT 480
#define WINDOW_WIDTH 900
#define WINDOW_LENGHT 580
#define BALL_RADIUS 10
#define BALL_INITIAL_X 440
#define BALL_INITIAL_Y 280
#define GAEMFIELD_INITIAL_X 50
#define GAEMFIELD_INITIAL_Y 50
#define TAW_RADIUS 15
#define TAWS_NUMBER 5

#define NO_MOVING 0
#define MOVING 1

class Ball
{
public:
	void putInitialBall(Window *window)
	{
		window->draw_img("ball.png",Rectangle(BALL_INITIAL_X, BALL_INITIAL_Y, 2 * BALL_RADIUS, 2 * BALL_RADIUS));
		xPosition = BALL_INITIAL_X;
		yPosition = BALL_INITIAL_Y;
		acceleration = 0;
		initialVelocityX = 0;
		initialVelocityY = 0;
		movingStatus = NO_MOVING;
	}
private:
	int xPosition;
	int yPosition;
	int acceleration;
	int initialVelocityX;
	int initialVelocityY;
	bool movingStatus;
};

class Taw
{
public:
	Taw(int x, int y, string Id, Window* window)
	{
		xPosition = x;
		yPosition = y;
		ID = Id;
		initialVelocityX = 0;
		initialVelocityY = 0;
		acceleration = 0;
		if (ID.erase(1) == "R")
			window->draw_img("Player2.png", Rectangle(xPosition - 15, yPosition - 15, 30, 30));
		if (ID.erase(1) == "G")
			window->draw_img("Player1.png", Rectangle(xPosition - 15, yPosition - 15, 30, 30));
		int movingStatus = NO_MOVING;
	}

	bool isChosenTaw(Point mousePosition)
	{
		if (isXInRange(mousePosition.x) && isYInRange(mousePosition.y))
			return true;
		return false;
	}

private:

	bool isXInRange(int x)
	{
		 return (x <= (pow(TAW_RADIUS, 2) - pow(yPosition, 2))) && (x >= (pow(yPosition, 2) - pow(TAW_RADIUS, 2)));
	}

	bool isYInRange(int y)
	{
		return (y <= (pow(TAW_RADIUS, 2) - pow(xPosition, 2))) && (y >= (pow(xPosition, 2) - pow(TAW_RADIUS, 2)));
	}


	int xPosition;
	int yPosition;
	int acceleration;
	int initialVelocityX;
	int initialVelocityY;
	string ID;
	int movingStatus;
};

class Player
{
public:


private:
	bool turn;
	int achievedPoint;
	int status;
	string color;
};



class Goal
{
public:

private:
	int top;
	int down;
	int linesCoordinate;
	int color;
};

class GetGameInformation
{
public:

private:
	int goalsToWin;
	int round;
	bool finishedGame;
};

class StrikeInformation
{
public:

private:
	int mouseXPosition;
	int mouseYPosition;
};

class GameField
{
public:
	void buildGamefield(Window *window)
	{
		window->draw_img("field.jpg", Rectangle(GAEMFIELD_INITIAL_X, GAEMFIELD_INITIAL_Y, width, lenght));
	}


private:
	int width = GAMEFIELD_WIDTH;
	int lenght = GAEMFIELD_LENGHT;
};

class Game
{
public:
	Game()
	{
		Window *window = new Window(WINDOW_WIDTH, WINDOW_LENGHT, "Soccer Stars");
		gamefield.buildGamefield(window);
		ball->putInitialBall(window);
		intitialTawsPosition(window);
		startGame(window);
	}
private:

	void newRedTaw(Taw taw)
	{
		redTaws.push_back(taw);
	}
	void intitialRedTawsPosition(Window *window)
	{
		newRedTaw(Taw(300, 290, "R0", window));
		newRedTaw(Taw(350, 140, "R1", window));
		newRedTaw(Taw(350, 440, "R2", window));
		newRedTaw(Taw(100, 290, "R3", window));
		newRedTaw(Taw(200, 290, "R4", window));
	}

	void newGreenTaw(Taw taw)
	{
		greenTaws.push_back(taw);
	}

	void intitialGreenTawsPosition(Window *window)
	{
		newGreenTaw(Taw(600, 290, "G0", window));
		newGreenTaw(Taw(550, 140, "G1", window));
		newGreenTaw(Taw(550, 440, "G2", window));
		newGreenTaw(Taw(800, 290, "G3", window));
		newGreenTaw(Taw(700, 290, "G4", window));
	}

	void intitialTawsPosition(Window *window)
	{
		intitialGreenTawsPosition(window);
		intitialRedTawsPosition(window);
	}

	bool isGoodPosition(Point mousePosition)
	{
		for (int taw = 0; taw < TAWS_NUMBER; taw++)
			if (redTaws[taw].isChosenTaw(mousePosition))
				return true;
		return false;
	}

	//void shootingTaw()
	//{
	//}

	Taw* findTaw(Point mousePosition)
	{
		for (int taw = 0; taw < TAWS_NUMBER; taw++)
			if (redTaws[taw].isChosenTaw(mousePosition))
				return &redTaws[taw];
		for (int taw = 0; taw < TAWS_NUMBER; taw++)
			if (greenTaws[taw].isChosenTaw(mousePosition))
				return &greenTaws[taw];		
	}

	void startShootingProccess(Point mousePosition, Taw* chosenTaw)
	{
		chosenTaw->calculateInitialVelocity(mousePosition)
	//	chosenTaw->shootingTaw(mousePosition);
	}

	void startGame(Window *window)
	{
		while(true)
		{
			update(window);
			window->update_screen();
		}
	}
	void update(Window *window)
	{
		while (window->has_pending_event())
		{
			Event event = window->poll_for_event();
			switch (event.get_type())
			{
				case Event::EventType::QUIT:
					exit(0);
					break;
				case Event::EventType::LCLICK:
					Point mousePosition = event.get_mouse_position();
					bool goodPosition = false;
					if(isGoodPosition(mousePosition))
					{
						goodPosition = true;
						Taw* chosenTaw = findTaw(mousePosition);
					}
					break;
				case Event::EventType::RCLICK:
					Point mousePosition = event.get_mouse_position();
					if (goodPosition == true)
						startShootingProccess(mousePosition, chosenTaw);
			}
		}
	}

	GameField gamefield;
	Goal redsGoal;
	Goal bluesGoal;
	Ball *ball;
	Player *redPlayer;
	Player *bluePlayer;
	vector <Taw> redTaws;
	vector <Taw> greenTaws;
};

int main()
{
	Game();
}