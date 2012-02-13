//-----------------------------------------------------------------
// CollisionMesh Object
// C++ Source - CollisionMesh.cpp - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#include "StdAfx.h"

#include "CollisionMesh.h"
#include "Painter.h"
#include "ResourceLoader.h"
#include "SvgLoader.h"
#include "HitOutline.h"

CollisionMesh::CollisionMesh(ID2D1Factory *d2DFactoryPtr, IWICImagingFactory *wICFactoryPtr)	
	:m_D2DFactoryPtr(d2DFactoryPtr)
	,m_WICFactoryPtr(wICFactoryPtr)
	,m_GeometryPtr(0)
	,m_OriginalGeometryPtr(0)
	,m_TransformedGeometryPtr(0)
	,m_HitOutlinePtr(0)
{
	m_MatTransform = Matrix2D::Identity();
}

CollisionMesh::~CollisionMesh()
{
	if(m_GeometryPtr != m_OriginalGeometryPtr)SafeRelease(&m_OriginalGeometryPtr);
	SafeRelease(&m_GeometryPtr);
	SafeRelease(&m_TransformedGeometryPtr);
	SafeDelete(&m_HitOutlinePtr);
}

bool CollisionMesh::CreateRect(double x, double y, double width, double height)
{
	ID2D1RectangleGeometry* RG = 0;
	hr = m_D2DFactoryPtr->CreateRectangleGeometry(
		D2D1::RectF((FLOAT)x, (FLOAT)y, static_cast<FLOAT>(x+width), static_cast<FLOAT>(y+height)),
		&RG
		);

	if(SUCCEEDED(hr))
	{
		//ID2D1EllipseGeometry inherits from interface ID2D1Geometry
		SafeRelease(&m_GeometryPtr);
		m_OriginalGeometryPtr = m_GeometryPtr = RG;
		CreateTransformedGeometry(0,0);
		return true;
	}
	else 
	{
		MessageBoxW(NULL, L"CollisionMesh CreateRect Failed" , L"ERROR",MB_ICONERROR);
		exit(-1);
	}
	return false;
}
bool CollisionMesh::CreateEllipse(double centerX, double centerY, double radiusX,double radiusY)
{
	ID2D1EllipseGeometry* EG = 0;
	hr = m_D2DFactoryPtr->CreateEllipseGeometry(
		D2D1::Ellipse(D2D1::Point2F((FLOAT)centerX, (FLOAT)centerY), static_cast<FLOAT>(radiusX), static_cast<FLOAT>(radiusY)),
		&EG
		);
	if(SUCCEEDED(hr))
	{
		//ID2D1EllipseGeometry inherits from interface ID2D1Geometry
		SafeRelease(&m_GeometryPtr);
		m_OriginalGeometryPtr = m_GeometryPtr = EG;
		CreateTransformedGeometry(0,0);
		return true;
	}
	else 
	{
		MessageBoxW(NULL, L"CollisionMesh CreateEllipse Failed" , L"ERROR",MB_ICONERROR);
		exit(-1);
	}
	return false;
}
bool CollisionMesh::CreatePolygon(const Point2D pntArr[], int count, bool close)
{
	ID2D1GeometrySink *pSink = NULL;
	ID2D1PathGeometry * pathGeometryPtr=0;
	HRESULT hr = m_D2DFactoryPtr->CreatePathGeometry(&pathGeometryPtr);
	if (SUCCEEDED(hr))
	{
		// Write to the path geometry using the geometry sink.
		hr = pathGeometryPtr->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->BeginFigure(
				D2D1::Point2F(static_cast<FLOAT>(pntArr[0].x), static_cast<FLOAT>(pntArr[0].y)),
				D2D1_FIGURE_BEGIN_FILLED
				);
			for(int i=1;i<count;++i)
			{
				pSink->AddLine(D2D1::Point2F(static_cast<FLOAT>(pntArr[i].x), static_cast<FLOAT>(pntArr[i].y)));
			}

			if(close)pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			else pSink->EndFigure(D2D1_FIGURE_END_OPEN);

			hr = pSink->Close();
		}
		SafeRelease(&pSink);
	}
	if (SUCCEEDED(hr))
	{
		SafeRelease(&m_GeometryPtr);
		m_OriginalGeometryPtr = m_GeometryPtr = pathGeometryPtr;
		CreateTransformedGeometry(0,0);
		return true;
	}
	else 
	{
		MessageBoxW(NULL, L"CollisionMesh CreatePolygon Failed" , L"ERROR",MB_ICONERROR);
		exit(-1);
	}
}

