using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{

    public class GrRange
    {
        private int m_minValue;
        private int m_maxValue;

        public GrRange()
        {
        }

        public GrRange(int minValue, int maxValue)
        {
            SetRange(minValue, maxValue);
        }

        public bool IsIn(int value)
        {
            if (value < m_minValue || value >= m_maxValue)
                return false;
            return true;
        }

        public int GetLength() { return m_maxValue - m_minValue; }

        public static bool operator ==(GrRange range1, GrRange range2)
        {
            if (range1.m_minValue != range2.m_minValue || range1.m_maxValue != range2.m_maxValue)
                return false;
            return true;
        }

        public static bool operator !=(GrRange range1, GrRange range2)
        {
            return !(range1 == range2);
        }

        public int GetMinValue()
        {
            return m_minValue;
        }

        public int GetMaxValue()
        {
            return m_maxValue;
        }

        public void SetMinValue(int value)
        {
            if (value > m_maxValue)
            {
                m_minValue = m_maxValue;
                m_maxValue = value;
            }
            else
            {
                m_minValue = value;
            }
        }

        public void SetMaxValue(int value)
        {
            if (value < m_minValue)
            {
                m_maxValue = m_minValue;
                m_minValue = value;
            }
            else
            {
                m_maxValue = value;
            }
        }

        public void SetRange(int minValue, int maxValue)
        {
            if (minValue > maxValue)
            {
                m_minValue = maxValue;
                m_maxValue = minValue;
            }
            else
            {
                m_minValue = minValue;
                m_maxValue = maxValue;
            }
        }

        public static GrRange Empty = new GrRange();

    }
}
