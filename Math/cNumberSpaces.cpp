
#include "Math/cNumberSpaces.h"

namespace yame
{

container::string format(const math::Rational& i_value)
{
    math::rationalPair thisValue = i_value.get_raw();

    return container::string::Format("%d/%d", thisValue[0], thisValue[1]);
}
container::string format(const math::Real& i_value)
{
    return container::string::Format("%f", i_value.get_raw());
}
container::string format(const math::Complex& i_value)
{
    math::complexPair thisValue = i_value.get_raw();

    return container::string::Format("%f + %fi", thisValue[0], thisValue[1]);
}
container::string format(const math::Quaternion& i_value)
{
    math::quaternionQuartet thisValue = i_value.get_raw();

    return container::string::Format("%f + %fi + %fj + %fk", thisValue[0], thisValue[1], thisValue[2], thisValue[3]);
}

}
