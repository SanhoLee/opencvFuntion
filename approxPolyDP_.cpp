#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat img, imgPre, imgHSV;
Mat mask;

int rtn = 0;
String dataSrc = "data/shapes.png";

int hmin = 0, smin = 158, vmin = 174;
int hmax = 16, smax = 183, vmax = 255;

Mat preProcessing(Mat img)
{
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    // circle mask boundary in HSV color space.
    Scalar lower(hmin, smin, vmin);
    Scalar upper(hmax, smax, vmax);
    inRange(imgHSV, lower, upper, mask);

    return mask;
}

void getContours(Mat imgDil)
{

    // Description  : 입력된 이미지에서 외곽 테두리를 찾습니다.
    // input        : mask 처리된 이미지 소스
    // return       : 검출된 윤곽을 둘러싸는 직사각형에서 상단 센터 좌표를 반환함
    //
    // contours     : 각 도형의 윤관석을 표현하는 포인트 요소를 벡터로 표현해서 가지고 있게 된다.
    // hierarchy    : 4개 정수를 그 요소로 하는 벡터를 정의해줬다.

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    // 소스 이미지로 부터 윤곽선을 찾고, contour 벡터 안에 Point정보를 저장한다.
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // conPoly      : 윤곽선을 구성하는 데이터를 다각형 형태로 담기 위한 변수 선언.
    // boundRect    : 검출한 윤곽선을 감싸는 직사각형 도형을 생성할건데, 직사각형 데이터를 담아주기 위한 준비.
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    int maxArea = 0;

    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        if (area > 500)
        {
            // contours요소의 아크 길이를 리턴. 두번째 요소는 아크가 닫혀있으면(boolean true) 그 값을 리턴하는 뜻.
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.01 * peri, true);
            drawContours(img, conPoly, i, Scalar(255, 0, 0), 3);
            imwrite("data/img_polyDP_001.jpg", img);
        }
    }
}

int main()
{
    img = imread(dataSrc);
    if (img.empty())
    {
        cout << "\n ERROR :: IMG is not LOADED ! " << endl;
    }

    else
    {
        cout << "\n SUCCESS :: IMG is LOADED ! " << endl;
    }

    imgPre = preProcessing(img);
    getContours(imgPre);

    imshow("img", img);
    imshow("imgPre", imgPre);

    rtn = waitKey(0);
    if (rtn == 27)
    {
        return 0;
    }

    return 0;
}