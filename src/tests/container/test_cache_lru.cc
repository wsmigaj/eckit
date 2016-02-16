/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <string>

#include "ecbuild/boost_test_framework.h"

#include "eckit/container/CacheLRU.h"
#include "eckit/exception/Exceptions.h"

//----------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( test_cache_lru )

BOOST_AUTO_TEST_CASE( test_cache_lru_basic )
{
	eckit::CacheLRU<std::string,size_t> cache(3);

	BOOST_CHECK_EQUAL( cache.size() , 0 );

	BOOST_CHECK_NO_THROW( cache.insert("ddd", 40) );
	BOOST_CHECK_NO_THROW( cache.insert("aaa", 10) );

	BOOST_CHECK( cache.exists("ddd") );
	BOOST_CHECK( cache.exists("aaa") );

	BOOST_CHECK_EQUAL( cache.access("aaa") , 10 );
	BOOST_CHECK_EQUAL( cache.access("ddd") , 40 );

	BOOST_CHECK_NO_THROW( cache.insert("ccc", 30) );
	BOOST_CHECK_NO_THROW( cache.insert("eee", 50) );
	BOOST_CHECK_NO_THROW( cache.insert("bbb", 20) );

	BOOST_CHECK( cache.exists("bbb") );
	BOOST_CHECK( cache.exists("ccc") );
	BOOST_CHECK( cache.exists("eee") );

	BOOST_CHECK_EQUAL( cache.size() , 3 );

	BOOST_CHECK_EQUAL( cache.access("eee") , 50 );
	BOOST_CHECK_EQUAL( cache.access("bbb") , 20 );
	BOOST_CHECK_EQUAL( cache.access("ccc") , 30 );

	BOOST_CHECK_THROW( cache.access("ddd"), eckit::OutOfRange );
	BOOST_CHECK_THROW( cache.access("aaa"), eckit::OutOfRange );

	BOOST_CHECK_EQUAL( cache.access("bbb") , 20 );
	BOOST_CHECK_EQUAL( cache.access("ccc") , 30 );
	BOOST_CHECK_EQUAL( cache.access("eee") , 50 );

	BOOST_CHECK_NO_THROW( cache.clear() );

	BOOST_CHECK_THROW( cache.access("bbb"), eckit::OutOfRange );
	BOOST_CHECK_THROW( cache.access("ccc"), eckit::OutOfRange );
	BOOST_CHECK_THROW( cache.access("eee"), eckit::OutOfRange );

	BOOST_CHECK_EQUAL( cache.size() , 0 );
}

//----------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()