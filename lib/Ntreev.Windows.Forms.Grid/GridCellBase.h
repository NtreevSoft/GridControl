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
#include "GridBase.h"
#include "GridStyle.h"
#include "GridStyleProvider.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	typedef System::Drawing::StringAlignment	_StringAlignment;

	/// <summary>
	/// 셀을 표시하기 위한 기본 방법을 제공합니다.
	/// </summary>
	public ref class CellBase abstract : GridObject, IStyle//, System::Runtime::Serialization::ISerializable
	{
	public: // methods
		/// <summary>
		/// 셀의 전경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::CellForeColor"/> 속성의 값입니다.
		/// </remarks>
		void ResetForeColor();

		/// <summary>
		/// 셀의 배경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::CellBackColor"/> 속성의 값입니다.
		/// </remarks>
		void ResetBackColor();
		
		/// <summary>
		/// 셀의 배경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::CellBackColor"/> 속성의 값입니다.
		/// </remarks>
		void ResetFont();

	public: // properties
		/// <summary>
		/// 셀의 전경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다. 기본값은 <see cref="Design::Style::CellForeColor"/> 속성의 값입니다.
		/// </returns>
		/// <remarks>
		/// 일반적으로 문자열의 색상을 나타냅니다.
		/// </remarks>
		[_Description("셀의 전경색을 가져오거나 설정합니다.")]
		[_Category("Appearance")]
		property _Color ForeColor
		{
			virtual _Color get();
			void set(_Color);
		}

		/// <summary>
		/// 셀의 배경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다. 기본값은 <see cref="Design::Style::CellBackColor"/> 속성의 값입니다.
		/// </returns>
		[_Description("셀의 배경색을 가져오거나 설정합니다.")]
		[_Category("Appearance")]
		property _Color BackColor
		{
			virtual _Color get();
			void set(_Color);
		}

		/// <summary>
		/// 셀의 글꼴을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 글꼴을 나타내는 <see cref="System::Drawing::Font"/>입니다. 기본값은 <see cref="Design::Style::CellFont"/> 속성의 값입니다.
		/// </returns>
		[_Category("Appearance"), System::ComponentModel::AmbientValue((string^)nullptr)]
		property _Font^ Font
		{ 
			virtual _Font^ get();
			void set(_Font^);
		}

		/// <summary>
		/// 화면에 표시되고 있는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 화면상에 표시되고 있다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
#else
		[_Browsable(false)]
#endif
		property bool IsDisplayable
		{ 
			bool get(); 
		}

		/// <summary>
		/// 셀에 대한 데이터가 들어 있는 개체를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀에 대한 데이터가 들어 있는 <see cref="System::Object"/>입니다. 기본값은 null입니다.
		/// </returns>
		[System::ComponentModel::TypeConverter(System::ComponentModel::StringConverter::typeid)]
		[_Category("Data"), _DefaultValue((string^)nullptr)]
		virtual property object^ Tag;

		
		/// <summary>
		/// 화면에 표시되고 있는 셀의 영역을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 화면에 표시되지 않아도 셀의 영역을 가져올수 있습니다.
		/// </remarks>
		/// <returns>
		/// 화면에 표시되는 영역을 나타내는 <see cref="System::Drawing::Rectangle"/>입니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
#else
		[_Browsable(false)]
#endif
		property _Rectangle DisplayRectangle
		{
			_Rectangle get(); 
		}

		/// <summary>
		/// 화면에 표시되고 있는 셀의 안쪽 영역을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 화면에 표시되지 않아도 셀의 안쪽 영역을 가져올수 있습니다.
		/// </remarks>
		/// <returns>
		/// 화면에 표시되는 안쪽 영역을 나타내는 <see cref="System::Drawing::Rectangle"/>입니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
#else
		[_Browsable(false)]
#endif
		property _Rectangle ClientRectangle
		{
			_Rectangle get(); 
		}

		/// <summary>
		/// 셀의 내부 간격을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 내부 간격 특성을 나타내는 <see cref="System::Windows::Forms::Padding"/>입니다.
		/// </returns>
		[_Description("셀의 안쪽 여백을 가져오거나 설정합니다.")]
		[_Category("Layout")]
		property _Padding Padding 
		{
			_Padding get();
			void set(_Padding);
		}

	public: // events
		/// <summary>
		/// <see cref="ForeColor"/> 속성 값이 변경되면 발생합니다.
		/// </summary>
		[_Description("전경색이 변경되면 발생합니다.")]
		event _EventHandler^		ForeColorChanged
		{
			void add(_EventHandler^ p);
			void remove(_EventHandler^ p);
		private:
			void raise(object^ sender, _EventArgs^ e);
		}

		/// <summary>
		/// <see cref="BackColor"/> 속성 값이 변경되면 발생합니다.
		/// </summary>
		[_Description("배경색이 변경되면 발생합니다.")]
		event _EventHandler^		BackColorChanged
		{
			void add(_EventHandler^ p);
			void remove(_EventHandler^ p);
		private:
			void raise(object^ sender, _EventArgs^ e);
		}

		/// <summary>
		/// <see cref="Font"/> 속성 값이 변경되면 발생합니다.
		/// </summary>
		[_Description("글꼴이 변경되면 발생합니다.")]
		event _EventHandler^		FontChanged
		{
			void add(_EventHandler^ p);
			void remove(_EventHandler^ p);
		private:
			void raise(object^ sender, _EventArgs^ e);
		}

	internal: // methods
		CellBase(_GridControl^ gridControl, GrCell* pCell);

		CellBase(GrCell* pCell);

	internal: // properties
		property _Color RenderingForeColor
		{
			_Color get();
		}

		property _Color RenderingBackColor
		{
			_Color get();
		}

	protected: // methods
		/// <summary>
		/// 소멸자 입니다.
		/// </summary>
		virtual ~CellBase();

		///// <summary>
		///// 대상 개체를 serialize하는 데 필요한 데이터로 <see cref="System::Runtime::Serialization::SerializationInfo"/>를 채웁니다.
		///// </summary>
		///// <param name="info">
		///// 데이터로 채울 <see cref="System::Runtime::Serialization::SerializationInfo"/>입니다.
		///// </param>
		///// <param name="context">
		///// 이 serialization에 대한 대상입니다(<see cref="System::Runtime::Serialization::StreamingContext"/> 참조).
		///// </param>
		///// <exception cref="System::Security::SecurityException">
		///// 호출자에게 필요한 권한이 없는 경우
		///// </exception>
		//virtual void GetObjectData(System::Runtime::Serialization::SerializationInfo^ info, System::Runtime::Serialization::StreamingContext context) sealed 
		//	= System::Runtime::Serialization::ISerializable::GetObjectData;
    
		/// <summary>
		/// <see cref="ForeColorChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnForeColorChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="BackColorChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnBackColorChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="FontChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnFontChanged(_EventArgs^ e);

		/// <summary>
		/// 셀의 영역을 무효화하고 컨트롤을 다시 그립니다.
		/// </summary>
		/// <param name="cellBase">
		/// 무효화 할 영역을 포함하는 <see cref="CellBase"/>의 인스턴스 입니다.
		/// </param>
		/// <exception cref="System::ArgumentNullException">매개 변수 <paramref name="cellBase"/>의 값이 null일때</exception>
		void Invalidate(CellBase^ cellBase);

	private: // methods
		bool ShouldSerializeForeColor();
		bool ShouldSerializeBackColor();
		bool ShouldSerializeFont();
		bool ShouldSerializePadding();

		void ResetPadding();

	private: // variables
		GrCell*						m_pCell;
		_EventHandler^				m_eventForeColorChanged;
		_EventHandler^				m_eventBackColorChanged;
		_EventHandler^				m_eventFontChanged;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/