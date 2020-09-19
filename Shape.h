#pragma once
#include <map>
#include <vector>

enum roomSpase{poligon, viewer, room};

struct ShadowOverlapAngle {
	double Z;
	double W;

	double zRaper;
};

/* ========== The following abstractions build an object from Cartesian points to panorama viewed angles ========== */
struct _Point {
	int X;
	int Y;
	double dotRad;
	double theta;
};

struct Poligon
{
	double rad; //Average distance to the Poligon

	double A; // Form MIN angle view
	double B; // Form MAX angle view
	bool isVisible; //Indicator tells if the object is visible
	std::vector<_Point> poligonDots; //separate point values, which forms separate Polygon
};

struct PoligonMatrix
{
	_Point roomSize;
	_Point viewer; //where is the plase of the Viewer
	std::vector<Poligon>  matrixOfPoligons; //matrix of polygons which could help us to determine what's ..
											//.. the distance to every object and what's it's viewed angle from the Viewer
};