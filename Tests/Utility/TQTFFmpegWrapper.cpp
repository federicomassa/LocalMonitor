#include <iostream>
#include "Utility/QTFFmpegWrapper/QVideoEncoder.h"
#include "mywidget.h"
#include <QApplication>
#include <QLabel>


//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "Test QTFFmpegWrapper"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

using namespace std;

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE(SimpleEncoding)
{
    QApplication app(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

    QVideoEncoder enc;
  
    QImage image(":/icon.png");
    
    // create background with same format as image
    QImage background(800, 800, image.format());
    QImage scaledImage = image.scaledToWidth(200);
	
    QPainter* painter = new QPainter(&background);
      //a simple line
  painter->drawLine(1000,1000,100,100);

  //create a black pen that has solid line
  //and the width is 2.
  QPen myPen(Qt::black, 2, Qt::SolidLine);
  myPen.setColor(Qt::blue);
  painter->setPen(myPen);

  painter->drawLine(100,100,100,1);

  //draw a point
  painter->drawPoint(110,110);

  //draw a polygon
  QPolygon polygon;
  polygon << QPoint(130, 140) << QPoint(180, 170)
	  << QPoint(180, 140) << QPoint(220, 110)
	  << QPoint(140, 100);
  painter->drawPolygon(polygon);

  //draw an ellipse
  //The setRenderHint() call enables antialiasing, telling QPainter to use different
  //color intensities on the edges to reduce the visual distortion that normally
  //occurs when the edges of a shape are converted into pixels
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
  painter->drawEllipse(200, 80, 400, 240);

	enc.createFile("output.avi", background.width(), background.height(), 1000000, 20, 25);

	QImage painted(background);

	
	/*
    MyWidget myWidget;
    myWidget.setWindowIcon(QIcon(":/icon.png"));
    myWidget.show();


    QLabel label;
    label.setPixmap(QPixmap::fromImage(image));
    label.show();
*/
	
	delete painter;
	
    //10 seconds video
    for (int i = 0; i < 250; i++)
      {
		painter = new QPainter(&background);
		painter->drawImage(i, i, scaledImage);
		delete painter;
		enc.encodeImage(background);
		background = painted;
      }
      

}
