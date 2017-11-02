#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui>
#include <QSize>
#include <QWidget>

class MyWidget : public QWidget
{
      Q_OBJECT

 private:
      QSize sizeHint() const override;
 public:
      //  MyWidget();

      protected:
      void paintEvent(QPaintEvent *event);
 signals:

      public slots:

};

#endif // MYWIDGET_H
