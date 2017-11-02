#include "mywidget.h"
#include <QApplication>
#include <QDesktopWidget>

void MyWidget::paintEvent(QPaintEvent *event)
{
  //create a QPainter and pass a pointer to the device.
  //A paint device can be a QWidget, a QPixmap or a QImage
  QPainter painter(this);

  //a simple line
  painter.drawLine(1000,1000,100,100);

  //create a black pen that has solid line
  //and the width is 2.
  QPen myPen(Qt::black, 2, Qt::SolidLine);
  myPen.setColor(Qt::blue);
  painter.setPen(myPen);

  painter.drawLine(100,100,100,1);

  //draw a point
  painter.drawPoint(110,110);

  //draw a polygon
  QPolygon polygon;
  polygon << QPoint(130, 140) << QPoint(180, 170)
	  << QPoint(180, 140) << QPoint(220, 110)
	  << QPoint(140, 100);
  painter.drawPolygon(polygon);

  //draw an ellipse
  //The setRenderHint() call enables antialiasing, telling QPainter to use different
  //color intensities on the edges to reduce the visual distortion that normally
  //occurs when the edges of a shape are converted into pixels
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(QPen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap));
  painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
  painter.drawEllipse(200, 80, 400, 240);

}

QSize MyWidget::sizeHint() const
{
  QRect rect = QApplication::desktop()->screenGeometry();

  return QSize(rect.width(), rect.height());
}
