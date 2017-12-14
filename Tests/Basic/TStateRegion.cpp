#include "Basic/StateRegion.h"
#include "Utility/MyLogger.h"
#include <iostream>

//Link to Boost
 #define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
 #define BOOST_TEST_MODULE "Test StateRegion"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

MyLogger logger(std::cout);

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE( SimplifyCase )
{
	StateMap mapMin1;
	mapMin1["x"] = 0;
	mapMin1["y"] = 0;
	
	StateMap mapMax1;
	mapMax1["x"] = 1;
	mapMax1["y"] = 2;
	
	
	StateCell cell1((State(mapMin1)), (State(mapMax1)));
	
	StateMap mapMin2;
	mapMin2["x"] = 0.5;
	mapMin2["y"] = 0;
	
	StateMap mapMax2;
	mapMax2["x"] = 1.5;
	mapMax2["y"] = 3;
	
	
	StateCell cell2((State(mapMin2)), (State(mapMax2)));
	
	StateMap mapMin3;
	mapMin3["x"] = 1;
	mapMin3["y"] = 2;
	
	StateMap mapMax3;
	mapMax3["x"] = 2;
	mapMax3["y"] = 3;
	
	
	StateCell cell3((State(mapMin3)), (State(mapMax3)));
	
	
	logger << "Init cells: ";
	logger << cell1;
	logger << cell2;
	logger << cell3;
	
	StateRegion reg12;
	
	logger << "Before simplify" << logger.EndL();
	reg12.AddCell(cell1);
	reg12.AddCell(cell2);
	//reg12.AddCell(cell3);

	logger << reg12;

	
	reg12.Simplify();
	
	logger << "After simplify" << logger.EndL();
	logger << reg12;
		
}
