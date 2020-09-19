#pragma once
//#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include"Shape.h"
#include "FileOperation.h"

using namespace cv;
class Draw {
private:
	_Point room;
	_Point dotView;
public:

	Draw() {};
	~Draw() {};

	void drawingEnviroment(FileOperation& objectsContainer);
};