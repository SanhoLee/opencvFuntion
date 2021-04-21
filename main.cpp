#include <iostream>
#include <typeinfo>

#include "CV_header.hpp"
#include "functionFile.hpp"
#define SQURE 4
#define TRI 3

using namespace std;
using namespace cv;

int WIDTH = 600;
int HEIGHT = 400;
int GAP1 = 100;
int GAP2 = 300;

int rtnWaitKey = 0;

int main(int argc, char **argv)
{
    Mat zeros = Mat::zeros(Size(WIDTH, HEIGHT), CV_8UC1);
    Mat img1(HEIGHT, WIDTH, CV_8UC3, Scalar(255, 255, 255));
    Mat img2(HEIGHT, WIDTH, CV_8UC3, Scalar(255, 255, 255));

    vector<vector<Point>> squre_point(2, vector<Point>(SQURE));

    // #1 squre
    squre_point[0][0] = Point(0, 0);
    squre_point[0][1] = Point(GAP1, 0);
    squre_point[0][2] = Point(GAP1, HEIGHT);
    squre_point[0][3] = Point(0, HEIGHT);
    // #2 squre
    squre_point[1][0] = Point(0, 0);
    squre_point[1][1] = Point(WIDTH, 0);
    squre_point[1][2] = Point(WIDTH, GAP1);
    squre_point[1][3] = Point(0, GAP1);

    /* Todos

    [done].make different img with fillpoly. but imgs should be intersect on specific Area.
    [done].make 2 polygon shape with vertice points.
    .do bitwise Calculation with two imgs.

    */
    fillPoly(img1, {squre_point[0]}, Scalar(255, 0, 0), LINE_AA);
    fillPoly(img2, {squre_point[1]}, Scalar(255, 0, 0), LINE_AA);

    imshow("img1", img1);
    imshow("img2", img2);
    // imshow("zeros", zeros);

    rtnWaitKey = waitKey(0);
    if (rtnWaitKey == 27)
    {
        imwrite("data/img1.jpg", img1);
        imwrite("data/img2.jpg", img2);
        return 0;
    }
}