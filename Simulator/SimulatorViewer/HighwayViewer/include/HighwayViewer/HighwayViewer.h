#ifndef HIGHWAYVIEWER_H
#define HIGHWAYVIEWER_H

#include "SimulatorViewer/SimulatorViewer.h"
#include "EnvironmentParameters.h"
#include "Utility/QTFFmpegWrapper/QVideoEncoder.h"
#include <QApplication>

class QGraphicsScene;
class QGraphicsView;
class SimulatorConfiguration;
class QGraphicsLineItem;
 

class HighwayViewer : public SimulatorViewer
{
	QGraphicsView* view;
	int x;
	int width;
	int height;
	QGraphicsLineItem* line;
	QVideoEncoder enc;
	QApplication app;
public:
	HighwayViewer(const SimulatorConfiguration&, int&, char**);
	~HighwayViewer();
	void DrawEnvironment() override;
};

#endif
