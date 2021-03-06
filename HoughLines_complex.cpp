#include <iostream>
#include <typeinfo>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img, imgEdge;
String imgPath = "data/complex.png";

// Hough Tranform Variables.
int rho_unit = 1;
int theta_degree_unit = 1;
int ptr_votes_thres = 20;
double toRadian = (CV_PI / 180);
double toDegree = 1 / toRadian;

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

void drawHoughLines(vector<Vec2f> lines)
{
    for (int i = 0; i < lines.size(); i++)
    {
        // 다시 써보기.
        double rho = lines[i][0], theta = lines[i][1];
        double a = cos(theta);
        double b = sin(theta);

        // 직선 위의 한점을 (x0,y0) 로 정의
        double x0 = rho * a, y0 = rho * b;
        // 직선의 길이를 나타내기 위해, 충분히 큰 값으로 지정한다.
        // 직선의 픽셀 길이 값이 되기 때문에, 이미지 픽셀의 크기에 따라 조절해주면 된다.
        double dL = 1000;

        // 직선을 표현하기 위해 포인트 변수 생성.
        Point pt1, pt2;

        // pt1, pt2의 좌표를 가져오기 위해 동일 직선 위를 움직이는 변위 변수 dL과 cos, sin 함수의 조합을 활용했다.
        pt1.x = x0 - dL * b;
        pt1.y = y0 + dL * a;
        pt2.x = x0 + dL * b;
        pt2.y = y0 - dL * a;

        // 검출된 라인의 속성 값(rho, theta)과 해당 직선의 포인트 값을 출력.
        // cout << "line " << i + 1
        //      << "->> rho : " << lines[i][0] << "px, \t"
        //      << " theta : " << lines[i][1] * (180 / CV_PI)
        //      << "\t Point1(x,y) : " << pt1.x << "\t ," << pt1.y
        //      << "\t Point2(x,y) : " << pt2.x << "\t ," << pt2.y
        //      << endl;

        // 원본 이미지에 검출된 직선을 그림.
        line(img, pt1, pt2, Scalar(0, 0, 200), 3, LINE_AA);
    }
};

void showInformation(Mat img, vector<Vec2f> lines)
{
    cout << "\t-----check picture spec. -----\t" << endl;
    cout << "img pixels --> width : " << img.cols << ", height : " << img.rows << endl;
    cout << "lines : " << lines.size() << endl;
};

void showCurrentParmas(double rho_px, double theta_radian, int ptr_votes_thres)
{
    cout << ">>  rho_px / theta_degree / votes : \t"
         << rho_px << "\t" << theta_radian * toDegree << "\t" << ptr_votes_thres << endl;
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

    // float 값 두개를 가지는 lines 벡터 변수 선언.
    vector<Vec2f> lines;

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

        HoughLines(imgEdge, lines, rho_px, theta_radian, ptr_votes_thres);
        showCurrentParmas(rho_px, theta_radian, ptr_votes_thres);
        drawHoughLines(lines);

        imshow("img", img);
        rtnWaitKey = waitKey(1);
        // imwrite("data/output/HoughLines_complex.png", img);

        if (rtnWaitKey == 27)
        {
            return 0;
        }
    }
}