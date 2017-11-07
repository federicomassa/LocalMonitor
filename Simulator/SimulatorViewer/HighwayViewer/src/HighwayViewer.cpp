#include "HighwayViewer.h"

#include "SimulatorConfiguration.h"
#include "SimulatorBuildParams.h"
#include "Utility/LogFunctions.h"

#include <iostream>

#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QPixmap>
#include <QColor>

using namespace std;
using namespace LogFunctions;

HighwayViewer::HighwayViewer(const SimulatorConfiguration& c, int& argc, char** argv) : SimulatorViewer(c),
							view(nullptr), app(argc, argv)
{
	Q_INIT_RESOURCE(resources);
	
	cout << "QUI" << endl;
	
	QPen pen;
	pen.setColor("red");
	pen.setWidth(5);
	
	view = new QGraphicsView(new QGraphicsScene(nullptr));
	
	
	
	cout << "FINE" << endl;
	

	width = 1600;
	height = 400;
	try
	{
		int lanes = int(GetSimulatorConfiguration().GetParameters().at("lanes"));
		if (lanes < 1)
			Error("HighwayViewer::HighwayViewer", "Parameter \'lanes\' must contain an \
			integer > 1");
		
		height = height*lanes;
	}
	catch(out_of_range& e)
	{}
	

	enc.createFile(VIDEO_OUTPUT, width, height, 1000000, 20, 25);
	
	//view->setSceneRect(0, 0, width, height);
	
	//Add background
	QPixmap grass(":/GrassTexture.jpg");
	std::cout << "DEPTH = " << grass.depth() << std::endl;
	
	view->scene()->addRect(0, 0, width-1, height-1, QPen(), QBrush(grass));
	
}

HighwayViewer::~HighwayViewer()
{
	if (view)
		delete view;
}

void HighwayViewer::DrawEnvironment() 
{
	cout << "SCENE SIZE: " << view->scene()->width() << '\t' << view->scene()->height() << endl;
	
	
	QImage img(width, height, QImage::Format_RGB32);
	img.fill(Qt::transparent);
	
	QPainter painter(&img);
	painter.setRenderHint(QPainter::Antialiasing);
	
	view->scene()->render(&painter);
	
	enc.encodeImage(img);
}
