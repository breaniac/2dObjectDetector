#include "stdafx.h"
#include "CSV_Container.h"

CSVcontainer::CSVcontainer():fileStatus(false), file(""), countCSVRows(1)
{
}

CSVcontainer::CSVcontainer(char* path)
{
	countCSVRows = 1;
	file.open(path, ofstream::in | ofstream::binary);

	if (!file.is_open())
		fileStatus = false;
	else
		fileStatus = true;
}

CSVcontainer::~CSVcontainer()
{
	file.close();
}


std::string const& CSVcontainer::operator[](size_t index) const
{
	return m_data[index];
}


size_t CSVcontainer::size() const
{
	return m_data.size();
}

void CSVcontainer::diagCsvContainer(char* fileName)
{
	std::ofstream outfile (fileName ,std::ofstream::binary);
	for (std::map<int , vector<string> >::iterator it=csvMapContainer.begin(); it!=csvMapContainer.end(); ++it)
	{
		for (std::vector<string>::const_iterator i = it->second.begin();i != it->second.end(); ++i)
		{
			outfile << *i;
			if(i < (it->second.end() - 1))
				outfile<< ",";
		}
		outfile << "\r";
	}
	outfile.close();
}


std::istream& CSVcontainer::safeGetline(std::istream& is, std::string& t)
{
		t.erase(t.begin(), t.end());
		//t.clear();

		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for(;;) {
			int c = sb->sbumpc();
			switch (c) {
			case '\n':
				return is;
			case '\r':
				if(sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if(t.empty())
					is.setstate(std::ios::eofbit);
				return is;
			default:
				t += (char)c;
			}
		}

}

void CSVcontainer::readCSVRows()
{
	std::string         line;
	//std::getline(str, line);


	std::string         cell;

	csvMapContainer.clear();

	while (safeGetline(file, line))
	{
		{
			std::stringstream   lineStream(line);

			while (std::getline(lineStream, cell, ','))
			{
				csvMapContainer[countCSVRows].push_back(cell);
			}
			countCSVRows++;
		}
	}

	//m_data.clear();

	diagCsvContainer("readCSVRows.txt");
}


void CSVcontainer::chunckCSVRead()
{

  	ofstream debugFile;
	// open file for appending (creates file if it does not exist)
	debugFile.open("debug.txt", ios::out | ios::app);

	// check if the file to read from exists and if so read the file in chunks
	//ifstream ifile(logFilePath, std::ifstream::binary);
	if (file.good())
	{
		const int BUFFER_SIZE = 1024;
		std::vector<string> buffer;

		debugFile << "test.csv" << " exists!" << endl;

		std::string         line;
		std::string         cell;

		std::string			buffString = "";

		int cnt = 0;

		while(1)
		{
		
			while (safeGetline(file, line))
			{
				if (cnt < BUFFER_SIZE)
				{
					buffString += line;
					buffString += "\n";
					cnt++;
				}
				else
				{
					debugFile << buffString;
					debugFile << "/***** CHUNK ********/ \r\n";
					buffString = "";
					cnt = 0;
				}
			}

			debugFile << buffString;
			break;
		}
	}
	else
	{
	}
}

bool CSVcontainer::isCSV_FileOpened()
{
	return fileStatus;
}

