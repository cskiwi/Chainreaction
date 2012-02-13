//-----------------------------------------------------------------
// SVG geometry loader
// C++ Source - SvgLoader.cpp - version 2011 v1_00
// Author Peter Verswyvelen
// http://www.digitalartsandentertainment.be/
//
// Currently only supports a tiny subset of SVG,
// namely groups with transform matrices and paths (no arcs)
//-----------------------------------------------------------------

#include "StdAfx.h"
#include "SvgLoader.h"
#include "TinyXML.h"
#include "Matrix2D.h"

using namespace std;

// Replace all whitespace by space.
static char whiteSpaceMapper(char c)
{
    switch( c )
    {
    case '\n':
    case '\r':
    case '\t':
        return ' ';

    default:
        return c;
    }
}

// Throw a formatted std::exception
static void throwError(const char* msg, ...)
{
    char buffer[1000];

    va_list args;
    va_start(args,msg);
    vsprintf_s(buffer, msg, args);
    va_end(args);

    throw std::exception(buffer);
}

// Skips any optional commas in the stream
// SVG has a really funky format,
// not sure this code works for all cases.
// TODO: Test cases!
static void skipSvgComma(stringstream& ss, bool isRequired)
{
    while(true)
    {
        char c = ss.get();

        if( ss.eof() )
		{
			if( isRequired )
	            throwError("expected comma or whitespace");
			break;
		}

        if( c==',' )
            return;

        if( !isspace(c) )
        {
            ss.unget();
            return;
        }
    } 
}

static Scalar readSvgValue(stringstream& ss, Scalar defaultValue)
{
    Scalar s;
    ss >> s;

    if( ss.eof() )
    {
        s = defaultValue;
    }
    else
    {
        skipSvgComma(ss, false);
    }

    return s;
}

static Scalar readSvgValue(stringstream& ss, bool separatorRequired)
{
	Scalar s;
	ss >> s;
	skipSvgComma(ss, separatorRequired);
	return s;
}

// Reads a single point
static Point2D readSvgPoint(stringstream& ss)
{
    Point2D p;
	p.x = readSvgValue(ss, true);
	p.y = readSvgValue(ss, false);
    return p;
}


// Read the next point, 
// taking into account relative and absolute positioning.
// Advances the cursor if requested.
// Throws an exception if the figure is not open
static Point2D nextSvgPoint(stringstream& ss, Point2D& cursor, char cmd, bool isOpen, bool advance)
{
    if( isOpen )
        throwError("expected 'M' or 'm' command");

    Point2D p = readSvgPoint(ss);

    if( islower(cmd) )
    {
        // Relative point
        p = cursor + (p-Point2D());
    }

    if( advance )
    {
        cursor = p;
    }

    return p;
}

// Reads next point, given only the new x coordinate 
static Point2D nextSvgCoordX(stringstream& ss, Point2D& cursor, char cmd, bool isOpen)
{
    if( isOpen )
        throwError("expected 'M' or 'm' command");

    double c;
    ss >> c;

    if( islower(cmd) )
    {
        // Relative point
        cursor += Vector2D(c,0);
    }
    else
    {
        cursor.x = c;
    }

    return cursor;
}

// Reads next point, given only the new y coordinate 
static Point2D nextSvgCoordY(stringstream& ss, Point2D& cursor, char cmd, bool isOpen)
{
    if( isOpen )
        throwError("expected 'M' or 'm' command");

    double c;
    ss >> c;

    if( islower(cmd) )
    {
        // Relative point
        cursor += Vector2D(0,c);
    }
    else
    {
        cursor.y = c;
    }

    return cursor;
}

static Matrix2D readTransform(TiXmlElement* elem)
{
    int argCount;
	Matrix2D matrix;

	const char* transformData = elem->Attribute("transform");
	if( transformData != nullptr )
	{
		switch( transformData[0] )
		{
		case 'm':
			sscanf_s(transformData, "matrix(%lf,%lf,%lf,%lf,%lf,%lf)", 
				&matrix.dirX.x,
				&matrix.dirX.y,
				&matrix.dirY.x,
				&matrix.dirY.y,
				&matrix.orig.x,
				&matrix.orig.y);
			break;

		case 't':
            argCount = sscanf_s(transformData, "translate(%lf,%lf)",
    				&matrix.orig.x, &matrix.orig.y);
			break;

        case 's':
            argCount = sscanf_s(transformData, "scale(%lf,%lf)",
                        &matrix.dirX.x, &matrix.dirY.y);

            if( argCount == 1 )
            {
                matrix.dirY.y = matrix.dirX.x;
            }
            break;

        case 'r':
            {
                Scalar angle;
                Vector2D center;
                argCount = sscanf_s(transformData, "rotate(%lf,%lf,%lf)",
                    &angle, &center.x, &center.y);

                switch( argCount )
                {
                case 1:
                    matrix = Matrix2D::Rotation(toRadians(angle));
                    break;
                case 3:
                    matrix = Matrix2D::Translation(-center) * Matrix2D::Rotation(toRadians(angle)) * Matrix2D::Translation(center);
                    break;
                }
            }
            break;
		
		default:
			throwError("unsupported transform attribute");
			break;
		}
	}

	return matrix;
}

