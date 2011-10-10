//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	public ref class RowBase : CellBase
	{
	public: // properties
		/// <summary>
		/// 행의 높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MinHeight"/>보다 작거나 <see cref="MaxHeight"/>보다 클경우
		/// </exception>
		[_Description("열의 높이입니다.")]
		[_Category("Layout")]
		property int Height
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 행의 최소높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MaxHeight"/>보다 클경우
		/// </exception>
		[_Description("열의 최소높이입니다.")]
		[_Category("Layout")]
		property int MinHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 행의 최대높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MinHeight"/>보다 작을경우
		/// </exception>
		[_Description("열의 최대높이입니다.")]
		[_Category("Layout")]
		property int MaxHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 표시되고 있는지의 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 표시되고 있다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Description("표시되고 있는지의 여부를 가져오거나 설정합니다.")]
		[_Category("Appearance")]
		[_DefaultValue(true)]
		property bool IsVisible 
		{
			bool get();

		}

		/// <summary>
		/// 표시되는 인덱스를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 표시가 된다면 인덱스(0부터 시작)를 반환하고, 그렇지 않다면 0xffffffff를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 표시 여부를 확인하기 위해서는 <see cref="IsVisible"/>를 확인하면 됩니다.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("표시되는 인덱스입니다.")]
#else
		[_Browsable(false)]
#endif
		property uint VisibleIndex
		{
			uint get(); 
		}

		/// <summary>
		/// 화면에 표시되는 인덱스를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 화면에 표시가 된다면 인덱스(0부터 시작)를 반환하고, 그렇지 않다면 0xffffffff를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 화면에 표시 여부를 확인하기 위해서는 <see cref="IsDisplayable"/>를 확인하면 됩니다.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("화면에 표시되는 인덱스입니다")]
#else
		[_Browsable(false)]
#endif
		property uint DisplayIndex
		{
			uint get(); 
		}

	internal: // methods
		RowBase(_GridControl^ gridControl, IDataRow* pDataRow);

	private: // methods
		bool ShouldSerializeHeight();
		bool ShouldSerializeMinHeight();
		bool ShouldSerializeMaxHeight();

	private: // variables
		IDataRow*		m_pDataRow;
	};

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/