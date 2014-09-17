using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AMD.Util {
    public class SerialPortConnector {


        public static bool SerialSetup(SerialPort sp, String portName) {
            if (portName == null || portName.Length < 4) return false;

            sp.PortName = portName;
            sp.BaudRate = 115200;
            sp.Parity = Parity.None;
            sp.StopBits = StopBits.One;
            sp.Handshake = Handshake.None;
            sp.ReadTimeout = 1000;
            sp.ReadBufferSize = 65536;

            sp.Open();
            sp.ReadTimeout = 1000;
            return true;
        }

        public static String[] getAvalComPort() {
            return System.IO.Ports.SerialPort.GetPortNames();
        }

    }
}
