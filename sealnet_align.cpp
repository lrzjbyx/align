#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include "align.cpp"

namespace py = pybind11;
using json = nlohmann::json;

cv::Mat align(cv::Mat mat, json jsonObject) {

    Align align(600,100);
    cv::Mat result = align.run(mat,jsonObject);

    return result;
}


PYBIND11_MODULE(sealnet_align, m) {
    m.def("align", [](py::array_t<unsigned char> input, py::dict json_data) {
        py::object json_module = py::module::import("json");
        py::str json_str = json_module.attr("dumps")(json_data);
        cv::Mat image = cv::Mat(input.shape(0), input.shape(1), CV_8UC3, input.mutable_data());
        json data = json::parse(json_str.cast<std::string>().c_str());
        cv::Mat result = align(image, data);
        std::vector<size_t> shape = { static_cast<size_t>(result.rows), static_cast<size_t>(result.cols), static_cast<size_t>(result.channels())};
        return py::array_t<unsigned char>(shape, result.data);
    });
}
