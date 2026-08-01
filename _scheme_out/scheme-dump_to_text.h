// WARNING! All changes made in this file will be lost!
// Created from 'mtproto.tl', 'api.tl' by 'generate.py'
//
#pragma once

#include "mtproto/core_types.h"

namespace MTP::details {

struct DumpToTextBuffer;

[[nodiscard]] bool DumpToTextType(DumpToTextBuffer &to, const mtpPrime *&from, const mtpPrime *end, mtpPrime cons = 0, uint32 level = 0, mtpPrime vcons = 0);

} // namespace MTP::details
