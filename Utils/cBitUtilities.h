
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

#define BINARY2DEC(binNum,decNum){decNum=0;for(u16 i=0;i<binNum.getSize();i++){decNum+=binNum[i]*pow(10,i);}}
#define NUMBITS(dec,numBits){u32 temp=dec;for(numBits=0;temp;numBits++){temp &= temp - 1;}}
#define DEC2BINARY(decNum,binNum){binNum.clear();u16 temp=decNum;while(temp > 0){binNum.push_back(temp%2);temp = temp<<1;}}
#define MASK(init,long){((1<<long)-1)<<init;}
