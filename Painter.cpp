//-----------------------------------------------------------------
// Painter Object
// C++ Source - Painter.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#include "StdAfx.h"

#include "Painter.h"
#include "Image.h"
#include "TextFormat.h"
#include "CollisionMesh.h"

Painter::Painter(ID2D1HwndRenderTarget *renderTargetPtr, ID2D1Factory *d2DFactoryPtr)
	:m_RenderTargetPtr(renderTargetPtr)
	,m_TextFormatPtr(0)
	,m_ColorBrushPtr(0)
	,m_AntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED)
	,m_D2DFactoryPtr(d2DFactoryPtr)
{
	// Create a brush.
	m_RenderTargetPtr->CreateSolidColorBrush(
	D2D1::ColorF(D2D1::ColorF::Black),
	&m_ColorBrushPtr
	);
}


Painter::~Painter(void)
{
}

void Painter::SetColor(int r, int g, int b, int a)
{
	if(m_ColorBrushPtr)
	{
		m_ColorBrushPtr->SetColor(D2D1::ColorF((FLOAT)(r/255.0), (FLOAT)(g/255.0), (FLOAT)(b/255.0), (FLOAT)(a/255.0)));
	}
}

void Painter::SetColor(ColorRGBA color)
{
	SetColor(color.r, color.g, color.b, color.a);
}

void Painter::Clear(int r, int g, int b, int a)const
{
	m_RenderTargetPtr->Clear(D2D1::ColorF((FLOAT)(r/255.0), (FLOAT)(g/255.0), (FLOAT)(b/255.0), (FLOAT)(a/255.0)));
}

void Painter::DrawLine(double x1, double y1, double x2,double y2, double strokeWidth )const
{
	m_RenderTargetPtr->DrawLine(D2D1::Point2F((FLOAT)(x1),(FLOAT)(y1)), D2D1::Point2F((FLOAT)(x2),(FLOAT)(y2)), m_ColorBrushPtr, (FLOAT)(strokeWidth));
}

void Painter::DrawLine(const Point2D & p1, const Point2D & p2, double strokeWidth )const
{
	DrawLine(p1.x,p1.y,p2.x,p2.y,strokeWidth);
}

void Painter::DrawEllipse(double centerX, double centerY, double radiusX,double radiusY, double strokeWidth )const
{
	D2D1_ELLIPSE ellipse=D2D1::Ellipse(D2D1::Point2F((FLOAT)centerX,(FLOAT)centerY),(FLOAT)radiusX, (FLOAT)radiusY);
	m_RenderTargetPtr->DrawEllipse(ellipse, m_ColorBrushPtr, (FLOAT)strokeWidth);

}

void Painter::DrawEllipse(const Point2D & centerPt, double radiusX,double radiusY , double strokeWidth)const
{
	DrawEllipse(centerPt.x, centerPt.y, radiusX, radiusY, strokeWidth);
}

void Painter::FillEllipse(double centerX, double centerY, double radiusX,double radiusY )const
{
	D2D1_ELLIPSE ellipse=D2D1::Ellipse(D2D1::Point2F((FLOAT)centerX,(FLOAT)centerY),(FLOAT)radiusX, (FLOAT)radiusY);
	m_RenderTargetPtr->FillEllipse(ellipse, m_ColorBrushPtr);
}

void Painter::FillEllipse(const Point2D & centerPt, double radiusX,double radiusY  )const
{
	FillEllipse(centerPt.x, centerPt.y, radiusX, radiusY);
}

void Painter::DrawRect(double left, double top, double width, double height, double strokeWidth )const
{
	D2D1_RECT_F rect=D2D1::RectF((FLOAT)left, (FLOAT)top, (FLOAT)(left+width), (FLOAT)(top+height));
	m_RenderTargetPtr->DrawRectangle( rect, m_ColorBrushPtr, (FLOAT)strokeWidth);

}
void Painter::DrawRect(const Point2D & lefttop, double width, double height, double strokeWidth)const
{
	DrawRect(lefttop.x,lefttop.y, width, height, strokeWidth);
}

