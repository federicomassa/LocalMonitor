#ifndef HIGHWAYVIEWER_H
#define HIGHWAYVIEWER_H

#include "SimulatorViewer/SimulatorViewer.h"
#include "SimulAgent.h"
//#include "Utility/QTFFmpegWrapper/QVideoEncoder.h"
#include "FFmpegWriter.h"
#include "Frame.h"
#include <QApplication>
#include <QPointer>
#include <QImage>
#include <string>
#include <vector>
#include <QGraphicsScene>

class SimulatorConfiguration;
class QGraphicsRectItem;
class QPixmap;
class QGraphicsPixmapItem;
class QPaintEvent;

//namespace Ui
//{
//	class HighwayViewer;
//}

class HighwayViewer : public SimulatorViewer
{	
	//Q_OBJECT
	
	//Ui::HighwayViewer* ui;
	QGraphicsScene* scene;
	//16:9 ratio
	//Screen Width in pixel
	const int width;
	//Screen height in pixel
	const int height;
	
	//ID of subject vehicle
	const std::string subjID;
	
	// Active the first iteration, to set oldU0
	bool first;
	
	// Transform parameters of frame of reference:
	// u = a*x + u0
	// v = b*y + v0
	// NB this is in general for x,y with different scale, in practice |a| = |b|
	// for square pixels
	// (u,v) pixel --> (x,y) space
	double a;
	double b;
	double u0;
	double v0;
		
	// Used to move non-agent objects in scene
	int oldU0;
	
	// ================== GRAPHICS ITEMS ==================
	
	std::vector<std::vector<QGraphicsRectItem*> > markers;
	
	// FIXME Need both because pixmap is not rotated, so I can use absolute rotations
	std::map<std::string, QGraphicsPixmapItem* > agents;
	std::map<std::string, QPixmap > agentsPixmap;
	
	std::vector<QGraphicsPixmapItem*> treesTop;
	std::vector<QGraphicsPixmapItem*> treesBottom;
	
	// Distance among trees and spatial radius of the tree pixmap
	const double treesDistance;
	const double treeRadius;
	
	QGraphicsRectItem* grassTxt;
	// ====================================================
	
	//Zoom is defined as the visible road length in meters
	double visibleRoadLength;
	
	// Pixel height occupied by road
	int totalRoadHeight;
	
	// Real lane width (~ 3m)
	const double laneWidth;
	
	// Lane markers (~ 50cm x 10cm, distant ~50cm)
	const double markLength;
	const double markWidth;
	const double markDistance;
	
	// Scale from meters to pixel, based on set visibleRoadLength
	double spaceToPixScale;
	
	//QVideoEncoder* enc;
	openshot::FFmpegWriter ffmpegWriter;
	
	// Get x,y pixel of coordinate x,y
	int GetPixelX(const double& x) const;
	int GetPixelY(const double& y) const;
	
	// Pixels occupied by a space interval
	int SpaceToPixel(const double& deltaX) const;
	double PixelToSpace(const int& deltaPix) const;

	
	// Sets x coordinate of central subject
	int SetSubjectX(const double& subjX);
	
	// Draw vehicles
	void DrawVehicles(const SimulAgentVector&);
	
	// Draw environment decorations
	void DrawDecorations();
public:
	HighwayViewer(const SimulatorConfiguration&);
	~HighwayViewer();
	
	/**
	 * @brief Draw static objects in environment
	 * 
	 */
	void DrawStaticEnvironment() override;
	void DrawDynamicEnvironment(const SimulAgentVector&, const double& currentTime) override;
	void Encode() override;
	//void paintEvent(QPaintEvent*) override;
		
private:
	void paint(const SimulAgentVector&);

};

#endif
