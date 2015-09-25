using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Diagnostics;
using System.Timers;

namespace RGBWLampControl
{
    public class RGBWLamp
    {
        private
        SerialPort LampPort = new SerialPort();
        Exception UnspecifiedHandshake = new Exception("No specified Handshake, Connection aborted");

        public
            String[] Handshake = new string[2];

        public RGBWLamp()
        {


        }

        public RGBWLamp(String HandshakeMessage, String HandshakeAnswer)
        {
            Handshake[0] = HandshakeMessage;
            Handshake[1] = HandshakeAnswer;
            
        }

        //Start function ==> Try to connect to every available COM Port expecting specified answer to handshake, maintain connection if good answer.
        public Boolean Start()
        {
            Boolean LampFound = false;
            if (Handshake!=null)
            {
                String[] PortNames = SerialPort.GetPortNames();
                foreach (String item in PortNames)
                {
                    LampPort.PortName = item;
                    try
                    {
                        LampPort.Open();
                        if (LampPort.IsOpen)
                        {
                            LampFound = handshake();
                        }
                    }
                    catch (TimeoutException)
                    {
                        LampPort.Close();
                        LampFound = false;
                    }
                    catch (SystemException)
                    {
                        LampPort.Close();
                        LampFound = false;
                    }
                    if (LampFound) break;
                }
                if (!LampFound)
                {
                    throw new System.IO.IOException("No valid HW found on Serial Ports.");
                }
            }
            else
            {
                throw UnspecifiedHandshake;
            }
            return LampFound;
        }

        private Boolean handshake()
        {
            LampPort.DiscardInBuffer();

            Stopwatch Watch = new Stopwatch();
            LampPort.Write(Handshake[0]);
            Watch.Reset();
            Watch.Start();

            while (LampPort.BytesToRead < Handshake[1].Length && Watch.ElapsedMilliseconds < 1000) ;
            Watch.Stop();

            if (Watch.ElapsedMilliseconds >= 1000)
            {
                throw new TimeoutException();
            }
            else
            {

                char[] buffer = new char[Handshake[1].Length];
                LampPort.Read(buffer, 0, Handshake[1].Length);
                return new string(buffer) == Handshake[1];
            }
        }
        public void writeHSI(int hue, int saturation, int brightness)
        {
            if(LampPort.IsOpen)
            {
                byte[] buffer = new byte[10];
                buffer[0] = (byte)'S';
                buffer[1] = (byte)'H';
                buffer[2] = (byte)'S';
                buffer[3] = (byte)'I';
                buffer[4] = (byte)(hue & 0xFF);
                buffer[5] = (byte)((hue & 0xFF00) >> 8);
                buffer[6] = (byte)(saturation & 0xFF);
                buffer[7] = (byte)((saturation & 0xFF00) >> 8);
                buffer[8] = (byte)(brightness & 0xFF);
                buffer[9] = (byte)((brightness & 0xFF00) >> 8);
                //LampPort.Write("SHSI");
                LampPort.Write(buffer,0,10);
            }
        }
        public void fade()
        {
            if (LampPort.IsOpen)
            {
                byte[] buffer = new byte[4];
                buffer[0] = (byte)'F';
                buffer[1] = (byte)'A';
                buffer[2] = (byte)'D';
                buffer[3] = (byte)'E';
                LampPort.Write(buffer, 0, 4);
            }
        }
    }
}
