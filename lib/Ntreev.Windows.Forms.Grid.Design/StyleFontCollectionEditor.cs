using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Design
{
    //public class StyleFontCollectionEditor : CollectionEditor
    //{
    //    public StyleFontCollectionEditor()
    //        : base(typeof(List<StyleFont>))
    //    {

    //    }

    //    protected override object[] GetItems(object editValue)
    //    {
    //        StyleFontCollection fonts = editValue as StyleFontCollection;
    //        List<StyleFont> fontInstances = new List<StyleFont>();

    //        foreach (Font item in fonts)
    //        {
    //            fontInstances.Add(new StyleFont(item));
    //        }

    //        return fontInstances.ToArray();
    //    }

    //    protected override object SetItems(object editValue, object[] value)
    //    {
    //        StyleFontCollection fonts = editValue as StyleFontCollection;
    //        fonts.Clear();
    //        foreach (StyleFont item in value)
    //        {
    //            fonts.Add(item.Font);
    //        }
    //        return editValue;
    //    }

    //    protected override object CreateInstance(Type itemType)
    //    {
    //        return new StyleFont();

    //    }

    //    protected override Type CreateCollectionItemType()
    //    {
    //        Type itemType = base.CreateCollectionItemType();
    //        return itemType;
    //    }

    //    protected override System.Collections.IList GetObjectsFromInstance(object instance)
    //    {
    //        System.Collections.IList values = base.GetObjectsFromInstance(instance);
    //        return values;
    //    }

    //    public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
    //    {
    //        object v = base.EditValue(context, provider, value);
    //        return v;
    //    }

    //    class StyleFont
    //    {
    //        Font font;

    //        public StyleFont(Font font)
    //        {
    //            this.font = font;
    //        }

    //        public StyleFont()
    //        {
    //            this.font = Control.DefaultFont;
    //        }

    //        public Font Font
    //        {
    //            get { return font; }
    //            set { font = value; }
    //        }

    //        public override string ToString()
    //        {
    //            return System.ComponentModel.TypeDescriptor.GetConverter(this.font).ConvertToString(this.font);
    //        }
    //    }
    //}

}
