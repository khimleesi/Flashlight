#pragma once

/*******************************************************************************************************************
	FileManager.h, FileManager.cpp
	Created by Kim Kane
	Last updated: 04/01/2018

	*This class is still under construction.
	Singleton class that creates an fstream object and reads in data from files.

*******************************************************************************************************************/
#include <fstream>
#include <string>
#include <map>

#include "Singleton.h"

namespace file_constants {

	enum OffsetType { TYPE_BEGIN_OFFSET = 8, DIVIDER_OFFSET = 1, VERTICES_OFFSET = 2,
					  TEXTURE_COORDS_OFFSET = 3, NORMALS_OFFSET = 3, FACES_OFFSET = 2, };

	const std::string LINE_BREAK		= "-";
	const std::string DIVIDER			= ":";
	const std::string TYPE_BEGIN		= "TYPE >>";
	const std::string TYPE_END			= "----------END----------";
	const std::string END_OF_FILE		= "<";
	const std::string VERTICES			= "v ";
	const std::string TEXTURE_COORDS	= "vt ";
	const std::string NORMALS			= "vn ";
	const std::string FACES				= "f ";
}

class FileManager {

public:
	friend class Singleton<FileManager>;

public:
	bool OpenForReading(const std::string& fileLocation);
	bool BinaryOpenForReading(const std::string& fileLocation);
	bool BinaryOpenForWriting(const std::string& fileLocation);
	void Close(const std::string& fileLocation);
	std::istream& ExtractFileData();

public:
	int					GetFileSize();
	size_t				Find(const std::string& data);
	size_t				FileDataContains(const std::string& data);
	std::string			CutFileData(int startPosition = 0, int endPosition = std::string::npos);
	const std::istream& Seek(int startPosition, int endPosition);

public:
	bool GetObjectData(const std::string& objectName, std::map<std::string, std::map<std::string, std::string>>& container);
	void PrintObjectData();

private:
	FileManager();
	FileManager(const FileManager&)				= delete;
	FileManager& operator=(const FileManager&)	= delete;

private:
	std::fstream	m_file;
	std::string		m_fileData;

private:
	std::map<std::string, std::map<std::string, std::string>> m_objectData;
};

typedef Singleton<FileManager> File;