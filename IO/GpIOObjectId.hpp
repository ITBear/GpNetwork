#pragma once

#include "../GpNetwork_global.hpp"

namespace GPlatform {

class GpIOObjectId
{
public:
    using RawT = s_int_64;

public:
                    GpIOObjectId    (void) noexcept {}
                    GpIOObjectId    (const GpIOObjectId& aId) noexcept: iValue(aId.iValue) {}
                    GpIOObjectId    (GpIOObjectId&& aId) noexcept: iValue(aId.iValue) {}

    template<typename T>
                    GpIOObjectId    (T aId) noexcept {Set<T>(aId);}

                    ~GpIOObjectId   (void) noexcept {}

    template<typename T>
    T               As              (void) const noexcept;

    template<typename T>
    void            Set             (T aId) noexcept;

    RawT            RawValue        (void) const noexcept {return iValue;}

private:
    RawT            iValue  = 0;
};

template<typename T>
T   GpIOObjectId::As (void) const noexcept
{
    static_assert(   (sizeof(T) <= sizeof(RawT))
                  && (alignof(T) <= alignof(RawT)));

    T dst;
    std::memcpy(&dst, &iValue, sizeof(T));
    return dst;
}

template<typename T>
void    GpIOObjectId::Set (T aId) noexcept
{
    static_assert(   (sizeof(T) <= sizeof(RawT))
                  && (alignof(T) <= alignof(RawT)));

    std::memcpy(&iValue, &aId, sizeof(T));
}

}//namespace GPlatform
