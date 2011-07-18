using System;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;
using System.Drawing;

namespace ColumnExtension
{
    class ColumnUserDropDown : ColumnDropDown<MonthCalendar>
    {
        /// <summary>
        /// 컬럼의 몇가지 속성을 설정해준다.
        /// </summary>
        public ColumnUserDropDown()
        {
            this.DataType = typeof(DateTime);
            this.Width = 150;

            // 컨트롤의 필요한 속성을 설정해준다.
            this.EditingControl.MaxSelectionCount = 1;

            // 컨트롤의 값의 변경을 확인하기 위하여 이벤트 핸들러를 등록한다.
            this.EditingControl.DateSelected += new DateRangeEventHandler(EditingControl_DateSelected);
        }

        /// <summary>
        /// 값이 변경되었을때 편집을 종료한다.
        /// </summary>
        void EditingControl_DateSelected(object sender, DateRangeEventArgs e)
        {
            this.EditingResult = EditingResult.Ok;
        }

        /// <summary>
        /// MonthCalendar 모양새는 늘일수 있는 컨트롤이 아니므로 이 속성의 값을 false로 반환한다.
        /// </summary>
        public override bool Sizable
        {
            get
            {
                return false;
            }
        }

        /// <summary>
        /// 이 컨트롤이 표시 될때 최적의 크기를 계산하여 넘겨준다.
        /// </summary>
        public override System.Drawing.Size GetPreferredSize(System.Drawing.Size proposedSize)
        {
            Size size = EditingControl.Size;
            size.Width += 2;
            return size;
        }

        /// <summary>
        /// 값의 유효성을 확인하고 수정한다.
        /// </summary>
        private DateTime ValidateValue(object value)
        {
            /// DataSource에 따라 기본값이 null이거나 System.DBNull.Value일 수 있으므로 값의 유효성을 확인한다.
            if (value == null || value.GetType() != typeof(DateTime))
                return DateTime.Today;
            return (DateTime)value;
        }

        /// <summary>
        /// 컨트롤에서 값을 가져온다.
        /// </summary>
        protected override object GetEditingValue(MonthCalendar control)
        {
            return control.SelectionEnd;
        }

        /// <summary>
        /// 컨트롤에서 값을 설정한다.
        /// </summary>
        protected override void SetEditingValue(MonthCalendar control, object value)
        {
            DateTime dateTime = ValidateValue(value);
            control.SelectionStart = dateTime;
            control.SelectionEnd = dateTime;
        }
    }
}
