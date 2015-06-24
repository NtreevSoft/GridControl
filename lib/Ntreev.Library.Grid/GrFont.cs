using System;
using System.Collections.Generic;
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
            return DefaultFont;
        }

        public static GrFont DefaultFont;

        public static GrFont Empty;
    }
}
