using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    public abstract class RowBase : CellBase
    {
        IDataRow m_pDataRow;
        RowBaseCollection m_childs;

        internal RowBase(GridControl gridControl, IDataRow pDataRow)
            : base(gridControl, pDataRow)
        {
            m_pDataRow = pDataRow;
        }

        internal RowBase(IDataRow pDataRow)
            : base(pDataRow)
        {
            m_pDataRow = pDataRow;
        }

        public void Focus()
        {
            this.Focuser.Set(m_pDataRow);
        }

        public void DisplayFirst()
        {
            this.GridControl.DisplayFirst(this);
        }

        [Description("열의 높이입니다.")]
        [Category("Layout")]
        public override int Height
        {
            get { return m_pDataRow.Height; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("value");
                m_pDataRow.Height = value;
            }
        }

        [Description("표시되고 있는지의 여부를 가져오거나 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public virtual bool IsVisible
        {
            get { return m_pDataRow.IsVisible; }
            set { throw new NotImplementedException(); }
        }

#if DEBUG
        [Category("Debug")]
        [Description("표시되는 인덱스입니다.")]
#else
        [Browsable(false)]
#endif
        public int VisibleIndex
        {
            get { return (int)m_pDataRow.GetVisibleIndex(); }
        }

#if DEBUG
        [Category("Debug")]
        [Description("화면에 표시되는 인덱스입니다")]
#else
        [Browsable(false)]
#endif
        public int DisplayIndex
        {
            get { return (int)m_pDataRow.GetDisplayIndex(); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public bool IsFocused
        {
            get { return m_pDataRow.HasFocused(); }
        }

        public bool IsExpanded
        {
            get { return m_pDataRow.IsExpanded(); }
            set { m_pDataRow.Expand(value); }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public RowBaseCollection Childs
        {
            get
            {
                if (m_childs == null)
                {
                    m_childs = new RowBaseCollection(m_pDataRow);
                }
                return m_childs;
            }
        }

        /// <summary>
        /// 부모를 가져옵니다.
        /// </summary>
#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public RowBase Parent
        {
            get
            {
                IDataRow pParent = m_pDataRow.GetParent() as IDataRow;
                if (pParent == null)
                    return null;

                return FromNative.Get(pParent);
            }
        }


        internal new IDataRow NativeRef
        {
            get { return m_pDataRow; }
        }

        private bool ShouldSerializeHeight()
        {
            return m_pDataRow.Height != GrRow.DefaultHeight;
        }
    }
}
