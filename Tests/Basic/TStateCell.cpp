#include "Basic/StateCell.h"
#include "Utility/MyLogger.h"
#include <iostream>

//Link to Boost
 #define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
 #define BOOST_TEST_MODULE "Test StateCell"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

MyLogger logger(std::cout);

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE( ContainTest )
{
	StateMap minMap;
	minMap["x"] = 0;
	minMap["y"] = 0;
	minMap["theta"] = 0;
	
	State min(minMap);
	
	StateMap maxMap;
	maxMap["x"] = 100;
	maxMap["y"] = 1;
	maxMap["theta"] = 6.28;
	
	State max(maxMap);
	
	StateCell cell1(min, max);
	
	StateMap pointMap;
	pointMap["x"] = 95;
	pointMap["y"] = 0.5;
	pointMap["theta"] = 3;
	State inside(pointMap);
	
	BOOST_CHECK(cell1.Contains(inside));
	
	pointMap["x"] = 102;
	State outside(pointMap);
	
	BOOST_CHECK(!cell1.Contains(outside));
}

BOOST_AUTO_TEST_CASE( IntersectionTest )
{
	StateMap minMap1;
	minMap1["x"] = 0;
	minMap1["y"] = 2;
	
	State min1(minMap1);
	
	StateMap maxMap1;
	maxMap1["x"] = 10;
	maxMap1["y"] = 20;

	State max1(maxMap1);
	
	StateMap minMap2;
	minMap2["x"] = 7;
	minMap2["y"] = 0;
	
	State min2(minMap2);
	
	StateMap maxMap2;
	maxMap2["x"] = 11;
	maxMap2["y"] = 217;
	
	State max2(maxMap2);
	
	StateCell cell1(min1, max1);
	StateCell cell2(min2, max2);
	
	StateCell cellInter = cell1*cell2;
	
	if (!cellInter.IsEmpty())
	{
		logger << cell1 << logger.EndL();
		logger << "INTERSECT WITH" << logger.EndL();
		logger << cell2 << logger.EndL();
		logger << " ========== " << logger.EndL();
		logger << (cell1*cell2) << logger.EndL();
	}
	else
		logger << "EMPTY!!" << logger.EndL();
	
}

BOOST_AUTO_TEST_CASE( Align )
{
	logger << "------ Begin align test case -----" << logger.EndL();
	
	StateMap minMap1;
	minMap1["x"] = 0;
	minMap1["y"] = 0;
	
	State min1(minMap1);
	
	StateMap maxMap1;
	maxMap1["x"] = 1.5;
	maxMap1["y"] = 2;

	State max1(maxMap1);
	
	StateMap minMap2;
	minMap2["x"] = 0.5;
	minMap2["y"] = 2;
	
	State min2(minMap2);
	
	StateMap maxMap2;
	maxMap2["x"] = 1.5;
	maxMap2["y"] = 3;
	
	State max2(maxMap2);
	
	StateCell cell1(min1, max1);
	StateCell cell2(min2, max2);
	
	logger << "Aligned X? " << cell1.IsAligned(cell2, "x") << logger.EndL();
	logger << "Aligned Y? " << cell1.IsAligned(cell2, "y") << logger.EndL();
	
	logger << "Intersection? " << cell1.HasIntersection(cell2) << logger.EndL();
	
	logger << "----------------------------------" << logger.EndL();
	
}