static ID2D1PathGeometry* readSvgPath(ID2D1Factory* factory, const char* data)
{
	HRESULT hr ;

	// Create path geometry
	ID2D1PathGeometry* pathGeometryPtr = nullptr;
	hr = factory->CreatePathGeometry(&pathGeometryPtr);
	if( FAILED(hr) )
	{
		SafeRelease(&pathGeometryPtr);
		throwError("Failed to create path geometry, code 0x%08x", hr);
	}

	// Write to the path geometry using the geometry sink.
	ID2D1GeometrySink *geometrySinkPtr = nullptr;
	hr = pathGeometryPtr->Open(&geometrySinkPtr);
	if( FAILED(hr) )
	{
		SafeRelease(&geometrySinkPtr);
		SafeRelease(&pathGeometryPtr);
		throwError("Failed to open path geometry, code 0x%08x", hr);
	}

	// Use streamstream for parsing
	stringstream ss(data);

	char cmd;
	const char* svgCmds = "mMZzLlHhVvCcSsQqTtAa";
	Point2D cursor;
	Point2D abscur;

	union
	{
		D2D1_BEZIER_SEGMENT cbs;
		D2D1_QUADRATIC_BEZIER_SEGMENT qbs;
	};

	bool isOpen = true;

	while(true)
	{
		char c;
		ss >> c;

		if( ss.eof() )
			break;

		if( strchr(svgCmds, c) != nullptr )
		{
			cmd = c;
		}
		else
		{
			ss.putback(c);
		}

		switch( cmd )
		{
		case 'Z':
		case 'z':
			geometrySinkPtr->EndFigure(D2D1_FIGURE_END_CLOSED);
			isOpen = true;
			break;

		case 'M':
		case 'm':
			if( isOpen )
			{
				cursor = readSvgPoint(ss);
				cbs.point2 = cursor;
				geometrySinkPtr->BeginFigure(cbs.point2, D2D1_FIGURE_BEGIN_FILLED);
				isOpen = false;
				break;
			}
			// Fallthrough when isOpen
		case 'L':
		case 'l':
			cbs.point2 = nextSvgPoint(ss, cursor, cmd, isOpen, true) ;
			geometrySinkPtr->AddLine(cbs.point2);
			break;

		case 'h':
		case 'H':
			cbs.point2 = nextSvgCoordX(ss, cursor, cmd, isOpen) ;
			geometrySinkPtr->AddLine(cbs.point2);
			break;

		case 'v':
		case 'V':
			cbs.point2 = nextSvgCoordY(ss, cursor, cmd, isOpen);
			geometrySinkPtr->AddLine(cbs.point2);
			break;

		case 'C':
		case 'c':
			cbs.point1 = nextSvgPoint(ss, cursor, cmd, isOpen, false);
			cbs.point2 = nextSvgPoint(ss, cursor, cmd, isOpen, false);
			cbs.point3 = nextSvgPoint(ss, cursor, cmd, isOpen, true);
			geometrySinkPtr->AddBezier(cbs);
			break;

		case 'S':
		case 's':
			abscur = cursor;
			cbs.point1 = abscur + (abscur - cbs.point2);
			cbs.point2 = nextSvgPoint(ss, cursor, cmd, isOpen, false);
			cbs.point3 = nextSvgPoint(ss, cursor, cmd, isOpen, true);
			geometrySinkPtr->AddBezier(cbs);
			break;

		case 'Q':
		case 'q':
			qbs.point1 = nextSvgPoint(ss, cursor, cmd, isOpen, false);
			qbs.point2 = nextSvgPoint(ss, cursor, cmd, isOpen, true);
			geometrySinkPtr->AddQuadraticBezier(qbs);
			break;

		case 'T':
		case 't':
			abscur = cursor;
			qbs.point1 = abscur + (abscur - qbs.point2);
			qbs.point2 = nextSvgPoint(ss, cursor, cmd, isOpen, true);
			geometrySinkPtr->AddQuadraticBezier(qbs);
			break;

		case 'A':
		case 'a':
			// (rx ry x-axis-rotation large-arc-flag sweep-flag x y)+	
			// Draws an elliptical arc from the current point to (x, y). 
			// The size and orientation of the ellipse are defined by two radii (rx, ry) and an x-axis-rotation, 
			// which indicates how the ellipse as a whole is rotated relative to the current coordinate system. 
			// The center (cx, cy) of the ellipse is calculated automatically to satisfy the constraints imposed by the other parameters. 
			// large-arc-flag and sweep-flag contribute to the automatic calculations and help determine how the arc is drawn.
			{
				OutputDebugStringA("WARNING: SVG Arcs not supported yet\n");
				D2D1_ARC_SEGMENT arc;
				arc.size.width = (float)readSvgValue(ss,true);
				arc.size.height = (float)readSvgValue(ss,true);
				arc.rotationAngle = (float)readSvgValue(ss,true);
				arc.arcSize = (D2D1_ARC_SIZE)(int)readSvgValue(ss,true);
				arc.sweepDirection = (D2D1_SWEEP_DIRECTION)(int)readSvgValue(ss,true);
				cbs.point2 = arc.point = nextSvgPoint(ss, cursor, cmd, isOpen, true) ;
				geometrySinkPtr->AddArc(arc);
			}
			break;

		default:
			throwError("%c is not a supported SVG command");
			break;
		}
	}

	if( !isOpen )
	{
		geometrySinkPtr->EndFigure(D2D1_FIGURE_END_CLOSED);
	}

	geometrySinkPtr->Close();
	SafeRelease(&geometrySinkPtr);
	return pathGeometryPtr;
}

