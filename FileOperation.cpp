#include "stdafx.h"
//#include "CDS_Atribute_Fields.h"
#include "FileOperation.h"

//hiden function for "FileOperation"
static void coordinateDispatcher(string& _X, string& _Y, string _strSpaces)
{
	bool swtch = false;
	char buff[8] = { 0 };
	const char* strSpaces = _strSpaces.c_str();
	int j = 0;

	for (size_t i = 0; i < _strSpaces.size(); i++)
	{
		if (strSpaces[i] >= '0' && strSpaces[i] <= '9')
		{
			if (!swtch)
				buff[j] = strSpaces[i];
			else
				buff[j] = strSpaces[i];

			j++;
		}
		else if (strSpaces[i] == ',' || strSpaces[i] == '.')
		{
			swtch = true;
			j = 0;

			string tmp(buff);
			_X = tmp;
			memset(buff, 0, sizeof(buff));
		}
	}

	string tmp(buff);
	_Y = tmp;
	memset(buff, 0, sizeof(buff));

}

FileOperation::FileOperation() : countCSVRows(0)
{
}

FileOperation::~FileOperation()
{
}

std::istream& FileOperation::safeGetline(std::istream& is, std::string& t)
{
	t.erase(t.begin(), t.end());

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}

void FileOperation::detailledDotInfo(_Point* dot)
{
	double xDist = (double)dot->X - (double)surroundingObjects.viewer.X;
	double yDist = (double)dot->Y - (double)surroundingObjects.viewer.Y;

	double r;
	double theta;

	r = sqrt((pow(xDist, 2)) + (pow(yDist, 2)));
	theta = atan2(yDist, xDist);
	theta = (theta * 180) / 3.141592654;
	if (theta < 0.0)
		theta += 360;

	dot->theta = theta;
	dot->dotRad = r;
}

void FileOperation::howManyObjectAreVisible(void)
{
	vector<Poligon> printObjects;
	for (size_t i = 0; i < result.size(); i++)
	{
		if (result[i].isVisible)
			printObjects.push_back(result[i]);
	}

	cout << "Visible objects are: " << printObjects.size() << endl;
	cout << "===============================" << endl;
	for (size_t i = 0; i < printObjects.size(); i++)
	{
		cout << "---- Object " << i+1 << " ----" << endl;
		for (size_t j = 0; j < printObjects[i].poligonDots.size(); j++)
			cout << "Dot_" << j + 1 << " X:" << printObjects[i].poligonDots[j].X << " Y:" << printObjects[i].poligonDots[j].Y << endl;
		cout << endl;
	}

}

//let's determine where is the Viewer and what types are the polygons
void FileOperation::buildObjects(string xC, string yC, roomSpase sw, Poligon *obj)
{
	int i_tmp_X;
	int i_tmp_Y;

	i_tmp_X = stoi(xC);
	i_tmp_Y = stoi(yC);

	switch (sw)
	{
		case room:
		surroundingObjects.roomSize.X = i_tmp_X;
		surroundingObjects.roomSize.Y = i_tmp_Y;

		break;

		case viewer:
			surroundingObjects.viewer.X = i_tmp_X;
			surroundingObjects.viewer.Y = i_tmp_Y;

			break;

		case poligon:
			_Point sepDot;
			sepDot.X = i_tmp_X;
			sepDot.Y = i_tmp_Y;

			detailledDotInfo(&sepDot);
			obj->poligonDots.push_back(sepDot);

			break;

	}
}


static void selectMIN_viewAngle(Poligon* poligonAlpha)
{
	int i=0, j, min_idx;
	size_t maxDOts = poligonAlpha->poligonDots.size();

	min_idx = i;
	for (j = i + 1; j < maxDOts; j++)
		if (poligonAlpha->poligonDots[j].theta < poligonAlpha->poligonDots[min_idx].theta)
			min_idx = j;

	poligonAlpha->A = poligonAlpha->poligonDots[min_idx].theta;
}

static void selectMAX_viewAngle(Poligon* poligonAlpha)
{
	int i = 0, j, max_idx;
	size_t maxDOts = poligonAlpha->poligonDots.size();

	max_idx = i;
	for (j = i + 1; j < maxDOts; j++)
		if (poligonAlpha->poligonDots[j].theta > poligonAlpha->poligonDots[max_idx].theta)
			max_idx = j;

	poligonAlpha->B = poligonAlpha->poligonDots[max_idx].theta;
}

//Update the rest of information of "Polygon" type
void FileOperation::upgradePoligon(Poligon *obj)
{
	obj->rad = 0;
	obj->A = 0;
	obj->B = 0;

	obj->isVisible = true;

//	{
		for (size_t i = 0; i < obj->poligonDots.size(); i++)
		{
			obj->rad += obj->poligonDots[i].dotRad;

		}

		// avarage rad distanse with help us to determine is behind or in front of a n other object
		obj->rad /= obj->poligonDots.size();
//	}

	selectMIN_viewAngle(obj);
	selectMAX_viewAngle(obj);

}

bool FileOperation::buildPoligons(void)
{
	string x_Coord = "";
	string y_Coord = "";

	map<double, Poligon> sortObjects;
	map< int, vector<string> >::iterator it;

	int j = 0;
	for(it = csvMapContainer.begin() ; it != csvMapContainer.end(); j++, it++)
		// Find coordinates of the Viewer, as an entry point of our calculations ...
		if (j == 0)
		{
			vector<string> tmp = it->second;
			coordinateDispatcher(x_Coord, y_Coord, tmp[0]);
			buildObjects(x_Coord, y_Coord, room, NULL);
		}
		else if (j == 1)
		{
			vector<string> tmp = it->second;
			coordinateDispatcher(x_Coord, y_Coord, tmp[0]);
			buildObjects(x_Coord, y_Coord, viewer, NULL);
		}
		// ... then build every poligon in the room
		else if (j > 2)
		{
			vector<string> tmp = it->second;
			Poligon poligonObj;
			for (size_t i = 0; i < tmp.size(); i++) // Build specific poligon
			{
				coordinateDispatcher(x_Coord, y_Coord, tmp[i] );
				buildObjects(x_Coord, y_Coord, poligon, &poligonObj);

			}
			//fill other parameters of the Poligon object
			upgradePoligon(&poligonObj);

			// let's to sort them 
			sortObjects.insert(std::pair<double, Poligon>(poligonObj.rad, poligonObj));
		}

	// at the end let's fill matrixOfPoligons with ordered Poligons list
	for (auto& x : sortObjects) {
		surroundingObjects.matrixOfPoligons.push_back(x.second);
	}

	return true;
}

//Read row by row the opened file and update "csvMapContainer"
void FileOperation::readCSVRows(ifstream& file)
{
	std::string         line;
	std::string         cell;
	csvMapContainer.clear();
	while (safeGetline(file, line))
	{
		{
			std::stringstream   lineStream(line);

			while (std::getline(lineStream, cell, ' '))
			{
				csvMapContainer[countCSVRows].push_back(cell);
			}


			countCSVRows++;
		}
	}
}

/*==========================================================================================================================*/
// Basic function before start actual reading
bool FileOperation::readTemplateFile(const char* pathFile)
{
	fileToAnalize.open(pathFile, ifstream::in | ifstream::binary);

	if (!fileToAnalize.is_open())
		return false;
	else
	{
		readCSVRows(fileToAnalize);

		fileToAnalize.close();
		return true;
	}
}

