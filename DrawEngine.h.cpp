#include "stdafx.h"
#include "DrawEngine.h"

static void MyFilledCircle(Mat img, int rd, Point center)
{
	circle(img,
		center,
		rd / 128,
		Scalar(0, 255, 0),
		FILLED,
		LINE_8);
}

static void MyLine(Mat img, Point start, Point end)
{
	int thickness = 2;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(0, 0, 255),
		thickness,
		lineType);
}

void Draw::drawingEnviroment(FileOperation& objectsContainer)
{
	room.X = objectsContainer.surroundingObjects.roomSize.X;
	room.Y = objectsContainer.surroundingObjects.roomSize.Y;
	dotView.X = objectsContainer.surroundingObjects.viewer.X;
	dotView.Y = objectsContainer.surroundingObjects.viewer.Y;
	char rook_window[] = "Drawing 2: Rook";
	Mat rook_image = Mat::zeros(room.X, room.X, CV_8UC3);

	//plase the Viewer on his/her spot :)
	MyFilledCircle(rook_image, room.X, Point(dotView.X, dotView.Y));

	//rotate polygons from matrix of Poligons
	for (size_t i = 0; i < objectsContainer.surroundingObjects.matrixOfPoligons.size(); i++)
	{
		//rotate thought the Dots to see what shape will be build
		for (size_t j = 0; j < objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots.size(); j++)
		{
			//ring loop to draw polygon line by line
			if (objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots.size() ==
				(j + 1))
			{
				MyLine(rook_image
					, Point(objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[j].X
						, objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[j].Y)
					, Point(objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[0].X
						, objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[0].Y));
			}
			else {
				MyLine(rook_image
					, Point(objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[j].X
						, objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[j].Y)
					, Point(objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[j + 1].X
						, objectsContainer.surroundingObjects.matrixOfPoligons[i].poligonDots[j + 1].Y));

			}
		}

	}

	imshow(rook_window, rook_image);
	moveWindow(rook_window, 0, 0);
	waitKey(0);
	destroyAllWindows();
}