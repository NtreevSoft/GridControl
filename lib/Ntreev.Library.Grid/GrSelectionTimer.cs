using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    class GrSelectionTimer : GrObject, IDisposable
    {
        bool m_horzEnable;
        bool m_vertEnable;

        bool m_scrollAcceleration;
        bool m_intervalAcceleration;

        int m_horzScroll;
        int m_vertScroll;

        GrTimer m_pTimer;
        GrScroll m_pHorzScroll;
        GrScroll m_pVertScroll;

        public GrSelectionTimer()
        {
            m_pTimer = null;
            m_horzEnable = true;
            m_vertEnable = true;

            m_horzScroll = 0;
            m_vertScroll = 0;

            m_scrollAcceleration = true;
            m_intervalAcceleration = true;
        }

        public bool DoScroll()
        {
            bool result = false;
            if (GetHScrollEnabled() == true)
            {
                int newHorz = m_pHorzScroll.GetValue() + m_horzScroll;
                if (m_pHorzScroll.DoScroll(newHorz) == true)
                    result = true;
            }

            if (GetVScrollEnabled() == true)
            {
                int newVert = m_pVertScroll.GetValue() + m_vertScroll;
                if (m_pVertScroll.DoScroll(newVert) == true)
                    result = true;
            }
            return result;
        }

        public void Start()
        {
            m_pTimer.Start();
        }

        public void Stop()
        {
            m_pTimer.Stop();
        }

        public void SetInterval(TimeSpan interval)
        {
            m_pTimer.SetInterval(interval);
        }

        public event ElapsedEventHandler Elapsed;

        public void SetMouseLocation(GrPoint point)
        {
            GrRect inside = GetInsideRectangle();
            GrRect outside = GetOutsideRectangle();

            m_vertScroll = 0;
            m_horzScroll = 0;

            if (outside.Contains(point) == false)
                return;

            if (GetHScrollEnabled() == true)
            {
                if (point.X < inside.Left && point.X >= outside.Left)
                {
                    if (m_pHorzScroll.GetValue() > m_pHorzScroll.GetMinimum())
                    {
                        m_horzScroll = -ComputeSpeed(inside.Left - point.X, inside.Left - outside.Left);
                    }
                }
                else if (point.X >= inside.Right && point.X < outside.Right)
                {
                    if (m_pHorzScroll.GetValue() < m_pHorzScroll.GetMaximum() - m_pHorzScroll.GetLargeChange() + 1)
                    {
                        m_horzScroll = ComputeSpeed(point.X - inside.Right, outside.Right - inside.Right);
                    }
                }
            }

            if (GetVScrollEnabled() == true)
            {
                if (point.Y < inside.Top && point.Y >= outside.Top)
                {
                    if (m_pVertScroll.GetValue() > m_pVertScroll.GetMinimum())
                    {
                        m_vertScroll = -ComputeSpeed(inside.Top - point.Y, inside.Top - outside.Top);
                    }
                }
                else if (point.Y >= inside.Bottom && point.Y < outside.Bottom)
                {
                    if (m_pVertScroll.GetValue() < m_pVertScroll.GetMaximum() - m_pVertScroll.GetLargeChange() + 1)
                    {
                        m_vertScroll = ComputeSpeed(point.Y - inside.Bottom, outside.Bottom - inside.Bottom);
                    }
                }
            }

            int speed = Math.Max(Math.Abs(m_horzScroll), Math.Abs(m_vertScroll));
            if (m_scrollAcceleration == false)
                speed = 1;
            TimeSpan interval;
            switch (speed)
            {
                case 1:
                    interval = new TimeSpan(200);
                    break;
                case 2:
                    interval = new TimeSpan(125);
                    break;
                default:
                    interval = new TimeSpan(50);
                    break;
            }

            if (m_intervalAcceleration == false)
                interval = new TimeSpan(200);

            m_pTimer.SetInterval(interval);
        }

        public bool GetHScrollEnabled()
        {
            return m_horzEnable && m_pHorzScroll.GetVisible() == true;
        }

        public void SetHScrollEnabled(bool value)
        {
            m_horzEnable = value;
        }

        public bool GetVScrollEnabled()
        {
            return m_vertEnable && m_pVertScroll.GetVisible() == true;
        }

        public void SetVScrollEnabled(bool value)
        {
            m_vertEnable = value;
        }

        public bool GetScrollAcceleration()
        {
            return m_scrollAcceleration;
        }

        public void SetScrollAcceleration(bool value)
        {
            m_scrollAcceleration = value;
        }

        public bool GetIntervalAcceleration()
        {
            return m_intervalAcceleration;
        }

        public void SetIntervalAcceleration(bool value)
        {
            m_intervalAcceleration = value;
        }

        public bool CanHScroll()
        {
            return m_horzScroll != 0;
        }

        public bool CanVScroll()
        {
            return m_vertScroll != 0;
        }

        public GrScroll GetHorzScroll()
        {
            return m_pHorzScroll;

        }

        public GrScroll GetVertScroll()
        {
            return m_pVertScroll;
        }

        protected virtual GrRect GetInsideRectangle()
        {
            GrGridWindow pGridWindow = this.GridCore.GetGridWindow();
            return pGridWindow.ClientToScreen(this.GridCore.GetDataRect());
        }

        protected virtual GrRect GetOutsideRectangle()
        {
            GrGridWindow pGridWindow = this.GridCore.GetGridWindow();
            return pGridWindow.GetSrceenRect();
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            GrGridWindow pGridWindow = this.GridCore.GetGridWindow();

            m_pHorzScroll = pGridWindow.GetHorzScroll();
            m_pVertScroll = pGridWindow.GetVertScroll();

            m_pTimer = pGridWindow.CreateTimer();
            m_pTimer.Elapsed += timer_Elapsed;

            //this.GridCore.AttachObject(m_pTimer);
        }

        private int ComputeSpeed(int pos, int length)
        {
            if (length == 0)
                throw new Exception("DivideByZeroException");
            else if (length == 1)
                return 3;
            return (int)Math.Ceiling((pos / (float)length) * 3.0f);
        }

        private void timer_Elapsed(object sender, GrElapsedEventArgs e)
        {
            Elapsed(this, e);
        }

        enum GrScrollSpeed
        {
            GrScrollSpeed_Normal,
            GrScrollSpeed_Fast,
            GrScrollSpeed_Fastest,
        }

        public void Dispose()
        {
            if (m_pTimer != null)
                this.GridCore.GetGridWindow().DestroyTimer(m_pTimer);
        }
    }
}
