//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
#include "CellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class RowBaseCollection;

    public ref class RowBase abstract : CellBase 
    {
    public: // methods

        /// <summary>
        /// 포커스를 설정합니다.
        /// </summary>
        void Focus();

		/// <summary>
        /// 화면에 첫번째로 표시되도록 스크롤을 조정합니다.
        /// </summary>
		void DisplayFirst();

    public: // properties

        /// <summary>
        /// 행의 높이를 가져오거나 설정합니다.
        /// </summary>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// 매개변수가 0보다 작을 경우
        /// </exception>
        [System::ComponentModel::DescriptionAttribute("열의 높이입니다.")]
        [System::ComponentModel::CategoryAttribute("Layout")]
        property int Height
        {
			virtual int get() override;
            void set(int);
        }

        /// <summary>
        /// 표시되고 있는지의 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 표시되고 있다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("표시되고 있는지의 여부를 가져오거나 설정합니다.")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        [System::ComponentModel::DefaultValueAttribute(true)]
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
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("표시되는 인덱스입니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property int VisibleIndex
        {
            int get(); 
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
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("화면에 표시되는 인덱스입니다")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property int DisplayIndex
        {
            int get(); 
        }

        /// <summary>
        /// 포커스 설정 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 포커스가 설정되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property bool IsFocused
        {
            bool get(); 
        }

        property bool IsExpanded
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 자식들의 컬렉션을 가져옵니다.
        /// </summary>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property RowBaseCollection^ Childs
        {
            RowBaseCollection^ get();
        }

        /// <summary>
        /// 부모를 가져옵니다.
        /// </summary>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property RowBase^ Parent
        {
            RowBase^ get();
        }

    internal: // methods

        RowBase(_GridControl^ gridControl, IDataRow* pDataRow);

        RowBase(IDataRow* pDataRow);

	internal: // properties

		property IDataRow* NativeRef
        {
            IDataRow* get() new;
        }

    private: // methods

        bool ShouldSerializeHeight();

    private: // variables

        IDataRow* m_pDataRow;
        RowBaseCollection^ m_childs;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/