#pragma once

struct UnkMSLStruct1 {};

namespace Metrowerks {
// Elements the vector moves around with memmove once the erased ones are destroyed (std::basic_string).
template <class T> struct __vec_relocatable {
    static const bool value = false;
};
}
