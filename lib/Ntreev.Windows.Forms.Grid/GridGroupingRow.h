#pragma once
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// ������ ǥ���ϴ� ��ü�Դϴ�.
	/// </summary>
	[System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class GroupingRow : CellBase
	{
	public: // methods
		/// <summary>
		/// ���� <see cref="System:Object"/>�� ��Ÿ���� <see cref="System::String"/>�� ��ȯ�մϴ�.
		/// </summary>
		/// <returns>
		/// ���� <see cref="System:Object"/>�� ��Ÿ���� <see cref="System::String"/>�Դϴ�.
		/// </returns>
		virtual string^ ToString() override
		{
			return Text; 
		}

	public: // properties
		/// <summary>
		/// ������ �������ų� �����մϴ�.
		/// </summary>
		/// <returns>
		/// ������ ��Ÿ���� ���ڿ��Դϴ�.
		/// </returns>
		[_Category("Layout")]
		property string^ Text
		{
			string^ get();
			void set(string^);
		}

		/// <summary>
		/// ������ ǥ�� ���θ� �������ų� �����մϴ�.
		/// </summary>
		/// <returns>
		/// ������ ǥ�õǸ� true��, �׷��� ������ false�� ��ȯ�մϴ�.
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