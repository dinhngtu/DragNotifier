using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DragNotifierTest {
    internal static class NativeMethods {
        public const uint WM_COPYDATA = 0x004A;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct COPYDATASTRUCT {
        public UIntPtr dwData;
        public uint cbData;
        public IntPtr lpData;
    }
}
