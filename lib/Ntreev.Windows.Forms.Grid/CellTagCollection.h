//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "Cell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    public ref class CellTagCollection
        : System::Collections::IEnumerable
        , System::Collections::ICollection
    {
    private: // classes

        ref class Enumerator : System::Collections::IEnumerator
        {
        public:
            Enumerator(GrDataRow* pDataRow, GrColumnList* pColumnList);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

        public:
            property System::Object^ Current
            {
                virtual System::Object^ get();
            }

        private:
            unsigned int m_index;
            GrDataRow* m_pDataRow;
            GrColumnList* m_pColumnList;
        };

    public: // methods

        /// <summary>
        /// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
        /// </returns>
        virtual System::Collections::IEnumerator^ GetEnumerator();

    public: // properties

        /// <summary>
        /// 열의 이름으로 셀을 가져옵니다.
        /// </summary>
        /// <param name="columnName">
        /// 열의 이름입니다.
        /// </param>
        /// <exception cref="System::ArgumentException">
        /// 열의 이름으로 셀을 찾지 못하는 경우
        /// </exception>
        property System::Object^ default[System::String^]
        {
            System::Object^ get(System::String^ columnName);
            void set(System::String^ columnName, System::Object^ value);
        }

        /// <summary>
        /// 열의 인스턴스로 셀을 가져옵니다.
        /// </summary>
        /// <param name="column">
        /// 찾을 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
        /// </param>
        /// <returns>
        /// 찾지 못하는 경우 null을 반환합니다.
        /// </returns>
        /// <exception cref="System::NullReferenceException">
        /// 매개변수가 null일 경우
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 열의 인스턴스롤 셀을 찾지 못하는 경우
        /// </exception>
        property System::Object^ default[Ntreev::Windows::Forms::Grid::Column^]
        {
            System::Object^ get(Ntreev::Windows::Forms::Grid::Column^ column);
            void set(Ntreev::Windows::Forms::Grid::Column^ column, System::Object^ value);
        }

        /// <summary>
        /// 지정한 인덱스에 있는 셀을 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 셀의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 셀입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// 인덱스가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        property System::Object^ default[int]
        {
            System::Object^ get(int index);
            void set(int index, System::Object^ value);
        }

        /// <summary>
        /// 셀의 갯수를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
        /// </returns>
        property int Count
        {
            int get();
        }

    private: // methods

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

    internal: // methods 

        CellTagCollection(Ntreev::Windows::Forms::Grid::Row^ row);

    private: // variables

        Ntreev::Windows::Forms::Grid::Row^ m_row;
        GrColumnList* m_pColumnList;
    }; 
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/