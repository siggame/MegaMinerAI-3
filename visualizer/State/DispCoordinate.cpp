//Ben Murrell

#include "DispCoordinate.h"
#include "Engine.h"

#include <math.h>

DispCoordinate::DispCoordinate()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_heading = 0;
}

DispCoordinate::~DispCoordinate()
{

}

DispCoordinate::DispCoordinate(const DispCoordinate& copy)
{
	*this = copy;
}

DispCoordinate& DispCoordinate::operator=(const DispCoordinate& copy)
{
	m_x = copy.m_x;
	m_y = copy.m_y;
	m_z = copy.m_z;
	m_heading = copy.m_heading;

	return *this;
}

void DispCoordinate::fromGameCoordinate(const GameCoordinate& hex)
{
	static float d = 2 * (float)sin(PI_OVER_3) * 3.0f;
	static float cosPiOver6 = cos(PI_OVER_6);
	static float sinPiOver6 = sin(PI_OVER_6);
	
	m_x = d * cosPiOver6 * hex.x() + d * cosPiOver6 * hex.y();
	m_y = d * sinPiOver6 * hex.x() - d * sinPiOver6 * hex.y();
	m_z = 0;

	//Todo: heading
	m_heading = (float)hex.heading()*60 - 30.0f;
}

float DispCoordinate::x() const
{
	return m_x;
}

float DispCoordinate::y() const
{
	return m_y;
}

float DispCoordinate::z() const
{
	return m_z;
}

float DispCoordinate::heading() const
{
	return m_heading;
}

void DispCoordinate::set(const float& x, const float& y, const float& z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void DispCoordinate::setX(const float& x)
{
	m_x = x;
}

void DispCoordinate::setY(const float& y)
{
	m_y = y;
}

void DispCoordinate::setZ(const float& z)
{
	m_z = z;
}

void DispCoordinate::setHeading(const float& heading)
{
	m_heading = heading;
}

