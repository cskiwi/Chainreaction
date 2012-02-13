#pragma once

#include "Vector2D.h"
#include "Point2D.h"
#include "Rect2D.h"

// A 2D matrix class
// 
// A matrix holds the coordinates of a coordinate matrix.
//
// NOTE: This class is designed for math educational purposes only. 
// The matrix class of a real game engine would use all kinds of optimization tricks,
// but this would make the code harder to read. However, a good C++ compiler will usually 
// optimize the hell out of the code anyway, so in practice, this design is not as bad as it looks.
//
struct Matrix2D
{
	// The first matrix vector (the "x-axis")
	Vector2D dirX;

	// The second matrix vector (the "y-axis")
	Vector2D dirY;

	// The origin of  the coordinate matrix (the "translation")
	Point2D  orig;

	// Default constructor gives a standard matrix
	Matrix2D(Vector2D dirX=Vector2D(1,0), Vector2D dirY=Vector2D(0,1), Point2D orig=Point2D(0,0)) 
		: dirX(dirX), dirY(dirY), orig(orig) {}

	// Constructor, origin as first required argument.
	Matrix2D(Point2D orig, Vector2D dirX=Vector2D(1,0), Vector2D dirY=Vector2D(0,1)) 
		: dirX(dirX), dirY(dirY), orig(orig) {}

	// Constructor, using scalars, all required
	Matrix2D(Scalar e1X, Scalar e1Y, Scalar e2X, Scalar e2Y, Scalar oX, Scalar oY)
		:dirX(e1X,e1Y)
		,dirY(e2X,e2Y)
		,orig(oX,oY)
	{
	}

	// Copy constructor
	// Usage: Matrix2D e(f);
	//   where f is a matrix
	Matrix2D(const Matrix2D& source) 
		: dirX(source.dirX), dirY(source.dirY), orig(source.orig) {}

	// Transform a vector by this matrix.
	// The coordinates of the vector are assumed to be relative to this matrix
	// Usage: Vector2D w = b.Transform(v);
	//   where b is a matrix and v is a vector
	Vector2D Transform(Vector2D v) const
	{
		return v.x * dirX + v.y * dirY;
	}

	// Untransform a vector by this matrix.
	// This computes the coordinates of (v) relative to (b)
	// Usage: Vector2D w = b.Untransform(v);
	//   where b is a matrix and v is a vector
	Vector2D Untransform(Vector2D v) const
	{
		Scalar x = v.Det(dirY);
		Scalar y = dirX.Det(v);
		Scalar d = dirX.Det(dirY);
		return Vector2D(x/d,y/d);
	}

	// Transform a point by this matrix.
	// The coordinates of the point are assumed to be relative to this matrix
	// Usage: Point2D q = f.Transform(p);
	//   where f is a matrix and p is a Point2D
	Point2D Transform(Point2D p) const
	{
		Vector2D v = Transform(p - Point2D(0,0));
		return orig + v;
	}

	// Untransform a point by this matrix.
	// This computes the coordinates of (p) relative to this matrix
	// Usage: Point2D q = f.Untransform(p);
	//   where f is a basis and p is a point
	Point2D Untransform(Point2D p) const
	{
		Vector2D v = Untransform(p-orig);
		return Point2D(0,0) + v;
	}

	// Transform a vector by a matrix (operator form)
	// Usage: Vector2D w = v * b;
	//   where b is a matrix and v is a vector
	friend Vector2D operator* (Vector2D v, const Matrix2D& b) { return b.Transform(v); }

	// Untransform a vector by a matrix (operator form)
	// Usage: Vector2D w = v / b;
	//   where b is a matrix and v is a vector
	friend Vector2D operator/ (Vector2D v, const Matrix2D& b) { return b.Untransform(v); }

	// Transform a point by a matrix (operator form)
	// Usage: Point2D q = p * f;
	//   where f is a matrix and p is a point
	friend Point2D operator* (Point2D p, const Matrix2D& f) { return f.Transform(p); }

	// Untransform a point by a matrix (operator form)
	// Usage: Point2D q = p / f;
	//   where f is a matrix and p is a point
	friend Point2D operator/ (Point2D p, const Matrix2D& f) { return f.Untransform(p); }

	// Transform a matrix by this matrix.
	// The coordinates of the matrix to be transformed 
	// are assumed to be relative to this matrix
	// Usage: Matrix2D g = e.Transform(f);
	//   where e and f are frames
	Matrix2D Transform(const Matrix2D& f) const
	{
		return Matrix2D( Transform(f.dirX), Transform(f.dirY), Transform(f.orig) );
	}

	// Untransform a matrix by this matrix.
	// Usage: Matrix2D g = e.Untransform(f);
	//   where e and f are frames
	Matrix2D Untransform(const Matrix2D& f) const
	{
		return Matrix2D( Untransform(f.dirX), Untransform(f.dirY), Untransform(f.orig) );
	}

	// Transform matrix f1 by matrix f2 (operator form)
	friend Matrix2D operator* (const Matrix2D& f1, const Matrix2D& f2) { return f2.Transform(f1); }

