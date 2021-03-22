#include <iostream>
#include <typeinfo>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img, imgEdge;
String imgPath = "data/squre.png";

int isEmptyImg(Mat img)
{
    if (img.empty())
    {
        cout << " FAILED::IMG LOADING " << endl;
        return -1;
    }
    else
    {
        cout << " SUCCESS::IMG LOADING " << endl;
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

int main()
{
    img = imread(imgPath);
    isEmptyImg(img);

    // 1. preprocessing img.
    imgEdge = preProcessing(img);

    // 2. Hough line Transform
    // float 값 두개를 가지는 lines 벡터 변수 선언.
    vector<Vec2f> lines;

    /* 이 값 설정으로 근처에 있는 서로 다른 직선을 구별한다
    . rho_thres       : 각도가 같은 경우, 직선의 평행 방향으로 인식 할 수 있는 최대 경계값을 정해준다.
    . theta_thres     : rho값이 일정할 경우, 직선 으로 인식 할 수 있는 최대 경계값을 설정해준다.
    --> 위의 thres 값은 클수록 불필요한 직선들이 많이 검출된다. 노이즈가 커지는 방향
    . ptr_votes_thres : 직선으로 인식하기 위한 캐니 엣지 상의 픽셀 갯수를 의미한다. 이 값이 너무 작으면, 길이가 짧은 엣지들은
                        그 만큼 캐니엣지 검출로 검출된 픽셀수가 적기 때문에 직선으로 구별해내지 못하게 된다.
     */
    double rho_thres = 0.2;
    double theta_thres = (CV_PI / 180);
    int ptr_votes_thres = 20;

    HoughLines(imgEdge, lines, rho_thres, theta_thres, ptr_votes_thres, 0, 0);

    cout << "lines : " << lines.size() << endl;

    // need to studying this part...
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(img, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
        cout << "line " << i + 1 << "->> rho : " << lines[i][0] << "px, \t"
             << " theta : " << lines[i][1] * (180 / CV_PI) << endl;
    }

    imshow("img", img);
    waitKey(0);

    return 0;
}