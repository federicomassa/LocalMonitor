#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/viz/vizcore.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <stdio.h>


#include <iostream>

//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "Test Basic OpenCV functionalities"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

using namespace cv;
using namespace std;

int w = 400;

void MyEllipse(Mat img, double angle)
{
  int thickness = 2;
  int lineType = 8;

  ellipse( img,
           Point( w/2.0, w/2.0 ),
           Size( w/4.0, w/16.0 ),
           angle,
           0,
           360,
           Scalar( 255, 0, 0 ),
           thickness,
           lineType );
}


void MyFilledCircle( Mat img, Point center )
{
 int thickness = -1;
 int lineType = 8;

 circle( img,
         center,
         w/32.0,
         Scalar( 0, 0, 255 ),
         thickness,
         lineType );
}

void MyLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = 8;
  line( img,
        start,
        end,
        Scalar( 0, 0, 0 ),
        thickness,
        lineType );
}

void MyPolygon( Mat img )
{
  int lineType = 8;

  /** Create some points */
  Point rook_points[1][20];
  rook_points[0][0] = Point( w/4.0, 7*w/8.0 );
  rook_points[0][1] = Point( 3*w/4.0, 7*w/8.0 );
  rook_points[0][2] = Point( 3*w/4.0, 13*w/16.0 );
  rook_points[0][3] = Point( 11*w/16.0, 13*w/16.0 );
  rook_points[0][4] = Point( 19*w/32.0, 3*w/8.0 );
  rook_points[0][5] = Point( 3*w/4.0, 3*w/8.0 );
  rook_points[0][6] = Point( 3*w/4.0, w/8.0 );
  rook_points[0][7] = Point( 26*w/40.0, w/8.0 );
  rook_points[0][8] = Point( 26*w/40.0, w/4.0 );
  rook_points[0][9] = Point( 22*w/40.0, w/4.0 );
  rook_points[0][10] = Point( 22*w/40.0, w/8.0 );
  rook_points[0][11] = Point( 18*w/40.0, w/8.0 );
  rook_points[0][12] = Point( 18*w/40.0, w/4.0 );
  rook_points[0][13] = Point( 14*w/40.0, w/4.0 );
  rook_points[0][14] = Point( 14*w/40.0, w/8.0 );
  rook_points[0][15] = Point( w/4.0, w/8.0 );
  rook_points[0][16] = Point( w/4.0, 3*w/8.0 );
  rook_points[0][17] = Point( 13*w/32.0, 3*w/8.0 );
  rook_points[0][18] = Point( 5*w/16.0, 13*w/16.0 );
  rook_points[0][19] = Point( w/4.0, 13*w/16.0) ;

  const Point* ppt[1] = { rook_points[0] };
  int npt[] = { 20 };

  fillPoly( img,
            ppt,
            npt,
            1,
            Scalar( 255, 255, 255 ),
            lineType );
 }

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE(NewWindow)
{
	IplImage* img = cvLoadImage("../Tests/HighwayViewer/Images/Lenna.png", 1);
	Mat mtx2;
	mtx2 = imread("../Tests/HighwayViewer/Images/Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat mtx = cvarrToMat(img);
	//namedWindow( "Display windows", WINDOW_AUTOSIZE );// Create a window for display.
	imshow("Display windows", mtx2);
	
	imshow("Display window!!", mtx2);
	
	
	
	waitKey(0);
}

BOOST_AUTO_TEST_CASE(BasicDrawing)
{
	const char* atomDrawing = "Drawing 1: Atom";
	const char* rookDrawing = "Drawing 2: Rook";
	
	Mat atom_img = Mat::zeros(w, w, CV_8UC3);
	Mat rook_img = Mat::zeros(w, w, CV_8UC3);
	
	MyEllipse(atom_img, 90);
	MyEllipse(atom_img, 0);
	MyEllipse(atom_img, 45);
	MyEllipse(atom_img, -45);
	
	MyFilledCircle(atom_img, Point(w/2.0, w/2.0));
	
	
	MyPolygon(rook_img);
	
	rectangle(rook_img,
			  Point(0, 7./8.*w),
			  Point(w, w),
			  Scalar(0, 255, 255),
			  -1,
		   8);
	
	MyLine( rook_img, Point( 0, 15*w/16 ), Point( w, 15*w/16 ) );
	MyLine( rook_img, Point( w/4, 7*w/8 ), Point( w/4, w ) );
	MyLine( rook_img, Point( w/2, 7*w/8 ), Point( w/2, w ) );
	MyLine( rook_img, Point( 3*w/4, 7*w/8 ), Point( 3*w/4, w ) );

	imshow( atomDrawing, atom_img );
	moveWindow( atomDrawing, 0, 200 );
	imshow( rookDrawing, rook_img );
	moveWindow( rookDrawing, w, 200 );

	waitKey( 0 );
	
	
}

BOOST_AUTO_TEST_CASE(ReadImage)
{
	int delay = 5;
	
	RNG rng(0xFFFFFFF);
	
	int wSize = 800;
	int imgSize = 150;
	double f = double(imgSize)/double(wSize);
		
	Mat lenna = imread("../Tests/HighwayViewer/Images/Lenna.png");
	Mat lennaResized;
	Mat blackLenna;
	
	resize(lenna, lennaResized, Size(),f, f);
	
	int x = rng.uniform(0, wSize - imgSize), y = rng.uniform(0, wSize - imgSize);
	x = 0; y = 0;
	bool forwardX = true, forwardY = true;
	
	while (waitKey(30) == -1) 
	{
	Mat black = Mat::zeros(Size(wSize, wSize), lenna.type());
	
	//lennaResized.copyTo(black(Rect(wSize/2.0, wSize/2.0, lennaResized.cols, lennaResized.rows)));
	lennaResized.copyTo(black(Rect(x,y, lennaResized.cols, lennaResized.rows)));
	
	if (forwardX)
	{
		if (x + imgSize < wSize)
			x += 5;
		else
		{
		forwardX = false;
		}
	}
	else
	{
		if (x > 5)
			x -= 5;
		else
			forwardX = true;
	}
	
	if (forwardY)
	{
		if (y + imgSize < wSize)
			y += 5;
		else
		{
		forwardY = false;
		}
	}
	else
	{
		if (y > 5)
			y -= 5;
		else
			forwardY = true;
	}
	
		

	imshow("Moving box", black);
	}	
}


BOOST_AUTO_TEST_CASE(Viz)
{

    /// Create a window
    viz::Viz3d myWindow("Coordinate Frame");

    /// Add coordinate axes
    myWindow.showWidget("Coordinate Widget", viz::WCoordinateSystem());

    /// Add line to represent (1,1,1) axis
    viz::WLine axis(Point3f(-1.0f,-1.0f,-1.0f), Point3f(1.0f,1.0f,1.0f));
    axis.setRenderingProperty(viz::LINE_WIDTH, 4.0);
    myWindow.showWidget("Line Widget", axis);

    /// Construct a cube widget
    viz::WCube cube_widget(Point3f(0.5,0.5,0.0), Point3f(0.0,0.0,-0.5), true, viz::Color::blue());
    cube_widget.setRenderingProperty(viz::LINE_WIDTH, 4.0);

    /// Display widget (update if already displayed)
    myWindow.showWidget("Cube Widget", cube_widget);

    /// Rodrigues vector
    Mat rot_vec = Mat::zeros(1,3,CV_32F);
    float translation_phase = 0.0, translation = 0.0;
    while(!myWindow.wasStopped())
    {
        /* Rotation using rodrigues */
        /// Rotate around (1,1,1)
        rot_vec.at<float>(0,0) += CV_PI * 0.01f;
        rot_vec.at<float>(0,1) += CV_PI * 0.01f;
        rot_vec.at<float>(0,2) += CV_PI * 0.01f;

        /// Shift on (1,1,1)
        translation_phase += CV_PI * 0.01f;
        translation = sin(translation_phase);

        Mat rot_mat;
        Rodrigues(rot_vec, rot_mat);

        /// Construct pose
        Affine3f pose(rot_mat, Vec3f(translation, translation, translation));

        myWindow.setWidgetPose("Cube Widget", pose);
		myWindow.spinOnce(1, true);
    }
	
}

