﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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
#include "GridObject.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 스크롤 되는 열의 컬렉션을 나타냅니다.
    /// </summary>
    public ref class GroupRowCollection sealed : GridObject
        , System::Collections::Generic::IEnumerable<GroupRow^>
        , System::Collections::ICollection
    {
    private: // classes

        ref class Enumerator 
            : Ntreev::Windows::Forms::Grid::GridObject
            , System::Collections::Generic::IEnumerator<GroupRow^>
        {
        public:
            Enumerator(_GridControl^ gridControl);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

            property GroupRow^ Current
            {
                virtual GroupRow^ get();
            }

        private:
            property System::Object^ Current_System_Collections_IEnumerator
            {
                virtual System::Object^ get() sealed = System::Collections::IEnumerator::Current::get;
            }

        private:
            unsigned int m_index;
        };

    public: // methods

        /// <summary>
        /// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<GroupRow^>^ GetEnumerator();

    public: // properties

        /// <summary>
        /// 지정한 인덱스에 있는 <see cref="GroupRow"/>을 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 <see cref="GroupRow"/>의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 <see cref="GroupRow"/>입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        property GroupRow^ default[int]
        {
            GroupRow^ get(int index);
        }

        /// <summary>
        /// 지정한 인덱스에 있는 <see cref="GroupRow"/>을 가져옵니다.
        /// </summary>
        /// <param name="text">
        /// 가져올 <see cref="GroupRow"/>의 표시되는 문자열입니다.
        /// </param>
        /// <returns>
        /// 지정된 문자열을 가진 <see cref="GroupRow"/>를 반환합니다. 찾지 못한다면 null을 반환합니다.
        /// </returns>
        property GroupRow^ default[System::String^]
        {
            GroupRow^ get(System::String^ text);
        }

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

    internal: // methods

        GroupRowCollection(_GridControl^ gridControl);

    private: // methods

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerable() sealed = System::Collections::IEnumerable::GetEnumerator;

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

        GrDataRowList* m_pDataRowList;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/