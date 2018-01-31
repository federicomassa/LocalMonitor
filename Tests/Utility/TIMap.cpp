//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "Test TIMap"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "Utility/IMap.h"
#include <iostream>

using namespace std;

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE(IMapTest)
{
	IMap<int> map1;
	IMap<int> map2;
	
	map1["key1"] = 0;
	map1["key2"] = 0;
	map1["key3"] = 0;
	
	map2["key1"] = 0;
	map2["key2"] = 0;
	map2["key3"] = 0;
	
	
	BOOST_ASSERT(map1.IsConsistent(map2));
	
	map2["key4"] = 0;
	
	BOOST_ASSERT(!map1.IsConsistent(map2));
	
	
}
