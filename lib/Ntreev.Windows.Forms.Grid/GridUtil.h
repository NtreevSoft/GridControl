//=====================================================================================================================
// Ntreev Grid for .Net 1.0
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


#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 편집 시작 방식을 지정합니다.
	/// </summary>
	public enum class EditingReasonType : int
	{
		/// <summary>
		/// 마우스 위치를 이용해 편집을 시작합니다.
		/// </summary>
		Mouse,

		/// <summary>
		/// 키를 이용해 편집을 시작합니다.
		/// </summary>
		/// <remarks>
		/// 알파벳과 한글키등을 제외한 숫자키, 특수문자, f1 ~ f12키 등이 해당됩니다.
		/// </remarks>
		Key,

		/// <summary>
		/// 문자열키 정보를 이용하여 편집을 시작합니다.
		/// </summary>
		/// <remarks>
		/// 일반적으로 알파벳에 해당합니다.
		/// </remarks>
		Char,

		/// <summary>
		/// ime키 정보를 이용하여 편집을 시작합니다.
		/// </summary>
		Ime,

		/// <summary>
		/// 추가적인 데이터를 이용하여 편집을 시작합니다.
		/// </summary>
		Data,

		/// <summary>
		/// 추가적인 정보 없이 편집합니다.
		/// </summary>
		None,
	};

	typedef Ntreev::Windows::Forms::Grid::EditingReasonType	_EditingReasonType;

	/// <summary>
	/// 편집 시작 방식을 나타내는 개체입니다.
	/// </summary>
	public ref class EditingReason
	{
	private: // typedefs
		typedef System::Char					_Char;
		typedef System::Windows::Forms::Keys	_Keys;

	public: // methods
		/// <summary>
		/// <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		EditingReason();

		/// <summary>
		/// 마우스로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="location">그리드 컨트롤에 대한 마우스 커서의 좌표입니다.</param>
		EditingReason(_Point location);

		/// <summary>
		/// 키로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="key">추가적으로 제공할 키 정보입니다.</param>
		EditingReason(_Keys key);

		/// <summary>
		/// 문자열 키로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="charKey">문자열 키 정보입니다.</param>
		/// <param name="ime">문자열 키가 ime키인지에 대한 여부를 설정합니다.</param>
		EditingReason(_Char charKey, bool ime);

		/// <summary>
		/// 추가적인 데이터로 편집이 시작됨을 알리는 <see cref="EditingReason"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="data">추가적으로 제공할 데이터입니다.</param>
		EditingReason(object^ data);

	public: // properties
		/// <summary>
		/// 편집 시작 방식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 편집 시작 방식을 나타내는 <see cref="EditingReasonType"/>입니다.
		/// </returns>
		property _EditingReasonType EditingReasonType
		{
			_EditingReasonType get(); 
		}

		/// <summary>
		/// 마우스로 인한 편집시에 마우스 커서의 좌표를 가져옵니다.
		/// </summary>
		property _Point Location
		{
			_Point get();
		}

		/// <summary>
		/// 키로 인한 편집시에 키 데이터를 가져옵니다.
		/// </summary>
		property _Keys Key
		{
			_Keys get();
		}

		/// <summary>
		/// 문자열 키로 인한 편집시에 문자 데이터를 가져옵니다.
		/// </summary>
		property _Char Char
		{
			_Char get(); 
		}

		/// <summary>
		/// 추가적인 데이터로 인한 편집시에 추가적인 데이터를 가져옵니다.
		/// </summary>
		property object^ Data
		{
			object^ get(); 
		}
	
	internal: // properties
		property bool Handled
		{
			bool get();
			void set(bool);
		}

	public: // variables
		static EditingReason	None;

	private: // variables
		_EditingReasonType		inputType;
		object^				data;
		bool				m_handled;
	};

	private ref class ToNativeString
	{
	public: // methods
		static const wchar_t* Convert(string^ text);
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/