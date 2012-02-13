#include "stdafx.h"
#include "Vector2D.h"

String Vector2D::ToString() const
{
	char buf[80];
	sprintf_s(buf,80,"[%.2f, %.2f]",x,y);
	return String("")+buf;
}