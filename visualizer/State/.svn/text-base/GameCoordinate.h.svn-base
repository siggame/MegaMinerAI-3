//Ben Murrell
#pragma once


//Represents a hex coordinate on the discrete gameboard
class GameCoordinate
{
	public:
		GameCoordinate();
		~GameCoordinate();
		GameCoordinate(const GameCoordinate& copy);
		GameCoordinate& operator=(const GameCoordinate& copy);
	
		int distanceTo(const GameCoordinate& to) const;

		int x() const;
		int y() const;
		int heading() const;
		
		void set(int x, int y, int heading);
		void setX(int x);
		void setY(int y);
		void setHeading(int heading);
		
	private:
		int m_gameX;
		int m_gameY;
		int m_gameHeading;
};
