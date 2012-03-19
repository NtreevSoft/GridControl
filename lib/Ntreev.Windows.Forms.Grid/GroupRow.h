//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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
#include "RowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    public ref class GroupRow
        : Ntreev::Windows::Forms::Grid::RowBase
    {
    public: // methods

        /// <summary>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
        /// </summary>
        /// <returns>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        virtual System::String^ ToString() override;

    public: // properties

        property bool IsExpanded
        {
            bool get();
            void set(bool);
        }

        property System::String^ Text
        {
            System::String^ get();
        }

    internal: // methods

        GroupRow(GrGroupRow* pGroupRow);

    private: // variables

        GrGroupRow* m_pGroupRow;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/