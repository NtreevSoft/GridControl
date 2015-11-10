//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 지정된 속성이 스크롤될 수 없는지 설정합니다.
    /// </summary>
    [System::AttributeUsage(System::AttributeTargets::Property)]
    public ref class FrozenAttribute
        : System::Attribute
    {
    public: // methods

        /// <summary>
        /// <see cref="FrozenAttribute"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="frozen">스크롤될 수 없는지에 대한 여부를 설정합니다.</param>
        FrozenAttribute(bool frozen);

    public: // properties

        /// <summary>
        /// 스크롤 될 수 없는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 스크롤 될 수 없으면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsFrozen
        {
            bool get();
        }

    private: // variables

        bool m_frozen;
    };

    /// <summary>
    /// 지정된 속성이 위치 이동이 되는지 설정합니다.
    /// </summary>
    [System::AttributeUsage(System::AttributeTargets::Property)]
    public ref class MovableAttribute
        : System::Attribute
    {
    public: // methods

        MovableAttribute(bool movable);

    public: // properties

        property bool IsMovable
        {
            bool get();
        }

    private: // variables

        bool m_movable;
    };

    [System::AttributeUsage(System::AttributeTargets::Property)]
    public ref class ResizableAttribute
        : System::Attribute
    {
    public: // methods

        ResizableAttribute(bool resizable);

    public: // properties

        property bool IsResizable
        {
            bool get();
        }

    private: // variables

        bool m_resizable;
    };

    [System::AttributeUsage(System::AttributeTargets::Property)]
    public ref class SortableAttribute
        : System::Attribute
    {
    public: // methods

        SortableAttribute(bool sortable);

    public: // properties

        property bool IsSortable
        {
            bool get();
        }

    private: // variables

        bool m_sortable;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/