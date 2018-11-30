#pragma once

namespace yame
{
namespace utils
{

template<typename Interface>
class cInterfaceAccess
{
public:
    cInterfaceAccess(Interface& i_interface);
    ~cInterfaceAccess();

    Interface* operator->();
    const Interface* operator->() const;

private:
    void enter();
    void leave();

    Interface& m_interface;
    bool m_accessed;
};

}
}
