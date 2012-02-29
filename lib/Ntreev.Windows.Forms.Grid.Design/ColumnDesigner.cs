using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design;
using System.ComponentModel;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class ColumnDesigner : ComponentDesigner
    {
        public ColumnDesigner()
        {
                
        }


        public override void Initialize(System.ComponentModel.IComponent component)
        {
            base.Initialize(component);

            Column column = this.Component as Column;

            this.IsVisible = column.IsVisible;
            this.IsResizable = column.IsResizable;
            this.IsMovable = column.IsMovable;
        }

        protected override void PreFilterProperties(System.Collections.IDictionary properties)
        {
            base.PreFilterProperties(properties);

            string[] strArray = new string[] { "IsVisible", "IsResizable", "IsMovable"};
            Attribute[] attributes = new Attribute[0];
            for (int i = 0; i < strArray.Length; i++)
            {
                PropertyDescriptor oldPropertyDescriptor = (PropertyDescriptor)properties[strArray[i]];
                if (oldPropertyDescriptor != null)
                {
                    PropertyDescriptor p = System.ComponentModel.TypeDescriptor.CreateProperty(typeof(ColumnDesigner), 
                                                            oldPropertyDescriptor, attributes);
                    properties[strArray[i]] = p;
                }
            }
        }

        public bool IsMovable
        {
            get;
            set;
        }

        public bool IsResizable
        {
            get;
            set;
        }

        public bool IsVisible
        {
            get; 
            set;
        }
    }
}
