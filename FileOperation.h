#pragma once
//#pragma warning(disable : 4996)
//#pragma warning(disable: 4786)
//#include <string>
//#include <windows.h>
//#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <math.h> 
#include <vector>
#include <map>

// Custom interface(s)
#include "Shape.h"


using namespace std;

class FileOperation 
{
private:
//===  atributes which help us to read the file correctly
	ifstream fileToAnalize;
	int countCSVRows;
	map< int, vector<string> > csvMapContainer;
	/* functions */
	void readCSVRows(ifstream& file);	//void DoProgressBar(char label[], int step, int total);
	std::istream& safeGetline(std::istream& is, std::string& t);

//=== abstract view of the objects, based of the data reading from the file
	PoligonMatrix surroundingObjects;
	/* functions */
	void buildObjects(string xC, string yC, roomSpase sw, Poligon* obj);
	void upgradePoligon(Poligon* obj);
	void detailledDotInfo(_Point* dot);

//set of poligons from which we coudl determine every object is visible or not
	vector<Poligon> result;

public:

	FileOperation();
	~FileOperation();

	void howManyObjectAreVisible(void);
	bool buildPoligons(void);
	bool readTemplateFile(const char * pathFile);

	friend class  PanoramaView;
	friend class  Draw;
};

