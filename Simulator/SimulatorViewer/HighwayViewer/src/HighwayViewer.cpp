#include "HighwayViewer.h"

#include "Frame.h"

#include "SimulatorConfiguration.h"
#include "SimulatorBuildParams.h"
#include "Utility/SimulationParameters.h"
#include "Utility/LogFunctions.h"
#include "Basic/State.h"
#include "Utility/Math.h"
#include "ConvertState.h"

#include <iostream>

#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QTransform>
#include <QColor>
#include <math.h>
#include <iomanip>

//#include "ui_HighwayViewer.h"

using namespace std;
using namespace LogFunctions;
using namespace Utility;
using namespace openshot;

//FIXME Draw during app event loop, EDIT probably useless
// TODO Draw road edge (yellow line) beyond lane width, so that each lane has the same width (plus the edge for the extreme ones)

HighwayViewer::HighwayViewer(const SimulatorConfiguration &c) : SimulatorViewer(c),
    /*ui(new Ui::HighwayViewer), */ scene(nullptr), width(1920), height(9.0 / 16.0 * double(width)), 
	subjID(c.GetCustomEntries().at("subject_vehicle_id").c_str()),
    treesDistance(20), treeRadius(2), laneWidth(ToDouble(c.GetCustomEntries().at("lane_width"))),
    markLength(ToDouble(c.GetCustomEntries().at("mark_length"))), 
    markWidth(ToDouble(c.GetCustomEntries().at("mark_width"))),
    markDistance(ToDouble(c.GetCustomEntries().at("mark_distance"))),
    ffmpegWriter(VIDEO_OUTPUT)
{	
	//ui->setupUi(this);
    Q_INIT_RESOURCE(resources);
	
	//ui->graphicsView->setScene(new QGraphicsScene);
	//scene = ui->graphicsView->scene();
	scene = new QGraphicsScene(nullptr);
	
	//connect(this, SIGNAL(finished(const SimulAgentVector&)), this, SLOT(paint(const SimulAgentVector&)));
    first = true;
	u0 = 0;
    oldU0 = 0;

    // 50m of road visible ~ 10 vehicles
    visibleRoadLength = 40;
    spaceToPixScale = width / visibleRoadLength;

    int singleLaneHeight = laneWidth * spaceToPixScale;
    totalRoadHeight = 0;

    int lanes;

    try {
        lanes = atoi(GetSimulatorConfiguration().GetCustomEntries().at("lanes").c_str());
        if (lanes < 1)
            Error("HighwayViewer::DrawStaticEnvironment", "Parameter \'lanes\' must contain an \
			integer > 1");

        totalRoadHeight = singleLaneHeight * lanes;
    } catch (out_of_range &e) {
        Error("HighwayViewer::DrawStaticEnvironment", "Parameter \'lanes\' was not set");
    }


    a = width / visibleRoadLength;

    //Normal case, the whole road height is visible
    if (height > totalRoadHeight) {
        b = -double(totalRoadHeight) / (double(lanes) * laneWidth);
        v0 = round(double(height) / 2 + double(totalRoadHeight) / 2);
    } else { // Very zoomed case
        b = -double(height) / (double(lanes) * laneWidth);
        v0 = round(double(height));
    }

	//view.setScene(new QGraphicsScene(&view));
    scene->setSceneRect(0, 0, width, height);

		 
	// TODO FPS decoupled from simulation discrete time
	unsigned fps = round(1.0/GetSimulatorConfiguration().GetSimulationTimeStep());
	
	//enc = new QVideoEncoder;
    //enc->createFile(VIDEO_OUTPUT, width, height, 1000000, 20, fps);
	ffmpegWriter.SetVideoOptions(true, "libvpx", openshot::Fraction(fps,1), width, height, openshot::Fraction(1,1), false, false, 300000); // FPS: fps, Size: widthxheight, Pixel Ratio: 1/1, Bitrate: 300000
	ffmpegWriter.Open();
}

