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
#include "Cell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    public ref class CellTagCollection : System::Collections::IEnumerable, System::Collections::ICollection
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
        /// �÷����� �ݺ��ϴ� �����ڸ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �����ڸ� ��Ÿ���� <see cref="System::Collections::Generic::IEnumerator"/>�Դϴ�.
        /// </returns>
        virtual System::Collections::IEnumerator^ GetEnumerator();

    public: // properties

        /// <summary>
        /// ���� �̸����� ���� �����ɴϴ�.
        /// </summary>
        /// <param name="columnName">
        /// ���� �̸��Դϴ�.
        /// </param>
        /// <exception cref="System::ArgumentException">
        /// ���� �̸����� ���� ã�� ���ϴ� ���
        /// </exception>
        property System::Object^ default[System::String^]
        {
            System::Object^ get(System::String^ columnName);
            void set(System::String^ columnName, System::Object^ value);
        }

        /// <summary>
        /// ���� �ν��Ͻ��� ���� �����ɴϴ�.
        /// </summary>
        /// <param name="column">
        /// ã�� <see cref="Ntreev::Windows::Forms::Grid::Column"/>�� �ν��Ͻ��Դϴ�.
        /// </param>
        /// <returns>
        /// ã�� ���ϴ� ��� null�� ��ȯ�մϴ�.
        /// </returns>
        /// <exception cref="System::NullReferenceException">
        /// �Ű������� null�� ���
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// ���� �ν��Ͻ��� ���� ã�� ���ϴ� ���
        /// </exception>
        property System::Object^ default[Ntreev::Windows::Forms::Grid::Column^]
        {
            System::Object^ get(Ntreev::Windows::Forms::Grid::Column^ column);
            void set(Ntreev::Windows::Forms::Grid::Column^ column, System::Object^ value);
        }

        /// <summary>
        /// ������ �ε����� �ִ� ���� �����ɴϴ�.
        /// </summary>
        /// <param name="index">
        /// ������ ���� �ε���(0���� ����)�Դϴ�.
        /// </param>
        /// <returns>
        /// ������ �ε����� ���Դϴ�.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// �ε����� 0���� �۰ų�, <see cref="Count"/>���� Ŭ ���
        /// </exception>
        property System::Object^ default[int]
        {
            System::Object^ get(int index);
            void set(int index, System::Object^ value);
        }

        /// <summary>
        /// ���� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ ��Ÿ���� <see cref="System::Int32"/>�� �������Դϴ�.
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