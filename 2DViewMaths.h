#pragma once
#include <iostream>
#include <map>
#include <vector>

#include "FileOperation.h"

using namespace std;

class PanoramaView
{
private:

	struct ShadowOverlapAngle {
		double Z;
		double W;
		double zRaper;
	};

public:
	PanoramaView() {};
	~PanoramaView() {};

	void perspective(FileOperation& entrData);
};

