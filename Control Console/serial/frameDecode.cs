using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;



namespace serial
{
    class frameDecode
    {
        const byte STARTCHAR = 0x7E;
        private byte thisaddr;

        enum frameState : byte { S_READY, S_SYNC, S_IDENT, S_READ, S_CHECK };

        private frameState state;
        private byte length;
        private byte checksum;
        private string decode;

        public frameDecode(byte address)
        {
            state = frameState.S_READY;
            thisaddr = address;
        }

        public void setAddr(byte addr)
        {
            thisaddr = addr;
        }

        public bool frameDecoder(byte b)
        {
            switch(state)
            {
                case frameState.S_READY:
                    
                    if (b == STARTCHAR) state = frameState.S_SYNC;
                    else state = frameState.S_READY;
                    break;
                 case frameState.S_SYNC:
                    if (b == thisaddr) state = frameState.S_IDENT;
                    else state = frameState.S_READY;
                    break;
                case frameState.S_IDENT:
                    length = b;
                    checksum = 0;
                    decode = "";
                    if (length != 0) state = frameState.S_READ;
                    else state = frameState.S_READY;
                    break;
                case frameState.S_READ:
                    checksum ^= b;
                    decode += (char)b;
                    if (--length == 0) state = frameState.S_CHECK;
                    else state = frameState.S_READ;
                    break;
                case frameState.S_CHECK:
                    state = frameState.S_READY;
                    if (checksum == b) return true;
                    break;      
            }
            return false;
        }

        public string getDecoded()
        {
            return decode;
        }

    }
}
