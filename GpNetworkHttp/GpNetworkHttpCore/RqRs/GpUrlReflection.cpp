#include "GpUrlReflection.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectType.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectContainerType.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectModel.hpp"
#include "../../../../GpCore2/GpUtils/Other/GpCallHandler2.hpp"

namespace GPlatform {

std::u8string   GpUrlReflection::SReflectToUrl (const GpReflectObject& aObj)
{
    std::u8string strRes;
    strRes.reserve(1024);

    const GpReflectModel&   model   = aObj.ReflectModel();
    const void*             dataPtr = aObj.ReflectDataPtr();

    GpCallHandler2 callHandler;

    for (const GpReflectProp& propInfo: model.Props())
    {
        callHandler.CallIfNonFirst([&](){strRes.append(u8"&");});

        std::u8string_view                  propName        = propInfo.Name();
        const GpReflectType::EnumT          propType        = propInfo.Type();
        const GpReflectContainerType::EnumT propContainer   = propInfo.Container();

        THROW_COND_GP
        (
            propContainer == GpReflectContainerType::NO,
            [&](){return u8"Property '"_sv + propName + u8"' container must be NO"_sv;}
        );

        strRes.append(propName);
        strRes.append(u8"=");

        switch (propType)
        {
            case GpReflectType::S_INT_8:
            {
                strRes.append(StrOps::SFromSI64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt8(dataPtr))));
            } break;
            case GpReflectType::U_INT_8:
            {
                strRes.append(StrOps::SFromUI64(NumOps::SConvert<u_int_64>(propInfo.Value_UInt8(dataPtr))));
            } break;
            case GpReflectType::S_INT_16:
            {
                strRes.append(StrOps::SFromSI64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt16(dataPtr))));
            } break;
            case GpReflectType::U_INT_16:
            {
                strRes.append(StrOps::SFromUI64(NumOps::SConvert<u_int_64>(propInfo.Value_UInt16(dataPtr))));
            } break;
            case GpReflectType::S_INT_32:
            {
                strRes.append(StrOps::SFromSI64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt32(dataPtr))));
            } break;
            case GpReflectType::U_INT_32:
            {
                strRes.append(StrOps::SFromUI64(NumOps::SConvert<u_int_64>(propInfo.Value_UInt32(dataPtr))));
            } break;
            case GpReflectType::S_INT_64:
            {
                strRes.append(StrOps::SFromSI64(NumOps::SConvert<s_int_64>(propInfo.Value_SInt64(dataPtr))));
            } break;
            case GpReflectType::U_INT_64:
            {
                strRes.append(StrOps::SFromUI64(NumOps::SConvert<u_int_64>(propInfo.Value_UInt64(dataPtr))));
            } break;
            case GpReflectType::DOUBLE:
            {
                strRes.append(StrOps::SFromDouble(propInfo.Value_Double(dataPtr)));
            } break;
            case GpReflectType::FLOAT:
            {
                strRes.append(StrOps::SFromDouble(double(propInfo.Value_Float(dataPtr))));
            } break;
            case GpReflectType::UUID:
            {
                //TODO: add escape
                strRes.append(propInfo.Value_UUID(dataPtr).ToString());
            } break;
            case GpReflectType::STRING:
            {
                //TODO: add escape
                strRes.append(propInfo.Value_String(dataPtr));
            } break;
            case GpReflectType::BLOB:
            {
                //TODO: add escape
                strRes.append(StrOps::SFromBytesHex(propInfo.Value_BLOB(dataPtr)));
            } break;
            case GpReflectType::ENUM:
            {
                //TODO: add escape
                strRes.append(propInfo.Value_Enum(dataPtr).ToString());
            } break;
            case GpReflectType::ENUM_FLAGS: [[fallthrough]];
            case GpReflectType::BOOLEAN:    [[fallthrough]];
            case GpReflectType::OBJECT:     [[fallthrough]];
            case GpReflectType::OBJECT_SP:  [[fallthrough]];
            case GpReflectType::NOT_SET:    [[fallthrough]];
            default:
            {
                THROW_GP(u8"Unsupported type '"_sv + GpReflectType::SToString(propType) + u8"' of prop '"_sv + propName + u8"'"_sv);
            }
        }//switch (propType)
    }

    return strRes;
}

}//namespace GPlatform
