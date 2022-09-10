#pragma once

#include "GpNetworkEmailCore_global.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_EMAIL_CORE_API, GpEmailHeaderType,
    CC,
    CONTENT_TRANSFER_ENCODING,
    CONTENT_TYPE,
    DATE,
    FROM,
    LIST_UNSUBSCRIBE,
    MESSAGE_ID,
    MIME_VERSION,
    SUBJECT,
    TO
);

}//namespace GPlatform
