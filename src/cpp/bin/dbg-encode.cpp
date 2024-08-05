#include <filesystem>
#include <iostream>

#include "cm.hpp"

namespace fs = std::filesystem;

int main()
{
    const fs::path imgpath{R"(D:\code\cpp\ContourMatching\test_image\20220318182259.jpg)"};
    const cv::Mat img = cv::imread(imgpath.string());

    auto encoder = cm::FourierEncoder();
    cv::Mat code;
    encoder.encode(img, code);

    std::cout << "code: " << code << std::endl;
}
