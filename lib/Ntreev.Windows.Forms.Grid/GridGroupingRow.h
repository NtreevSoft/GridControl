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