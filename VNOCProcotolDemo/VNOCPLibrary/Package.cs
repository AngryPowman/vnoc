using System;
using System.Collections.Generic;

namespace VNOCPLibrary
{
    /// <summary>
    /// VNOCP数据包基类
    /// </summary>
    public abstract class Package
    {
        //-------------------------------------------------------------------
        //
        //  保护常量
        //
        //-------------------------------------------------------------------

        #region 保护常量

        /// <summary>
        /// 起始符字段长度
        /// </summary>
        protected const int StartFlagSize = 1;

        /// <summary>
        /// 令牌字段长度
        /// </summary>
        protected const int VersionSize = 1;

        /// <summary>
        /// 序号字段长度
        /// </summary>
        protected const int PackageIdSize = 1;

        /// <summary>
        /// 令牌字段长度
        /// </summary>
        protected const int TokenSize = 4;

        /// <summary>
        /// 指令字段长度
        /// </summary>
        protected const int CommandSize = 1;

        /// <summary>
        /// 保留字段长度
        /// </summary>
        protected const int ReservedSpaceSize = 4;

        /// <summary>
        /// 参数数量长度
        /// </summary>
        protected const int ParameterCountSize = 1;

        /// <summary>
        /// 包体大小长度
        /// </summary>
        protected const int BodyLengthSize = 4;

        /// <summary>
        /// 信息头长度
        /// </summary>
        protected const int HeaderSize = StartFlagSize + PackageIdSize + TokenSize + CommandSize + ReservedSpaceSize + BodyLengthSize;

        #endregion

        //-------------------------------------------------------------------
        //
        //  公有属性
        //
        //-------------------------------------------------------------------

        #region 公有属性

        # region 包头信息

        /// <summary>
        /// 1. 起始符
        /// </summary>
        public const byte StartFlag = 0x55;

        /// <summary>
        /// 2. 版本号
        /// </summary>
        public byte Version { get; set; }

        /// <summary>
        /// 3. 序号
        /// </summary>
        public byte PackageId { get; set; }

        /// <summary>
        /// 4. 令牌
        /// </summary>
        public Guid Token { get; set; }

        /// <summary>
        /// 5. 指令
        /// </summary>
        public abstract byte Command { get; set; }

        /// <summary>
        /// 6. 保留空间
        /// </summary>
        public int ReservedSpace = 0;

        /// <summary>
        /// 7. 参数数量
        /// </summary>
        public byte ParameterCount { get; set; }

        /// <summary>
        /// 8. 包体总长度
        /// </summary>
        public int BodyLength { get; set; }

        #endregion

        /// <summary>
        /// 参数长度列表
        /// </summary>
        public List<byte> parameterSizeList = new List<byte>();

        /// <summary>
        /// 9. 包体
        /// TODO(Allen):封包和拆包操作要完成,需要定义各种Data
        /// </summary>
        public byte[] Body { get; set; }

        /// <summary>
        /// 包头数据流
        /// </summary>
        public byte[] HeaderData
        {
            get
            {
                byte[] bytes = new byte[HeaderSize];
                int offset = 0;
                Buffer.SetByte(bytes, 0, 0);

                // 设置起始标志
                bytes[offset] = StartFlag;
                offset += StartFlagSize;

                // 设置协议版本
                bytes[offset] = this.Version;
                offset += VersionSize;

                // 设置包编号
                bytes[offset] = this.PackageId;
                offset += PackageIdSize;

                // 设置令牌
                Buffer.BlockCopy(this.Token.ToByteArray(), 0, bytes, offset, TokenSize);
                offset += TokenSize;

                // 设置指令
                bytes[offset] = this.Command;
                offset += CommandSize;

                // 设置保留空间
                Buffer.BlockCopy(BitConverter.GetBytes(this.ReservedSpace), 0, bytes, offset, ReservedSpaceSize);
                offset += ReservedSpaceSize;

                // 设置参数数量
                bytes[offset] = this.ParameterCount;
                offset += ParameterCountSize;

                // 设置包体长度
                Buffer.BlockCopy(BitConverter.GetBytes(this.BodyLength), 0, bytes, offset, BodyLengthSize);
                offset += BodyLengthSize;


                return bytes;
            }

            set
            {
                byte[] bytes = value;

                int offset = 0;

                offset += StartFlagSize;

                // 设置协议版本
                this.Version = bytes[offset];
                offset += VersionSize;

                // 设置包编号
                this.PackageId = bytes[offset];
                offset += PackageIdSize;

                // 设置令牌
                byte[] tepmData = new byte[TokenSize];
                Array.ConstrainedCopy(bytes, offset, tepmData, 0, TokenSize);
                this.Token = new Guid(tepmData);
                offset += TokenSize;

                // 设置指令
                this.Command = bytes[offset];
                offset += CommandSize;

                // 设置保留空间
                Buffer.BlockCopy(BitConverter.GetBytes(this.ReservedSpace), 0, bytes, offset, ReservedSpaceSize);
                offset += ReservedSpaceSize;

                // 设置参数数量
                this.ParameterCount = bytes[offset];
                offset += ParameterCountSize;

                this.BodyLength = BitConverter.ToInt32(bytes, offset);
                offset += this.BodyLength;
            }
        }

        /// <summary>
        /// 获取Byte流
        /// </summary>
        /// <returns>字节流</returns>
        public byte[] Data
        {
            get
            {
                byte[] bytes = new byte[this.PackageSize];
                int offset = 0;
                Buffer.SetByte(bytes, 0, 0);

                // 获取消息头信息
                Buffer.BlockCopy(this.HeaderData, 0, bytes, offset, this.HeaderData.Length);
                offset += this.HeaderData.Length;

                // 获取消息内容
                if (this.Body != null)
                {
                    Buffer.BlockCopy(this.Body, 0, bytes, offset, this.BodyLength);
                    offset += this.BodyLength;
                }

                return bytes;
            }

            set
            {
                byte[] bytes = value;
                int offset = 0;

                // 存储消息头信息
                try
                {
                    byte[] headerDataBytes = new byte[HeaderSize];
                    Buffer.BlockCopy(bytes, offset, headerDataBytes, 0, HeaderSize);
                    this.HeaderData = headerDataBytes;
                    offset += HeaderSize;

                    // 存储消息内容
                    this.Body = new byte[this.BodyLength];
                    Buffer.BlockCopy(bytes, offset, this.Body, 0, this.BodyLength);
                    offset += this.BodyLength;
                }
                catch (Exception exception)
                {
                }
            }
        }

        #endregion

        //-------------------------------------------------------------------
        //
        //  保护属性
        //
        //-------------------------------------------------------------------

        #region 保护属性

        /// <summary>
        /// 包长度
        /// </summary>
        protected int PackageSize
        {
            get { return HeaderSize + this.BodyLength; }
        }

        #endregion

        //-------------------------------------------------------------------
        //
        //  构造函数
        //
        //-------------------------------------------------------------------

        #region 构造函数

        /// <summary>
        /// 构造函数，无参数
        /// </summary>
        public Package()
        {
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="Data">数据信息</param>
        public Package(byte[] headerData)
        {
            this.HeaderData = headerData;
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="Data">数据信息</param>
        public Package(byte[] headerData, byte[] body)
            : this(headerData)
        {
            this.Body = body;
        }

        #endregion
    }
}
