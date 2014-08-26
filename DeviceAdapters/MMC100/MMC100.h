///////////////////////////////////////////////////////////////////////////////
// FILE:          MMC100.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   MMC100 Micromanipulator Controller
//                XY Stage
//                Z  Stage
//
// COPYRIGHT:     Sutter Instrument,
//                Mission Bay Imaging, San Francisco, 2011
//
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER(S) OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//
// AUTHOR:        Lon Chu (lonchu@yahoo.com) created on March 2011
//                Lon Chu (lonchu@yahoo.com) modified on 
//

#ifndef _MMC100_H_
#define _MMC100_H_

#include <string>
#include <map>	

// Global function to reset the serial port
int ClearPort(MM::Device& device, MM::Core& core, const char* port);

//
// class MMC100 define all global varaiable in singleton class
// it can only be accessed via Instance().
//
class MMC100
{
public:
    ~MMC100();   // Destructor

    typedef int SKStr;
    enum _SKStr
    {
        SKSTR_CtrlDevName       = 0,            // MMC100 controller device name
        SKSTR_XYStgaeDevName    = 1,            // MMC100 XY stage device name
        SKSTR_ZStageDevName     = 2,            // MMC100 Z stage device name
        SKSTR_MMC100Version      = 3,            // MMC100 adapter version
        SKSTR_LogFilename       = 4,            // MMC100 log filename
        SKSTR_CtrlDevNameLabel  = 5,            // MMC100 controller device name label
		SKSTR_CtrlDevDescLabel  = 6,			// MMC100 controller device decription label
        SKSTR_FirmwareVerLabel  = 7,            // MMC100 FIRMWARE VERSION label
        SKSTR_MMC100VerLabel     = 8,            // MMC100 Adapter version label
        SKSTR_DebugLogFlagLabel = 9,            // MMC100 Debug Log Flag label
        SKSTR_CommStateLabel    = 10,           // MMC100 COMM. STATUS label
        SKSTR_ResolutionLabel   = 11,           // MMC100 RESOLUION label
        SKSTR_AccelLabel        = 12,           // MMC100 ACCELERATION label
        SKSTR_Um2UStepUnit      = 13,           // MMC100 um to ustep label
        SKSTR_UStep2NmUnit      = 14,           // MMC100 ustep to nm label
        SKSTR_VelocityLabel     = 15,           // MMC100 VELOCITY label
        SKSTR_MotionMode        = 16,           // MMC100 MODE label
        SKSTR_SetOrigin         = 17,           // MMC100 ORIGIN label
        SKSTR_TimeoutInterval   = 18,           // MMC100 Timeout Interval label
        SKSTR_TimeoutTrys       = 19,           // MMC100 Timeout Trys label
        SKSTR_XYDevNameLabel    = 20,           // MMC100 controller device name label
		SKSTR_XYDevDescLabel    = 21,			// MMC100 controller device decription label
        SKSTR_SetPositionX      = 22,           // MMC100 POSITION X label
        SKSTR_SetPositionY      = 23,           // MMC100 POSITION Y label
        SKSTR_GetPositionX      = 24,           // MMC100 CURRENT POSITION X label
        SKSTR_GetPositionY      = 25,           // MMC100 CURRENT POSITION Y label
        SKSTR_ZDevNameLabel     = 26,           // MMC100 controller device name label
		SKSTR_ZDevDescLabel     = 27,			// MMC100 controller device decription label
        SKSTR_SetPositionZ      = 28,           // MMC100 POSITION Z label
        SKSTR_GetPositionZ      = 29,           // MMC100 CURRENT POSITION Z label
        SKSTR_PauseMode         = 30,           // property PAUSE label
        SKSTR_Reset             = 31,           // property RESET label
        SKSTR_Status            = 32            // property STATUS label
    };

    enum
    {
        MMC100_TxTerm            = 0x0D,         // EOL transmit symbole
        MMC100_RxTerm            = 0x0D,         // EOL receiving symbol
                                                //////////////////////////////////////
                                                // Serial CommunicationError codes
                                                //////////////////////////////////////
        MMC100_SP_OVER_RUN		= 0x30,			// The previous character was not unloaded before the latest was received
        MMC100_FRAME_ERROR		= 0x31,			// The vald stop bits was not received during the appropriate time period
        MMC100_BUFFER_OVER_RUN	= 0x32,			// The input buffer is filled and CR has not been received
        MMC100_BAD_COMMAND		= 0x34,			// Input cannot be interpreted -- command byte not valid
        MMC100_MOVE_INTERRUPTED	= 0x38			// A requested move was interrupted by input of serial port.  This code is
                                                // ORed with any other error code.  The value normally returned is "*",
                                                // i.e., 8 ORed with 4. "4" is reported on the vacuum fluorescent display
    };

