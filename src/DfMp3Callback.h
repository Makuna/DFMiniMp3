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

template <class T_CALLBACK_TARGET>
class DfMp3Callback
{
public:
    typedef T_CALLBACK_TARGET TargetType;

    static void SetTarget(TargetType* target)
    {
        s_target = target;
    }

    ~DfMp3Callback()
    {
        s_target = nullptr;
    }

    static void OnError(uint16_t errorCode)
    {
        if (s_target)
        {
            s_target->OnError(errorCode);
        }
    }

    static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
    {
        if (s_target)
        {
            s_target->OnPlayFinished(source, track);
        }
    }

    static void OnPlaySourceOnline(DfMp3_PlaySources source)
    {
        if (s_target)
        {
            s_target->OnPlaySourceOnline(source);
        }
    }

    static void OnPlaySourceInserted(DfMp3_PlaySources source)
    {
        if (s_target)
        {
            s_target->OnPlaySourceInserted(source);
        }
    }

    static void OnPlaySourceRemoved(DfMp3_PlaySources source)
    {
        if (s_target)
        {
            s_target->OnPlaySourceRemoved(source);
        }
    }

protected:
    static T_CALLBACK_TARGET* s_target;
};

template <class T_CALLBACK_TARGET> T_CALLBACK_TARGET* DfMp3Callback< T_CALLBACK_TARGET>::s_target = nullptr;