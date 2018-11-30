
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

#include <time.h>
#include <unistd.h>
#include "Utils/engineTypes.h"

namespace yame
{
namespace time
{
inline void Delay(s32 milisecs)
{
    //This delay implementation is blocking in the sense that does not deliver the CPU control to the rest of threads

    usleep(milisecs*1000);

//    asm(
//        "mov %%ax,%0;\n\t"
//        "mov %%ds,%%ax;\n\t"
//        "clr_scr;\n\t"
//        "mov %%dx,%%offset;\n\t"
//        "mov %%ah,9;\n\t"
//        "int 21h;\n\t"
//        "mov %%ax,10000;\n\t"
//        "mov %%dx,500;\n\t"
//        "mul %%dx;\n\t"
//        "mov %%cx,%%dx;\n\t"
//        "mov %%dx,%%ax;\n\t"
//        "xor %%ax,%%ax;\n\t"
//        "mov %%ah,83h;\n\t"
//        "int 15h;\n\t"
//        "clr_scr;\n\t"
//        "mov %%dx,offset;\n\t"
//        "mov %%ah,9;\n\t"
//        "int 21h;\n\t"
//        "exit:\n\t"
//        "mov %%ax,4c00h;\n\t"
//        "int 21h;\n\t"
//        : /* no output */
//        :"m"(milisecs)
//  );

    return;
}

inline void Sleep(s32 milisecs)
{
    //This delay function delivers the CPU to th rest of threads by suspending the thread and scheduling a signal after milisecs miliseconds
    usleep(milisecs*1000);

    return;
}

inline s32 getTicks()
{
    return clock();
}

inline s32 secToMilisec(s32 secs)
{
    s32 res = 0;



    return res;
}

inline s32 milisecToSecs(s32 milisecs)
{
    s32 res = 0;



    return res;
}

inline s32 secToNanosecs(s32 secs)
{
    s32 res = 0;


    return res;
}

inline s32 milisecToNanosecs(s32 milis)
{
    s32 res = 0;


    return res;
}
}
}
