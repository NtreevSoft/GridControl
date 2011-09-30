﻿//=====================================================================================================================
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
#include "GrGridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 선택 방법을 지정합니다.
	/// </summary>
	public enum class SelectionType : int
	{
		/// <summary>
		/// 모든 선택된 개체를 제거후 대상을 선택합니다.
		/// </summary>
		Normal,

		/// <summary>
		/// 선택된 개체 리스트에 대상을 추가합니다.
		/// </summary>
		Add,

		/// <summary>
		/// 선택된 개체 리스트에 대상을 제거합니다.
		/// </summary>
		Remove,
	};

	/// <summary>
	/// 선택 범위를 지정합니다.
	/// </summary>
	public enum class SelectionRange : int
	{
		/// <summary>
		/// 선택 범위중 최종적인 하나만 선택합니다.
		/// </summary>
		One,

		/// <summary>
		/// 선택 범위중 모든것을 선택합니다.
		/// </summary>
		Multi,
	};

	/// <summary>
	/// 행의 강조표시 방법을 지정합니다.
	/// </summary>
	public enum class RowHighlightType : int
	{
		/// <summary>
		/// 행의 강조표시를 영역을 색상으로 채워 표시합니다.
		/// </summary>
		Fill,

		/// <summary>
		/// 행의 강조표시를 영역의 외곽선을 그려 표시합니다.
		/// </summary>
		Line,

		/// <summary>
		/// 행의 강조표시를 외곽선과 색상 채우기 모두를 사용합니다.
		/// </summary>
		Both,
	};

	typedef Ntreev::Windows::Forms::Grid::SelectionType		_SelectionType;
	typedef Ntreev::Windows::Forms::Grid::SelectionRange	_SelectionRange;
	typedef Ntreev::Windows::Forms::Grid::RowHighlightType	_RowHighlightType;

	/// <summary>
	/// 마우스 이벤트에 의해서 HitTest를 수행한후 필요한 정보들을 갱신한다.
	/// </summary>
	private value struct HitTest
	{
	public: // variables
		GrCell*		pHittedCell;
		_Point		localPoint;
		_Rectangle	cellRect;

	public: // properties
		property int Width
		{
			int get(); 
		}

		property int Height
		{
			int get();
		}

		property _Point Location
		{
			_Point get();
		}

		property _Size Size
		{
			_Size get(); 
		}

		property GrCellType CellType
		{
			GrCellType get();
		}

	public:
		static HitTest Empty;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/