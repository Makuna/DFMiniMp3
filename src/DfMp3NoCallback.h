/*-------------------------------------------------------------------------
DFMiniMp3 library

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/DFMiniMp3)

-------------------------------------------------------------------------
This file is part of the Makuna/DFMiniMp3 library.

DFMiniMp3 is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

DFMiniMp3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with DFMiniMp3.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#pragma once

class DfMp3NoCallback
{
public:
    typedef void TargetType;

    static void SetTarget(TargetType*)
    {
    }

    static void OnError(uint16_t)
    {
    }

    static void OnPlayFinished(DfMp3_PlaySources, uint16_t)
    {
    }

    static void OnPlaySourceOnline(DfMp3_PlaySources)
    {
    }

    static void OnPlaySourceInserted(DfMp3_PlaySources)
    {
    }

    static void OnPlaySourceRemoved(DfMp3_PlaySources)
    {
    }
};