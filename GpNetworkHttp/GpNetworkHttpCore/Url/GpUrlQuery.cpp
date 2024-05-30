#include "GpUrlQuery.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpUrlQuery, GP_MODULE_UUID)

GpUrlQuery::~GpUrlQuery (void) noexcept
{
}

void    GpUrlQuery::Clear (void)
{
    params.clear();
}

GpUrlQuery& GpUrlQuery::operator= (const GpUrlQuery& aUrl)
{
    params = aUrl.params;

    return *this;
}

std::string GpUrlQuery::SToString (const GpUrlQuery& aQuery)
{
    const ParamsMapT& params = aQuery.params;
    if (params.empty())
    {
        return {};
    }

    return StrOps::SJoin<std::string>
    (
        params,
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

GpUrlQuery  GpUrlQuery::SFromString (std::string_view aQuery)
{
    ParamsMapT paramsMap;

    std::vector<std::string_view> parts = StrOps::SSplit(aQuery, '&', 0, 0, Algo::SplitMode::SKIP_ZERO_LENGTH_PARTS);

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

GpUrlQuery& GpUrlQuery::operator= (GpUrlQuery&& aUrl) noexcept
{
    params = std::move(aUrl.params);

    return *this;
}

void    GpUrlQuery::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(params);
}

}// namespace GPlatform
