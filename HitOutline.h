#pragma once

#include "Point2D.h"
#include "Vector2D.h"

class Geometry;
class Painter;

struct Hit
{
    Scalar Time;
    Point2D Point;
    Vector2D Normal;
};

class HitOutline
{
public:
    // Converts geometry to an outline suitable for ray casting.
    // maxDegree=1 => outline only contains line segments
    // maxDegree=2 => outline contains line segments and quadratic curve segments
    HitOutline(ID2D1Geometry* geometryPtr, ID2D1GeometrySink* targetSink, 
        int maxDegree=2, Scalar flatteningTolerance=1);

    virtual ~HitOutline();

    // Intersects this outline with a ray.
    // All points on the ray are p + time*v, where time >= 0
    // Returns all hits with time >= 0 && time < maxTime.
    // Return number of hits.
    // Ignores hits facing in the same direction as v,
    // i.e. with dot(normal, v) * sense > 0
    int Raycast(Point2D p, Vector2D v, Scalar maxTime, 
        Hit* hitArr, int hitArrSize,
        Scalar sense=-1);

private:
    friend class CollisionMesh;

    std::vector<Point2D> m_Lines;
    std::vector<Point2D> m_Quads;

    // Private methods
    //---------------------------

    // -------------------------
    // Disabling default copy constructor and default assignment operator.
    // If you get a linker error from one of these functions, your class is internally trying to use them. This is
    // an error in your class, these declarations are deliberately made without implementation because they should never be used.
    // -------------------------
    HitOutline(const Geometry& hrRef);				
    HitOutline& operator=(const Geometry& hrRef);	

};

