#include "Utils/engine.h"

namespace yame
{

container::string format(unsigned char i_value)
{
    return container::string::Format("%c", i_value);
}
container::string format(char i_value)
{
    return container::string::Format("%c", i_value);
}
container::string format(unsigned short i_value)
{
    return container::string::Format("%uh", i_value);
}
container::string format(short i_value)
{
    return container::string::Format("%dh", i_value);
}
container::string format(unsigned int i_value)
{
    return container::string::Format("%u", i_value);
}
container::string format(int i_value)
{
    return container::string::Format("%d", i_value);
}
container::string format(float i_value)
{
    return container::string::Format("%f", i_value);
}
container::string format(double i_value)
{
    return container::string::Format("%g", i_value);
}
container::string format(unsigned long i_value)
{
    return container::string::Format("%lu", i_value);
}
container::string format(long i_value)
{
    return container::string::Format("%ld", i_value);
}

}