bool CollisionMesh::HitTest(CollisionMesh* regPtr)const
{
	HRESULT hr;
	ExitIfNotExists(regPtr->GetTransformedGeometry());
	ExitIfNotExists(m_TransformedGeometryPtr);

	D2D1_GEOMETRY_RELATION result = D2D1_GEOMETRY_RELATION_UNKNOWN;
	hr = m_TransformedGeometryPtr->CompareWithGeometry(regPtr->GetTransformedGeometry(), D2D1::IdentityMatrix(), &result);
	if (SUCCEEDED(hr))
	{
		if (result != D2D1_GEOMETRY_RELATION_DISJOINT) return true;
	}
	return false;
}

bool CollisionMesh::HitTest(double x, double y)const
{
	ExitIfNotExists(m_TransformedGeometryPtr);

	BOOL bHit;
	m_TransformedGeometryPtr->FillContainsPoint(D2D1::Point2F(FLOAT(x), FLOAT(y)), D2D1::Matrix3x2F::Identity(), &bHit);
	if(bHit)
		return true;
	return false;
}

bool CollisionMesh::HitTest(Point2D pt)const
{
	return HitTest(pt.x,pt.y);
}

Rect2D CollisionMesh::CollisionTest(CollisionMesh* regPtr)const
{
	HRESULT hr;
	ExitIfNotExists(regPtr->GetTransformedGeometry());
	ExitIfNotExists(m_TransformedGeometryPtr);

	D2D1_RECT_F bounds=D2D1::RectF();

	//if there is no intersection, there is no need to calculate the intersection CollisionMesh -> So return an empty rect
	Rect2D nulrect;
	if(!HitTest(regPtr))return nulrect;

	ID2D1PathGeometry *pPathGeometryIntersect = NULL;
	ID2D1GeometrySink *pGeometrySink = NULL;
	hr = m_D2DFactoryPtr->CreatePathGeometry(&pPathGeometryIntersect);
	hr = pPathGeometryIntersect->Open(&pGeometrySink);
	if (SUCCEEDED(hr))
	{
		hr = m_TransformedGeometryPtr->CombineWithGeometry(
			regPtr->GetTransformedGeometry(),
			D2D1_COMBINE_MODE_INTERSECT,
			NULL,
			NULL,
			pGeometrySink
			);
	}
	else
	{
		DebugBreak();
	}

	if (SUCCEEDED(hr))
	{
		hr = pGeometrySink->Close();
	}
	else
	{
		DebugBreak();
	}

	SafeRelease(&pGeometrySink);

	if (SUCCEEDED(hr))
	{
		hr = pPathGeometryIntersect->GetBounds(D2D1::IdentityMatrix(), &bounds);
		// If the bounds are empty, this will be a rect where bounds.left > bounds.right.
		if(bounds.left > bounds.right)bounds=D2D1::RectF();
	}
	else
	{
		DebugBreak();
	}
	SafeRelease(&pPathGeometryIntersect);
	//return boundingbox;
	Rect2D rect(bounds.left, bounds.top, bounds.right, bounds.bottom);
	return rect;
}

Rect2D CollisionMesh::GetBounds()const
{
	HRESULT hr;
	ExitIfNotExists(m_TransformedGeometryPtr);
	D2D1_RECT_F bounds=D2D1::RectF();

	Rect2D nulrect;

	hr = m_TransformedGeometryPtr->GetBounds(
		D2D1::IdentityMatrix(),
		&bounds
		);
	// If the bounds are empty, this will be a rect where bounds.left > bounds.right.
	if(bounds.left > bounds.right)bounds=D2D1::RectF();

	Rect2D rect(bounds.left, bounds.top, bounds.right, bounds.bottom);
	return rect;
}

