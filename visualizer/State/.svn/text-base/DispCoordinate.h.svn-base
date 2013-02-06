//Ben Murrell
#pragma once

#include "GameCoordinate.h"

//Represents a continuous coordinate in the standard cartesian coordinate system
class DispCoordinate
{
	public:
		DispCoordinate();
		~DispCoordinate();
		DispCoordinate(const DispCoordinate& copy);
		DispCoordinate& operator=(const DispCoordinate& copy);

		void fromGameCoordinate(const GameCoordinate& hex);
		
		float x() const;
		float y() const;
		float z() const;
		float heading() const;

		void set(const float& x, const float& y, const float& z);
		void setX(const float& x);
		void setY(const float& y);
		void setZ(const float& z);
		void setHeading(const float& heading);

	private:
		float m_x;
		float m_y;
		float m_z;
		float m_heading;
};

