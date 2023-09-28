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

std::u8string   GpEmailEncoder::Encode (void)
{
    Mime();
    Date();
    std::u8string messageId = MessageID();
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

std::u8string   GpEmailEncoder::MessageID (void)
{
    std::u8string addr = iEmail.from.addr;
    std::replace(addr.begin(), addr.end(), '@', '.');
    std::u8string messageId = GpUUID::SGenRandomV4().ToString() + u8"@"_sv + addr;

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
    if (!iEmail.cc.empty())
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
    std::u8string_view  aBoundary
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
        .Bytes(GpHttpProtoHeaders::sContentType.at(NumOps::SConvert<size_t>(GpEnum::value_type(aPart.content_type.Value()))))
        .Bytes("; "_sv).Bytes(GpHttpProtoHeaders::sCharset.at(GpHttpCharset::UTF_8));

    WriteHeaderBegin(GpEmailHeaderType::CONTENT_TRANSFER_ENCODING);
    iWriter.Bytes("base64"_sv);

    //Body
    iWriter.Bytes("\r\n\r\n"_sv);
    GpBase64::SEncode(GpSpanPtrByteR(aPart.data.data(), aPart.data.size()), iWriter, 74);
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
        Part(iEmail.parts.at(0).V(), {});
    } else
    {
        const std::u8string boundary = u8"_----"_sv + GpUUID::SGenRandomV4().ToString();

        WriteHeaderBegin(GpEmailHeaderType::CONTENT_TYPE);
        iWriter
            .Bytes(u8"multipart/alternative; boundary=\""_sv)
            .Bytes(boundary)
            .Bytes(u8"\""_sv);

        for (const auto& part: iEmail.parts)
        {
            iWriter.Bytes(u8"\r\n\r\n"_sv);
            Part(part.V(), boundary);
        }

        //close boundary
        iWriter
            .Bytes(u8"\r\n"_sv)
            .Bytes(u8"--"_sv)
            .Bytes(boundary)
            .Bytes(u8"--"_sv);
    }
}

void    GpEmailEncoder::Addr (const GpEmailAddr& aAddr)
{
    if (aAddr.name.length() > 0)
    {
        EscapeStrBase64(aAddr.name);
        iWriter.Bytes(u8" "_sv);
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
            iWriter.Bytes(u8", "_sv);
        } else
        {
            isFirst = false;
        }

        Addr(addr.V());
    }
}

void    GpEmailEncoder::WriteHeaderBegin
(
    const GpEmailHeaderType::EnumT  aHeader
)
{
    if (!iIsFirstHeader)
    {
        iWriter.Bytes(u8"\r\n"_sv);
    } else
    {
        iIsFirstHeader = false;
    }

    iWriter
        .Bytes(GpEmailHeaders::sHeadersNames.at(NumOps::SConvert<size_t>(GpEnum::value_type(aHeader))))
        .Bytes(u8": "_sv);
}

void    GpEmailEncoder::BraceStr (std::u8string_view aStr)
{
    iWriter
        .Bytes(u8"<"_sv)
        .Bytes(aStr)
        .Bytes(u8">"_sv);
}

void    GpEmailEncoder::EscapeStrBase64 (std::u8string_view aStr)
{
    if (aStr.length() == 0)
    {
        return;
    }

    iWriter.Bytes(u8"=?utf-8?B?"_sv);
    GpBase64::SEncode(aStr, iWriter, 0);
    iWriter.Bytes(u8"?="_sv);
}

}//namespace GPlatform
