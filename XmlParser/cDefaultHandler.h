
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "container/cString.h"
#include "cProperty.h"

namespace yame
{
namespace xml
{

struct cDefaultHandler
{
    virtual void StartHandler(const container::string& tag, const container::cArray<cProperty>& properties)=0;
    virtual void ContentHandler(const container::string& content)=0;
    virtual void EndHandler(const container::string& tag)=0;
    virtual void InfoHandler(const container::string& info)=0;
    virtual void VersionHandler(const container::string& version)=0;
    virtual void ErrorHanlder(const container::string& error)=0;
};

}
}
