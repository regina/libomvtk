//
// ArchiveStrategy.h
//
// $Id: //poco/1.3/Foundation/include/Poco/ArchiveStrategy.h#2 $
//
// Library: Foundation
// Package: Logging
// Module:  FileChannel
//
// Definition of the ArchiveStrategy class and subclasses.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_ArchiveStrategy_INCLUDED
#define Foundation_ArchiveStrategy_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/LogFile.h"
#include "Poco/File.h"
#include "Poco/DateTimeFormatter.h"


namespace Poco {


class ArchiveCompressor;


class Foundation_API ArchiveStrategy
	/// The ArchiveStrategy is used by FileChannel 
	/// to rename a rotated log file for archiving.
	///
	/// Archived files can be automatically compressed,
	/// using the gzip file format.
{
public:
	ArchiveStrategy();
	virtual ~ArchiveStrategy();

	virtual LogFile* archive(LogFile* pFile) = 0;
		/// Renames the given log file for archiving
		/// and creates and returns a new log file.
		/// The given LogFile object is deleted.

	void compress(bool flag = true);
		/// Enables or disables compression of archived files.	

protected:
	void moveFile(const std::string& oldName, const std::string& newName);
	bool exists(const std::string& name);
	
private:
	ArchiveStrategy(const ArchiveStrategy&);
	ArchiveStrategy& operator = (const ArchiveStrategy&);
	
	bool _compress;
	ArchiveCompressor* _pCompressor;
};


class Foundation_API ArchiveByNumberStrategy: public ArchiveStrategy
	/// A monotonic increasing number is appended to the
	/// log file name. The most recent archived file
	/// always has the number zero.
{
public:
	ArchiveByNumberStrategy();
	~ArchiveByNumberStrategy();
	LogFile* archive(LogFile* pFile);
};


template <class DT>
class ArchiveByTimestampStrategy: public ArchiveStrategy
	/// A timestamp (YYYYMMDDhhmmss) is appended to archived
	/// log files.
{
public:
	ArchiveByTimestampStrategy()
	{
	}
	
	~ArchiveByTimestampStrategy()
	{
	}
	
	LogFile* archive(LogFile* pFile)
	{
		std::string path = pFile->path();
		delete pFile;
		std::string archPath = path;
		archPath.append(".");
		archPath.append(DateTimeFormatter::format(DT().timestamp(), "%Y%m%d%H%M%S"));
		moveFile(path, archPath);
		return new LogFile(path);
	}
};


} // namespace Poco


#endif // Foundation_ArchiveStrategy_INCLUDED
