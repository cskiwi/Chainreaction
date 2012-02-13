#pragma once

#include "Scalar.h"
#include "Vector2D.h"
#include "Point2D.h"

// Rectangular bounds
struct Rect2D
{
	Scalar MinX;
	Scalar MinY;
	Scalar MaxX;
	Scalar MaxY;

    void Set(Scalar x1, Scalar y1, Scalar x2, Scalar y2)
    {
        if( x1 < x2 )
        {
            MinX = x1;
            MaxX = x2;
        }
        else
        {
            MinX = x2;
            MaxX = x1;
        }

        if( y1 < y2 )
        {
            MinY = y1;
            MaxY = y2;
        }
        else
        {
            MinY = y2;
            MaxY = y1;
        }
    }

	Rect2D():MinX(0),MinY(0),MaxX(0),MaxY(0){}

    Rect2D(Scalar x1, Scalar y1, Scalar x2, Scalar y2) 
    {
        Set(x1,y1,x2,y2);
    }

	Rect2D(Point2D min, Point2D max) 
    {
        Set(min.x, min.y, max.x, max.y);
    }
	
    Rect2D(Point2D min, Vector2D size) 
    {
        Set(min.x, min.y, min.x + size.x, min.y + size.y);
    }
    
    Rect2D(const D2D1_RECT_F& rect)
    {
        Set(rect.left, rect.top, rect.right, rect.bottom);
    }

    Rect2D(const RECT& rect)
    {
        Set(rect.left, rect.top, rect.right, rect.bottom);
    }

	Point2D GetMin() const { return Point2D(MinX, MinY); }
	Point2D GetMax() const { return Point2D(MaxX, MaxY); }
	
    Vector2D GetSize() const { return GetMax()-GetMin(); }

	Scalar GetMinX() const { return MinX; }
	Scalar GetMinY() const { return MinY; }
	
    Scalar GetMaxX() const { return MaxX; }
	Scalar GetMaxY() const { return MaxY; }
	
    Scalar GetWidth() const { return MaxX-MinX; }
	Scalar GetHeight() const { return MaxY-MinY; }

	void GetVertices(Point2D ps[4])
	{
		ps[0].x = MinX;
		ps[0].y = MinY;
		ps[1].x = MaxX;
		ps[1].y = MinY;
		ps[2].x = MaxX;
		ps[2].y = MaxY;
		ps[3].x = MinX;
		ps[3].y = MaxY;
	}

	bool Contains(Point2D p) const
	{
		return MinX <= p.x && MaxX >= p.x && MinY <= p.y && MaxY >= p.y;
	}

	friend Point2D randomPointInside(Rect2D b) 
	{
		return Point2D( randomBetween(b.MinX, b.MaxX), randomBetween(b.MinY, b.MaxY) );
	}

	Point2D RandomPointInside() const
	{
		return randomPointInside(*this);
	}

	operator D2D1_RECT_F() const
	{
		return D2D1::RectF((FLOAT)MinX, (FLOAT)MinY, (FLOAT)MaxX, (FLOAT)MaxY);
	}


	// Print to a stream.
	friend std::ostream& operator<<(std::ostream& os, const Rect2D& r) 
	{
		os << "Rect2D( " << r.MinX << ", " << r.MinY << ", " << r.MaxX <<  ", " << r.MaxY << " )";
		return os;
	}
};
