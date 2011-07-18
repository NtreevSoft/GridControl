using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ColumnExtension
{
    partial class CheckControl : UserControl
    {
        public CheckControl()
        {
            InitializeComponent();
        }

        public bool Value
        {
            get
            {
                return this.radioButton1.Checked == true;
            }
            set
            {
                bool oldValue = this.Value;
                if (value == true)
                    this.radioButton1.Checked = true;
                else
                    this.radioButton2.Checked = true;
            }
        }

        /// <summary>
        /// Enter키와 Escape키에 대한 기능 구현(필수 사항은 아님)
        /// </summary>
        /// <remarks>
        /// 기본적으로 Control이 편집되기 시작할때 GridControl에서는 PreviewKeyDown 이벤트에 Enter키와 Escape키에 대해서 반응할 수 있도록 되있다.
        /// 이 컨트롤은 포커스가 2개의 라디오 버튼으로 가있기 때문에 PreviewKeyDown 이벤트가 발생하지 않는다. 
        /// 그래서 이 함수에서 Enter키와 Escape키가 발생했을때 강제로 PreviewKeyDown 이벤트를 발생시켜 GridControl에서 편집 종료 행동을 할 수 있도록 해준다.
        /// </remarks>
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Enter || keyData == Keys.Escape)
            {
                OnPreviewKeyDown(new PreviewKeyDownEventArgs(keyData));
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }
    }
}