static ID2D1Geometry* getGeometryOrGroup(ID2D1Factory* factory, std::vector<ID2D1Geometry*>& children)
{
	ID2D1Geometry* groupPtr = nullptr;

	if( !children.empty() )
	{
		if( children.size() == 1 )
		{
			groupPtr = children[0];
		}
		else
		{
			ID2D1GeometryGroup* geometryGroupPtr;
			HRESULT hr = factory->CreateGeometryGroup(D2D1_FILL_MODE_WINDING, &children[0], children.size(), &geometryGroupPtr);
			if( FAILED(hr) )
				throwError("CreateTransformedGeometry failed");

			groupPtr = geometryGroupPtr;
		}
	}

	return groupPtr;
}

// Reads geometry from a tiny XML node.
// Points are transformed by groups into the same space.
static ID2D1Geometry* readSvgFromXml(ID2D1Factory* factory, class TiXmlElement* parent)
{
	std::vector<ID2D1Geometry*> outGeometryArr;

	HRESULT hr;

    for( TiXmlElement* elem = parent->FirstChildElement(); 
        elem != nullptr; 
        elem=elem->NextSiblingElement() )
    {
        const char* elemName = elem->Value();

        if( strcmp(elemName, "g") == 0 )
        {
            // A group
            Matrix2D groupTransform = readTransform(elem);
			ID2D1Geometry* childPtr = readSvgFromXml(factory, elem);

			if( childPtr )
			{
				ID2D1TransformedGeometry* transformedGeometryPtr;
				hr = factory->CreateTransformedGeometry(childPtr, groupTransform, &transformedGeometryPtr);
				if( FAILED(hr) )
					throwError("CreateTransformedGeometry failed");

				outGeometryArr.push_back(transformedGeometryPtr);
			}
        }
        else if( strcmp(elemName, "path") != 0 )
        {
            // Not a path element, recurse
			ID2D1Geometry* childPtr = readSvgFromXml(factory, elem);
			if( childPtr != nullptr )
			{
	            outGeometryArr.push_back(childPtr);
			}
        }
        else
        {
            // Found a path element. Parse it
			Matrix2D pathTransform = readTransform(elem);

            const char* data = elem->Attribute("d");
            if( data == nullptr )
                throwError("Missing data attribute");

			ID2D1PathGeometry* pathGeometryPtr = readSvgPath(factory, data);

			if( pathTransform != Matrix2D() )
			{
				ID2D1TransformedGeometry* transformedGeometryPtr;
				hr = factory->CreateTransformedGeometry(pathGeometryPtr, pathTransform, &transformedGeometryPtr);
				if( FAILED(hr) )
					throwError("CreateTransformedGeometry failed");

				outGeometryArr.push_back(transformedGeometryPtr);
			}
			else
			{
				outGeometryArr.push_back(pathGeometryPtr);
			}
        }
    }
	
	ID2D1Geometry* groupPtr = getGeometryOrGroup(factory, outGeometryArr);
	return groupPtr;
}

ID2D1Geometry* loadGeometryFromSvgFile(ID2D1Factory* factoryPtr, const char* svgFilePath)
{
    try
    {
        TiXmlDocument doc(svgFilePath);

        if( !doc.LoadFile() )
            throwError("%s could not be loaded. Does it exist?", svgFilePath);

        ID2D1Geometry* geometryPtr = readSvgFromXml(factoryPtr, doc.RootElement());
		return geometryPtr;
    }

    catch( std::exception& ex )
    {
        OutputDebugStringA("Exception while loading geometry from file ");
        OutputDebugStringA(svgFilePath);
        OutputDebugStringA("\n");
        OutputDebugStringA(ex.what());
        OutputDebugStringA("\n");

        char caption[1000];
        sprintf_s(caption, "Failed to load %s", svgFilePath);
        MessageBoxA(NULL, ex.what(), caption, MB_ICONERROR);

        exit(-1);
    }
}
