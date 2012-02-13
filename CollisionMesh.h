//-----------------------------------------------------------------
// CollisionMesh Object
// C++ Source - CollisionMesh.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Matrix2D.h"
#include "Rect2D.h"
#include "HitOutline.h"
//forward class declarations
class Image;
class Painter;
class ResourceLoader;

//-----------------------------------------------------
// CollisionMesh Class									
//-----------------------------------------------------
class CollisionMesh
{
public: 
	//---------------------------
	// Constructor(s)
	//---------------------------
	CollisionMesh(ID2D1Factory *d2DFactoryPtr, IWICImagingFactory *wICFactoryPtr);						
	//CollisionMesh(ResourceLoader* resourceLoaderPtr);						

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~CollisionMesh();			

	//---------------------------
	// General Methods
	//---------------------------
	/*Creates a rectangular CollisionMesh in object space*/
	bool CreateRect(double x, double y, double width, double height);	
	/*Creates an elliptical CollisionMesh in object space*/
	bool CreateEllipse(double centerX, double centerY, double radiusX,double radiusY);	
	/*Creates a polygon using the Point2D array*/
	bool CreatePolygon(const Point2D pntArr[], int count, bool close);

	/*Returns true if the Geometries overlap in World space*/
	bool HitTest(CollisionMesh* regPtr)const;	
	/*Returns true if geometry includes x,y in World Space*/
	bool HitTest(double x, double y)const;	
	/*Returns true if geometry includes pt in World Space*/
	bool HitTest(Point2D pt)const;	
	/*Returns true if CollisionMesh is created*/
	bool Exists()const{return m_GeometryPtr==0 ?true :false;}
	/*Returns rectangular intersection of this CollisionMesh and regPtr in World Space*/
	Rect2D CollisionTest(CollisionMesh* regPtr)const;	
	/*Returns the bounding rectangle of the geometry.*/
	Rect2D GetBounds()const;	

	/*Positions the CollisionMesh from object space to world space at position x,y*/
	void SetPos(double x, double y);				
	/*Positions the CollisionMesh at position pt*/
	void SetPos(Point2D pt);

    // Internal usage only.
    ID2D1TransformedGeometry *GetTransformedGeometry() { return m_TransformedGeometryPtr; }
    ID2D1Geometry *GetGeometry() { return m_GeometryPtr; }

	//the matrix is used to set the position, rotation and scale of the geometry*/
    //The matrix that will transform vertices of this geometry 
    //from object space to world space.
	void SetTransformMatrix(const Matrix2D &matTrans);
	
	const Matrix2D& GetTransformMatrix() const { return m_MatTransform; }

   // Load geometry from a file.
    // Currently only a tiny subset of SVG is supported,
    // namely groups and paths (but no arcs)
    void CreateFromFile(const char* filePath);

	void CreateFromOutline(HitOutline* HitOutline);

    // Simplify the geometry: is performed automaticly.
    void Simplify(int maxDegree=2, Scalar flatteningTolerance=5);

    // Intersect simplified geometry with a ray.
    // You *must* call Simplify first!
    // All points on the ray are p + time*v, where time >= 0
    // Returns all hits with time >= 0 && time < maxTime.
    // Return number of hits.
    // Ignores hits facing in the same direction as v,
    // i.e. with dot(normal, v) * sense > 0
    int Raycast(Point2D p, Vector2D v, Scalar maxTime, 
        Hit* hitArr, int hitArrSize, Scalar sense=-1);

	// Exclude this CollisionMesh with the excludingMesh
	// Return a new CollisionMesh object
	CollisionMesh * Exclude(CollisionMesh *excludingMeshPtr);

private:

	void CreateTransformedGeometry(double posX, double posY);
	void Exit(const char* errorMsg);
	void ExitIfNotExists(ID2D1Geometry * collisionMeshPtr)const;


	//---------------------------
	// Datamembers
	//---------------------------
	ID2D1Factory *m_D2DFactoryPtr;
	IWICImagingFactory *m_WICFactoryPtr;

	//object spaced and simplified geometry
	ID2D1Geometry* m_GeometryPtr;
	//Simplify may only be executed once on a geometry, so this one is allways used to simplify
	ID2D1Geometry* m_OriginalGeometryPtr;
	//world spaced, NOT view spaced
	ID2D1TransformedGeometry *m_TransformedGeometryPtr;

    // used for raycasting
    HitOutline* m_HitOutlinePtr;

	//used to create TransformedGeometry
	Matrix2D m_MatTransform;

	HRESULT hr;

	//---------------------------
	// Private methods
	//---------------------------

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	CollisionMesh(const CollisionMesh& hrRef);				
	CollisionMesh& operator=(const CollisionMesh& hrRef);	
};