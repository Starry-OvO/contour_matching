#pragma once

namespace cm::err {

constexpr int OK = 0x0000;

constexpr int OUT_OF_MEMORY = 0x0001;

constexpr int EMPTY_IMAGE = 0x0010;
constexpr int INVALID_DEPTH = 0x0011;
constexpr int INVALID_CHANNELS = 0x0012;

constexpr int NO_CONTOUR = 0x0020;

} // namespace cm::error
