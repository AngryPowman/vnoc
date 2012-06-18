using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using VNOCPLibrary.Data;

namespace VNOCPLibraryTest
{
    
    
    /// <summary>
    ///This is a test class for RVCTest and is intended
    ///to contain all RVCTest Unit Tests
    ///</summary>
    [TestClass()]
    public class RVCTest
    {


        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        //[ClassInitialize()]
        //public static void MyClassInitialize(TestContext testContext)
        //{
        //}
        //
        //Use ClassCleanup to run code after all tests in a class have run
        //[ClassCleanup()]
        //public static void MyClassCleanup()
        //{
        //}
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for RVC Constructor
        ///</summary>
        [TestMethod()]
        public void RVCConstructorTest()
        {
            RVC target = new RVC();
            Assert.Inconclusive("TODO: Implement code to verify target");
        }

        /// <summary>
        ///A test for Data
        ///</summary>
        [TestMethod()]
        public void DataTest()
        {
            RVC target = new RVC(); // TODO: Initialize to an appropriate value
            byte[] expected = null; // TODO: Initialize to an appropriate value
            byte[] actual;
            target.Data = expected;
            actual = target.Data;
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for DataSize
        ///</summary>
        [TestMethod()]
        [DeploymentItem("VNOCPLibrary.dll")]
        public void DataSizeTest()
        {
            RVC_Accessor target = new RVC_Accessor(); // TODO: Initialize to an appropriate value
            int actual;
            actual = target.DataSize;
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for DeviceAddress
        ///</summary>
        [TestMethod()]
        public void DeviceAddressTest()
        {
            RVC target = new RVC(); // TODO: Initialize to an appropriate value
            Guid expected = new Guid(); // TODO: Initialize to an appropriate value
            Guid actual;
            target.DeviceAddress = expected;
            actual = target.DeviceAddress;
            Assert.AreEqual(expected, actual);
            Assert.Inconclusive("Verify the correctness of this test method.");
        }

        /// <summary>
        ///A test for ParameterCount
        ///</summary>
        [TestMethod()]
        [DeploymentItem("VNOCPLibrary.dll")]
        public void ParameterCountTest()
        {
            RVC_Accessor target = new RVC_Accessor(); // TODO: Initialize to an appropriate value
            int actual;
            actual = target.ParameterCount;
            Assert.Inconclusive("Verify the correctness of this test method.");
        }
    }
}
