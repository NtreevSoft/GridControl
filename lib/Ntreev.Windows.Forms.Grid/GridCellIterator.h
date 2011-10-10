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
#include "GridEvent.h"
#include "GridBase.h"
#include "GridScrollProperty.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 셀의 포커스 및 선택방법을 지정하는 개체를 나타냅니다.
	/// </summary>
	public ref class CellIterator : GridObject
	{
	public: // methods
		/// <summary>
		/// 현재의 행에서 첫번째 셀로 이동합니다.
		/// </summary>
		/// <remarks>
		/// Frozen된 열이 있다면 Frozen된 열을 제외한 첫번째 셀로 이동합니다.
		/// </remarks>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void FirstCell(_SelectionRange range);

		/// <summary>
		/// 현재의 행에서 마지막 셀로 이동합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void LastCell(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 한페이지 위로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void PageUp(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 한페이지 아래로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void PageDown(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 첫번째 열로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void FirstRow(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 마지막 열로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void LastRow(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 한칸 좌측으로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void MoveLeft(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 한칸 위로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void MoveUp(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 한칸 우측으로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void MoveRight(_SelectionRange range);

		/// <summary>
		/// 셀의 위치를 한칸 아래로 이동하고 선택합니다.
		/// </summary>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void MoveDown(_SelectionRange range);

	internal: // methods
		CellIterator(_GridControl^ gridControl);

	private: // methods
		void gridControl_OnFocusedCellChanged(object^ sender, CellEventArgs^ e);

		void SelectOne(IDataRow* pDataRow);
		void SelectMulti(IDataRow* pBegin, IDataRow* pEnd);

	private: // properties
		property HScrollProperty^ HorizontalScroll
		{
			HScrollProperty^ get(); 
		}

		property VScrollProperty^ VerticalScroll
		{
			VScrollProperty^ get(); 
		}
		
	private: // variables
		GrDataRowList*				m_pDataRowList;
		GrColumn*				m_pLastDataColumn;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/