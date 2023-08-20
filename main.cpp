// // #include <opencv2/opencv.hpp>
// // #include <iostream>

// // int main() {
// //     // 读取图像
// //     cv::Mat img = cv::imread("/home/zzu/scene_seal/align/t0303png.png");

// //     // 检查图像是否正确读取
// //     std::cout<<img.size()<<std::endl;
// //     std::cout << "图像宽度: " << img.size().width << std::endl;
// //     std::cout << "图像高度: " << img.size().height << std::endl;
    




// //     // 等待用户按键，然后关闭窗口
// //     cv::waitKey(0);
// //     return 0;
// // }


// #include <opencv2/opencv.hpp>
// #include <fstream>
// #include <nlohmann/json.hpp>
// #include "align.cpp"
// using json = nlohmann::json;

// int main2() {
//     // 读取图像
//     cv::Mat src = cv::imread("/home/zzu/scene_seal/align/t0303png.png");

//     // 创建 x 和 y 映射矩阵
//     cv::Mat map_x, map_y;
//     map_x.create(src.size(), CV_32FC1);
//     map_y.create(src.size(), CV_32FC1);

//     // 以任意方式填充映射矩阵，这里我们只是创建一个简单的线性映射
//     for (int j = 0; j < src.rows; ++j) {
//         for (int i = 0; i < src.cols; ++i) {
//             map_x.at<float>(j, i) = static_cast<float>(i);
//             map_y.at<float>(j, i) = static_cast<float>(j);
//         }
//     }

//     // 使用 remap 进行插值
//     cv::Mat dst;
//     cv::remap(src, dst, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());

//     // 显示图像
//     // cv::imshow("Remapped Image", dst);
//     // cv::waitKey(0);
//     cv::imwrite("dst.png",dst);

//     return 0;
// }



// int main(){
//     cv::Mat src = cv::imread("/home/zzu/scene_seal/align/image.png");
//     Align align(600,100);

//     std::string jsonStringArc = R"({
                "la":true,
                "mu":false,
                "x": 11.64244186046517,
                "y": 328.3168604651162,
                "rect": [
                    95.0,
                    -219.0,
                    584.0755813953488,
                    584.0755813953488
                ],
                "rotation": 0,
                "text": "郑州大学档案项目组测试专用章",
                "sequence": "从左到右",
                "type": 5,
                "startAngle": 5152,
                "spanAngle": 4112,
                "a": 292.0377906976744,
                "b": 292.0377906976744,
                "h": 154
//             }
//         )";
    
//     std::string jsonStringLine = R"({
//                 "la":false,
//                 "mu":true,
//                 "x": 83.8255813953487,
//                 "y": 728.8168604651161,
//                 "rect": [
//                     154.0,
//                     -119.0,
//                     325.6133720930233,
//                     10.0
//                 ],
//                 "rotation": 0,
//                 "text": "测试专用章",
//                 "type": 2,
//                 "sequence": "从左到右",
//                 "l": 325.6133720930233,
//                 "h": 97
// }
//         )";


//     std::cout<<jsonStringLine<<std::endl;
//     json jsonObject = json::parse(jsonStringLine);

//     // json jsonObject = json::parse(jsonString);

//     cv::Mat result = align.run(src,jsonObject);

//     cv::imwrite("111.png",result);


//     return 0;
// }
#include <pybind11/pybind11.h>

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(libexample, m) {
    m.def("add", &add, "A function which adds two numbers");
}