#include "StdAfx.h"
#include "HitOutline.h"
#include "CollisionMesh.h"
#include "Painter.h"

//#pragma optimize( "", off )

bool sortHitOnTime(const Hit& hit1, const Hit& hit2)
{
    return hit1.Time <= hit2.Time;
}

struct Cubic
{
	Point2D p0, p1, p2, p3;

	Cubic()
	{
	}

	Cubic(Point2D p0, Point2D p1, Point2D p2, Point2D p3)
		:p0(p0)
		,p1(p1)
		,p2(p2)
		,p3(p3)
	{
	}

	void Split(Cubic& c0, Cubic& c1)
	{
		auto p01 = lerp(p0, p1, 0.5);
		auto p12 = lerp(p1, p2, 0.5);
		auto p23 = lerp(p2, p3, 0.5);
		auto p02 = lerp(p01, p12, 0.5);
		auto p13 = lerp(p12, p23, 0.5);
		auto p03 = lerp(p02, p13, 0.5);

		c0.p0 = p0;
		c0.p1 = p01;
		c0.p2 = p02;
		c0.p3 = p03;

		c1.p0 = p03;
		c1.p1 = p13;
		c1.p2 = p23;
		c1.p3 = p3;
	}

	void FitQuads(Scalar squareTolerance, std::vector<Point2D>& qs, int recurse=3, bool split=true)
	{
		Scalar time1;
		Scalar time2;

		if( !split )
		{
			// If the lines are parallel, we must split
			split = !IntersectLines(p0, p1, p2, p3, time1, time2);;
		}

		if( !split )
		{
			// Compute intersection point of p0..p1 and p2..p3
			// This is the control point of the quadratic curve.
			Point2D q = p0+time1*(p1-p0);

			// Measure distance between midpoint on cubic curve [p0,p1,p2,p3] and quadratic curve [p0,q,p3]
			Scalar dx = (4*q.x+p3.x-3*p2.x-3*p1.x+p0.x)/8;
			Scalar dy = (4*q.y+p3.y-3*p2.y-3*p1.y+p0.y)/8;
			Scalar dd = dx*dx+dy*dy;

			// split curve if the quadratic isn't close enough
			split = dd > squareTolerance;

			if( !split )
			{
				// no need to split, stop recursion
				qs.push_back(p0);
				qs.push_back(q);
				qs.push_back(p3);
			}
		}

		if( split )
		{
			if( recurse == 0 )
			{
				qs.push_back(p0);
				qs.push_back(lerp(p1,p2,0.5));
				qs.push_back(p3);
			}
			else
			{
				Cubic c0, c1;
				Split(c0, c1);
				c0.FitQuads(squareTolerance,qs, recurse-1, false);
				c1.FitQuads(squareTolerance,qs, recurse-1, false);
			}
		}
	}
};

class SimplifiedGeometrySink : public ID2D1SimplifiedGeometrySink
{
private:
    ID2D1GeometrySink* m_BaseSink;
    std::vector<Point2D>& m_Lines;
    std::vector<Point2D>& m_Quads;
    Scalar m_FlatteningTolerance;
    Point2D m_Cursor;
	Point2D m_StartPoint;//Bart: used to close the figure

public:
	SimplifiedGeometrySink(
        ID2D1GeometrySink* baseSink, 
        Scalar flatteningTolerance, 
        std::vector<Point2D>& lines, 
        std::vector<Point2D>& quads)
		:m_FlatteningTolerance(flatteningTolerance)
        ,m_BaseSink(baseSink)
		,m_Lines(lines)
		,m_Quads(quads)
	{
	}

    HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void** ppvObject)
    {
        return E_NOTIMPL;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        return 0;
    }

    virtual ULONG STDMETHODCALLTYPE Release()
    {
        return 0;
    }

    void STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE fillMode) 
    {
        if( m_BaseSink )
            m_BaseSink->SetFillMode(fillMode);
    }

    void STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT vertexFlags)
    {
        if( m_BaseSink )
            m_BaseSink->SetSegmentFlags(vertexFlags);
    }

    void STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN figureBegin) 
    {
		m_StartPoint = startPoint;//Bart: remember, may be needed when endfigure is drawn
        m_Cursor = startPoint;
        if( m_BaseSink )
            m_BaseSink->BeginFigure(startPoint, figureBegin);
    }
    
    void STDMETHODCALLTYPE AddLines(CONST D2D1_POINT_2F *points, UINT pointsCount) 
    {
        if( m_BaseSink )
            m_BaseSink->AddLines(points, pointsCount);

        for( size_t i=0; i<pointsCount; ++i )
        {
            m_Lines.push_back(m_Cursor);
            m_Lines.push_back(points[i]);
            m_Cursor = points[i];
        }

    }

    void STDMETHODCALLTYPE AddBeziers(CONST D2D1_BEZIER_SEGMENT *beziers, UINT beziersCount) 
    {
        size_t quadIndex0 = m_Quads.size();

        for( size_t i=0; i<beziersCount; ++i )
        {
			Cubic c;
			c.p0 = m_Cursor;
			c.p1 = beziers[i].point1;
			c.p2 = beziers[i].point2;
			c.p3 = beziers[i].point3;
			m_Cursor = beziers[i].point3;
			c.FitQuads(m_FlatteningTolerance*m_FlatteningTolerance, m_Quads);
        }

        size_t quadIndex1 = m_Quads.size();

        if( m_BaseSink )
        {
            for( size_t i=quadIndex0; i<quadIndex1; i += 3 )
            {
                D2D1_QUADRATIC_BEZIER_SEGMENT s;
                s.point1 = m_Quads[i+1];
                s.point2 = m_Quads[i+2];
                m_BaseSink->AddQuadraticBezier(&s);
            }
        }
    }

    void STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END figureEnd) 
    {
        if( m_BaseSink )
            m_BaseSink->EndFigure(figureEnd); 
		if(figureEnd == D2D1_FIGURE_END_CLOSED)
		{
			m_Lines.push_back(m_Cursor);
			m_Lines.push_back(m_StartPoint);
		}
    }

    HRESULT STDMETHODCALLTYPE Close()
    {
        HRESULT hr = S_OK;

        if( m_BaseSink )
        {
            hr = m_BaseSink->Close();
        }

        return hr;
    }
};