void CollisionMesh::SetPos(double x, double y)
{
	ExitIfNotExists(m_GeometryPtr);

	D2D_MATRIX_3X2_F matTransform = D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(x), static_cast<FLOAT>(y));
	SafeRelease(&m_TransformedGeometryPtr);
	m_D2DFactoryPtr->CreateTransformedGeometry(m_GeometryPtr, &matTransform, &m_TransformedGeometryPtr);
}

void CollisionMesh::SetPos(Point2D pt)
{
	SetPos(pt.x,pt.y);
}

void CollisionMesh::SetTransformMatrix(const Matrix2D &matTrans)
{
	ExitIfNotExists(m_GeometryPtr);

	m_MatTransform = matTrans;

	SafeRelease(&m_TransformedGeometryPtr);
	D2D1_MATRIX_3X2_F d2dTransform = m_MatTransform;
	m_D2DFactoryPtr->CreateTransformedGeometry(m_GeometryPtr, &(d2dTransform), &m_TransformedGeometryPtr);
}

void CollisionMesh::CreateFromFile(const char* filePath)
{
    // TODO: Check if SVG file.
    ID2D1Geometry *geometryPtr = loadGeometryFromSvgFile(m_D2DFactoryPtr, filePath);
 	SafeRelease(&m_GeometryPtr);
	m_OriginalGeometryPtr = m_GeometryPtr = geometryPtr;
	Simplify(2,5);//make it usable for raycast
	CreateTransformedGeometry(0,0);
}

void CollisionMesh::CreateFromOutline(HitOutline* HitOutline)
{
	ID2D1GeometrySink *pSink = NULL;
	ID2D1PathGeometry * pathGeometryPtr;
	HRESULT hr = m_D2DFactoryPtr->CreatePathGeometry(&pathGeometryPtr);
	if (SUCCEEDED(hr))
	{
		// Write to the path geometry using the geometry sink.
		hr = pathGeometryPtr->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			Point2D noPoint(DBL_MAX, DBL_MAX);
			Point2D prevPoint = noPoint;

			auto& lines = HitOutline->m_Lines;
			size_t lineCount = lines.size();

			if( lineCount != 0 )
			{
				for(size_t i=0;i<lineCount; i+=2 )
				{
					Point2D p1 = lines[i+0];
					Point2D p2 = lines[i+1];

					if( prevPoint != p1 )
					{
						if( prevPoint != noPoint )
						{
							pSink->EndFigure(D2D1_FIGURE_END_OPEN);
						}

						pSink->BeginFigure(p1, D2D1_FIGURE_BEGIN_FILLED);
					}

					pSink->AddLine(p2);

					prevPoint = p2;
				}
			}

			auto& quads = HitOutline->m_Quads;
			size_t quadCount = quads.size();

			if( quadCount != 0 )
			{
				for(size_t i=0;i<quadCount; i+=3 )
				{
					Point2D p1 = quads[i+0];
					Point2D p2 = quads[i+1];
					Point2D p3 = quads[i+2];

					if( prevPoint != p1 )
					{
						if( prevPoint != noPoint )
						{
							pSink->EndFigure(D2D1_FIGURE_END_OPEN);
						}

						pSink->BeginFigure(p1, D2D1_FIGURE_BEGIN_FILLED);
					}

					D2D1_QUADRATIC_BEZIER_SEGMENT qbs = {p2,p3};
					pSink->AddQuadraticBezier(qbs);

					prevPoint = p3;
				}
			}

			if( prevPoint != noPoint )
			{
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			}

			hr = pSink->Close();
		}
		SafeRelease(&pSink);
	}
 	if (SUCCEEDED(hr))
	{
		SafeRelease(&m_GeometryPtr);
		m_OriginalGeometryPtr = m_GeometryPtr = pathGeometryPtr;
		Simplify(2,5);//make it usable for raycast
		CreateTransformedGeometry(0,0);
		return;
	}
    Exit("Geometry::CreateFromOutline failed");
}

