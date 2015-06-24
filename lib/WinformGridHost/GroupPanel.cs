﻿using Ntreev.Library.Grid;
using Ntreev.Windows.Forms.Grid.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class GroupPanel : CellBase
    {
        private readonly GrGroupPanel groupPanel;

        internal GroupPanel(GridControl gridControl, GrGroupPanel groupPanel)
            : base(gridControl, groupPanel)
        {
            this.groupPanel = groupPanel;
        }

        [Category("Layout")]
        public string Text
        {
            get { return this.groupPanel.GetText(); }
            set { this.groupPanel.SetText(value); }
        }

        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsVisible
        {
            get { return this.groupPanel.GetVisible(); }
            set { this.groupPanel.SetVisible(value); }
        }

        private bool ShouldSerializeText()
        {
            return this.Text != Resources.GroupPanel_Text;
        }
    }
}