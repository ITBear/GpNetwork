#pragma once

#include "../GpNetwork_global.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequest
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpRequest)
    CLASS_DECLARE_DEFAULTS(GpHttpRequest)

public:
                                GpHttpRequest   (void) noexcept;
                                GpHttpRequest   (std::string_view   aUrl,
                                                 std::string_view   aContentType,
                                                 GpBytesArray&&     aBody);
                                ~GpHttpRequest  (void) noexcept;

    std::string_view            Url             (void) const noexcept {return iUrl;}
    std::string_view            ContentType     (void) const noexcept {return iContentType;}
    const GpBytesArray&         Body            (void) const noexcept {return iBody;}
    GpBytesArray&               Body            (void) noexcept {return iBody;}

private:
    std::string                 iUrl;
    std::string                 iContentType;
    GpBytesArray                iBody;
};

}//namespace GPlatform
