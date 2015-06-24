using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    public class EditingReason
    {
        private readonly EditingReasonType reasonType;
        private readonly object data;

        private EditingReason()
        {

        }

        public EditingReason(Point location)
        {
            this.reasonType = EditingReasonType.Mouse;
            this.data = location;
        }

        public EditingReason(Keys key)
        {
            this.reasonType = EditingReasonType.Key;
            this.data = key;
        }

        public EditingReason(char charKey)
        {
            this.reasonType = EditingReasonType.Char;
            this.data = charKey;
        }

        public EditingReason(int keyValue)
        {
            this.reasonType = EditingReasonType.Ime;
            this.data = keyValue;
        }

        internal EditingReason(GrEditingReason reason)
        {
            switch (reason.editingType)
            {
                case GrEditingType.None:
                    data = null;
                    break;
                case GrEditingType.Mouse:
                    data = (Point)reason.location;
                    break;
                case GrEditingType.Key:
                    data = (Keys)reason.key;
                    break;
                case GrEditingType.Char:
                    data = reason.character;
                    break;
                case GrEditingType.Ime:
                    data = (int)reason.keyValue;
                    break;
            }

            reasonType = (EditingReasonType)reason.editingType;
        }

        public EditingReasonType ReasonType
        {
            get { return this.reasonType; }
        }

        public Point Location
        {
            get { return (Point)this.data; }
        }

        public Keys Key
        {
            get { return (Keys)this.data; }
        }

        public char Char
        {
            get { return (char)this.data; }
        }

        public int KeyValue
        {
            get { return (int)this.data; }
        }

        public object Data
        {
            get { return this.data; }
        }

        public static readonly EditingReason None = new EditingReason();

        internal GrEditingReason ToNative()
        {
            GrEditingReason reason = new GrEditingReason();

            switch (reasonType)
            {
                case EditingReasonType.Mouse:
                    reason.location = this.Location;
                    break;
                case EditingReasonType.Key:
                    reason.key = (GrKeys)this.Key;
                    break;
                case EditingReasonType.Char:
                    reason.character = this.Char;
                    break;
                case EditingReasonType.Ime:
                    reason.keyValue = this.KeyValue;
                    break;
            }
            reason.editingType = (GrEditingType)this.reasonType;
            return reason;
        }
    }
}
