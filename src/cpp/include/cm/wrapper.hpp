#pragma once

#include <opencv2/imgcodecs.hpp>

#include "common/const.h"
#include "common/defines.h"
#include "encoder/fourier.impl.hpp"

#ifdef __cplusplus
extern "C" {
#endif

CM_API inline void* c_decode_jpeg_from_bytes(const uint8_t* p_bytes, size_t len)
{
    auto dst = new cv::Mat;
    cv::Mat bytes(1, static_cast<int>(len), CV_8UC1, (void*)p_bytes);
    cv::imdecode(bytes, cv::IMREAD_GRAYSCALE, dst);

    if (0 == dst->rows) {
        dst = nullptr;
    }

    return static_cast<void*>(dst);
}

CM_API inline void c_destruct_image(void* vp_image) { delete static_cast<cv::Mat*>(vp_image); }

CM_API inline void* c_construct_fourier_encoder() { return static_cast<void*>(new cm::FourierEncoder()); }

CM_API inline void c_destruct_fourier_encoder(void* vp_encoder) { delete static_cast<cm::FourierEncoder*>(vp_encoder); }

CM_API inline void c_encode(void* vp_encoder, const void* p_src, float* p_dst)
{
    auto p_encoder = static_cast<cm::FourierEncoder*>(vp_encoder);

    auto src = static_cast<const cv::Mat*>(p_src);
    cv::Mat temp;
    p_encoder->encode(*src, temp);

    cv::Mat dst(1, cm::consts::CODE_LEN, CV_32FC1, (void*)p_dst);
    temp.copyTo(dst);
}

#ifdef __cplusplus
}
#endif