void Painter::DrawRect(Rect2D r, double strokeWidth )const
{
	D2D1_RECT_F rect=D2D1::RectF((FLOAT)r.GetMinX(), (FLOAT)r.MinY, (FLOAT)(r.MaxX), (FLOAT)(r.MaxY));
	m_RenderTargetPtr->DrawRectangle( rect, m_ColorBrushPtr, (FLOAT)strokeWidth);
}

void Painter::FillRect(double left, double top, double width, double height)const
{
	D2D1_RECT_F rect=D2D1::RectF((FLOAT)left, (FLOAT)top, (FLOAT)(left+width), (FLOAT)(top+height));
	m_RenderTargetPtr->FillRectangle( rect, m_ColorBrushPtr);
}

void Painter::FillRect(const Point2D & lefttop, double width, double height )const
{
	FillRect(lefttop.x,lefttop.y, width, height);
}

void Painter::FillRect(Rect2D r)const
{
	D2D1_RECT_F rect=D2D1::RectF((FLOAT)r.GetMinX(), (FLOAT)r.MinY, (FLOAT)(r.MaxX), (FLOAT)(r.MaxY));
	m_RenderTargetPtr->FillRectangle( rect, m_ColorBrushPtr);
}

void Painter::DrawImage(Image* imagePtr, const Point2D & lefttop)const
{
	DrawImage(imagePtr, lefttop.x, lefttop.y);
}

void Painter::DrawImage(Image* imagePtr, const Point2D & lefttop, Rect2D srcRect)const
{
	DrawImage(imagePtr, lefttop.x, lefttop.y, srcRect);

}

