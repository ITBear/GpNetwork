#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Url/GpUrlQuery.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GpUrlQuery::~GpUrlQuery (void) noexcept
{
}

void    GpUrlQuery::Clear (void)
{
    iParams.clear();
}

std::string GpUrlQuery::ToString (void) const
{
    if (iParams.empty())
    {
        return {};
    }

    return StrOps::SJoin<std::string>
    (
        iParams,
        [](const auto& i) -> std::string
        {
            std::string_view key = i->first;
            std::string_view val = i->second;

            if (!val.empty())
            {
                return StrOps::SPercentEncode(key) + "=" + StrOps::SPercentEncode(val);
            } else
            {
                return StrOps::SPercentEncode(key);
            }
        },
        "&"
    );
}

GpUrlQuery  GpUrlQuery::SFromString (std::string_view aUrlQueryStr)
{
    ParamsMapT paramsMap;

    std::vector<std::string_view> parts = StrOps::SSplit(aUrlQueryStr, '&', 0, 0, Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS);

    for (std::string_view part: parts)
    {
        std::string_view key;
        std::string_view val;
        if (const auto pos = part.find("="_sv); pos != std::string::npos)
        {
            key = part.substr(0, pos);
            val = part.substr(pos + 1);
        } else
        {
            key = part;
        }

        paramsMap[std::string(key)] = val;
    }

    return GpUrlQuery{paramsMap};
}

GpUrlQuery& GpUrlQuery::operator= (const GpUrlQuery& aUrlQuery)
{
    iParams = aUrlQuery.iParams;

    return *this;
}

GpUrlQuery& GpUrlQuery::operator= (GpUrlQuery&& aUrlQuery) noexcept
{
    iParams = std::move(aUrlQuery.iParams);

    return *this;
}

}// namespace GPlatform