    static MMC100* Instance();                                                               // only interface for singleton
    std::string GetSKStr(int nSKStrCode) const;                                             // access prdefined strings
    static void SetDeviceAvailable(bool yFlag) { m_yDeviceAvailable = yFlag; }              // set MMC100 device availability
    static bool GetDeviceAvailability() { return m_yDeviceAvailable; }                      // get MMC100 device availability
	static int  GetDebugLogFlag() { return m_nDebugLogFlag; }								// get MMC100 debug log flag
	static void SetDebugLogFlag(int nDebugLogFlag) { m_nDebugLogFlag = nDebugLogFlag; }		// set MMC100 debug log flag
    static void SetVelocity(long lVelocity) { m_lVelocity = lVelocity; }                    // set MMC100 device velocity
    static long GetVelocity() { return m_lVelocity; }                                       // get MMC100 device velocity
    static void SetResolution(int nResolution) { m_nResolution = nResolution; }             // set MMC100 device resolution
    static int  GetResolution() { return m_nResolution; }                                   // get MMC100 resolution
    static void SetUm2UStep(int nUm2UStep) { m_nUm2UStep = nUm2UStep; }                     // set MMC100 Um to UStep conversion unit
    static int  GetUm2UStep() { return m_nUm2UStep; }                                       // get MMC100 Um to UStep conversion unit
    static void SetUStep2Nm(int nUStep2Nm) { m_nUStep2Nm = nUStep2Nm; }                     // set MMC100 UStep to Nm conversion unit
    static int  GetUStep2Nm() { return m_nUStep2Nm; }                                       // get MMC100 UStep to NM conversion unit
    static void SetMotionMode(int nMotionMode) { m_nMotionMode = nMotionMode; }             // set Motor motion mode
    static int  GetMotionMode() { return m_nMotionMode; }                                   // get Motor motion mode
    static void SetTimeoutInterval(int nInterval) { m_nTimeoutInterval = nInterval; }       // set Timwout Interval
    static int  GetTimeoutInterval() { return m_nTimeoutInterval; }                         // get Timeout Interval
    static void SetTimeoutTrys(int nTrys) { m_nTimeoutTrys = nTrys; }                       // set Timeout Trys
    static int  GetTimeoutTrys() { return m_nTimeoutTrys; }                                 // get Timeout Trys
    //static void SetNumberOfAxes(int nNumberOfAxes) { m_nNumberOfAxes = nNumberOfAxes; }   // set number of axes controlled by MMC100
    //static int  GetNumberOfAxes() { return m_nNumberOfAxes; }                             // get numebr of axes controlled by MMC100
    static void SetPositionX(double dPosition) { m_dPositionX = dPosition; }                // set position x
    static double GetPositionX() { return m_dPositionX; }                                   // get position x
    static void SetPositionY(double dPosition) { m_dPositionY = dPosition; }                // set position y
    static double GetPositionY() { return m_dPositionY; }                                   // get position y
    static void SetPositionZ(double dPosition) { m_dPositionZ = dPosition; }                // set position z
    static double GetPositionZ() { return m_dPositionZ; }                                   // get position z
    static std::string& GetSerialPort() { return m_sPort; }                                 // get serial port symbol
    static void SetSerialPort(std::string sPort) { m_sPort = sPort; }                       // set serial port symbol
    static int ByteCopy(unsigned char* bDst, const unsigned char* bSrc, int nLength);       // copy byte buffer for iLength
    static void Byte2Hex(const unsigned char bByte, char* sHex);                            // convert byte number to hex

protected:
    MMC100();    // Constructor

private:
    static bool                 m_yInstanceFlag;            // singleton flag
    static MMC100*               m_pMMC100;                   // singleton copy
    static bool                 m_yDeviceAvailable;         // MMC100 availability
	static int					m_nDebugLogFlag;			// MMC100 debug log flag
    static int                  m_nResolution;              // MMC100 resolution
    static int                  m_nUm2UStep;                // unit to convert um to uStep
    static int                  m_nUStep2Nm;                // unit to convert uStep to nm
    static int                  m_nTimeoutInterval;         // timeout interval
    static int                  m_nTimeoutTrys;             // timeout trys
    //static int                m_nNumberOfAxes;            // number of MMC100 axes
    static int                  m_nMotionMode;              // motor motion mode
    static double               m_dPositionX;               // position X
    static double               m_dPositionY;               // position Y
    static double               m_dPositionZ;               // position Z
    static long                 m_lVelocity;                // MMC100 velocity
    static std::string          m_sPort;                    // serial port symbols
    std::map<int, std::string>  m_sSKStr;                   // constant strings
};

#endif  //_MMC100_H_