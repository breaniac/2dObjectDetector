// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <complex>
#include <math.h>

// Custom interface
#include "FileOperation.h"
#include "2DViewMaths.h"
#include "DrawEngine.h"

#ifndef _DEBUG
#define ACTION_PARAMETER	(1)
#define IN_FILE				(2)

const string framework =
"\n\n\
============================ Usage ver1.0 (Base) =======================\n\
Developer: Sergey Mihaylov                                               \n\
************************************************************************\n\
<> - Mandatory params                                                    \n\
[] - Optional params                                                     \n\
                                                                         \n\
   GDS_Generator.exe <Action Parameter> <Input File> 		          \n\n\
<Action Parameter> - there are two options:                             \n\
   '-r' shows dots coordinates of visible object and how many are they \n\
   '-d' displays objects in 2d viewed room                             \n\
<Input File>       - any file with extension .txt					    \n\n\
-------------------------------- Example -------------------------------\n\
   ExampleRoomTest.exe -r in.txt			                            \n\
========================================================================\n\
";
#endif

using namespace std;

#ifdef _DEBUG
int main()
{
	FileOperation obj;
	obj.readTemplateFile("in.txt");
	obj.buildPoligons();

	PanoramaView viewMaths;
	viewMaths.perspective(obj);

	obj.howManyObjectAreVisible();

	Draw formView;
	formView.drawingEnviroment(obj);

	system("PAUSE");
	return 0;
}
#else
int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		cout << framework << endl;
	}
	else if (argc > 1 && argc < 3)
		cout << "Missing arguments" << endl;
	else if (argc > 4)
		cout << "Too much arguments" << endl;
	else
	{
		if (!strcmp(argv[ACTION_PARAMETER], "-r") ||
			!strcmp(argv[ACTION_PARAMETER], "-d"))
		{
			FileOperation obj;
			obj.readTemplateFile(argv[IN_FILE]);
			obj.buildPoligons();

			PanoramaView viewMaths;
			viewMaths.perspective(obj);
			if (!strcmp(argv[ACTION_PARAMETER], "-d"))
			{
				Draw formView;
				formView.drawingEnviroment(obj);
			}
			else
				obj.howManyObjectAreVisible();

		}
		else
			cout << "Invalid action parameter" << endl;
	}

	return 0;
	//system("PAUSE");
}
#endif


