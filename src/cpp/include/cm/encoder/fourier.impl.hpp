#pragma once

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "cm/common/const.h"
#include "cm/common/defines.h"
#include "cm/common/error.h"

namespace cm {

/**
 * @brief Fourier Factor Encoder
 * @warning DO NOT use with multi-thread
 */
class FourierEncoder
{
public:
    CM_API FourierEncoder() : dft_complex_(1, consts::DFT_SIZE, CV_32FC2), thresholded_src_(){};
    FourierEncoder(FourierEncoder&) = delete;
    FourierEncoder(FourierEncoder&&) = delete;

    /**
     * @brief Encode the fourier factors of given image
     * @param src High contrast image. The major item must be black
     * @param code Fourier factors
     * @return !0 if any error
     */
    CM_API int encode(const cv::Mat& src, cv::Mat& code);

private:
    /**
     * @brief Compute the fourier factors of given contour
     * @param contour ...
     * @param code Rotational invariant Fourier factors
     */
    inline void _computeFourierFactors(const std::vector<cv::Point>& contour, cv::Mat& code);

    cv::Mat dft_complex_;     // DFT cache
    cv::Mat thresholded_src_; // threshold cache
};

/**
 * @brief Find out the main contour in `src`
 * @param src Must be type of 8UC1
 * @param contour The main contour
 */
void _find_contour(const cv::Mat& src, std::vector<cv::Point>& contour)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(src, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);

    for (size_t i = 0; i < contours.size(); i++) {
        if (32ull > contours[i].size() || -1 != hierarchy[i][3]) {
            continue; // points less than 32 or **has parent** (is inner contour)
        }
        auto total_size = static_cast<float>(src.rows * src.cols);
        float upper_size = total_size * (0.95f * 0.95f);
        float lower_size = total_size * (0.1f * 0.1f);
        cv::RotatedRect contour_rect = cv::minAreaRect(contours[i]);
        if (lower_size < contour_rect.size.area() && upper_size > contour_rect.size.area()) {
            // should never access `contours` after this `move` operation
            contour = std::move(contours[i]);
            break;
        }
    }
}

int FourierEncoder::encode(const cv::Mat& src, cv::Mat& code)
{
    if (src.empty() || CV_8U != src.depth()) {
        return err::INVALID_DEPTH;
    }

    if (1 != src.channels()) {
        cv::cvtColor(src, thresholded_src_, cv::COLOR_BGR2GRAY);
        cv::threshold(thresholded_src_, thresholded_src_, 0x7f, 0xff, cv::THRESH_BINARY_INV);
    } else {
        cv::threshold(src, thresholded_src_, 0x7f, 0xff, cv::THRESH_BINARY_INV);
    }

    std::vector<cv::Point> accept_contour;
    _find_contour(thresholded_src_, accept_contour);
    if (accept_contour.empty()) {
        return err::NO_CONTOUR;
    }

    _computeFourierFactors(accept_contour, code);

    return err::OK;
}

void FourierEncoder::_computeFourierFactors(const std::vector<cv::Point>& contour, cv::Mat& code)
{
    // Reinterpret data from `contour` to `i_complex`
    // OpenCV: We cannot create a `const Mat` from a `const pointer`. Hence, we must use `const_cast` here.
    const cv::Mat i_complex(1, static_cast<int>(contour.size()), CV_32SC2, const_cast<cv::Point*>(contour.data()));
    // The method below is much slower due to the memcpy
    // cv::Mat_<cv::Point> i32ComplexMat(acceptContour, false);
    // i32ComplexMat = i32ComplexMat.reshape(0, 1);
    cv::Mat f_complex;
    i_complex.convertTo(f_complex, CV_32FC2);
    cv::resize(f_complex, dft_complex_, dft_complex_.size(), 0., 0., cv::INTER_LINEAR);
    cv::dft(dft_complex_, dft_complex_, cv::DFT_ROWS, dft_complex_.rows);

    cv::Mat channels[2];
    // amp[0] is DC component
    // amp[1] is used for normalization
    // thus we need `CODE_LEN + 2` factors
    cv::Mat dft_complex_roi = dft_complex_(cv::Range::all(), cv::Range(1, 2 + consts::CODE_LEN));
    cv::split(dft_complex_roi, channels);

    cv::Mat& chan_re = channels[0];
    cv::Mat& chan_re_squared = chan_re;
    cv::multiply(chan_re, chan_re, chan_re_squared); // memory of `chan_re` is reused

    cv::Mat& chan_im = channels[1];
    cv::Mat& chan_im_squared = chan_im;
    cv::multiply(chan_im, chan_im, chan_im_squared); // memory of `chan_im` is reused

    cv::Mat& amp_squared = chan_re;
    cv::Mat& amp = chan_re;
    cv::add(chan_re_squared, chan_im_squared, amp_squared); // memory of `chan_re` is reused
    cv::sqrt(amp_squared, amp);                             // memory of `chan_re` is reused

    float ampFirst = amp.at<float>(0, 0);
    amp /= ampFirst;                                                  // normalization
    code = amp(cv::Range::all(), cv::Range(1, 1 + consts::CODE_LEN)); // amp[1]===1.0, so just drop it
}

} // namespace cm
