using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Ntreev.Windows.Forms.Grid;
using System.ComponentModel;

namespace ColumnPropertyByAttribute
{
    struct TestStruct
    {
        #region System.ComponentModel에서 지원하는 Attribute

        [Browsable(false)]
        [Description("이 컬럼은 보이지 않습니다.")]
        public bool IsVisible { get; set; }

        [ReadOnly(true)]
        [Description("이 컬럼은 수정할 수 없습니다.")]
        public bool IsReadOnly { get; set; }

        [DisplayName("DisplayName")]
        [Description("이 컬럼은 속성의 이름과 다르게 표시 됩니다..")]
        public bool HasDiaplayName { get; set; }

        #endregion

        #region Ntreev.Windows.Forms.Grid에서 지원하는 Attribute

        [Frozen(true)]
        [Description("이 컬럼은 스크롤에 영향을 받지 않습니다.")]
        public bool IsFrozen { get; set;}

        [Movable(false)]
        [Description("이 컬럼은 컬럼간 이동이 제한됩니다.")]
        public bool IsFixed { get; set; }

        [Resizable(false)]
        [Description("이 컬럼은 마우스로 이용한 크기 변경을 할 수가 없습니다.")]
        public bool IsResizable { get; set; }

        [Sortable(false)]
        [Description("이 컬럼은 정렬기능이 제한됩니다.")]
        public bool CanSort { get; set; }

        #endregion
    }
}