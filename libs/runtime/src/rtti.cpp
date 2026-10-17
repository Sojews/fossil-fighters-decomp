// Metrowerks C++ runtime: type_info classes and dynamic_cast.

#include "types.h"
#include <string.h>

namespace std {

class type_info {
public:
    virtual ~type_info();
    bool operator==(const type_info &rhs) const;

    const char *__type_name;
};

}

namespace __cxxabiv1 {

class __class_type_info : public std::type_info {
public:
    virtual ~__class_type_info();
};

class __si_class_type_info : public __class_type_info {
public:
    virtual ~__si_class_type_info();

    const __class_type_info *__base_type;
};

struct __base_class_type_info {
    const __class_type_info *__base_type;
    s32 __offset_flags;     // offset << 8, bit 0: virtual, bit 1: public
};

class __vmi_class_type_info : public __class_type_info {
public:
    virtual ~__vmi_class_type_info();

    u32 __flags;
    s32 __base_count;
    __base_class_type_info __base_info[1];
};

}

using namespace __cxxabiv1;

// The start of a vtable: offset of the subobject in the complete object, then its type.
struct VTableHead {
    s32 offset_to_top;
    const std::type_info *type;
};

extern "C" VTableHead _ZTVN10__cxxabiv120__si_class_type_infoE;
extern "C" VTableHead _ZTVN10__cxxabiv121__vmi_class_type_infoE;

static inline VTableHead *GetVTableHead(const void *object) {
    return (VTableHead *)(*(char **)object - 8);
}

// Whether a type_info object with this vtable is of the class whose vtable is given.
static inline bool IsKind(VTableHead *vt, VTableHead *kind) {
    if (vt == kind) {
        return true;
    }
    if (vt != NULL) {
        const std::type_info *a = vt->type;
        const std::type_info *b = kind->type;
        if (a != NULL) {
            return strcmp((char *)a->__type_name, (char *)b->__type_name) == 0;
        }
    }
    return false;
}

struct DynamicCastSearch {
    void *object;                   // the object being cast
    const std::type_info *src_type; // its static type
    const std::type_info *dst_type;
    char *dst;                      // a dst_type subobject that is not above the object
    bool dst_public;
    bool dst_ambiguous;
    bool object_public;
    char *result;                   // a dst_type subobject the object derives from
    bool result_public;
    bool result_ambiguous;
    char *current_dst;              // the dst_type subobject on the path being walked
    bool current_dst_set;
    bool path_public;
};

std::type_info::~type_info() {}

bool std::type_info::operator==(const type_info &rhs) const {
    return this == &rhs || strcmp((char *)__type_name, (char *)rhs.__type_name) == 0;
}

__cxxabiv1::__class_type_info::~__class_type_info() {}

__cxxabiv1::__si_class_type_info::~__si_class_type_info() {}

__cxxabiv1::__vmi_class_type_info::~__vmi_class_type_info() {}

extern "C" {

void func_02087e48(char *object, const std::type_info *type, DynamicCastSearch *s) {
    if (s->object == object && *s->src_type == *type) {
        if (s->path_public) {
            s->object_public = true;
        }
        if (s->current_dst != NULL) {
            if (s->result == NULL) {
                s->result = s->current_dst;
            } else if (s->result != s->current_dst) {
                s->result_ambiguous = true;
            }
            if (s->current_dst_set) {
                s->result_public = true;
            }
        }
    } else {
        char *current_dst = s->current_dst;
        bool current_dst_set = s->current_dst_set;
        bool path_public = s->path_public;

        if (*s->dst_type == *type) {
            if (s->dst == NULL) {
                s->dst = object;
            } else if (s->dst != object) {
                s->dst_ambiguous = true;
            }
            if (s->path_public) {
                s->dst_public = true;
            }
            s->current_dst = object;
            s->current_dst_set = true;
        }
        VTableHead *vt = GetVTableHead(type);
        if (IsKind(vt, &_ZTVN10__cxxabiv121__vmi_class_type_infoE)) {
            const __vmi_class_type_info *vmi = (const __vmi_class_type_info *)type;
            const __base_class_type_info *base;
            int count;
            bool set;
            bool pub;
            int i;

            i = 0;
            set = s->current_dst_set;
            pub = s->path_public;
            count = vmi->__base_count;
            if (count > 0) {
                base = vmi->__base_info;
                do {
                    s32 offset;
                    if (!(base->__offset_flags & 2)) {
                        s->current_dst_set = false;
                        s->path_public = false;
                    }
                    if (base->__offset_flags & 1) {
                        offset = *(s32 *)(*(char **)object + (base->__offset_flags >> 8));
                    } else {
                        offset = base->__offset_flags >> 8;
                    }
                    func_02087e48(object + offset, base->__base_type, s);
                    s->current_dst_set = set;
                    s->path_public = pub;
                    i++;
                    base++;
                } while (i < count);
            }
        } else if (IsKind(vt, &_ZTVN10__cxxabiv120__si_class_type_infoE)) {
            func_02087e48(object, ((const __si_class_type_info *)type)->__base_type, s);
        }
        s->current_dst = current_dst;
        s->current_dst_set = current_dst_set;
        s->path_public = path_public;
    }
}

void *__dynamic_cast(void *object, const std::type_info *src_type, const std::type_info *dst_type) {
    VTableHead *vt = GetVTableHead(object);
    DynamicCastSearch s;

    if (vt->type == NULL) {
        return NULL;
    }
    s.dst_type = dst_type;
    s.src_type = src_type;
    s.object = object;
    s.dst = NULL;
    s.dst_public = false;
    s.dst_ambiguous = false;
    s.object_public = false;
    s.result = NULL;
    s.result_public = false;
    s.result_ambiguous = false;
    s.current_dst = NULL;
    s.current_dst_set = false;
    s.path_public = true;
    func_02087e48((char *)object + vt->offset_to_top, vt->type, &s);
    if (s.result != NULL && s.result_public && !s.result_ambiguous) {
        return s.result;
    }
    if (s.dst != NULL && s.dst_public && s.object_public && !s.dst_ambiguous) {
        return s.dst;
    }
    return NULL;
}

}