void Painter::DrawImage(Image* imagePtr, double x, double y, Rect2D srcRect)const
{
	//The size and position, in device-independent pixels in the bitmap's coordinate space, of the area within the bitmap to draw.
	D2D1_RECT_F srcRect_f;
	srcRect_f.left = static_cast<float>(srcRect.GetMinX());
	srcRect_f.right = static_cast<float>(srcRect.MaxX);
	srcRect_f.top = static_cast<float>(srcRect.MinY);
	srcRect_f.bottom = static_cast<float>(srcRect.MaxY);

	//http://msdn.microsoft.com/en-us/library/dd371880(v=VS.85).aspx
	//The size and position, in device-independent pixels in the render target's coordinate space, 
	//of the area to which the bitmap is drawn. If the rectangle is not well-ordered, nothing is drawn, 
	//but the render target does not enter an error state.
	D2D1_RECT_F dstRect_f;
	dstRect_f.left = (FLOAT)x;
	dstRect_f.right =dstRect_f.left + (FLOAT)srcRect.MaxX-(FLOAT)srcRect.GetMinX();
	dstRect_f.top = (FLOAT)y;
	dstRect_f.bottom =  dstRect_f.top + (FLOAT)srcRect.MaxY-(FLOAT)srcRect.MinY;

	m_RenderTargetPtr->DrawBitmap(imagePtr->GetBitmapPtr(), dstRect_f, imagePtr->GetOpacity(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect_f);
}

void Painter::DrawImage(Image* imagePtr, double x, double y)const
{
	Rect2D srcRect;
	srcRect.MinX=0;
	srcRect.MaxX=imagePtr->GetWidth();
	srcRect.MinY=0;
	srcRect.MaxY=imagePtr->GetHeight();

	DrawImage(imagePtr, x, y, srcRect);

}

void Painter::DrawString(TextFormat *textFormatPtr,const String &text,double xPos, double yPos, double width, double height)const
{
	D2D1_SIZE_F dstSize_f;
	dstSize_f = m_RenderTargetPtr->GetSize();
	if(width==0)width = dstSize_f.width;
	if(height==0)height = dstSize_f.height;
	D2D1_RECT_F layoutRect = RectF((FLOAT)xPos, (FLOAT)yPos, (FLOAT)width, (FLOAT)height);
	m_RenderTargetPtr->DrawTextW(text.ToTChar(),text.GetLength(), textFormatPtr->GetTextFormat(), layoutRect, m_ColorBrushPtr);
}

void Painter::DrawString(TextFormat *textFormatPtr, const String &text, const Point2D & pos, double width, double height)const
{
	DrawString(textFormatPtr, text, pos.x, pos.y, width, height); 
}

void Painter::DrawCollisionMesh(CollisionMesh* collisionMeshPtr)const
{
	m_RenderTargetPtr->DrawGeometry(collisionMeshPtr->GetTransformedGeometry(), m_ColorBrushPtr);
}

void Painter::FillCollisionMesh(CollisionMesh* collisionMeshPtr)const
{
	m_RenderTargetPtr->FillGeometry(collisionMeshPtr->GetTransformedGeometry(), m_ColorBrushPtr);
}

//void Painter::DrawPolygon(const Point2D pntArr[], int count, bool close)
//{
//	CollisionMesh collisionMeshPtr(m_D2DFactoryPtr,0);
//	collisionMeshPtr.CreatePolygon(pntArr, count, close);
//
//	m_RenderTargetPtr->DrawCollisionMesh(collisionMeshPtr.GetTransformedGeometry(), m_ColorBrushPtr);
//}

//void Painter::FillPolygon(const Point2D pntArr[], int count, bool close)
//{
//	CollisionMesh collisionMeshPtr(m_D2DFactoryPtr,0);
//	collisionMeshPtr.CreatePolygon(pntArr, count, close);
//
//	m_RenderTargetPtr->FillGeometry(collisionMeshPtr.GetTransformedGeometry(), m_ColorBrushPtr);
//}

void Painter::DrawCoordinateSystem(double unitSize)const
{
	DrawLine(Point2D(0,0),Point2D(unitSize*1.3,0));
	DrawLine(Point2D(0,0),Point2D(0, unitSize*1.3));	
}
//getters 
double Painter::GetWidth()const
{
	D2D1_SIZE_F renderTargetSize = m_RenderTargetPtr->GetSize();
	return renderTargetSize.width;
}
double Painter::GetHeight()const
{
	D2D1_SIZE_F renderTargetSize = m_RenderTargetPtr->GetSize();
	return renderTargetSize.height;

}

//matrix operations
//void Painter::ResetTransformMatrix()
//{
//	m_MatWorld = Matrix3x2F::Identity();
//	m_RenderTargetPtr->SetTransform(m_MatWorld);
//}

void Painter::SetTransformMatrix(const Matrix2D &mat)
{
	m_MatWorld = mat;
	//Matrix3x2F matDirect2D = m_MatWorld.ToMatrix3x2F();
	m_RenderTargetPtr->SetTransform(m_MatWorld);
}

Matrix2D Painter::GetTransformMatrix()const
{
	return m_MatWorld;
}

void Painter::EnableAntiAlias(bool b)
{
	if(b)		m_AntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
	else		m_AntialiasMode = D2D1_ANTIALIAS_MODE_ALIASED;
	m_RenderTargetPtr->SetAntialiasMode(m_AntialiasMode);
}


void Painter::DrawPoint(Point2D p, Scalar pointRadius)
{
	FillEllipse(p, pointRadius, pointRadius);
}

void Painter::DrawVector(Vector2D v, Point2D p, Scalar relArrowPos, Scalar arrowSize, Scalar lineSize)
{
	Scalar s = arrowSize;
	Scalar l = v.Norm();

	if( l > 0.01 )
	{
		Vector2D u = v/l;

		//DrawLine( p, p+v-s*u, lineSize);
		DrawLine( p, p+v, lineSize);

		if( s > l/2 )
		{
			s = l/2;
		}

		Point2D p0 = p + relArrowPos*(l-s)*u;

		Matrix2D m( p0, u.Orthogonal(), u );

		Point2D ps[3] =
		{
			Point2D(+0.5*s,0) * m,
			Point2D(0,s) * m,
			Point2D(-0.5*s,0) * m,
		};

		DrawLine(ps[0],ps[1],lineSize);
		DrawLine(ps[1],ps[2],lineSize);
	}
}

void Painter::DrawMatrix(const Matrix2D& m, Scalar pointRadius, Scalar arrowSize, Scalar lineSize)
{
	DrawPoint(m.orig, pointRadius);
	DrawVector(m.dirX, m.orig, 1, arrowSize, lineSize);
	DrawVector(m.dirY, m.orig, 1, arrowSize, lineSize);
}
