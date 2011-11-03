//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridUtil.h"
#include "GridColumn.h"
#include "GridRow.h"
#include "GridCell.h"
#include "GridControl.h"
#include "GridCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	const wchar_t* ToNativeString::Convert(string^ text)
	{
		pin_ptr<const wchar_t> unmngStr = PtrToStringChars(text);
		return unmngStr;
	}

	EditingReason::EditingReason(_Point pt) : data(pt), inputType(_EditingReasonType::Mouse)
	{

	}

	EditingReason::EditingReason(_Keys key) : data(key), inputType(_EditingReasonType::Key)
	{

	}

	EditingReason::EditingReason(_Char charKey, bool ime) : data(charKey), inputType(ime == true ? _EditingReasonType::Ime : _EditingReasonType::Char)
	{

	}

	EditingReason::EditingReason(object^ data) : data(data), inputType(_EditingReasonType::Data)
	{

	}

	EditingReason::EditingReason() : inputType(_EditingReasonType::None)
	{

	}

	EditingReasonType EditingReason::EditingReasonType::get()
	{
		return inputType;
	}

	_Point EditingReason::Location::get()
	{
		return (_Point)data;
	}

	EditingReason::_Keys EditingReason::Key::get()
	{
		return (EditingReason::_Keys)data;
	}

	EditingReason::_Char EditingReason::Char::get()
	{
		return (EditingReason::_Char)data;
	}

	object^ EditingReason::Data::get()
	{
		return data;
	}

	bool EditingReason::Handled::get()
	{
		return m_handled;
	}

	void EditingReason::Handled::set(bool value)
	{
		m_handled = value;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/