HitOutline::HitOutline(ID2D1Geometry* geometryPtr, ID2D1GeometrySink* targetSink, int maxDegree, Scalar flatteningTolerance)
{
    SimplifiedGeometrySink sink(targetSink, flatteningTolerance, m_Lines, m_Quads);

    // TODO: Call Outline first?
    geometryPtr->Simplify(
		maxDegree >= 2 
		? D2D1_GEOMETRY_SIMPLIFICATION_OPTION_CUBICS_AND_LINES 
		: D2D1_GEOMETRY_SIMPLIFICATION_OPTION_LINES, 
        Matrix2D(), 
        (float)flatteningTolerance, 
        &sink);
}

HitOutline::~HitOutline()
{
}

int HitOutline::Raycast(Point2D p, Vector2D v, Scalar maxTime, 
    Hit* hitArr, int hitArrSize, Scalar sense)
{
	//local data: Bart
	Hit localHitArr[100];
	int localHitArrSize=100;

    int hitCount = 0;

    // TODO: Currently very slow, brute force!
    int lineVertexCount = m_Lines.size();
	int quadVertexCount = m_Quads.size();

    Scalar epsilon = 1e-6;

	Point2D p1 = p;
	Point2D p2 = p+v;
    Point2D orig(0,0);
	Scalar vDv = v.Dot(v);
    Vector2D n = v.Orthogonal();

	// Line-line intersections.
	if( lineVertexCount > 0 )
	{
		Point2D* lines = &m_Lines[0];

		for( int i=0; i<lineVertexCount && hitCount<localHitArrSize; i += 2 )
		{
			Point2D q1 = lines[i+0];
			Point2D q2 = lines[i+1];

			Vector2D ortho = (q2-q1).Orthogonal();
			if( (dot(ortho, v) * sense < 0) || sense==0)
			{
				Scalar time1;
				Scalar time2;
				if( IntersectLines(p1, p2, q1, q2, time1, time2) )
				{
					if( time1 >= 0 && time1 < maxTime &&
						time2 >= 0 && time2 <= 1 )
					{
						Hit hit;
                        hit.Time = time1;
						hit.Point = p + time1 * v;
						hit.Normal = ortho.Normalized();
                        localHitArr[hitCount++] = hit;
					}
				}
			}
		}
	}

	// Line-quadratic intersections.
	if( quadVertexCount > 0 )
	{
		Point2D* quads = &m_Quads[0];
		for( int i=0; i<quadVertexCount && hitCount<localHitArrSize; i += 3 )
		{
			Vector2D q0 = quads[i+0] - p;
			Vector2D q1 = quads[i+1] - p;
			Vector2D q2 = quads[i+2] - p;

            // Hit only possible if:
            // (1) not all points are on same side of ray,
            //     
            // (2) and some points are in front of ray (currently not checked, handled by abc formula)

            Scalar dot_n_q0 = dot(n,q0);
            Scalar dot_n_q1 = dot(n,q1);
            Scalar dot_n_q2 = dot(n,q2);

            if( dot_n_q0 <= 0 && dot_n_q1 <= 0 && dot_n_q2 <= 0 )
                continue;

            if( dot_n_q0 >= 0 && dot_n_q1 >= 0 && dot_n_q2 >= 0 )
                continue;

            Scalar ts[2];
            Scalar a = 2*dot_n_q1-dot_n_q0-dot_n_q2;
            Scalar b = 2*dot_n_q0-2*dot_n_q1;
            Scalar c = -dot_n_q0;
			int n = solveQuadratic(a,b,c,ts);

			for( int j=0; j<n && hitCount < localHitArrSize; ++j )
			{
				Scalar t = ts[j];
				if( t >= 0 && t <= 1 )
				{
					Vector2D diff = (2*(q2-q1)+2*(q0-q1))*t+2*(q1-q0);
					Vector2D ortho = diff.Orthogonal();
					if( (dot(ortho, v) * sense < 0) || sense==0)
					{
						Vector2D h = lerp( lerp(q0,q1,t), lerp(q1,q2,t), t);
						Scalar time = h.Dot(v) / vDv;
						//time >= -maxTime
                        if( time >= 0 && time < maxTime )
                        {
						    Hit hit;
						    hit.Point = p+h;
						    hit.Time = time;
						    hit.Normal = ortho.Normalized();
                            localHitArr[hitCount++] = hit;
                        }
					}
				}
			}
		}
	}

    std::sort(localHitArr, localHitArr+hitCount, sortHitOnTime);
	//Bart
	for( int i=0; i<hitCount && i < hitArrSize; ++i )
	{
		hitArr[i]=localHitArr[i];
	}
    return hitCount;
}




