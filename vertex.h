#pragma once
#include "maths.h"

struct Vertex{
    maths::vec3f normal;
    maths::vec3f position;
    maths::vec2f texCoords;
    maths::vec3f color;

    Vertex() = default;
    /*Vertex(const Point& pos)
		:
		pos(pos)
	{}
	Vertex(const Point& pos, const Vertex& src)
		:
		color(src.color),
		pos(pos)
	{}*/
    Vertex(const maths::vec3f &pos, const maths::vec3f &normal, const maths::vec3f &c)
        : color(c),
          normal(normal),
          position(pos)
    {
    
    }
    Vertex &operator+=(const Vertex &rhs)
    {
        position = maths::add(position,rhs.position);
        color = maths::add(color,rhs.color);
        return *this;
    }
    Vertex operator+(const Vertex &rhs) const
    {
        return Vertex(*this) += rhs;
    }
    Vertex &operator-=(const Vertex &rhs)
    {
        position = maths::sub(position,rhs.position);
        color = maths::sub(color,rhs.color);
        return *this;
    }
    Vertex operator-(const Vertex &rhs) const
    {
        return Vertex(*this) -= rhs;
    }
    Vertex &operator*=(float rhs)
    {
        position = maths::mul(position,rhs);
        color = maths::mul(color,rhs);
        return *this;
    }
    Vertex operator*(float rhs) const
    {
        return Vertex(*this) *= rhs;
    }
    Vertex &operator/=(float rhs)
    {
        position = maths::mul(position,1/rhs);
        color = maths::mul(color,1/rhs);
        return *this;
    }
    Vertex operator/(float rhs) const
    {
        return Vertex(*this) /= rhs;
    }
};  