//////////////////////////////////////////////////////////////////////////////
// FILE:          MMC100.cpp
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   MMC100s Controller Driver
//                XY Stage
//                Z  Stage
//
// COPYRIGHT:     Sutter Instrument,
//				  Mission Bay Imaging, San Francisco, 2011
//                All rights reserved
//
// LICENSE:       This library is free software; you can redistribute it and/or
//                modify it under the terms of the GNU Lesser General Public
//                License as published by the Free Software Foundation.
//                
//                You should have received a copy of the GNU Lesser General Public
//                License along with the source distribution; if not, write to
//                the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
//                Boston, MA  02111-1307  USA
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
//
// AUTHOR:        Lon Chu (lonchu@yahoo.com), created on March 2011
//

#ifdef WIN32
   #include <windows.h>
   #define snprintf _snprintf 
#endif

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <time.h>
#include "../../MMCore/MMCore.h"
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceUtils.h"
#include "MMC100Error.h"
#include "MMC100Ctrl.h"
#include "MMC100ZStage.h"

using namespace std;

MMC100* g_pMMC100;
MPError* g_pMPError;

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

//
// Initialize the MMDevice name
//
MODULE_API void InitializeModuleData()
{
    g_pMMC100 = MMC100::Instance();       // Initiate the MMC100 instance
    g_pMPError = MPError::Instance();   // Initiate the MPError instance


	// initialize the controller device name
    RegisterDevice( MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str(), MM::GenericDevice, MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str());

	// initialize the Z stage device name
    RegisterDevice(MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str(),MM::StageDevice, MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str());
}

//
// Creating the MMDevice
//
MODULE_API MM::Device* CreateDevice(const char* sDeviceName)
{
    // checking for null pinter
    if (sDeviceName == 0) return 0;

    //struct tm tmNewTime;
    //__time64_t lLongTime;

    //_time64(&lLongTime);                        // Get time as 64-bit integer.
    //                                            // Convert to local time.
    //_localtime64_s(&tmNewTime, &lLongTime );    // C4996

    //std::ofstream ofsLogfile;
    //ofsLogfile.open(MMC100::Instance()->GetSKStr(MMC100::SKSTR_LogFilename).c_str(), ios_base::out | ios_base::app);

    //if (ofsLogfile.is_open())
    //{
    //    ofsLogfile << "[" << tmNewTime.tm_year << "::" << tmNewTime.tm_mon << "::" << tmNewTime.tm_mday << "::" << tmNewTime.tm_hour << "::" << tmNewTime.tm_min << "::" << tmNewTime.tm_sec << "]   ";
    //    ofsLogfile << "<MMC100::CreateDevice> deviceName = (" << sDeviceName << ") :: MMC100Ctrl = (" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str() << ") \n" << flush;
    //}

    if (strcmp(sDeviceName, MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str()) == 0) 
    {
        // if device name is MMC100 Controller, create the MMC100 device
        MMC100Ctrl*  pCtrlDev = new MMC100Ctrl();
        return pCtrlDev;
    }

    if (strcmp(sDeviceName, MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str()) == 0)
    {
        // if device name is Z Stage, create the Z Stage Device 
        ZStage* pZStage = new ZStage();
        return pZStage;
    }
    //else
    //{
    //    std::ostringstream sMessage;
    //    sMessage << "<MMC100::CreateDevice> deviceName = (" << sDeviceName << ") :: MMC100Z = (" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str() << ") ";
    //    std::cerr << sMessage.str().c_str() << "\n" << flush;
    //}

    //ofsLogfile.close();

    // device name is not recognized, return null
    return NULL;
}

//
// delete the device --> invoke device destructor
//
MODULE_API void DeleteDevice(MM::Device* pDevice)
{
    if (pDevice != 0) delete pDevice;
}

//
// General utility function
//
int ClearPort(MM::Device& device, MM::Core& core, const char* sPort)
{
    // Clear contents of serial port 
    const int nBufSize = 255;
    unsigned char sClear[nBufSize];                                                        
    unsigned long lRead = nBufSize;                                               
    int ret;

    // reset the communication port buffer
    while ((int) lRead == nBufSize)                                                     
    { 
        // reading from the serial port
        ret = core.ReadFromSerial(&device, sPort, sClear, nBufSize, lRead);

        std::ostringstream sMessage;
        sMessage << "<MMC100::ClearPort> port = (" <<  sPort << ") :: clearBuffer(" << lRead << ")  = (" << sClear << ")";
        core.LogMessage(&device, sMessage.str().c_str(), false);

        // verify the read operation
        if (ret != DEVICE_OK) return ret;                                                           
    } 

    // upon successful restting the port
    return DEVICE_OK;                                                           
} 

