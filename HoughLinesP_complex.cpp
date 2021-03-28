#include <iostream>
#include <typeinfo>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img, imgEdge;
String imgPath = "data/complex.png";

// Hough Tranform Variables.(initial Value.)
int rho_unit = 1;
int theta_degree_unit = 1;
int ptr_votes_thres = 20;
double toRadian = (CV_PI / 180);
double toDegree = 1 / toRadian;

int minLineLen = 1;
int maxGap = 5;

// Found parameters
// rho_px=0.4, theta_degree=0.4, ptr_votes_thres=5, minLineLen=1, maxGap=1

int rtnWaitKey = 0;

int isEmptyImg(Mat img)
{
    if (img.empty())
    {
        cout << " FAILED::IMG LOADING " << endl;
        return -1;
    }
    else
    {
        // cout << " SUCCESS::IMG LOADING " << endl;
        return 0;
    }
}

Mat preProcessing(Mat img)
{
    Mat imgGray, imgBlur, imgDil;
    Mat imgCanny1;
    int kernelSize = 3;

    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(kernelSize, kernelSize), kernelSize, kernelSize);
    Canny(imgBlur, imgCanny1, 40, 60);
    imshow("imgCanny", imgCanny1);

    // Mat kernel = getStructuringElement(MORPH_RECT, Size(kernelSize - 5, kernelSize - 5));
    // dilate(imgCanny1, imgDil, kernel);

    return imgCanny1;
}

void drawHoughLinesP(vector<Vec4i> linePoints)
{
    cout << "No. of lines : " << linePoints.size() << endl;
    // 직선을 표현하기 위해 포인트 변수 생성.
    Point pt1, pt2;
    for (int i = 0; i < linePoints.size(); i++)
    {
        pt1.x = linePoints[i][0];
        pt1.y = linePoints[i][1];
        pt2.x = linePoints[i][2];
        pt2.y = linePoints[i][3];

        // 원본 이미지에 검출된 직선을 그림.
        line(img, pt1, pt2, Scalar(0, 0, 200), 3, LINE_AA);
    }
};

void showCurrentParmas(double rho_px, double theta_radian, int ptr_votes_thres, int minLineLen, int maxGap)
{
    cout << ">>  rho_px / theta_degree / votes / min Line Length / max Gap : \t"
         << rho_px << "\t" << theta_radian * toDegree << "\t" << ptr_votes_thres << "\t" << minLineLen << "\t" << maxGap << endl;
};

int main(int argc, char **argv)
{
    // 0. handle Window Trackbar...
    //namedWindow(윈도우 이름, 윈도우 사이즈 조절);
    namedWindow("Trackbars", WINDOW_NORMAL);
    resizeWindow("Trackbars", Size(600, 200));

    // 트랙바 조절에 따른 값 변화
    // rho 1 unit -> 0.2px 증감
    // theta 1 unit -> 0.1degree 증감
    // pixel vote 1 unit -> 1 vote 증감
    createTrackbar("Rho unit", "Trackbars", &rho_unit, 20);
    createTrackbar("theta unit", "Trackbars", &theta_degree_unit, 50);
    createTrackbar("min vote unit", "Trackbars", &ptr_votes_thres, 100);
    createTrackbar("min Line Length", "Trackbars", &minLineLen, 100);
    createTrackbar("max Gap for Pixel", "Trackbars", &maxGap, 100);

    // float 값 두개를 가지는 lines 벡터 변수 선언.
    vector<Vec4i> linePoints;

    while (true)
    {
        img = imread(imgPath);
        isEmptyImg(img);

        /* 1. preprocessing img. */
        imgEdge = preProcessing(img);

        /* 2. Hough line Transform

         1 unit parameter converts for Houghlines function.*/
        double rho_px = double(rho_unit) / 5;
        double theta_radian = (double(theta_degree_unit) / 10) * toRadian;

        HoughLinesP(imgEdge, linePoints, rho_px, theta_radian, ptr_votes_thres, minLineLen, maxGap);
        showCurrentParmas(rho_px, theta_radian, ptr_votes_thres, minLineLen, maxGap);
        drawHoughLinesP(linePoints);

        imshow("img", img);
        rtnWaitKey = waitKey(1);
        // imwrite("data/output/HoughLines_complex.png", img);

        if (rtnWaitKey == 27)
        {
            return 0;
        }
    }
}