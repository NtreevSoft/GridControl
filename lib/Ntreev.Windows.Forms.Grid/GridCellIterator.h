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
		/// 현재의 열에서 첫번째 셀로 이동합니다.
		/// </summary>
		/// <remarks>
		/// Frozen된 행이 있다면 Frozen된 행을 제외한 첫번째 셀로 이동합니다.
		/// </remarks>
		/// <param name="range">선택 범위를 나타내는 <see cref="Ntreev::Windows::Forms::Grid::SelectionRange"/>입니다.</param>
		void FirstCell(_SelectionRange range);

		/// <summary>
		/// 현재의 열에서 마지막 셀로 이동합니다.
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