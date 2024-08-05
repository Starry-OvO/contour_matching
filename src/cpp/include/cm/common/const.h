#pragma once

namespace cm::consts {

constexpr int DFT_SIZE = 2048; // size of DFT cache
constexpr int CODE_LEN = 16;

static_assert(DFT_SIZE >= CODE_LEN + 2, "DFT_SIZE is too small");

} // namespace cm::consts
