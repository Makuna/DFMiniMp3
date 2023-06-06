/*-------------------------------------------------------------------------
queueSimple_t - simple vector queue, as not all Arduino have stl types available 

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

template <class T_ITEM> class queueSimple_t
{
public:
    queueSimple_t(uint8_t length) :
        _queue(nullptr),
        _length(0),
        _front(0),
        _back(0)
    {
        enlarge(length);
    }

    ~queueSimple_t()
    {
        delete[] _queue;
        _queue = nullptr;
    }

    void Enqueue(const T_ITEM& item)
    {
        //Serial.print("Enqueue (");
        //item.printReply();
        //Serial.print(") front(");
        //Serial.print(_front);
        //Serial.print(") back(");
        //Serial.print(_back);
        //Serial.println(")");

        uint8_t newBack = _back + 1;
        if (newBack >= _length)
        {
            newBack = 0;
        }

        if (newBack == _front)
        {
            // no space left, enlarge
            enlarge(_length + 2);
            newBack = _back + 1;
        }
        _queue[_back] = item;
        _back = newBack;
    }

    bool Dequeue(T_ITEM* item)
    {
        if (_front == _back)
        {
            *item = {};
            return false;
        }

        *item = _queue[_front];
        _front++;
        if (_front >= _length)
        {
            _front = 0;
        }

        return true;
    }

private:
    T_ITEM* _queue;
    uint8_t _length;
    uint8_t _front; // location of removing present items
    uint8_t _back; // location of appending new items

    void enlarge(uint8_t newLength)
    {
#ifdef DfMiniMp3Debug
        // only debug output after construction
        if (_queue)
        {
            DfMiniMp3Debug.print("Notifications Queue Enlarged from ");
            DfMiniMp3Debug.print(_length);
            DfMiniMp3Debug.print(" to ");
            DfMiniMp3Debug.println(newLength);
        }
#endif
        if (newLength <= _length)
        {
            return;
        }

        // allocate new vector of items
        T_ITEM* queueNew = new T_ITEM[newLength];
        uint8_t backNew = 0;

        // copy items from old queue vector to new one
        T_ITEM* item = queueNew;
        while (Dequeue(item))
        {
            backNew++;
            item++;
        }

        // cleanup and use new queue
        delete[] _queue;
        _queue = queueNew;
        _length = newLength;
        _front = 0;
        _back = backNew;
    }
};