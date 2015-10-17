#include "vertex.h"

void Vertex::setPosition(float x, float y)
{
    position.setX(x);
    position.setY(y);
    position.setZ(0);
}

void Vertex::setPosition(float x, float y, float z)
{
    position.setX(x);
    position.setY(y);
    position.setZ(z);
}

void Vertex::setUV(float u, float v)
{
    UVCoordinates.setX(u);
    UVCoordinates.setY(v);
}

void Vertex::operator =(const Vertex &vertex)
{
    position = vertex.getPosition();
  //  UVCoordinates = vertex.g
    _exists = vertex.exists();
}
