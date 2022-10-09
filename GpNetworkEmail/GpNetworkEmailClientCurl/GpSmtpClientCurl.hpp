#pragma once

#include "GpNetworkEmailClientCurl_global.hpp"

typedef void CURL;

namespace GPlatform {

class GP_NETWORK_EMAIL_CLIENT_CURL_API GpSmtpClientCurl final: public GpSmtpClient
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSmtpClientCurl)
    CLASS_DD(GpSmtpClientCurl)

public:
                                    GpSmtpClientCurl    (void) noexcept;
    virtual                         ~GpSmtpClientCurl   (void) noexcept override final;

    virtual std::string/*msg_id*/   Send                (const GpEmail& aEmail) override final;

    virtual bool                    IsValid             (void) const noexcept override final;

private:
    void                            CurlInit            (void);
    void                            CurlClear           (void) noexcept;

    void                            FillAddrs           (void*                          aCurlList,
                                                         const GpEmailAddr::C::Vec::SP& aAddrList) const;

private:
    CURL*                           iCurl = nullptr;
};

}//namespace GPlatform