HighwayViewer::~HighwayViewer()
{	
	//delete enc;	
	//delete ui;
	ffmpegWriter.Close();
}

void HighwayViewer::DrawStaticEnvironment()
{
	// Add grass background
	QColor grassColor(qRgb(100, 230, 100));
	//QColor grassColor(0, 0, 0);
	
    grassTxt = scene->addRect(0, 0, width, height, QPen(), QBrush(grassColor));

    //Add road 
    if (height > totalRoadHeight)
        scene->addRect(0, (height - totalRoadHeight) / 2.0, width, totalRoadHeight, QPen(), QBrush(QColor("gray")));
    else
        scene->addRect(0, 0, width, height, QPen(), QBrush(QColor("gray")));

	// Draw road edges
	double edgeDistance = 0.15;
	double edgeWidth = 0.2;
	QColor edgeColor(qRgb(200, 200, 0));
	
	// Top edge
	scene->addRect(0, (height - totalRoadHeight) / 2.0 + SpaceToPixel(edgeDistance), width, SpaceToPixel(edgeWidth), QPen(), QBrush(edgeColor));
    // Bottom edge
	scene->addRect(0, (height + totalRoadHeight) / 2.0 - SpaceToPixel(edgeDistance + edgeWidth), width, SpaceToPixel(edgeWidth), QPen(), QBrush(edgeColor));

    // Draw markers in initial position
		int nLanes = atoi(GetSimulatorConfiguration().GetCustomEntries().at("lanes").c_str());
        // Draw markers in initial position
		
        for (int i = 0; i < nLanes - 1; i++) {
			
			std::vector<QGraphicsRectItem*> tmpMarkers;
			int uInit = 0;
			bool exitCondition = false;
            do {
				
                QGraphicsRectItem *marker = scene->addRect(uInit, GetPixelY(laneWidth*(i+1) + markWidth / 2), SpaceToPixel(markLength), SpaceToPixel(markWidth), QPen(), QBrush(QColor("white")));

                uInit += SpaceToPixel(markLength + markDistance);

                tmpMarkers.push_back(marker);

                if (uInit > width)
                    exitCondition = true;
            } while (!exitCondition);
			
			markers.push_back(tmpMarkers);
        
    }
    
    
    // Draw initial trees
    if (totalRoadHeight < height)
	{
		double xdec = 0.0;
		while (PixelToSpace(width/10) + xdec < visibleRoadLength)
		{
			treesTop.push_back(scene->addPixmap(QPixmap(":/Tree1.png").scaledToWidth(SpaceToPixel(2*treeRadius))));
			treesTop.back()->setOffset(width/10 + SpaceToPixel(xdec), height/4 - totalRoadHeight/4 - SpaceToPixel(treeRadius));
			xdec += treesDistance;
		};
		
		
		xdec = 0.0;
		while (PixelToSpace(width/3) + xdec < visibleRoadLength)
		{
			treesBottom.push_back(scene->addPixmap(QPixmap(":/Tree1.png").scaledToWidth(SpaceToPixel(2*treeRadius))));
			treesBottom.back()->setOffset(width/3 + SpaceToPixel(xdec), height - height/4 + totalRoadHeight/4 - SpaceToPixel(treeRadius));
			xdec += treesDistance;
		};
	}
	
	
	// Draw initial agents
	for (auto agent = GetSimulatorConfiguration().GetAgents().begin();
		 agent != GetSimulatorConfiguration().GetAgents().end(); agent++)
		 {
			 // Search for image name in config file
			 string imageName;
			 double vehicleLength;
			 try
			 {
			imageName = string(":/") + GetSimulatorConfiguration().GetAgentCustomEntries(agent->first)("image");
			
			vehicleLength = ToDouble(GetSimulatorConfiguration().GetAgentCustomEntries(agent->first).at("length"));
			 }
			 catch(out_of_range&)
			 {
				 Error("HighwayViewer::HighwayViewer", string("\'image\' entry not set for agent ") + agent->second.GetID());
			 }
			
			agentsPixmap[agent->first] = QPixmap(QString(imageName.c_str())).scaledToWidth(SpaceToPixel(vehicleLength));
			
			agents[agent->first] = 
				scene->addPixmap((agentsPixmap.at(agent->first)));
							
			// Z value sets layering order, default is 0
			// Vehicles are always on foreground
			agents.at(agent->first)->setZValue(1);
				
		 }
    
}

