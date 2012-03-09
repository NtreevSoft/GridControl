//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
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
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 선택된 행의 컬렉션을 나타냅니다.
    /// </summary>
    public ref class SelectedRowCollection : Ntreev::Windows::Forms::Grid::GridObject, 
        System::Collections::Generic::IEnumerable<Ntreev::Windows::Forms::Grid::Row^>, 
        System::Collections::ICollection
    {
    private: // classes

        ref class Enumerator : System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>
        {
        public:
            Enumerator(const GrSelectedRows* selectedRows);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

        public:
            property Ntreev::Windows::Forms::Grid::Row^ Current
            {
                virtual Ntreev::Windows::Forms::Grid::Row^ get();
            }

        private:
            property System::Object^ Current_System_Collections_IEnumerator
            {
                virtual System::Object^ get() sealed = System::Collections::IEnumerator::Current::get;
            }

        private:
            unsigned int m_index;
            const GrSelectedRows* m_selectedRows;
        };

    public: // methods

        /// <summary>
        /// Row::IsSelected의 값을 true로 설정하고 목록에 추가합니다.
        /// </summary>
        /// <remarks> 
        /// Row::IsSelected 의 값이 이미 true였다면 아무런 동작이 일어나지 않습니다. 
        /// 이 메서드는 Row::IsSelected = true 와 동일한 작업을 수행합니다.
        /// </remarks>
        virtual void Add(Ntreev::Windows::Forms::Grid::Row^ row); 

        /// <summary>
        /// Row::IsSelected의 값을 true로 설정하고 목록에 추가하고 focus행에 해당하는 셀의 포커스를 설정합니다.
        /// </summary>
        /// <remarks>
        /// </remarks>
        /// <param name="focusColumn">
        /// 포커스를 설정할 Column입니다.
        /// null 설정시 가장 최근값으로 포커스를 설정합니다.
        /// </param>
        void Add(Ntreev::Windows::Forms::Grid::Row^ row, Ntreev::Windows::Forms::Grid::Column^ focusColumn);

        /// <summary>
        /// Row::IsSelected의 값을 false로 설정하고 목록에 제거합니다.
        /// </summary>
        /// <remarks> 
        /// Row::IsSelected 의 값이 이미 false였다면 아무런 동작이 일어나지 않습니다. 
        /// 이 메서드는 Row::IsSelected = false 와 동일한 작업을 수행합니다.
        /// </remarks>
        virtual bool Remove(Ntreev::Windows::Forms::Grid::Row^ row);

        /// <summary>
        /// 모든 Row::IsSelected 의 값을 false로 설정하고 제거합니다.
        /// </summary>
        /// <remarks> 
        /// 이 메서드는 GridControl::ClearSelection() 와 동일한 작업을 수행합니다.
        /// </remarks>
        virtual void Clear();

        /// <summary>
        /// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>^ GetEnumerator();

    public: // properties

        /// <summary>
        /// 항목의 갯수를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
        /// </returns>
        property int Count
        {
            int get();
        }

        /// <summary>
        /// 지정한 인덱스에 있는 <see cref="Row"/>를 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 <see cref="Row"/>의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 <see cref="Row"/>입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        property Ntreev::Windows::Forms::Grid::Row^ default[int]
        {
            Ntreev::Windows::Forms::Grid::Row^ get(int index);
        }

    internal: // methods

        SelectedRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedRows* selectedRows);

    private: // methods

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerablen() sealed = System::Collections::IEnumerable::GetEnumerator;

        virtual void CopyTo_System_Collections_ICollection(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo;

    private: // properties

        property bool IsSynchronized_System_Collections_ICollection
        {
            virtual bool get() sealed = System::Collections::ICollection::IsSynchronized::get;
        }

        property System::Object^ SyncRoot_System_Collections_ICollection
        {
            virtual System::Object^ get() sealed = System::Collections::ICollection::SyncRoot::get;
        }

        property int Count_System_Collections_ICollection
        {
            virtual int get() sealed = System::Collections::ICollection::Count::get;
        }

    private: // variables

        const GrSelectedRows* m_selectedRows;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/