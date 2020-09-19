#ifndef _CSV_CONTAINER_
#define _CSV_CONTAINER_

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

class CSVcontainer
{
public:
	CSVcontainer();
	CSVcontainer(char* path);
	~CSVcontainer();
	std::string const& operator[](size_t index) const;

	size_t size() const;

	void readCSVRows();
	void chunckCSVRead();

	bool isCSV_FileOpened();

private:
	bool fileStatus;
	std::ifstream       file;
	std::vector<std::string>    m_data;

	int countCSVRows;
	map< int, vector<string> > csvMapContainer;
	void diagCsvContainer(char* fileName);

	std::istream& safeGetline(std::istream& is, std::string& t);
};

#endif