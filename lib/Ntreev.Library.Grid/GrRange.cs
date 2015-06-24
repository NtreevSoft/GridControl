using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrRange
    {
        private int minValue;
        private int maxValue;

        public GrRange()
        {
        }

        public GrRange(int minValue, int maxValue)
        {
            SetRange(minValue, maxValue);
        }

        public override int GetHashCode()
        {
            return this.minValue ^ this.maxValue;
        }

        public override bool Equals(object obj)
        {
            if (obj is GrRange == false)
                return false;
            return this == (GrRange)obj;
        }

        public bool IsIn(int value)
        {
            if (value < this.minValue || value >= this.maxValue)
                return false;
            return true;
        }

        public void SetRange(int minValue, int maxValue)
        {
            if (minValue > maxValue)
            {
                this.minValue = maxValue;
                this.maxValue = minValue;
            }
            else
            {
                this.minValue = minValue;
                this.maxValue = maxValue;
            }
        }

        public static bool operator ==(GrRange range1, GrRange range2)
        {
            if (range1.minValue != range2.minValue || range1.maxValue != range2.maxValue)
                return false;
            return true;
        }

        public static bool operator !=(GrRange range1, GrRange range2)
        {
            return !(range1 == range2);
        }

        public int Length
        {
            get { return this.maxValue - this.minValue; }
        }

        public int Minimum
        {
            get { return this.minValue; }
            set
            {
                if (value > this.maxValue)
                {
                    this.minValue = this.maxValue;
                    this.maxValue = value;
                }
                else
                {
                    this.minValue = value;
                }
            }
        }

        public int Maximum
        {
            get { return this.maxValue; }
            set
            {
                if (value < this.minValue)
                {
                    this.maxValue = this.minValue;
                    this.minValue = value;
                }
                else
                {
                    this.maxValue = value;
                }
            }
        }

        public static readonly GrRange Empty = new GrRange();
    }
}