bool            MMC100::m_yInstanceFlag      = false;        // instance flag
bool            MMC100::m_yDeviceAvailable   = false;        // MMC100 devices availability
int				MMC100::m_nDebugLogFlag		= 2;			// MMC100 debug log flag
MMC100*          MMC100::m_pMMC100             = NULL;         // single copy MMC100
int             MMC100::m_nResolution        = 10;           // MMC100 resolution
int             MMC100::m_nMotionMode        = 0;            // motor motion mode
int             MMC100::m_nUm2UStep          = 25;           // unit to convert um to uStep
int             MMC100::m_nUStep2Nm          = 40;           // unit to convert uStep to nm
int             MMC100::m_nTimeoutInterval   = 1000;        // timeout interval in millisecond
int             MMC100::m_nTimeoutTrys       = 1;            // timeout trys
long            MMC100::m_lVelocity          = 18000;         // velocity
double          MMC100::m_dPositionX         = 0.00;         // X Position
double          MMC100::m_dPositionY         = 0.00;         // Y Position
double          MMC100::m_dPositionZ         = 0.00;         // Z Position
//int           MMC100::m_nNumberOfAxes      = 3;            // number of axes attached to the controller, initial set to zero
std::string MMC100::m_sPort;                                 // serial port symbols

MMC100::MMC100()
{
    MMC100::m_sSKStr[MMC100::SKSTR_CtrlDevName]       = "MMC100 Controller";					// MMC100 Controllet device name
    MMC100::m_sSKStr[MMC100::SKSTR_XYStgaeDevName]    = "MMC100 XY Stage";						// MMC100 XY Stage device name
    MMC100::m_sSKStr[MMC100::SKSTR_ZStageDevName]     = "MMC100 Z Stage";						// MP286 Z Stage device name
    MMC100::m_sSKStr[MMC100::SKSTR_MMC100Version]      = "2.05.056";							// MMC100 adpater version number
    MMC100::m_sSKStr[MMC100::SKSTR_LogFilename]       = "MMC100Log.txt";						// MMC100 Logfile name
	MMC100::m_sSKStr[MMC100::SKSTR_CtrlDevNameLabel]  = "M.00 Controller ";					// MMC100 Controller device name label
	MMC100::m_sSKStr[MMC100::SKSTR_CtrlDevDescLabel]  = "M.01 Controller ";					// MMC100 Controller device description label
    MMC100::m_sSKStr[MMC100::SKSTR_FirmwareVerLabel]  = "M.02 Firmware Version";				// MMC100 FIRMWARE VERSION label
    MMC100::m_sSKStr[MMC100::SKSTR_MMC100VerLabel]     = "M.03 MMC100 Adapter Version";			// MMC100 ADAPTER VERSION label
	MMC100::m_sSKStr[MMC100::SKSTR_DebugLogFlagLabel] = "M.04 Debug Log Flag";				// MMC100 Debug Lg Flag Label
    MMC100::m_sSKStr[MMC100::SKSTR_CommStateLabel]    = "M.05 MMC100 Comm. Status";			// MMC100 COMM. STATUS label
    MMC100::m_sSKStr[MMC100::SKSTR_ResolutionLabel]   = "M.06 Resolution (10 or 50)";			// MMC100 RESOLUION label
    MMC100::m_sSKStr[MMC100::SKSTR_AccelLabel]        = "M.07 Acceleration";					// MMC100 ACCELERATION label
    MMC100::m_sSKStr[MMC100::SKSTR_Um2UStepUnit]      = "M.08 um to uStep";					// MMC100 um to ustep label
    MMC100::m_sSKStr[MMC100::SKSTR_UStep2NmUnit]      = "M.09 uStep to nm";					// MMC100 ustep to nm label
    MMC100::m_sSKStr[MMC100::SKSTR_VelocityLabel]     = "M.10 Velocity (uStep/s)";			// MMC100 VELOCITY label
    MMC100::m_sSKStr[MMC100::SKSTR_MotionMode]        = "M.11 Mode (0=ABS/1=REL)";			// MMC100 MODE label
    MMC100::m_sSKStr[MMC100::SKSTR_SetOrigin]         = "M.12 Origin (1=set)";                // MMC100 ORIGIN label
    MMC100::m_sSKStr[MMC100::SKSTR_TimeoutInterval]   = "M.13 Timeout Interval (ms)";         // MMC100 Timeout Interval
    MMC100::m_sSKStr[MMC100::SKSTR_TimeoutTrys]       = "M.14 Timeout Trys";                  // MMC100 Timeout Trys
	MMC100::m_sSKStr[MMC100::SKSTR_XYDevNameLabel]    = "M.15 XY Stage ";						// MMC100 XY stage device name label
	MMC100::m_sSKStr[MMC100::SKSTR_XYDevDescLabel]    = "M.16 XY Stage ";						// MMC100 XY stage device description label
    MMC100::m_sSKStr[MMC100::SKSTR_SetPositionX]      = "M.17 Set Position X (um)";			// MMC100 set POSITION X label
    MMC100::m_sSKStr[MMC100::SKSTR_SetPositionY]      = "M.18 Set Position Y (um)";			// MMC100 set POSITION Y label
    MMC100::m_sSKStr[MMC100::SKSTR_GetPositionX]      = "M.19 Get Position X (uStep)";	    // MMC100 get POSITION X label
    MMC100::m_sSKStr[MMC100::SKSTR_GetPositionY]      = "M.20 Get Position Y (uStep)";		// MMC100 get POSITION Y label
	MMC100::m_sSKStr[MMC100::SKSTR_ZDevNameLabel]     = "M.21 Z Stage ";						// MMC100 Z stage device name label
	MMC100::m_sSKStr[MMC100::SKSTR_ZDevDescLabel]     = "M.22 Z Stage ";						// MMC100 Z stage device description label
    MMC100::m_sSKStr[MMC100::SKSTR_SetPositionZ]      = "PositionZ(um)";			// MMC100 set POSITION Z label
    MMC100::m_sSKStr[MMC100::SKSTR_GetPositionZ]      = "M.24 Get Position Z (uStep)";		// MMC100 get POSITION Z label
    MMC100::m_sSKStr[MMC100::SKSTR_PauseMode]         = "M.25 Pause (0=continue/1=pause)";    // property PAUSE label
    MMC100::m_sSKStr[MMC100::SKSTR_Reset]             = "M.26 Reset (1=reset)";               // property RESET label
    MMC100::m_sSKStr[MMC100::SKSTR_Status]            = "M.27 Status (1=update)";             // property STATUS label
}

