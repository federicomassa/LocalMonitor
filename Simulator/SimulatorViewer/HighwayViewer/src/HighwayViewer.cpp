#include "HighwayViewer.h"

#include "SimulatorConfiguration.h"
#include "SimulatorBuildParams.h"
#include "SimulationParameters.h"
#include "Utility/LogFunctions.h"
#include "Basic/State.h"
#include "ConvertState.h"

#include <iostream>

#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPixmap>
#include <QColor>
#include <math.h>

using namespace std;
using namespace LogFunctions;



HighwayViewer::HighwayViewer(const SimulatorConfiguration &c, int &argc, char **argv, SimulatorViewer *_parent) : SimulatorViewer(c, _parent),
    app(new QApplication(argc, argv)), view(nullptr), width(1920), height(9.0 / 16.0 * double(width)),
    laneWidth(3), markLength(0.5), markWidth(0.1),
    markDistance(0.5)
{
    Q_INIT_RESOURCE(resources);

    grassTxt = nullptr;

    first = true;
    oldU0 = 0;

    // 50m of road visible ~ 10 vehicles
    visibleRoadLength = 40;
    spaceToPixScale = width / visibleRoadLength;

    int singleLaneHeight = laneWidth * spaceToPixScale;
    totalRoadHeight = 0;

    enc = new QVideoEncoder;

    int lanes;

    try {
        lanes = int(GetSimulatorConfiguration().GetParameters().at("lanes"));
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

    view = new QGraphicsView;
    view->setScene(new QGraphicsScene(view));
    view->scene()->setSceneRect(0, 0, width, height);

	for (auto agent = GetSimulatorConfiguration().GetAgents().begin();
		 agent != GetSimulatorConfiguration().GetAgents().end(); agent++)
		 {
			 // Search for image name in config file
			 string imageName;
			 double vehicleLength;
			 try
			 {
			imageName = ":/" + GetSimulatorConfiguration().GetAgentParameters(agent->first).at("image");
			vehicleLength = atof(GetSimulatorConfiguration().GetAgentParameters(agent->first).at("length").c_str());
			 }
			 catch(out_of_range&)
			 {
				 Error("HighwayViewer::HighwayViewer", string("\'image\' entry not set for agent ") + agent->second.GetID());
			 }
			 
			agents[agent->first] = 
				view->scene()->addPixmap(QPixmap(QString(imageName.c_str())).scaledToWidth(SpaceToPixel(vehicleLength)));
							
			// Z value sets layering order, default is 0
			// Vehicles are always on foreground
			agents.at(agent->first)->setZValue(1);
				
			view->scene()->addPixmap(QPixmap(QString(imageName.c_str())));	
				
		 }
		 
	// TODO FPS decoupled from simulation discrete time
	unsigned fps = round(1.0/GetSimulatorConfiguration().GetSimulationTimeStep());
    enc->createFile(VIDEO_OUTPUT, width, height, 1000000, 20, fps);

	
}

HighwayViewer::~HighwayViewer()
{
    delete enc;
    delete view;
    delete grass;
    delete app;
}

void HighwayViewer::DrawStaticEnvironment()
{
	// Add grass background
	QColor grassColor(qRgb(100, 230, 100));
    grassTxt = view->scene()->addRect(0, 0, width, height, QPen(), QBrush(grassColor));

    //Add road 
    if (height > totalRoadHeight)
        view->scene()->addRect(0, (height - totalRoadHeight) / 2.0, width, totalRoadHeight, QPen(), QBrush(QColor("gray")));
    else
        view->scene()->addRect(0, 0, width, height, QPen(), QBrush(QColor("gray")));

	// Draw road edges
	double edgeDistance = 0.15;
	double edgeWidth = 0.2;
	QColor edgeColor(qRgb(200, 200, 0));
	
	// Top edge
	view->scene()->addRect(0, (height - totalRoadHeight) / 2.0 + SpaceToPixel(edgeDistance), width, SpaceToPixel(edgeWidth), QPen(), QBrush(edgeColor));
    // Bottom edge
	view->scene()->addRect(0, (height + totalRoadHeight) / 2.0 - SpaceToPixel(edgeDistance + edgeWidth), width, SpaceToPixel(edgeWidth), QPen(), QBrush(edgeColor));

    // Draw markers in initial position
		int nLanes = int(GetSimulatorConfiguration().GetParameters().at("lanes"));
        // Draw markers in initial position
		
        for (int i = 0; i < nLanes - 1; i++) {
			
			std::vector<QGraphicsRectItem *> tmpMarkers;
			int uInit = 0;
			bool exitCondition = false;
            do {
				
                QGraphicsRectItem *marker = view->scene()->addRect(uInit, GetPixelY(laneWidth*(i+1) + markWidth / 2), SpaceToPixel(markLength), SpaceToPixel(markWidth), QPen(), QBrush(QColor("white")));

                uInit += SpaceToPixel(markLength + markDistance);

                tmpMarkers.push_back(marker);

                if (uInit > width)
                    exitCondition = true;
            } while (!exitCondition);
			
			markers.push_back(tmpMarkers);
        
    }
}

void HighwayViewer::DrawDynamicEnvironment(const SimulAgentVector& agents)
{

    const string subjID = GetProperty("SubjectID");
	
	
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
				
				markerV->push_back(view->scene()->addRect(
					markerV->back()->rect().right() + SpaceToPixel(markDistance), GetPixelY(laneWidth*(markerV - markers.begin() + 1) + markWidth / 2), SpaceToPixel(markLength), SpaceToPixel(markWidth), QPen(), QBrush(QColor("white"))));
                marker--;
                continue;
            }

        if (marker == (--(markerV->end())))
            if ((*marker)->rect().left() > width) {
                delete(*marker);
                markerV->erase(marker);
				markerV->insert(markerV->begin(),
								view->scene()->addRect(
									markerV->front()->rect().left() - SpaceToPixel(markDistance + markLength), GetPixelY(laneWidth*(markerV - markers.begin() + 1) + markWidth / 2), SpaceToPixel(markLength), SpaceToPixel(markWidth), QPen(), QBrush(QColor("white"))));
                marker--;
                continue;
            }
    }
    
    
    DrawVehicles(agents);


    first = false;
}

void HighwayViewer::DrawVehicles(const SimulAgentVector& a)
{
	for (auto agent = a.begin(); agent != a.end(); agent++)
	{
		double x, y, theta;
		ConvertFromState(agent->second.GetState(), agent->first, x, y, theta);
		QGraphicsPixmapItem* agentPixmap = agents.at(agent->first);
		agentPixmap->setOffset(GetPixelX(x) - agentPixmap->pixmap().width()/2, GetPixelY(y) - agentPixmap->pixmap().height()/2);
		
		
		cout << "Draw vehicle " << agent->first << " at pix: " << GetPixelX(x) - agentPixmap->pixmap().width()/2 << 
		" x " << GetPixelY(y) - agentPixmap->pixmap().height()/2 << endl;
		
	}
}

void HighwayViewer::Encode()
{
    QImage img(width, height, QImage::Format_RGB32);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);

    view->scene()->render(&painter);

    enc->encodeImage(img);
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

int HighwayViewer::SetSubjectX(const double &subjX)
{
    oldU0 = u0;
    u0 = round(double(width) * (0.5 - subjX / visibleRoadLength));
}


