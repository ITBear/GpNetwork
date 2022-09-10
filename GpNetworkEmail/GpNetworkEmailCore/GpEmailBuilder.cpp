#include "GpEmailBuilder.hpp"

namespace GPlatform {

GpEmailBuilder::GpEmailBuilder (void) noexcept
{
}

GpEmailBuilder::~GpEmailBuilder (void) noexcept
{
}

GpEmailBuilder& GpEmailBuilder::From
(
    std::string_view aName,
    std::string_view aAddress
)
{
    auto& from  = _Email().from;

    from.name   = aName;
    from.addr   = aAddress;

    return *this;
}

GpEmailBuilder& GpEmailBuilder::From (std::string_view aAddress)
{
    auto& from  = _Email().from;

    from.name.clear();
    from.addr   = aAddress;

    return *this;
}

GpEmailBuilder& GpEmailBuilder::To
(
    std::string_view aName,
    std::string_view aAddress
)
{
    auto& to = _Email().to;

    to.emplace_back(MakeSP<GpEmailAddr>(aName, aAddress));

    return *this;
}

GpEmailBuilder& GpEmailBuilder::To (std::string_view aAddress)
{
    auto& to = _Email().to;

    to.emplace_back(MakeSP<GpEmailAddr>(aAddress));

    return *this;
}

GpEmailBuilder& GpEmailBuilder::Cc
(
    std::string_view aName,
    std::string_view aAddress
)
{
    auto& cc = _Email().cc;

    cc.emplace_back(MakeSP<GpEmailAddr>(aName, aAddress));

    return *this;
}

GpEmailBuilder& GpEmailBuilder::Cc (std::string_view aAddress)
{
    auto& cc = _Email().cc;

    cc.emplace_back(MakeSP<GpEmailAddr>(aAddress));

    return *this;
}



GpEmailBuilder& GpEmailBuilder::Subject (std::string_view aSubject)
{
    _Email().subject = aSubject;

    return *this;
}

GpEmailBuilder& GpEmailBuilder::LinkUnsubscribe (std::string_view aLink)
{
    _Email().link_unsubscribe = aLink;

    return *this;
}

GpEmailBuilder& GpEmailBuilder::Part (GpEmailPart::SP aPart)
{
    _Email().parts.emplace_back(std::move(aPart));

    return *this;
}

GpEmailBuilder& GpEmailBuilder::Part
(
    const GpHttpContentType::EnumT  aContentType,
    std::string&&                   aData
)
{
    _Email().parts.emplace_back(MakeSP<GpEmailPart>(aContentType, std::move(aData)));

    return *this;
}

GpEmailBuilder& GpEmailBuilder::Part
(
    const GpHttpContentType::EnumT  aContentType,
    std::string_view                aData
)
{
    _Email().parts.emplace_back(MakeSP<GpEmailPart>(aContentType, aData));

    return *this;
}

GpEmail&    GpEmailBuilder::_Email (void)
{
    if (iEmail.IsNULL())
    {
        iEmail = MakeSP<GpEmail>();
    }

    return iEmail.Vn();
}

}//namespace GPlatform