void CollisionMesh::Simplify(int maxDegree, Scalar flatteningTolerance)
{
    HitOutline* newHitOutlinePtr; 
    ID2D1GeometrySink *pSink = NULL;
    ID2D1PathGeometry * pathGeometryPtr;
    HRESULT hr = m_D2DFactoryPtr->CreatePathGeometry(&pathGeometryPtr);

    if (SUCCEEDED(hr))
    {
        // Write to the path geometry using the geometry sink.
        hr = pathGeometryPtr->Open(&pSink);

        if (SUCCEEDED(hr))
        {
            newHitOutlinePtr = new HitOutline(m_OriginalGeometryPtr, pSink, maxDegree, flatteningTolerance);
            hr = pSink->Close();
        }
    }
	if (SUCCEEDED(hr))
	{
		//delete m_GeometryPtr if it is not same object as original: is second time simplefied is called
		if(m_OriginalGeometryPtr !=m_GeometryPtr)SafeRelease(&m_GeometryPtr);

		m_GeometryPtr = pathGeometryPtr;

		SafeDelete(&m_HitOutlinePtr);
		m_HitOutlinePtr = newHitOutlinePtr;

		return;
	}
	Exit("Geometry::Simplify failed");
}

int CollisionMesh::Raycast(Point2D p, Vector2D v, Scalar maxTime, 
    Hit* hitArr, int hitArrSize, Scalar sense)
{
    Point2D lp  =  p / m_MatTransform;
    Vector2D lv = v / m_MatTransform;

    if( m_HitOutlinePtr == nullptr )
    {
        Exit("You need to simplify the CollisonMesh before using RayCast!");
    }

    int numHits = m_HitOutlinePtr->Raycast(lp, lv, maxTime, hitArr, hitArrSize, sense);

    for( int i=0; i<numHits && i<hitArrSize; ++i )
    {
        hitArr[i].Point = hitArr[i].Point * m_MatTransform;
        hitArr[i].Normal = hitArr[i].Normal * m_MatTransform;
    }

    return numHits;
}

CollisionMesh *CollisionMesh::Exclude(CollisionMesh *excludingMeshPtr)
{
	//todo: do not use the transformed geometries
	HRESULT hr=0;

	ExitIfNotExists(excludingMeshPtr->GetTransformedGeometry());
	ExitIfNotExists(m_OriginalGeometryPtr);

	//transformmer de excludingmesh naar object space van dit mesh object
	ID2D1TransformedGeometry * excludingGeomInThisObjectSpacePtr=0;
	D2D1_MATRIX_3X2_F d2dTransform = m_MatTransform.Inverse();
	m_D2DFactoryPtr->CreateTransformedGeometry(excludingMeshPtr->GetTransformedGeometry(), &(d2dTransform), &excludingGeomInThisObjectSpacePtr);

	ID2D1PathGeometry *pPathGeometryIntersect = NULL;
	ID2D1GeometrySink *pGeometrySink = NULL;
	hr = m_D2DFactoryPtr->CreatePathGeometry(&pPathGeometryIntersect);
	hr = pPathGeometryIntersect->Open(&pGeometrySink);
	if (SUCCEEDED(hr))
	{
		hr = m_OriginalGeometryPtr->CombineWithGeometry(
			excludingGeomInThisObjectSpacePtr,
			D2D1_COMBINE_MODE_EXCLUDE,
			NULL,
			NULL,
			pGeometrySink
			);
	}
	else
	{
		DebugBreak();
	}

	if (SUCCEEDED(hr))
	{
		hr = pGeometrySink->Close();
	}
	else
	{
		DebugBreak();
	}

	SafeRelease(&pGeometrySink);

	if (SUCCEEDED(hr))
	{
		SafeRelease(&m_OriginalGeometryPtr);
		m_OriginalGeometryPtr = pPathGeometryIntersect;
		Simplify(2,5);
		CreateTransformedGeometry(0,0);
		return 0;
	}
	else
	{
		Exit("Excluding Failed");
	}

	return 0;
}

void CollisionMesh::CreateTransformedGeometry(double posX, double posY)
{
	SetPos(posX, posY);
}


void CollisionMesh::Exit(const char* errorMsg)
{
    MessageBoxA(NULL, errorMsg, "ERROR",MB_ICONERROR);
    DebugBreak();
    exit(-1);
}

void CollisionMesh::ExitIfNotExists(ID2D1Geometry * collisionMeshPtr)const
{
	if(collisionMeshPtr)return;
	MessageBoxW(NULL, L"CollisionMesh used before Create() is called.\n Exit" , L"GEOMETRY ERROR",MB_ICONERROR);
	exit(-1);
}


