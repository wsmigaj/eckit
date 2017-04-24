/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/exception/Exceptions.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/mpi/BroadcastFile.h"
#include "eckit/mpi/mpi.h"

//-----------------------------------------------------------------------------

namespace eckit {
namespace mpi {

//-----------------------------------------------------------------------------


bool broadcast_file( const PathName& p, std::ostream& out, const int root, const mpi::Comm& comm )
{
  ASSERT( static_cast<size_t>(root) < comm.size() );
  char* buf;
  int buf_len(0);
  enum { ERROR_CANT_OPEN_FILE = -1 };

  if( comm.rank() == static_cast<size_t>(root) )
  {
    if( p.exists() )
    {
      std::ifstream in;
      in.open ( p.asString().c_str() );
      if (!in)
      {
        buf_len = ERROR_CANT_OPEN_FILE;
        MPI_Bcast(&buf_len,1,eckit::mpi::datatype<int >(),root,comm);
      }
      else
      {
        std::stringstream stream;
        stream << in.rdbuf();
        in.close();
        std::string str = stream.str();
        buf = const_cast<char*>(str.c_str());
        buf_len = str.size();
        MPI_Bcast(&buf_len,1,eckit::mpi::datatype<int >(),root,comm);
        if (buf_len > 0)
        {
          MPI_Bcast(buf,buf_len,eckit::mpi::datatype<char>(),root,comm);
          out.write(buf,buf_len);
        }
      }
    }
    else
    {
      MPI_Bcast(&buf_len,1,eckit::mpi::datatype<int>(),root,comm);
    }
  }
  else
  {
    MPI_Bcast(&buf_len,1,eckit::mpi::datatype<int>(),root,comm);
    if( buf_len > 0 )
    {
      buf = new char[buf_len];
      MPI_Bcast(buf,buf_len,eckit::mpi::datatype<char>(),root,comm);
      out.write(buf,buf_len);
      delete[] buf;
    }
  }

  if( buf_len == ERROR_CANT_OPEN_FILE )
    throw CantOpenFile(p.asString(),Here());

  return ( buf_len > 0 );
}

//-----------------------------------------------------------------------------

} // namespace mpi
} // namespace eckit
