/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/Exceptions.h"
#include "eclib/StdPipe.h"


//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

StdPipe::StdPipe(const string& name,const string& mode)
{
	file_ = popen(name.c_str(),mode.c_str());

	if(file_ == 0)
		throw CantOpenFile(name);
}

StdPipe::~StdPipe() 
{ 
	if(file_) 
		if(pclose(file_) == -1)
			throw FailedSystemCall("pclose");
}

//-----------------------------------------------------------------------------

} // namespace eckit

