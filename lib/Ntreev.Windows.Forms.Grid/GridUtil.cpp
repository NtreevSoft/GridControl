#include "StdAfx.h"
#include "GridUtil.h"
#include "GridColumn.h"
#include "GridRow.h"
#include "GridCell.h"
#include "GridControl.h"
#include "GridCollection.h"
#include "GridDebug.h"

//#include "GrGridCell.h"
//#include "GrGridCore.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	//ToNativeString::ToNativeString(string^ text)
	//{
	//	ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(text);
	//}

	//ToNativeString::~ToNativeString()
	//{
	//	System::Runtime::InteropServices::Marshal::FreeHGlobal(ptr);
	//}

	//ToNativeString::!ToNativeString()
	//{

	//}

	//ToNativeString::operator const wchar_t*() 
	//{ 
	//	return (const wchar_t*)ptr.ToPointer(); 
	//}

	//const wchar_t* ToNativeString::c_str()
	//{
	//	return (const wchar_t*)ptr.ToPointer(); 
	//}

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