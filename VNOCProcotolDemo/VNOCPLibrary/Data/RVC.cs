using System;

namespace VNOCPLibrary.Data
{
    public class RVC : DataBase
    {
        protected const byte DeviceAddressLength = 0x10;

        public Guid DeviceAddress { get; set; }

        public override byte[] Data
        {
            get
            {
                byte[] bytes = new byte[this.DataSize];
                int offset = 0;
                Buffer.SetByte(bytes, 0, 0);

                bytes[offset] = DeviceAddressLength;
                offset += ParameterLengthSize;

                Buffer.BlockCopy(this.DeviceAddress.ToByteArray(), 0, bytes, offset, DeviceAddressLength);
                offset += DeviceAddressLength;

                return bytes;
            }

            set
            {
                byte[] bytes = value;
                int offset = 0;

                offset += ParameterLengthSize;

                byte[] tepmData = new byte[DeviceAddressLength];
                Array.ConstrainedCopy(bytes, offset, tepmData, 0, DeviceAddressLength);
                this.DeviceAddress = new Guid(tepmData);
                offset += DeviceAddressLength;
            }
        }

        protected override int DataSize
        {
            get
            {
                return 4 * ParameterCount + DeviceAddressLength;
            }
        }

        protected override int ParameterCount
        {
            get { return 1; }
        }
    }
}
