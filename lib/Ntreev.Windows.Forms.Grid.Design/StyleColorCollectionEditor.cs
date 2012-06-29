using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design;
using System.Windows.Forms;
using System.Drawing;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class StyleColorCollectionEditor : CollectionEditor
    {
        Color defaultColor = Color.Empty;
        StyleColorCollection colorCollection;

        public StyleColorCollectionEditor(Type type)
            : base(type)
        {
            
        }

        protected override object CreateInstance(Type itemType)
        {
            Style style = this.Context.Instance as Style;
            if(style == null)
                return base.CreateInstance(itemType);
            if (this.colorCollection != null)
                return this.colorCollection.DefaultColor;
            return defaultColor;
        }

        public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            this.colorCollection = value as StyleColorCollection;
            return base.EditValue(context, provider, value);
        }
    }
}
