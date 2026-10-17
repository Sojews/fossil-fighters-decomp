// Metrowerks C++ runtime: construction and destruction of arrays of class objects.

#include "types.h"
#include <exception>
#include <stddef.h>

typedef void (*ConstructorDestructor)(void *);

extern "C" {

// Destroys the constructed elements [begin, end) in reverse order.
void func_02087bbc(char *begin, char *end, size_t size, ConstructorDestructor dtor) {
    try {
        while (end > begin) {
            end -= size;
            dtor(end);
        }
    } catch (...) {
        std::terminate();
    }
}

void *__cxa_vec_new(size_t count, size_t size, size_t padding, ConstructorDestructor ctor, ConstructorDestructor dtor) {
    char *mem = (char *)::operator new[](padding + count * size);
    char *p;

    if (mem == NULL) {
        return NULL;
    }
    if (padding) {
        *(size_t *)(mem + padding - 4) = count;
        if (padding >= 8) {
            *(size_t *)(mem + padding - 8) = size;
        }
    }
    if (ctor) {
        p = mem + padding;
        try {
            for (; count; count--) {
                ctor(p);
                p += size;
            }
        } catch (...) {
            if (dtor) {
                func_02087bbc(mem + padding, p, size, dtor);
            }
            ::operator delete[](mem);
            throw;
        }
    }
    return mem + padding;
}

void __cxa_vec_ctor(void *array, size_t count, size_t size, ConstructorDestructor ctor, ConstructorDestructor dtor) {
    char *p;

    if (ctor) {
        if (dtor) {
            p = (char *)array;
            try {
                for (; count; count--) {
                    ctor(p);
                    p += size;
                }
            } catch (...) {
                func_02087bbc((char *)array, p, size, dtor);
                throw;
            }
        } else {
            for (; count; count--) {
                ctor(array);
                array = (char *)array + size;
            }
        }
    }
}

void __cxa_vec_dtor(void *array, size_t count, size_t size, ConstructorDestructor dtor) {
    char *p;

    if (dtor) {
        p = (char *)array + count * size;
        try {
            for (; count; count--) {
                p -= size;
                dtor(p);
            }
        } catch (...) {
            try {
                while (--count) {
                    p -= size;
                    dtor(p);
                }
            } catch (...) {
                std::terminate();
            }
            throw;
        }
    }
}

void __cxa_vec_cleanup(void *array, size_t count, size_t size, ConstructorDestructor dtor) {
    char *p;

    if (dtor) {
        try {
            p = (char *)array + count * size;
            for (; count; count--) {
                p -= size;
                dtor(p);
            }
        } catch (...) {
            std::terminate();
        }
    }
}

void __cxa_vec_delete(void *array, size_t size, size_t padding, ConstructorDestructor dtor) {
    if (array) {
        if (dtor) {
            __cxa_vec_dtor(array, ((size_t *)array)[-1], size, dtor);
        }
        ::operator delete[]((char *)array - padding);
    }
}

}
