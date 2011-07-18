using System;
using Ntreev.Windows.Forms.Grid;

namespace ColumnExtension
{
    class ColumnUserControl : ColumnControl<CheckControl>
    {
        /// <summary>
        /// 컬럼의 몇가지 속성을 설정해준다.
        /// </summary>
        public ColumnUserControl()
        {
            this.DataType = typeof(bool);
            this.Width = 120;
        }

        /// <summary>
        /// 컨트롤에서 값을 가져온다.
        /// </summary>
        protected override object GetEditingValue(CheckControl control)
        {
            return control.Value;
        }

        /// <summary>
        /// 컨트롤에 값을 설정한다.
        /// </summary>
        protected override void SetEditingValue(CheckControl control, object value)
        {
            // value가 유효한 값인지 확인을 해준다.
            if (value == null || value.GetType() != typeof(bool))
                value = false;

            control.Value = (bool)value;
        }
    }
}