void HighwayViewer::DrawDynamicEnvironment(const SimulAgentVector& agents, const double&)
{
	paint(agents);
}

void HighwayViewer::DrawVehicles(const SimulAgentVector& a)
{
	for (auto agent = a.begin(); agent != a.end(); agent++)
	{
		agents.at(agent->first)->setPixmap(agentsPixmap.at(agent->first));
		
		// FIXME Use world state!!!
		double x, y, theta;
		ConvertFromState(agent->second.GetState(), agent->first, x, y, theta);
		QGraphicsPixmapItem* agentPixmap = agents.at(agent->first);
		
		QTransform t;
		t.rotateRadians(-theta);
		agentPixmap->setPixmap(agentPixmap->pixmap().transformed(t, Qt::SmoothTransformation));
		
		agentPixmap->setOffset(GetPixelX(x) - agentPixmap->pixmap().width()/2, GetPixelY(y) - agentPixmap->pixmap().height()/2);
		
	}
}

void HighwayViewer::DrawDecorations()
{
	if (treesTop.empty() || treesBottom.empty())
		return;
	
	// Trees on top half
	auto first = treesTop.begin();
	
	int newFirst = (*first)->boundingRect().left() - SpaceToPixel(treesDistance);
	if (newFirst > -SpaceToPixel(2*treeRadius))
	{
		treesTop.insert(first, scene->addPixmap(QPixmap((*first)->pixmap())));
		treesTop.front()->setOffset(newFirst, height/4 - totalRoadHeight/4 - SpaceToPixel(treeRadius));
	}
	
	auto last = --(treesTop.end());

	int newLast = (*last)->boundingRect().left() + SpaceToPixel(treesDistance);
	if (newLast < width)
	{
		treesTop.push_back(scene->addPixmap(QPixmap((*last)->pixmap())));
		treesTop.back()->setOffset(newLast, height/4 - totalRoadHeight/4 - SpaceToPixel(treeRadius));
	}
	
	for (auto decoration = treesTop.begin(); decoration != treesTop.end();
		 decoration++)
		 {
			int currentX = (*decoration)->boundingRect().left();
			(*decoration)->setOffset(currentX - oldU0 + u0, height/4 - totalRoadHeight/4 - SpaceToPixel(treeRadius));
		 }
		 
	if (treesTop.front()->boundingRect().right() < 0)
	{
		delete treesTop.front();
		treesTop.erase(treesTop.begin());
	}
	
	if (treesTop.back()->boundingRect().left() > width)
	{	
		delete treesTop.back();
		treesTop.pop_back();
	}
	
	// Trees on bottom half
	first = treesBottom.begin();
	
	newFirst = (*first)->boundingRect().left() - SpaceToPixel(treesDistance);
	if (newFirst > -SpaceToPixel(2*treeRadius))
	{
		treesBottom.insert(first, scene->addPixmap(QPixmap((*first)->pixmap())));
		treesBottom.front()->setOffset(newFirst, height - height/4 + totalRoadHeight/4 - SpaceToPixel(treeRadius));
	}
	
	last = --(treesBottom.end());

	newLast = (*last)->boundingRect().left() + SpaceToPixel(treesDistance);
	if (newLast < width)
	{
		treesBottom.push_back(scene->addPixmap(QPixmap((*last)->pixmap())));
		treesBottom.back()->setOffset(newLast, height - height/4 + totalRoadHeight/4 - SpaceToPixel(treeRadius));
	}
	
	for (auto decoration = treesBottom.begin(); decoration != treesBottom.end();
		 decoration++)
		 {
			int currentX = (*decoration)->boundingRect().left();
			(*decoration)->setOffset(currentX - oldU0 + u0, height - height/4 + totalRoadHeight/4 - SpaceToPixel(treeRadius));
		 }
		 
	if (treesBottom.front()->boundingRect().right() < 0)
	{
		delete treesBottom.front();
		treesBottom.erase(treesBottom.begin());
	}
		
	if (treesBottom.back()->boundingRect().left() > width)
	{
		delete treesBottom.back();
		treesBottom.pop_back();
	}
}

