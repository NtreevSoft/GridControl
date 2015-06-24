using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class WinFormTimer : GrTimer
    {
        private readonly GridControl gridControl;
        private readonly System.Timers.Timer timer;

        public WinFormTimer(GridControl gridControl)
        {
            this.gridControl = gridControl;
            this.timer = new InternalTimer(this, this.gridControl);
            this.timer.AutoReset = true;
        }

        public void Start()
        {
            this.timer.Start();
        }

        public void Stop()
        {
            this.timer.Stop();
        }

        public void SetInterval(TimeSpan interval)
        {
            this.timer.Interval = (double)interval.Ticks;
        }

        public void Invoke(TimeSpan signalTime)
        {
            this.OnElapsed(new GrElapsedEventArgs(signalTime));
        }

        public event GrElapsedEventHandler Elapsed;

        protected virtual void OnElapsed(GrElapsedEventArgs e)
        {
            if (this.Elapsed != null)
            {
                this.Elapsed(this, e);
            }
        }

        class InternalTimer : System.Timers.Timer
        {
            private readonly WinFormTimer winformTimer;

            public InternalTimer(WinFormTimer winformTimer, GridControl gridControl)
            {
                this.winformTimer = winformTimer;
                this.BeginInit();
                this.Enabled = false;
                this.SynchronizingObject = gridControl;
                this.AutoReset = true;
                this.EndInit();

                this.Elapsed += elapsed;
            }

            private void elapsed(object sender, System.Timers.ElapsedEventArgs e)
            {
                this.winformTimer.Invoke(new TimeSpan(e.SignalTime.Millisecond));
            }
        }

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
