//-----------------------------------------------------------------
// Painter Object
// C++ Source - Painter.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#pragma once

//math
#include "Point2D.h"
#include "Vector2D.h"
#include "Matrix2D.h"

class Image;
class TextFormat;
class CollisionMesh;

class Painter
{
public:
	Painter(ID2D1HwndRenderTarget *renderTargetPtr, ID2D1Factory *d2DFactoryPtr);
	virtual ~Painter(void);
	//Sets the current Color using r(Red), g(Green) and b(Blue). a(Alpha) is an optional parameter setting the transparency of the color
	void SetColor	(int r, int g, int b, int a=255);
	//Sets the current Color using color. ColorRGBA is a struct defined in the AMPEngine: stdafx.h 
	void SetColor	(ColorRGBA color);
	//Clears the window using the color specified by r,g and b
	void Clear(int r, int g, int b, int a=255)const ;
	//Draws a line from x1,y1 to x2,y2. strokeWidth is an optional parameter defining the width of the stroke.
	void DrawLine	(double x1, double y1, double x2,double y2, double strokeWidth=1 )const;
	//Draws a line from p1 to p2. strokeWidth is an optional parameter defining the width of the stroke.
	void DrawLine	(const Point2D & p1, const Point2D & p2, double strokeWidth=1 )const;
	//Draws an Ellipse around the centerPt, using radiusX and radiusY. strokeWidth is an optional parameter defining the width of the stroke.
	void DrawEllipse(const Point2D &centerPt, double radiusX,double radiusY , double strokeWidth=1 )const;
	//Draws an Ellipse around the point defined by centerX and centerY, using radiusX and radiusY. strokeWidth is an optional parameter defining the width of the stroke.
	void DrawEllipse(double centerX, double centerY, double radiusX,double radiusY , double strokeWidth=1 )const;
	//Fills an Ellipse around the point defined by centerPt using radiusX and radiusY. strokeWidth is an optional parameter defining the width of the stroke. 
	void FillEllipse(const Point2D &centerPt, double radiusX,double radiusY  )const;
	//Fills an Ellipse around the point defined by centerX and centerY, using radiusX and radiusY. strokeWidth is an optional parameter defining the width of the stroke. 
	void FillEllipse(double centerX, double centerY, double radiusX,double radiusY  )const;
	//Draws a rectangle 
	//Param left: the x coordinate of the topleft corner
	//Param top: the y coordinate of the topleft corner
	//Param width: the width of the rectangle
	//Param height: the height of the rectangle
	//Param strokeWidth: the width of the stroke used to draw
	void DrawRect	(double left, double top, double width, double height, double strokeWidth=1 )const;
	//Draws a rectangle 
	//Param lefttop: the Point2D defining the coordinates of the topleft corner
	//Param width: the width of the rectangle
	//Param height: the height of the rectangle
	//Param strokeWidth: the width of the stroke used to draw
	void DrawRect	(const Point2D & lefttop, double width, double height, double strokeWidth=1 )const;
	//Draws a rectangle 
	//Param r: the Rect2D2D defining the coordinates of the topleft corner
	//Param strokeWidth: the width of the stroke used to draw
	void DrawRect	(Rect2D r, double strokeWidth=1 )const;
	//Fills a rectangle 
	//Param lefttop: the Point2D defining the coordinates of the topleft corner
	//Param width: the width of the rectangle
	//Param height: the height of the rectangle
	//Param strokeWidth: the width of the stroke used to draw
	void FillRect	(double left, double top, double width, double height )const;
	//Fills a rectangle 
	//Param lefttop: the Point2D defining the coordinates of the topleft corner
	//Param width: the width of the rectangle
	//Param height: the height of the rectangle
	//Param strokeWidth: the width of the stroke used to draw
	void FillRect	(const Point2D & lefttop, double width, double height )const;
	//Fills a rectangle 
	//Param r: the Rect2D2D defining the coordinates of the topleft corner
	void FillRect	(Rect2D r )const;
	//Draws an image
	//Param imagePtr: pointer to the Image object to be drawn
	//Param x: the x coordinate of the topleft corner of the destination rect
	//Param y: the y coordinate of the topleft corner of the destination rect
	void DrawImage(Image* imagePtr, double x, double y)const;
	//Draws an image
	//Param imagePtr: pointer to the Image object to be drawn
	//Param x: the x coordinate of the topleft corner of the destination rect
	//Param y: the y coordinate of the topleft corner of the destination rect
	//Param srcRect: the rect in object space(source rect) to be Drawn. used to draw a part of an Image, e.g. Animated sprites
	void DrawImage(Image* imagePtr, double x, double y, Rect2D srcRect)const;
	//Draws an image
	//Param imagePtr: pointer to the Image object to be drawn
	//Param lefttop: the point2D containing the coordinates of the topleft corner of the destination rect
	void DrawImage(Image* imagePtr, const Point2D & lefttop)const;
	//Draws an image
	//Param imagePtr: pointer to the Image object to be drawn
	//Param lefttop: the point2D containing the coordinates of the topleft corner of the destination rect
	//Param srcRect: the rect in object space(source rect) to be Drawn. used to draw a part of an Image, e.g. Animated sprites
	void DrawImage(Image* imagePtr, const Point2D & lefttop, Rect2D srcRect)const;
	//Draws a String
	//Param textFormatPtr: pointer to a TextFormat object
	//Param text: String to be drawn
	//Param xPos: the x coordinate of the topleft corner of the bounding rect of the text to be drawn 
	//Param yPos: the y coordinate of the topleft corner of the bounding rect of the text to be drawn 
	//Param width: the width of the bounding rect of the text to be drawn 
	//Param yPos: the height of the bounding rect of the text to be drawn 
	void DrawString(TextFormat *textFormatPtr, const String &text,double xPos, double yPos, double width=0, double height=0)const;
	//Draws a String
	//Param textFormatPtr: pointer to a TextFormat object
	//Param text: String to be drawn
	//Param pos: the Point2D containing the coordinates of the topleft corner of the bounding rect of the text to be drawn 
	//Param width: the width of the bounding rect of the text to be drawn 
	//Param yPos: the height of the bounding rect of the text to be drawn 
	void DrawString(TextFormat *textFormatPtr, const String &text,const Point2D & pos, double width=0, double height=0)const;
	//Draws a CollisionMesh
	void DrawCollisionMesh(CollisionMesh* collisionMeshPtr)const;
	//Fills a CollisionMesh
	void FillCollisionMesh(CollisionMesh* collisionMeshPtr)const;

