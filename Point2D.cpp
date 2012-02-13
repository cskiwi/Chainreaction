#include "stdafx.h"
#include "Point2D.h"

String Point2D::ToString() const
{
	char buf[80];
	sprintf_s(buf,80,"[%.2f, %.2f]",x,y);
	return String("")+buf;
}
