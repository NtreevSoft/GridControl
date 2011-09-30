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
#include "GridRowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 행을 나타냅니다.
	/// </summary>
	[System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class Row : RowBase
	{
	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override;

		void BeginEdit();
	
		/// <summary>
		/// 셀들의 변경된 내용을 취소합니다.
		/// </summary>
		/// <remarks>
		/// 셀들의 데이터가 변경되었을때 사용자는 변경된 내용을 취소하고 원래값으로 되돌릴수 있습니다.
		/// </remarks>
		void CancelEdit();

		/// <summary>
		/// 셀들의 변경된 내용을 적용합니다.
		/// </summary>
		/// <remarks>
		/// 셀들의 데이터가 변경되었을때 사용자는 변경된 내용을 취소하고 원래값으로 되돌릴수 있지만, 이 작업을 수행한 이후로는
		/// 원래값으로 되돌릴 수 없게 됩니다.
		/// </remarks>
		void ApplyEdit();

		/// <summary>
		/// 행을 화면에 표시되도록 스크롤을 조정합니다.
		/// </summary>
		void EnsureVisible();

		/// <summary>
		/// 행을 선택합니다.
		/// </summary>
		/// <param name="selectionType">
		/// 행을 선택하기 위한 방법을 지정합니다.
		/// </param>
		void Select(_SelectionType selectionType);

		/// <summary>
		/// 포커스를 설정합니다.
		/// </summary>
		void Focus();

		/// <summary>
		/// 셀들의 전경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::RowCellForeColors"/> 속성의 값입니다.
		/// </remarks>
		void ResetCellForeColor();

		/// <summary>
		/// 셀들의 배경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::RowCellBackColors"/> 속성의 값입니다.
		/// </remarks>
		void ResetCellBackColor();

		/// <summary>
		/// 지정한 인덱스에 있는 항목를 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 항목의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 항목입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작거나, <see cref="CellCount"/>보다 클 경우
		/// </exception>
		_Cell^ GetAt(int index)
		{
			return this[index];
		}

	public: // properties
		/// <summary>
		/// <see cref="Cells"/>의 컬랙션을 가져옵니다.
		/// </summary>
		/// <returns>
		///	<see cref="Cells"/>이 들어있는 <see cref="CellCollection"/>입니다.
		/// </returns>
		[_Description("셀들의 컬렉션입니다.")]
		[_Category("Appearance")]
		property CellCollection^ Cells
		{
			CellCollection^ get();
		}

		/// <summary>
		/// <see cref="Cells"/>의 갯수를 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("셀들의 갯수입니다.")]
#else
		[_Browsable(false)]
#endif
		property int CellCount
		{
			int get();
		}

		/// <summary>
		/// 지정한 인덱스에 있는 항목를 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 항목의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 항목입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작거나, <see cref="CellCount"/>보다 클 경우
		/// </exception>
		property Cell^ default[int]
		{
			Cell^ get(int index); 
		}

		/// <summary>
		/// 열의 이름으로 항목를 가져옵니다.
		/// </summary>
		/// <param name="columnName">
		/// 찾을 셀을 소유한 열의 이름입니다.
		/// </param>
		/// <returns>
		/// 찾지 못하는 경우 null을 반환합니다.
		/// </returns>
		property Cell^ default[string^]
		{
			Cell^ get(string^ columnName); 
		}

		/// <summary>
		/// 열의 인스턴스로 항목를 가져옵니다.
		/// </summary>
		/// <param name="column">
		/// 찾을 항목의 <see cref="Column"/>의 인스턴스 입니다.
		/// </param>
		/// <returns>
		/// 찾지 못하는 경우 null을 반환합니다.
		/// </returns>
		/// <exception cref="System::NullReferenceException">
		/// 매개변수가 null일 경우
		/// </exception>
		property Cell^ default[Column^]
		{
			Cell^ get(Column^ column);
		}
		
		/// <summary>
		/// 고유 아이디를 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("열의 고유 아이디입니다.")]
#else
		[_Browsable(false)]
#endif
		property uint RowID
		{
			uint get();
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
			virtual bool get() new;
			virtual void set(bool);
		}

		/// <summary>
		/// 읽기 전용인지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 읽기 전용이면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 이 속성의 값이 true일 경우 Row에 포함되어있는 <see cref="Cells"/>들도 읽기 전용상태로 됩니다.
		/// </remarks>
		[_Description("읽기 전용인지에 대한 여부를 가져오거나 설정합니다.")]
		[_Category("Appearance")]
		[_DefaultValue(false)]
		property bool IsReadOnly
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 선택되어 있는지의 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 소유하고 있는 <see cref="Cells"/>중 적어도 한개이상이 선택되었다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		/// <param name="value">
		/// 매개변수의 값이 true라면 모든 셀이 선택되어집니다.
		/// 반대로 매개변수의 값을 false로 설정하면 모든 셀의 선택이 해제가 됩니다.
		/// </param>
		/// <remarks>
		/// 모든 셀이 선택되었는지를 확인하려면 <see cref="IsFullSelected"/>를 사용하십시오.
		/// </remarks>
		[_Description("선택되어 있는지의 여부를 가져오거나 설정합니다.")]
		[_Category("Appearance")]
		[_DefaultValue(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property bool IsSelected
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 현재의 행이 편집중인지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 현재의 행이 편집중이면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("소유하고 있는 셀의 대한 편집 여부를 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property bool IsBeingEdited
		{
			bool get(); 
		}

		/// <summary>
		/// 소유하고 있는 <see cref="Cells"/>의 대한 편집 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 소유하고 있는 <see cref="Cells"/>중 적어도 한개이상이 편집되었다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("소유하고 있는 셀의 대한 편집 여부를 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property bool IsEdited
		{
			bool get(); 
		}

		/// <summary>
		/// <see cref="Ntreev::Windows::Forms::Grid::GridControl"/>내에 행의 인덱스를 가져옵니다.
		/// </summary>
		/// <returns>
		/// <see cref="Ntreev::Windows::Forms::Grid::GridControl"/>내에 포함되 있다면 인덱스(0부터 시작)를 반환하고, 그렇지 않다면 0xffffffff를 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("컨트롤에 내에 Row 인덱스를 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property int Index
		{
			int get(); 
		}

		/// <summary>
		/// 소유하고 있는 모든 셀이 선택되었는지의 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 소유하고 있는 모든 셀이 선택되었다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("소유하고 있는 모든 셀이 선택되었는지의 여부를 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property bool IsFullSelected
		{
			bool get(); 
		}

		/// <summary>
		/// <see cref="Cells"/>의 전경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	<see cref="Cells"/>의 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		[_Description("셀들의 전경색입니다")]
		[_Category("Appearance")]
		property _Color CellForeColor
		{
			_Color get();
			void set(_Color);
		}

		/// <summary>
		/// <see cref="Cells"/>의 배경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	<see cref="Cells"/>의 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		[_Description("셀들의 배경색입니다")]
		[_Category("Appearance")]
		property _Color CellBackColor
		{
			_Color get();
			void set(_Color); 
		}

	public: // events
	
	internal: // methodss
		Row(_GridControl^ gridControl);

		Row(_GridControl^ gridControl, GrDataRow* pDataRow);

		void AddEditedCell();
		void RemoveEditedCell();

		virtual void NewCell(_Column^ column);

		void RefreshCells();

		static Row^ FromNative(const GrDataRow* pDataRow);

	internal: // properties
		property object^ Component
		{
			object^ get()  { return m_component; }
			void set(object^);
		}

		property int ComponentIndex
		{
			int get() { return m_componentIndex; }
			void set(int value) { m_componentIndex = value; }
		}

		property GrDataRow* NativeRef
		{
			GrDataRow* get();
		}

		property Cell^ default[GrColumn*]
		{
			Cell^ get(GrColumn* pColumn);
		}

	private: // methods
		bool ShouldSerializeCellForeColor();
		bool ShouldSerializeCellBackColor();
		bool ShouldSerializeCellFont();

	private: // variables
		CellCollection^				m_cellCollection;
		GrDataRow*					m_pDataRow;
		object^						m_component;
		int							m_componentIndex;
		int							m_editedCount;
		bool						m_editing;
	};

	/// <summary>
	/// 새로운 행을 추가하기 위한 편집가능한 행을 제공합니다.
	/// </summary>
	public ref class InsertionRow : Row
	{
	public: // methods
		/// <summary>
		/// 각 셀의 값을 기본값으로 설정합니다.
		/// </summary>
		void						SetDefaultValue();

	internal: // methods
		InsertionRow(_GridControl^ gridControl, GrInsertionRow* pInsertionRow);
		virtual void NewCell(_Column^ column) override;

	internal: // variables
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/