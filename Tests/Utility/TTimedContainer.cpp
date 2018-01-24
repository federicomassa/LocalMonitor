//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "Test TTimedContainer"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "Utility/TimedContainer.h"
#include <iostream>

using namespace std;

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE(TimedContainerTest)
{
	TimedContainer<double> t;
	t.insert(2.7, -4.5);
	t.insert(5.6, 1.2);
	t.insert(0.1, 7.9);
	t.insert(9.1, -10.1);
	
	for (auto itr = t.begin(); itr != t.end(); ++itr)
	{
		cout << itr.time() << " --- " << itr.value() << endl;
	}
	
	BOOST_REQUIRE_EQUAL(t.size(), 4);
	BOOST_REQUIRE_EQUAL(t.begin().time(), 9.1);
	BOOST_REQUIRE_EQUAL(t.oldest().time(), 0.1);
	
	
	t.erase(t.begin());
	
	cout << endl << "==================" << endl << endl;
	
	for (auto itr = t.begin(); itr != t.end(); itr++)
	{
		cout << itr.time() << " --- " << itr.value() << endl;
	}
	
	BOOST_REQUIRE_EQUAL(t.size(), 3);
	BOOST_REQUIRE_EQUAL(t.begin().time(), 5.6);
	BOOST_REQUIRE_EQUAL(t.oldest().time(), 0.1);	
}
