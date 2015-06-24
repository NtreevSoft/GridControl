using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrFont : IDisposable
    {
        public abstract void Dispose();
        public abstract int GetCharacterWidth(char w);
        public abstract int GetHeight();
        public abstract int GetInternalLeading();
        public abstract int GetExternalLeading();
        public abstract int GetStringWidth(string text);

        //#ifdef _MANAGED
        //    gcroot<System::Drawing::Font^> ManagedRef;
        //    static System::Drawing::Font^ ToManaged(GrFont* pFont);
        //    static GrFont* FromManaged(System::Drawing::Font^ font);
        //#endif

        public object Tag { get; set; }


        public static GrFont GetDefaultFont()
        {
            if (defaultFont == null)
            {
                var query = from item in AppDomain.CurrentDomain.GetAssemblies()
                            from type in item.GetTypes()
                            where typeof(IDefaultFontProvider).IsAssignableFrom(type)
                            select type;

                var providerType = query.FirstOrDefault();
                if (providerType != null)
                {
                    IDefaultFontProvider provider = TypeDescriptor.CreateInstance(null, providerType, null, null) as IDefaultFontProvider;
                    defaultFont = provider.GetFont();
                }
            }
            return defaultFont;
        }

        private static GrFont defaultFont;

        public static GrFont Empty;
    }

    public interface IDefaultFontProvider
    {
        GrFont GetFont();
    }
}
