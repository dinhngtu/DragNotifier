using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using static DragNotifierTest.NativeMethods;

namespace DragNotifierTest {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        private void button1_MouseDown(object sender, MouseEventArgs e) {
            var signaldir = Directory.CreateDirectory(Path.Combine(Path.GetTempPath(), "{83120B49-8EFA-4CDE-8AFD-2954AE52B4A3}." + (UInt64)Handle));
            var data = new string[] { signaldir.FullName };
            button1.DoDragDrop(new DataObject(DataFormats.FileDrop, data), DragDropEffects.Copy);
        }

        protected override void WndProc(ref Message m) {
            if (m.Msg == WM_COPYDATA) {
                var container = (COPYDATASTRUCT)m.GetLParam(typeof(COPYDATASTRUCT));
                if (container.dwData == (UIntPtr)0x83120B49) {
                    var destpath = Marshal.PtrToStringUni(container.lpData, (int)(container.cbData / sizeof(Char)));
                    label1.Text = destpath;
                    m.Result = (IntPtr)1;
                    Directory.Delete(Path.Combine(Path.GetTempPath(), "{83120B49-8EFA-4CDE-8AFD-2954AE52B4A3}." + (UInt64)Handle));
                    return;
                }
            }
            base.WndProc(ref m);
        }
    }
}
