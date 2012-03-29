namespace VNOCPLibrary.Data
{
    /// <summary>
    /// 数据基类（用于对VNOCP数据包的包体数据的封装和解析提供支持）
    /// </summary>
    public abstract class DataBase
    {
        /// <summary>
        /// 参数长度字段长度
        /// </summary>
        protected const int ParameterLengthSize = 1;

        /// <summary>
        /// 参数个数
        /// </summary>
        protected abstract int ParameterCount { get; }

        /// <summary>
        /// 用于VNOCP数据包包体数据
        /// </summary>
        public abstract byte[] Data { get; set; }

        /// <summary>
        /// 数据长度
        /// </summary>
        protected abstract int DataSize { get; }
    }
}
