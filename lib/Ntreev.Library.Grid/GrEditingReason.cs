using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrEditingReason
    {
        public GrEditingReason()
        {
            editingType = GrEditingType.None;
        }

        public GrEditingReason(char character, bool ime)
        {
            if (ime == true)
                editingType = GrEditingType.Ime;
            else
                editingType = GrEditingType.Char;
            this.character = character;

        }
        public GrEditingReason(GrKeys key)
        {
            editingType = GrEditingType.Key;
            this.key = key;
        }
        public GrEditingReason(GrPoint location)
        {
            editingType = GrEditingType.Mouse;
            this.location = location;
        }

        public GrEditingType editingType;
        public char character;
        public GrKeys key;
        public GrPoint location;
        public int keyValue;
    }
}
