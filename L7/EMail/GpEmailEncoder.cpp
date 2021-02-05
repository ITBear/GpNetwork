#include "GpEmailEncoder.hpp"

namespace GPlatform {

GpEmailEncoder::GpEmailEncoder
(
    const GpEmail&  aEmail,
    GpByteWriter&   aWriter
) noexcept:
iEmail(aEmail),
iWriter(aWriter)
{
}

GpEmailEncoder::~GpEmailEncoder (void) noexcept
{
}

std::string GpEmailEncoder::Encode (void)
{
    Mime();
    Date();
    std::string messageId = MessageID();
    From();
    To();
    Cc();
    Subject();
    LinkUnsubscribe();
    Parts();

    return messageId;
}

void    GpEmailEncoder::Mime (void)
{
    WriteHeaderBegin(GpEmailHeaderType::MIME_VERSION);
    iWriter.Bytes("1.0"_sv);
}

void    GpEmailEncoder::Date (void)
{
    WriteHeaderBegin(GpEmailHeaderType::DATE);
    iWriter.Bytes(GpDateTimeOps::SUnixTsToStr(GpDateTimeOps::SUnixTS_s(), GpDateTimeFormat::RFC_2822));
}

std::string GpEmailEncoder::MessageID (void)
{
    std::string messageId = GpUUID::SGenRandom().ToString() + "@"_sv + StrOps::SReplaceAll(iEmail.from.addr, '@', '.');

    WriteHeaderBegin(GpEmailHeaderType::MESSAGE_ID);
    BraceStr(messageId);

    return messageId;
}

void    GpEmailEncoder::From (void)
{
    WriteHeaderBegin(GpEmailHeaderType::FROM);
    Addr(iEmail.from);
}

void    GpEmailEncoder::To (void)
{
    WriteHeaderBegin(GpEmailHeaderType::TO);
    Addrs(iEmail.to);
}

void    GpEmailEncoder::Cc (void)
{
    if (iEmail.cc.size() > 0)
    {
        WriteHeaderBegin(GpEmailHeaderType::CC);
        Addrs(iEmail.cc);
    }
}

void    GpEmailEncoder::Subject (void)
{
    WriteHeaderBegin(GpEmailHeaderType::SUBJECT);
    EscapeStrBase64(iEmail.subject);
}

void    GpEmailEncoder::LinkUnsubscribe (void)
{
    if (iEmail.link_unsubscribe.length() > 0)
    {
        WriteHeaderBegin(GpEmailHeaderType::LIST_UNSUBSCRIBE);
        BraceStr(iEmail.link_unsubscribe);
    }
}

void    GpEmailEncoder::Part
(
    const GpEmailPart&  aPart,
    std::string_view    aBoundary
)
{
    //boundary
    if (aBoundary.length() > 0)
    {
        iWriter
            .Bytes("--"_sv)
            .Bytes(aBoundary);
    }

    WriteHeaderBegin(GpEmailHeaderType::CONTENT_TYPE);
    iWriter
        .Bytes(GpProtoHeaders::sContentType.at(aPart.content_type.Value()))
        .Bytes("; "_sv).Bytes(GpProtoHeaders::sCharset.at(GpCharset::UTF_8));

    WriteHeaderBegin(GpEmailHeaderType::CONTENT_TRANSFER_ENCODING);
    iWriter.Bytes("base64"_sv);

    //Body
    iWriter.Bytes("\r\n\r\n"_sv);
    GpBase64::SEncode(aPart.data, iWriter, 74_cnt);
}

void    GpEmailEncoder::Parts (void)
{
    const size_t partsCount = iEmail.parts.size();

    if (partsCount == 0)
    {
        return;
    }

    if (partsCount == 1)
    {
        Part(iEmail.parts.at(0).VC(), ""_sv);
    } else
    {
        const std::string boundary = "_----"_sv + GpUUID::SGenRandom().ToString();

        WriteHeaderBegin(GpEmailHeaderType::CONTENT_TYPE);
        iWriter
            .Bytes("multipart/alternative; boundary=\""_sv)
            .Bytes(boundary)
            .Bytes("\""_sv);

        for (const auto& part: iEmail.parts)
        {
            iWriter.Bytes("\r\n\r\n"_sv);
            Part(part.VC(), boundary);
        }

        //close boundary
        iWriter
            .Bytes("\r\n"_sv)
            .Bytes("--"_sv)
            .Bytes(boundary)
            .Bytes("--"_sv);
    }
}

void    GpEmailEncoder::Addr (const GpEmailAddr& aAddr)
{
    if (aAddr.name.length() > 0)
    {
        EscapeStrBase64(aAddr.name);
        iWriter.Bytes(" "_sv);
    }

    BraceStr(aAddr.addr);
}

void    GpEmailEncoder::Addrs (const GpEmailAddr::C::Vec::SP& aAddr)
{
    bool isFirst = true;
    for (const auto& addr: aAddr)
    {
        if (!isFirst)
        {
            iWriter.Bytes(", "_sv);
        } else
        {
            isFirst = false;
        }

        Addr(addr.VC());
    }
}

void    GpEmailEncoder::WriteHeaderBegin
(
    const GpEmailHeaderType::EnumT  aHeader
)
{
    if (!iIsFirstHeader)
    {
        iWriter.Bytes("\r\n"_sv);
    } else
    {
        iIsFirstHeader = false;
    }

    iWriter
        .Bytes(GpEmailHeaders::sHeadersNames.at(aHeader))
        .Bytes(": "_sv);
}

void    GpEmailEncoder::BraceStr (std::string_view aStr)
{
    iWriter
        .Bytes("<"_sv)
        .Bytes(aStr)
        .Bytes(">"_sv);
}

void    GpEmailEncoder::EscapeStrBase64 (std::string_view aStr)
{
    if (aStr.length() == 0)
    {
        return;
    }

    iWriter.Bytes("=?utf-8?B?"_sv);
    GpBase64::SEncode(aStr, iWriter, 0_cnt);
    iWriter.Bytes("?="_sv);
}

}//namespace GPlatform