	// Untransform matrix f1 by matrix f2 (operator form)
	friend Matrix2D operator/ (const Matrix2D& f1, const Matrix2D& f2) { return f2.Untransform(f1); }

	// The inverse matrix
	friend Matrix2D inverse(const Matrix2D& f) { return f.Untransform(Matrix2D()); }
	Matrix2D Inverse() const { return inverse(*this); }

	// Are two matrices equal within a threshold?
	// Usage: p.Equals(q)
	//   where p and q are matrices
	bool Equals(Matrix2D p, Scalar epsilon=0.001)
	{
		return dirX.Equals(p.dirX) && dirY.Equals(p.dirY) && orig.Equals(p.orig);
	}

	// Are two matrices exactly equal?
	// Usage: p == q
	//   where p and q are matrices
	friend bool operator == (Matrix2D p, Matrix2D q)
	{
		return p.dirX==q.dirX && p.dirY==q.dirY && p.orig==q.orig;
	}

	// Are two matrices exactly unequal?
	// Usage: p != q
	//   where p and q are matrices
	friend bool operator != (Matrix2D p, Matrix2D q)
	{
		return p.dirX!=q.dirX || p.dirY!=q.dirY || p.orig!=q.orig;
	}

	// Convert to Direct2D
	operator Matrix3x2F () const
	{
		Matrix3x2F mat;
		mat._11 = (FLOAT)dirX.x;
		mat._12 = (FLOAT)dirX.y;
		mat._21 = (FLOAT)dirY.x;
		mat._22 = (FLOAT)dirY.y;
		mat._31 = (FLOAT)orig.x;
		mat._32 = (FLOAT)orig.y;
		return mat;
	}

	// Convert from Direct2D
	Matrix2D(const Matrix3x2F& mat)
	{
		dirX.x = mat._11;
		dirX.y = mat._12;
		dirY.x = mat._21;
		dirY.y = mat._21;
		orig.x = mat._31;
		orig.y = mat._31;
	}

	// Zet matrix om naar string.
	String ToString();

	// Print to a stream.
	friend std::ostream& operator<<(std::ostream& os, const Matrix2D& m) 
	{
		os << "Matrix2D( " 
			<< m.dirX.x << ", " << m.dirX.y << ", " 
			<< m.dirY.x << ", " << m.dirY.y << ", " 
			<< m.orig.x << ", " << m.orig.y << " )";
		return os;
	}


	//converts this matrix into a Rotate matrix
	//Voorbeeld:
	//Matrix mat;
	//mat.SetAsRotate(0.2);
	void SetAsRotate(double radians);

	//converts this matrix into a Translation matrix
	//Voorbeeld converts mat matrix into a matrix that translates a vector by (5,8):
	//Matrix mat;
	//mat.SetAsTranslate(5,8);
	void SetAsTranslate(double tx, double ty);

	//converts this matrix into a Translation matrix
	//Voorbeeld converts mat matrix into a matrix that translates a vector by (5,8):
	//Matrix mat;
	//Point2D pt(5,8);
	//mat.SetAsTranslate(pt);
	void SetAsTranslate(Point2D pt);

	//converts this matrix into a Scale matrix
	//Voorbeeld converts mat matrix into a matrix that Scales a vector by (2,3):
	//Matrix mat;
	//mat.SetAsScale(2,3);
	void SetAsScale(double sx, double sy);

	//converts this matrix into a Scale matrix
	//Voorbeeld converts mat matrix into a matrix that Scales a vector by (2,3):
	//Matrix mat;
	//mat.SetAsScale(2,3);
	void SetAsScale(double s);

	// Een rotatie matrix.
	//
	// Voorbeeld: Matrix r = Matrix::Rotation(M_PI/2);
	static Matrix2D Rotation(double radians);



   // De matrix met dirX=(1,0) dirY=(0,1) orig=(0,0)
   //   
   // Voorbeeld: Matrix2D m = Matrix2D::Identity()();
	static Matrix2D Identity();
	// Een uniforme schaal matrix.
	//
	// Voorbeeld: Matrix s = Matrix::Scaling(2);
	static Matrix2D Scaling(double scale);

	// Een niet-uniforme schaal matrix.
	//
	// Voorbeeld: Matrix s = Matrix::Scaling(2,-3);
	static Matrix2D Scaling(double scaleX, double scaleY);

	// Een niet-uniforme schaal matrix.
	//
	// Voorbeeld: Matrix s = Matrix::Scaling( Vector2D(2,-3) );
	static Matrix2D Scaling(Vector2D scaleXY);

	// Voorbeeld: Matrix t = Matrix::Translation( Point2(2,3) );
	static Matrix2D Translation(Point2D origin);

	// Voorbeeld: Matrix t = Matrix::Translation(2,3);
	static Matrix2D Translation(double tx, double ty);

	// Voorbeeld: Matrix t = Matrix::Translation( Vector2(2,3) );
	static Matrix2D Translation(Vector2D t);

	// Voorbeeld: Matrix o = Matrix::Orientation( Vector2D(1,2), Vector2D(3,4) );
	static Matrix2D Orientation(Vector2D dirX, Vector2D dirY);

};