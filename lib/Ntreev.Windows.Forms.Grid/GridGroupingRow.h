#pragma once
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 제목을 표시하는 개체입니다.
	/// </summary>
	[System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class GroupingRow : CellBase
	{
	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual string^ ToString() override
		{
			return Text; 
		}

	public: // properties
		/// <summary>
		/// 제목을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 제목을 나타내는 문자열입니다.
		/// </returns>
		[_Category("Layout")]
		property string^ Text
		{
			string^ get();
			void set(string^);
		}

		/// <summary>
		/// 제목의 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 제목이 표시되면 true를, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		[_Category("Appearance"), _DefaultValue(true)]
		property bool IsVisible
		{
			bool get(); 
			void set(bool); 
		}

	internal: // methods
		GroupingRow(_GridControl^ gridControl, GrGroupingList* pGroupingList);

	private: // variables
		GrGroupingList* m_pGroupingList;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/