	//Draws a polygon: WARNING use CollisionMesh if possible!! Performance issue
	//void DrawPolygon(const Point2D pntArr[], int count, bool close);
	//Fills a polygon: WARNING use CollisionMesh if possible!! Performance issue
	//void FillPolygon(const Point2D pntArr[], int count, bool close);

	//Draws the coordinatesystem: x and y
	void DrawCoordinateSystem(double unitSize)const;

	/*Returns the width of the client window area in pixels	*/
	double GetWidth()const;
	/*Returns the height of the client window area in pixels	*/
	double GetHeight()const;

	/* Sets the matrix to use for all drawing operations*/
	void SetTransformMatrix(const Matrix2D &mat);

	/* Returns the matrix that is used for all drawing operations.*/
	Matrix2D GetTransformMatrix()const;

	//if b is true, it enables the anti aliased drawing of lines: Slow but nice
	//if b is false, it disables the anti aliased drawing of lines: faster
	void EnableAntiAlias(bool b);
	// Draws a point
	void DrawPoint(Point2D p, Scalar pointRadius=m_DefaultPointRadius);
	void DrawVector(Vector2D v, Point2D p, Scalar relArrowPos=1, Scalar arrowSize=m_DefaultArrowSize, Scalar lineSize=m_DefaultPenSize);
	void DrawMatrix(const Matrix2D& m, Scalar pointRadius=m_DefaultPointRadius, Scalar arrowSize=m_DefaultArrowSize, Scalar lineSize=m_DefaultPenSize);
	static const int m_DefaultPenSize=(2);
	static const int m_DefaultPointRadius=(5);
	static const int m_DefaultArrowSize=(7);

private:
    ID2D1HwndRenderTarget *m_RenderTargetPtr;
    IDWriteTextFormat *m_TextFormatPtr;
    ID2D1SolidColorBrush *m_ColorBrushPtr;
	D2D1_ANTIALIAS_MODE m_AntialiasMode;
	Matrix2D m_MatWorld;
	//used for Draw and PaintPolygon
	ID2D1Factory *m_D2DFactoryPtr;

};

