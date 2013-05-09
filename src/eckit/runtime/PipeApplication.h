/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file PipeApplication.h
/// @author Baudouin Raoult

#ifndef eckit_PipeApplication_h
#define eckit_PipeApplication_h

#include "eckit/runtime/Application.h"
#include "eckit/serialisation/Stream.h"
#include "eckit/config/Resource.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class PipeApplication : public Application {

public: // methods

	PipeApplication(int,char**);

	~PipeApplication();

	virtual void process(Stream&) = 0;
	virtual void endBatch();
    virtual void init(Stream&);
	virtual void waiting();	
	
	static void launch(const string& name, int in, int out);

private: // members

	Resource<long> in_;
	Resource<long> out_;

    /// overridden from Application
	virtual void run();

};

//-----------------------------------------------------------------------------

} // namespace eckit

#endif
