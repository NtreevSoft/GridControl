#pragma once
#include "ICellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    public interface class ICell : ICellBase
    {
        property System::Object^ Value
        {
            System::Object^ get();
        }

        property System::Object^ Tag
        {
            System::Object^ get();
        }
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/
