#include "Utility/QCustomPlot/qcustomplot.h"
#include "Utility/MyLogger.h"

#include <QtWidgets>
#include <QApplication>

#include <iostream>

//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "Test Basic plots with QCustomPlot"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

MyLogger logger(std::cout);

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE(BasicPlot)
{
	QApplication app(boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv);
	
	QCustomPlot* customPlot = new QCustomPlot;
	
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i = 0; i < 101; ++i) {
        x[i] = i / 50.0 - 1; // x goes from -1 to 1
        y[i] = x[i] * x[i]; // let's plot a quadratic function
    }
// create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
// give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
// set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();
	
	customPlot->show();
	
	app.exec();
}
