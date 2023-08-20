#include<iostream>
#include<cmath>
#include<vector>
#include<opencv2/opencv.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Align{

static double radians(double t) {
    return t * M_PI / 180.0;
}

static  std::pair<float,float> line(float x0,float y0, float t,float h){
    float x1 = x0 + h* cos(radians(t));
    float y1 = y0 + h* sin(radians(t));
    
    return std::make_pair(x1,y1);
}

static  std::pair<float,float> oval(float h,float k, float a,float t,float c,float b){
    float x = h + a * cos(t) * cos(c) + b * sin(t) * sin(c);
    float y = k + a * cos(t) * sin(c) - b * sin(t) * cos(c);

    return std::make_pair(x,y);
}
// static  std::pair<float,float> oval(float h,float k, float a,float t,float c,float b){
//     float x = h + a * cos(t) * cos(c) + b * sin(t) * sin(c);
//     float y = k + a * cos(t) * sin(c) - b * sin(t) * cos(c);

//     return std::make_pair(x,y);
// }


static int elliptical_arc_length( float a, float  b, float  theta1, float  theta2){
    float h = std::pow(((a - b) / (a + b)), 2);
    float L = M_PI * (a + b) * (1 + ((3 * h) / (10 + sqrt(4 - 3 * h))));
    return L * abs(theta2 - theta1) / (2 * M_PI);
}

static std::vector<float> linspace(float start, float end, int num){

    std::vector<float> points(num);
    float step = static_cast<float>(end - start) / (num - 1);

    for (int i = 0; i < num; ++i) {
        points[i] = start + i * step;
    }

    return points;
}



public:

Align(int width,int height,bool adaptive=true){
    this->adaptive = adaptive;
    if (adaptive){
        this->width = 0;
        this->height = 0;
    }else{
        this->width = width;
        this->height = height;
    }
}

cv::Mat run(cv::Mat image, json item){
    bool la = item["la"];
    bool mu = item["mu"];

    if(la == true){
        return arc_align(image,item);
    }else if (mu == true){
        return line_align(image,item);
    }

}

private:

int width;
int height;
bool adaptive;


cv::Mat arc_align(cv::Mat image, json item){
    float ro = radians(item["rotation"].get<float>());
    float start_angle = radians(item["startAngle"].get<float>() / 16);
    float span_angle = radians(item["spanAngle"].get<float>()/16 );

    this->width = Align::elliptical_arc_length(item["a"].get<float>(),item["b"].get<float>(),start_angle,start_angle + span_angle);
    this->height = item["h"];
    // 坐标圆点
    std::vector<float> cc = {item["rect"][2].get<float>() / 2 + item["x"].get<float>() + 
    item["rect"][0].get<float>(),item["rect"][3].get<float>() / 2 + +item["y"].get<float>() + item["rect"][1].get<float>()};
    std::vector<float> xx = linspace(start_angle, start_angle + span_angle, this->width);
    std::vector<float> aa = linspace(item["a"].get<float>() - (item["h"].get<float>() / 2),item["a"].get<float>() + (item["h"].get<float>() / 2), this->height);
    std::reverse(aa.begin(), aa.end());
    std::vector<float> bb = linspace(item["b"].get<float>() - (item["h"].get<float>() / 2), item["b"].get<float>() + (item["h"].get<float>() / 2), this->height);
    std::reverse(bb.begin(), bb.end());

    // 创建 x 和 y 映射矩阵
    cv::Mat map_x, map_y;
    // cv::Mat dst(this->height, this->width, CV_8UC1);;
    map_x.create(cv::Size(this->width, this->height), CV_32FC1);
    map_y.create(cv::Size(this->width, this->height), CV_32FC1);


    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            std::pair<float,float> xy = Align::oval(cc[0],cc[1],aa[j],xx[i],ro,bb[j]);
            // int j = int(xy.second);
            // int i = int(xy.first);
            map_x.at<float>(j, i) = static_cast<float>(xy.first);
            map_y.at<float>(j, i) = static_cast<float>(xy.second);
            // map_y.at<float>(int(xy.second), int(xy.first)) = static_cast<float>(row);
        }
    }

    cv::Mat dst;
    cv::remap(image, dst, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());


    cv::Mat result;
    cv::flip(dst, result ,1);
    return result;

}

cv::Mat line_align(cv::Mat image,json item){
    this->height = item["h"];
    this->width = item["l"];
    int l = item["l"];
    float x0 = item["rect"][2].get<float>() / 2 + item["x"].get<float>() + item["rect"][0].get<float>();
    float y0 = item["rect"][3].get<float>() / 2 + +item["y"].get<float>() + item["rect"][1].get<float>();
    int h = item["h"];
    float t = item["rotation"];
    std::vector<float> hh = linspace(-h / 2, h / 2, this->height);
    std::vector<float> ll = linspace(-l / 2, l / 2, this->width);
    std::reverse(ll.begin(), ll.end());

    // 创建 x 和 y 映射矩阵
    cv::Mat map_x, map_y;
    // cv::Mat dst(this->height, this->width, CV_8UC1);;
    map_x.create(cv::Size(this->width, this->height), CV_32FC1);
    map_y.create(cv::Size(this->width, this->height), CV_32FC1);


    for(int j=0;j<this->height;j++){
        float tx = x0 - hh[j] * cos(M_PI/2 - radians(t));
        float ty = y0 + hh[j] * sin(M_PI/2 - radians(t));
        for(int i=0;i<this->width;i++){
            std::pair<float,float> xy = Align::line(tx,ty,t,ll[i]);
            map_x.at<float>(j, i) = static_cast<float>(xy.first);
            map_y.at<float>(j, i) = static_cast<float>(xy.second);
        }
    }

    cv::Mat dst;
    cv::remap(image, dst, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

    cv::Mat result;
    cv::flip(dst, result ,1);
    return result;

}


};