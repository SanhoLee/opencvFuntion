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

    HoughLines(imgEdge, lines, rho_thres, theta_thres, ptr_votes_thres);

    cout << "\t-----check picture spec. -----\t" << endl;
    cout << "img pixels --> width : " << imgEdge.cols << ", height : " << imgEdge.rows << endl;
    cout << "lines : " << lines.size() << endl;

    // need to studying this part...
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
        cout << "line " << i + 1
             << "->> rho : " << lines[i][0] << "px, \t"
             << " theta : " << lines[i][1] * (180 / CV_PI)
             << "\t Point1(x,y) : " << pt1.x << "\t ," << pt1.y
             << "\t Point2(x,y) : " << pt2.x << "\t ," << pt2.y
             << endl;

        // 원본 이미지에 검출된 직선을 그림.
        line(img, pt1, pt2, Scalar(0, 0, 200), 3, LINE_AA);
    }

    imshow("img", img);
    imwrite("data/output/HoughLines_ini.png", img);
    waitKey(0);

    return 0;
}