MMC100::~MMC100()
{
    if (m_pMMC100) delete m_pMMC100;
    m_yInstanceFlag = false;
}

MMC100* MMC100::Instance()
{
    if(!m_yInstanceFlag)
    {
        m_pMMC100 = new MMC100();
        m_yInstanceFlag = true;
    }

    return m_pMMC100;
}

//
// Get MMC100 constant string
//
std::string MMC100::GetSKStr(int nSKStrCode) const
{ 
   string sText;        // MMC100 String

   if (m_pMMC100 != NULL)
   {
       map<int, string>::const_iterator nIterator;
       nIterator = m_sSKStr.find(nSKStrCode);   
       if (nIterator != m_sSKStr.end())
          sText = nIterator->second;
   }

   return sText;
}

//
// Copy byte data buffer for iLength
//
int MMC100::ByteCopy(unsigned char* bDst, const unsigned char* bSrc, int nLength)
{
    int nBytes = 0;
    if (bSrc == NULL || bDst == NULL) return(nBytes);
    for (nBytes = 0; nBytes < nLength; nBytes++) bDst[nBytes] = bSrc[nBytes];
    return nBytes;
}

//
// Convert byte data to hex string
//
void MMC100::Byte2Hex(const unsigned char bByte, char* sHex)
{
    char sHexDigit[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    sHex[2] =  NULL;
    sHex[1] = sHexDigit[(int)(bByte & 0xF)];
    sHex[0] = sHexDigit[(int)(bByte / 0x10)];
    return;
}

