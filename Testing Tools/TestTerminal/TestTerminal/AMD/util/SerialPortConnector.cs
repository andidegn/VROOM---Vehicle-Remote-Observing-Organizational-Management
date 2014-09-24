using AMD.Util.Serial.BaudRates;
using System;
using System.IO.Ports;

namespace AMD.Util {
    public class SerialPortConnector {

        public static bool SerialSetup(SerialPort sp, String portName, BaudRate baud) {
            if (portName == null || portName.Length < 4) return false;

            sp.PortName = portName;
            sp.BaudRate = baud.Value;
            sp.Parity = Parity.None;
            sp.StopBits = StopBits.One;
            sp.Handshake = Handshake.None;
            sp.DataBits = 8;
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
