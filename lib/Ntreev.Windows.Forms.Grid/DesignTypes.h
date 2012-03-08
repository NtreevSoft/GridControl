#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    /// <summary>
    /// 셀을 편집하는 방식을 나타냅니다.
    /// </summary>
    enum class EditStyle : int
    {
        /// <summary>
        /// 단일 컨트롤을 사용하여 셀을 편집합니다.
        /// </summary>
        Control,

        /// <summary>
        /// DropDown에 컨트롤을 부착시켜 셀을 편집할 수 있도록 합니다.
        /// </summary>
        DropDown,

        /// <summary>
        /// Modal 형태를 사용하여 셀을 편집합니다.
        /// </summary>
        Modal,
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/