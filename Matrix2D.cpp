#include "stdafx.h"
#include "Matrix2D.h"

void Matrix2D::SetAsRotate(double radians)
{
	dirX = Vector2D(cos(radians),sin(radians));
	dirY = dirX.Orthogonal();
	orig = Point2D(0,0);
}

void Matrix2D::SetAsTranslate(double tx, double ty)
{
	dirX = Vector2D(1,0);
	dirY = Vector2D(0,1);
	orig = Point2D(tx, ty);
}

void Matrix2D::SetAsTranslate(Point2D pt)
{
	dirX = Vector2D(1,0);
	dirY = Vector2D(0,1);
	orig = Point2D(pt.x, pt.y);
}

void Matrix2D::SetAsScale(double sx, double sy)
{
	dirX = Vector2D(sx,0);
	dirY = Vector2D(0,sy);
	orig = Point2D(0,0);
}

void Matrix2D::SetAsScale(double s)
{
	SetAsScale(s, s);
}


Matrix2D Matrix2D::Rotation(double angle)
{
	Vector2D dirX(cos(angle),sin(angle));
	Vector2D dirY = dirX.Orthogonal();
	return Orientation(dirX, dirY);
}

Matrix2D Matrix2D::Identity()
{
	Vector2D dirX = Vector2D(1,0);
	Vector2D dirY = Vector2D(0,1);
	Point2D	 orig = Point2D(0,0);

	return Matrix2D(dirX, dirY, Point2D());
}
Matrix2D Matrix2D::Scaling(double scale)
{
	return Scaling(scale,scale);
}

Matrix2D Matrix2D::Scaling(Vector2D scaleXY)
{
	return Scaling(scaleXY.x, scaleXY.y);
}

Matrix2D Matrix2D::Scaling(double scaleX, double scaleY)
{
	Vector2D dirX(scaleX,0);
	Vector2D dirY(0,scaleY);
	return Orientation(dirX, dirY);
}

Matrix2D Matrix2D::Translation(Point2D origin)
{
	return Matrix2D(Vector2D(1,0), Vector2D(0,1), origin);
}

Matrix2D Matrix2D::Translation(double tx, double ty)
{
	return Translation(Point2D(tx,ty));
}

Matrix2D Matrix2D::Translation(Vector2D t)
{
	return Translation(Point2D(t.x,t.y));
}

Matrix2D Matrix2D::Orientation(Vector2D dirX, Vector2D dirY)
{
	return Matrix2D(dirX, dirY, Point2D());
}
String Matrix2D::ToString()
{
	char buf[80];
	sprintf_s(buf,80,
		"X=[%.02f, %.02f]\n"
		"Y=[%.02f, %.02f]\n"
		"O=[%.02f, %.02f]",
		dirX.x, dirX.y,
		dirY.x, dirY.y,
		orig.x, orig.y);

	return String(buf);
}