//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


#include "StdAfx.h"
#include "EditingReason.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    EditingReason::EditingReason(Point pt)
        : data(pt), reasonType(EditingReasonType::Mouse)
    {

    }

    EditingReason::EditingReason(Keys key)
        : data(key), reasonType(EditingReasonType::Key)
    {

    }

    EditingReason::EditingReason(System::Char charKey)
        : data(charKey), reasonType(EditingReasonType::Char)
    {

    }

    EditingReason::EditingReason(int keyValue)
        : data(keyValue), reasonType(EditingReasonType::Ime)
    {

    }

    EditingReason::EditingReason(const GrEditingReason& reason)
    {
        switch(reason.editingType)
        {
        case GrEditingType_None:
            data = nullptr;
            break;
        case GrEditingType_Mouse:
            data = (Point)reason.location;
            break;
        case GrEditingType_Key:
            data = (Keys)reason.key;
            break;
        case GrEditingType_Char:
            data = (System::Char)reason.character;
            break;
        case GrEditingType_Ime:
            data = (int)reason.keyValue;
            break;
        }

        reasonType = (EditingReasonType)reason.editingType;
    }

    GrEditingReason EditingReason::ToNative()
    {
        GrEditingReason reason;

        switch(reasonType)
        {
        case EditingReasonType::Mouse:
            reason.location = this->Location;
            break;
        case EditingReasonType::Key:
            reason.key = (GrKeys)this->Key;
            break;
        case EditingReasonType::Char:
            reason.character = (wchar_t)this->Char;
            break;
        case EditingReasonType::Ime:
            reason.keyValue = this->KeyValue;
            break;
        }

        reason.editingType = (GrEditingType)reasonType;
        return reason;
    }

    EditingReasonType EditingReason::ReasonType::get()
    {
        return reasonType;
    }

    Point EditingReason::Location::get()
    {
        return (Point)data;
    }

    Keys EditingReason::Key::get()
    {
        return (Keys)data;
    }

    System::Char EditingReason::Char::get()
    {
        return (System::Char)data;
    }

    int EditingReason::KeyValue::get()
    {
        return (int)data;
    }

    System::Object^ EditingReason::Data::get()
    {
        return data;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/