void HighwayViewer::Encode()
{
  	QImage copyImg(width, height, QImage::Format_RGBA8888);
	copyImg.fill(Qt::transparent);
	
    QPainter painter(&copyImg);
    painter.setRenderHint(QPainter::Antialiasing);

    scene->render(&painter);
	
  //  enc->encodeImage(img);
	std::shared_ptr<QImage> imgPtr(new QImage(copyImg));
	
	Frame* f = new Frame;
	f->AddImage(imgPtr);
		
	std::shared_ptr<Frame> framePtr(f);

	ffmpegWriter.WriteFrame(framePtr);
}

int HighwayViewer::GetPixelX(const double &x) const
{
    return round(a * x + u0);
}

int HighwayViewer::GetPixelY(const double &y) const
{
    return round(b * y + v0);
}

int HighwayViewer::SpaceToPixel(const double &deltaX) const
{
    //Pixel are square, so there is only one such function to convert deltaSpace -> deltaPixel

    return round(spaceToPixScale * deltaX);
}

double HighwayViewer::PixelToSpace(const int &deltaPix) const
{
    //Pixel are square, so there is only one such function to convert deltaSpace -> deltaPixel

    return double(deltaPix)/spaceToPixScale;
}


int HighwayViewer::SetSubjectX(const double &subjX)
{
    oldU0 = u0;
    u0 = round(double(width) * (0.5 - subjX / visibleRoadLength));
}

//void HighwayViewer::paintEvent(QPaintEvent* event)
//{
//}


void HighwayViewer::paint(const SimulAgentVector& agents)
{		
    // NB This method updates the frame of reference transform matrix
	try
	{
		State subjState = agents.at(subjID).GetState();
		double x, y, theta;
		ConvertFromState(subjState, subjID, x, y, theta);
		SetSubjectX(x);
	}
	catch (out_of_range&)
	{
		Error("HighwayViewer::DrawDynamicEnvironment", "Subject vehicle was not found");
	}

    for (auto markerV = markers.begin();
            markerV != markers.end(); markerV++) 
        for  (auto marker = markerV->begin(); marker != markerV->end(); marker++)
		{
		
		QRectF movedRect = (*marker)->rect();

        if (!first) {
            movedRect.moveLeft((*marker)->rect().left() - (oldU0 - u0));
            (*marker)->setRect(movedRect);
        }

        // Check if first and last marker have disappeared, if they have, delete them
        if (marker == markerV->begin())
            if ((*marker)->rect().right() < 0) {
                delete(*marker);
                markerV->erase(marker);
				
				markerV->push_back(scene->addRect(
					markerV->back()->rect().right() + SpaceToPixel(markDistance), GetPixelY(laneWidth*(markerV - markers.begin() + 1) + markWidth / 2), SpaceToPixel(markLength), SpaceToPixel(markWidth), QPen(), QBrush(QColor("white"))));
                marker--;
                continue;
            }

        if (marker == (--(markerV->end())))
            if ((*marker)->rect().left() > width) {
                delete(*marker);
                markerV->erase(marker);
				markerV->insert(markerV->begin(),
								scene->addRect(
									markerV->front()->rect().left() - SpaceToPixel(markDistance + markLength), GetPixelY(laneWidth*(markerV - markers.begin() + 1) + markWidth / 2), SpaceToPixel(markLength), SpaceToPixel(markWidth), QPen(), QBrush(QColor("white"))));
                marker--;
                continue;
            }
    }
    
    
    DrawVehicles(agents);
	DrawDecorations();


    first = false;
	
}
