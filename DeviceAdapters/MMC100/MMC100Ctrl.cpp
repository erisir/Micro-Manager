//////////////////////////////////////////////////////////////////////////////
// FILE:          MMC100Ctrl.cpp
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   MMC100 Controller Driver
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
// AUTHOR:        Lon Chu (lonchu@yahoo.com), created on June 2011
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
//#include <strsafe.h>
#include "../../MMCore/MMCore.h"
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceUtils.h"
#include "MMC100Error.h"
#include "MMC100.h"
#include "MMC100Ctrl.h"

using namespace std;


//////////////////////////////////////////////////////
// MMC100 Controller
//////////////////////////////////////////////////////
//
// Controller - Controller for XYZ Stage.
// Note that this adapter uses two coordinate systems.  There is the adapters own coordinate
// system with the X and Y axis going the 'Micro-Manager standard' direction
// Then, there is the MMC100s native system.  All functions using 'steps' use the MMC100 system
// All functions using Um use the Micro-Manager coordinate system
//


//
// MMC100 Controller Constructor
//
MMC100Ctrl::MMC100Ctrl() :
    //m_nAnswerTimeoutMs(1000),   // wait time out set 1000 ms
    m_yInitialized(false)       // initialized flag set to false
{
    // call initialization of error messages
    InitializeDefaultErrorMessages();

    m_nAnswerTimeoutMs = MMC100::Instance()->GetTimeoutInterval();
    m_nAnswerTimeoutTrys = MMC100::Instance()->GetTimeoutTrys();

    // Port:
    CPropertyAction* pAct = new CPropertyAction(this, &MMC100Ctrl::OnPort);
    int ret = CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);
    
    std::ostringstream osMessage;

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::class-constructor> CreateProperty(" << MM::g_Keyword_Port << " = Undfined), ReturnCode=" << ret;
		this->LogMessage(osMessage.str().c_str());
	}
}

//
// MMC100 Controller Destructor
//
MMC100Ctrl::~MMC100Ctrl()
{
    Shutdown();
}

//
// return device name of the MMC100 controller
//
void MMC100Ctrl::GetName(char* sName) const
{
    CDeviceUtils::CopyLimitedString(sName, MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str());
}

//
// Initialize the MMC100 controller
//
int MMC100Ctrl::Initialize()
{
    std::ostringstream osMessage;

    // empty the Rx serial buffer before sending command
    int ret = ClearPort(*this, *GetCoreCallback(), MMC100::Instance()->GetSerialPort().c_str());

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::Initialize> ClearPort(Port = " << MMC100::Instance()->GetSerialPort().c_str() << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    // Name
    char sCtrlName[120];
	sprintf(sCtrlName, "%s%s", MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevNameLabel).c_str(), MM::g_Keyword_Name);
    ret = CreateProperty(sCtrlName, MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str(), MM::String, true);

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::Initialize> CreateProperty(" << sCtrlName << " = " << MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevName).c_str() << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    // Description
    char sCtrlDesc[120];
	sprintf(sCtrlDesc, "%s%s", MMC100::Instance()->GetSKStr(MMC100::SKSTR_CtrlDevDescLabel).c_str(), MM::g_Keyword_Description);
    ret = CreateProperty(sCtrlDesc, "Sutter MP-285 Controller", MM::String, true);

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::Initialize> CreateProperty(" << sCtrlDesc << " = " << "Sutter MP-285 Controller" << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK)  return ret;

    // Create read-only property for version info
    // MMC100 Adpater Version Property
    // const char* sVersionLabel = MMC100::Instance()->GetSKStr(MMC100::SKSTR_MMC100VerLabel).c_str();
    // const char* sVersion = MMC100::Instance()->GetSKStr(MMC100::SKSTR_MMC100Version).c_str();
    ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_MMC100VerLabel).c_str(), MMC100::Instance()->GetSKStr(MMC100::SKSTR_MMC100Version).c_str(), MM::String, true);

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::Initialize> CreateProperty(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_MMC100VerLabel).c_str() << " = " << MMC100::Instance()->GetSKStr(MMC100::SKSTR_MMC100Version).c_str() << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    char sTimeoutInterval[20];
    memset(sTimeoutInterval, 0, 20);
    sprintf(sTimeoutInterval, "%d", MMC100::Instance()->GetTimeoutInterval());

    CPropertyAction* pActOnTimeoutInterval = new CPropertyAction(this, &MMC100Ctrl::OnTimeoutInterval);
    ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutInterval).c_str(), sTimeoutInterval, MM::Integer,  false, pActOnTimeoutInterval); 

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::Initialize> CreateProperty(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutInterval).c_str() << " = " << sTimeoutInterval << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    char sTimeoutTrys[20];
    memset(sTimeoutTrys, 0, 20);
    sprintf(sTimeoutTrys, "%d", MMC100::Instance()->GetTimeoutTrys());

    CPropertyAction* pActOnTimeoutTrys = new CPropertyAction(this, &MMC100Ctrl::OnTimeoutTrys);
    ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutTrys).c_str(), sTimeoutTrys, MM::Integer,  false, pActOnTimeoutTrys); 

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::Initialize> CreateProperty(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutTrys).c_str() << " = " << sTimeoutTrys << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;


    // Read status data
    unsigned int nLength = 256;
    unsigned char sResponse[256];
    ret = CheckStatus(sResponse, nLength);

    if (ret != DEVICE_OK) return ret;


    ret = UpdateStatus();
    if (ret != DEVICE_OK) return ret;

    // Create  property for debug log flag
    int nDebugLogFlag = MMC100::Instance()->GetDebugLogFlag();
    CPropertyAction* pActDebugLogFlag = new CPropertyAction (this, &MMC100Ctrl::OnDebugLogFlag);
	ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_DebugLogFlagLabel).c_str(), CDeviceUtils::ConvertToString(nDebugLogFlag), MM::Integer, false, pActDebugLogFlag);

    if (MMC100::Instance()->GetDebugLogFlag() > 0)
    {
        osMessage.str("");
        osMessage << "MMC100Ctrl::Initialize> CreateProperty(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_DebugLogFlagLabel).c_str() << " = " << nDebugLogFlag << "), ReturnCode = " << ret;
        this->LogMessage(osMessage.str().c_str());
    }

	m_yInitialized = true;
    MMC100::Instance()->SetDeviceAvailable(true);

    return DEVICE_OK;
}

//
// check controller's status bytes
//
int MMC100Ctrl::CheckStatus(unsigned char* sResponse, unsigned int nLength)
{
    std::ostringstream osMessage;
    unsigned char sCommand[8] ="1VER?\n\r";
    int ret = WriteCommand(sCommand, 8);

    if (ret != DEVICE_OK) return ret;

    //unsigned int nBufLen = 256;
    //unsigned char sAnswer[256];

    memset(sResponse, 0, nLength);
    ret = ReadMessage(sResponse, 8);
//"FINE-01r"
	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::CheckStatus::ReadMessage> (ReturnCode = " << ret << ")return msg=("<<sResponse<<")";
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

//
// shutdown the controller
//
int MMC100Ctrl::Shutdown()
{ 
    m_yInitialized = false;
    MMC100::Instance()->SetDeviceAvailable(false);
    return DEVICE_OK;
}

//////////////// Action Handlers (Hub) /////////////////

//
// check for valid communication port
//
int MMC100Ctrl::OnPort(MM::PropertyBase* pProp, MM::ActionType pAct)
{
    std::ostringstream osMessage;

	osMessage.str("");

    if (pAct == MM::BeforeGet)
    {
        pProp->Set(MMC100::Instance()->GetSerialPort().c_str());
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnPort> (BeforeGet::PORT=<" << MMC100::Instance()->GetSerialPort().c_str() << ">";
			osMessage << " PROPSET=<" << MMC100::Instance()->GetSerialPort().c_str() << ">)";
		}
	}
    else if (pAct == MM::AfterSet)
    {
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnPort> (AfterSet::PORT=<" << MMC100::Instance()->GetSerialPort().c_str() << ">";
		}
        if (m_yInitialized)
        {
            pProp->Set(MMC100::Instance()->GetSerialPort().c_str());
			if (MMC100::Instance()->GetDebugLogFlag() > 1)
			{
				osMessage << "Initialized::SET=<" << MMC100::Instance()->GetSerialPort().c_str() << ">";
				this->LogMessage(osMessage.str().c_str());
			}
            return DEVICE_INVALID_INPUT_PARAM;
        }
        pProp->Get(MMC100::Instance()->GetSerialPort());
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << " SPROPGET=<" << MMC100::Instance()->GetSerialPort().c_str() << ">)";
		}
    }
	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}
    return DEVICE_OK;
}

//
// get/set debug log flag
//
int MMC100Ctrl::OnDebugLogFlag(MM::PropertyBase* pProp, MM::ActionType pAct)
{
    long lDebugLogFlag = (long)MMC100::Instance()->GetDebugLogFlag();
    std::ostringstream osMessage;

	osMessage.str("");

    if (pAct == MM::BeforeGet)
    {
        pProp->Set(lDebugLogFlag);
        if (MMC100::Instance()->GetDebugLogFlag() > 1)
        {
			osMessage << "<MMC100Ctrl::OnDebugLogFalg> (BeforeGet::<" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_DebugLogFlagLabel).c_str() << "> PROPSET=<" << lDebugLogFlag << ">)";
        }
    }
    else if (pAct == MM::AfterSet)
    {
        pProp->Get(lDebugLogFlag);
        MMC100::Instance()->SetDebugLogFlag((int)lDebugLogFlag);
        if (MMC100::Instance()->GetDebugLogFlag() > 1)
        {
            osMessage << "<MMC100Ctrl::OnDebugLogFalg> (AfterSet::<" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_DebugLogFlagLabel).c_str() << "> PROPSET=<" << lDebugLogFlag << ">)";
        }
    }

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
        this->LogMessage(osMessage.str().c_str());
    }

    return DEVICE_OK;
}

//
// Set current position as origin (0,0) coordinate of the controller.
//
int MMC100Ctrl::SetOrigin()
{
    unsigned char sCommand[6] = { 0x6F, MMC100::MMC100_TxTerm, 0x0A, 0x00, 0x00, 0x00 };
    int ret = WriteCommand(sCommand, 3);

    std::ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100::MMC100::SetOrigin> (ReturnCode=" << ret << ")";
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret!=DEVICE_OK) return ret;

    unsigned char sResponse[64];

    memset(sResponse, 0, 64);
    ret = ReadMessage(sResponse, 2);

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::CheckStatus::SetOrigin> (ReturnCode = " << ret << ")";
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    bool yCommError = CheckError(sResponse[0]);

    char sCommStat[30];
    if (yCommError)
        sprintf(sCommStat, "Error Code ==> <%2x>", sResponse[0]);
    else
        strcpy(sCommStat, "Success");

    ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), sCommStat);

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

//
// Set resolution.
//
int MMC100Ctrl::SetResolution(long lResolution)
{
    std::ostringstream osMessage;
    //unsigned char sCmdSet[6] = { 0x56, 0x00, 0x00, MMC100::MMC100_TxTerm, 0x0A, 0x00 };
    //unsigned char sResponse[64];
    //int ret = DEVICE_OK;
    //char sCommStat[30];
    //bool yCommError = false;

    //if (MMC100::Instance()->GetResolution() == 50)
    //    lVelocity = (lVelocity & 0x7FFF) | 0x8000;
    // else
    //    lVelocity = lVelocity & 0x7FFF;

    //sCmdSet[1] = (unsigned char)((lVelocity & 0xFF00) / 256);
    //sCmdSet[2] = (unsigned char)(lVelocity & 0xFF);
        
    //ret = WriteCommand(sCmdSet, 5);

	//if (MMC100::Instance()->GetDebugLogFlag() > 1)
    //{
	//	osMessage.str("");
	//	osMessage << "<MMC100Ctrl::SetVelocity> = " << lVelocity << ", ReturnCode = " << ret;
	//	this->LogMessage(osMessage.str().c_str());
	//}

    //if (ret != DEVICE_OK) return ret;

    //ret = ReadMessage(sResponse, 2);

    //if (ret != DEVICE_OK) return ret;

    //MMC100::Instance()->SetVelocity(lVelocity);

    //yCommError = CheckError(sResponse[0]);
    //if (yCommError)
    //    sprintf((char*)sCommStat, "Error Code ==> <%2x>", sResponse[0]);
    //else
    //    strcpy(sCommStat, "Success");

    //ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), sCommStat);

    //if (ret != DEVICE_OK) return ret;

	MMC100::Instance()->SetResolution(lResolution);

    return DEVICE_OK;
}

//
// Set velocity.
//
int MMC100Ctrl::SetVelocity(long lVelocity)
{
    std::ostringstream osMessage;
    unsigned char sCmdSet[6] = { 0x56, 0x00, 0x00, MMC100::MMC100_TxTerm, 0x0A, 0x00 };
    unsigned char sResponse[64];
    int ret = DEVICE_OK;
    char sCommStat[30];
    bool yCommError = false;

    if (MMC100::Instance()->GetResolution() == 50)
        lVelocity = (lVelocity & 0x7FFF) | 0x8000;
    else
        lVelocity = lVelocity & 0x7FFF;

    sCmdSet[1] = (unsigned char)((lVelocity & 0xFF00) / 256);
    sCmdSet[2] = (unsigned char)(lVelocity & 0xFF);
        
    ret = WriteCommand(sCmdSet, 5);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::SetVelocity> = " << lVelocity << ", ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    ret = ReadMessage(sResponse, 2);

    if (ret != DEVICE_OK) return ret;

    MMC100::Instance()->SetVelocity(lVelocity);

    yCommError = CheckError(sResponse[0]);
    if (yCommError)
        sprintf((char*)sCommStat, "Error Code ==> <%2x>", sResponse[0]);
    else
        strcpy(sCommStat, "Success");

    ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), sCommStat);

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

//
// Set Motion Mode
//
int MMC100Ctrl::SetMotionMode(long lMotionMode)
{
    std::ostringstream osMessage;
    unsigned char sCommand[6] = { 0x00, MMC100::MMC100_TxTerm, 0x0A, 0x00, 0x00, 0x00 };
    unsigned char sResponse[64];
    int ret = DEVICE_OK;
    char sCommStat[30];
    bool yCommError = false;
        
    if (lMotionMode == 0)
        sCommand[0] = 'a';
    else
        sCommand[0] = 'b';

    ret = WriteCommand(sCommand, 3);

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::SetMotionMode> = [" << lMotionMode << "," << sCommand[0] << "], Returncode =" << ret;
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    ret = ReadMessage(sResponse, 2);

    if (ret != DEVICE_OK) return ret;

    MMC100::Instance()->SetMotionMode(lMotionMode);

    yCommError = CheckError(sResponse[0]);
    if (yCommError)
        sprintf((char*)sCommStat, "Error Code ==> <%2x>", sResponse[0]);
    else
        strcpy((char*)sCommStat, "Success");

    ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), (const char*)sCommStat);

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

//
// stop and interrupt Z stage motion
//
int MMC100Ctrl::Stop()
{
    unsigned char sCommand[6] = { 0x03, MMC100::MMC100_TxTerm, 0x0A, 0x00, 0x00, 0x00 };

    int ret = WriteCommand(sCommand, 3);

    ostringstream osMessage;

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100::MMC100Ctrl::Stop> (ReturnCode = " << ret << ")";
		this->LogMessage(osMessage.str().c_str());
	}

    return ret;
}

/*
 * Resolution as returned by device is in 0 or 1 of Bits 15
 */
int MMC100Ctrl::OnResolution(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    std::ostringstream osMessage;
    int ret = DEVICE_OK;
	long lResolution = (long)MMC100::Instance()->GetResolution();

	osMessage.str("");

    if (eAct == MM::BeforeGet)
    {
        pProp->Set(lResolution);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnResolution> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_ResolutionLabel).c_str() << " = [" << lResolution << "], ReturnCode = " << ret;
		}
    }
    else if (eAct == MM::AfterSet)
    {
        pProp->Get(lResolution);

        ret = SetResolution(lResolution);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnResolution> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_ResolutionLabel).c_str() << " = [" << lResolution << "], ReturnCode = " << ret;
		}
    }

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}



/*
 * Speed as returned by device is in um/s
 */
int MMC100Ctrl::OnSpeed(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    std::ostringstream osMessage;
    int ret = DEVICE_OK;
    long lVelocity = MMC100::Instance()->GetVelocity();

	osMessage.str("");

    if (eAct == MM::BeforeGet)
    {
        pProp->Set(lVelocity);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnSpeed> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_VelocityLabel).c_str() << " = [" << lVelocity << "], ReturnCode = " << ret;
		}
    }
    else if (eAct == MM::AfterSet)
    {
        pProp->Get(lVelocity);

        ret = SetVelocity(lVelocity);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnSpeed> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_VelocityLabel).c_str() << " = [" << lVelocity << "], ReturnCode = " << ret;
		}
    }

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

/*
 * Speed as returned by device is in um/s
 */
int MMC100Ctrl::OnMotionMode(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    std::string sMotionMode;
    std::ostringstream osMessage;
    long lMotionMode = (long)MMC100::Instance()->GetMotionMode();
    int ret = DEVICE_OK;

	osMessage.str("");

    if (eAct == MM::BeforeGet)
    {
        pProp->Set(lMotionMode);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnMotionMode> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_MotionMode).c_str() << " = " << lMotionMode << "), ReturnCode = " << ret;
		}
    }
    else if (eAct == MM::AfterSet)
    {
        pProp->Get(lMotionMode);

        ret = SetMotionMode(lMotionMode);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnSpeed> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_MotionMode).c_str() << " = " << lMotionMode <<  "), ReturnCode = " << ret;
		}
    }    
	
	if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

/*
 * Set/Get Timeout Interval
 */
int MMC100Ctrl::OnTimeoutInterval(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    std::ostringstream osMessage;
    long lTimeoutInterval = (long)MMC100::Instance()->GetTimeoutInterval();
    int ret = DEVICE_OK;

	osMessage.str("");

    if (eAct == MM::BeforeGet)
    {
        pProp->Set(lTimeoutInterval);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnTimeoutInterval> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutInterval).c_str() << " = " << lTimeoutInterval << "), ReturnCode = " << ret;
		}
    }
    else if (eAct == MM::AfterSet)
    {
        pProp->Get(lTimeoutInterval);
        MMC100::Instance()->SetTimeoutInterval((int)lTimeoutInterval);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnTimeoutInterval> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutInterval).c_str() << " = " << MMC100::Instance()->GetTimeoutInterval() <<  "), ReturnCode = " << ret;
		}
    }

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

/*
 * Set/Get Timeout Trys
 */
int MMC100Ctrl::OnTimeoutTrys(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    std::ostringstream osMessage;
    long lTimeoutTrys = (long)MMC100::Instance()->GetTimeoutTrys();
    int ret = DEVICE_OK;

	osMessage.str("");

    if (eAct == MM::BeforeGet)
    {
        pProp->Set(lTimeoutTrys);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnTimeoutTrys> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutTrys).c_str() << " = " << lTimeoutTrys << "), ReturnCode = " << ret;
		}
    }
    else if (eAct == MM::AfterSet)
    {
        pProp->Get(lTimeoutTrys);
        MMC100::Instance()->SetTimeoutTrys((int)lTimeoutTrys);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnTimeoutTrys> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_TimeoutTrys).c_str() << " = " << MMC100::Instance()->GetTimeoutTrys() <<  "), ReturnCode = " << ret;
		}
    }

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Internal, helper methods
///////////////////////////////////////////////////////////////////////////////

//
// Write a coomand to serial port
//
int MMC100Ctrl::WriteCommand(unsigned char* sCommand, int nLength)
{
    int ret = DEVICE_OK;
    ostringstream osMessage;

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		osMessage.str("");
		osMessage << "<MMC100Ctrl::WriteCommand> (Command=";
		char sHex[4] = { NULL, NULL, NULL, NULL };
		for (int n=0; n < nLength; n++)
		{
			MMC100::Instance()->Byte2Hex((const unsigned char)sCommand[n], sHex);
			osMessage << "[" << n << "]=<" << sHex << ">";
		}
		osMessage << ")";
		this->LogMessage(osMessage.str().c_str());
	}

    for (int nBytes = 0; nBytes < nLength && ret == DEVICE_OK; nBytes++)
    {
        ret = WriteToComPort(MMC100::Instance()->GetSerialPort().c_str(), (const unsigned char*)&sCommand[nBytes], 1);
        CDeviceUtils::SleepMs(1);
    }

    if (ret != DEVICE_OK) return ret;

    return DEVICE_OK;
}

//
// Read a message from serial port
//
int MMC100Ctrl::ReadMessage(unsigned char* sResponse, int nBytesRead)
{
    // block/wait for acknowledge, or until we time out;
    unsigned int nLength = 256;
    unsigned char sAnswer[256];
    memset(sAnswer, 0, nLength);
    unsigned long lRead = 0;
    unsigned long lStartTime = GetClockTicksUs();

    ostringstream osMessage;
    char sHex[4] = { NULL, NULL, NULL, NULL };
    int ret = DEVICE_OK;
    bool yRead = false;
    bool yTimeout = false;
    while (!yRead && !yTimeout && ret == DEVICE_OK )
    {
        unsigned long lByteRead;

        const MM::Device* pDevice = this;
        ret = (GetCoreCallback())->ReadFromSerial(pDevice, MMC100::Instance()->GetSerialPort().c_str(), (unsigned char *)&sAnswer[lRead], (unsigned long)nLength-lRead, lByteRead);
       
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage.str("");
			osMessage << "<MMC100Ctrl::ReadMessage> (ReadFromSerial = (" << nBytesRead << "," << lRead << "," << lByteRead << ")::<";
		
			for (unsigned long lIndx=0; lIndx < lByteRead; lIndx++)
			{
				// convert to hext format
				MMC100::Instance()->Byte2Hex(sAnswer[lRead+lIndx], sHex);
				osMessage << "[" << sHex  << "]";
			}
			osMessage << ">";
			this->LogMessage(osMessage.str().c_str());
		}

        // concade new string
        lRead += lByteRead;
//
//        if (lRead > 2)
//        {
//            yRead = (sAnswer[0] == 0x30 || sAnswer[0] == 0x31 || sAnswer[0] == 0x32 || sAnswer[0] == 0x34 || sAnswer[0] == 0x38) &&
//                    (sAnswer[1] == 0x0D) &&
//                    (sAnswer[2] == 0x0D);
//        }
//        else if (lRead == 2)
//        {
//            yRead = (sAnswer[0] == 0x0D) && (sAnswer[1] == 0x0D);
//        }
//
//        yRead = yRead || (lRead >= (unsigned long)nBytesRead);
//
         yRead =   (lRead >= (unsigned long)nBytesRead);
        if (yRead) break;
        
        // check for timeout
        yTimeout = ((double)(GetClockTicksUs() - lStartTime) / 10000. ) > (double) m_nAnswerTimeoutMs;
        if (!yTimeout) CDeviceUtils::SleepMs(3);

		//if (MMC100::Instance()->GetDebugLogFlag() > 2)
		//{
		//	osMessage.str("");
		//	osMessage << "<MMC100Ctrl::ReadMessage> (ReadFromSerial = (" << nBytesRead << "," << lRead << "," << yRead << yTimeout << ")";
		//	this->LogMessage(osMessage.str().c_str());
		//}
    }

    // block/wait for acknowledge, or until we time out
    // if (!yRead || yTimeout) return DEVICE_SERIAL_TIMEOUT;
    // MMC100::Instance()->ByteCopy(sResponse, sAnswer, nBytesRead);
    // if (checkError(sAnswer[0])) ret = DEVICE_SERIAL_COMMAND_FAILED;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<MMC100Ctrl::ReadMessage> (ReadFromSerial = <";
	}

	for (unsigned long lIndx=0; lIndx < (unsigned long)nBytesRead; lIndx++)
	{
		sResponse[lIndx] = sAnswer[lIndx];
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			MMC100::Instance()->Byte2Hex(sResponse[lIndx], sHex);
			osMessage << "[" << sHex  << ",";
			MMC100::Instance()->Byte2Hex(sAnswer[lIndx], sHex);
			osMessage << sHex  << "]";
		}
	}

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage << ">";
		this->LogMessage(osMessage.str().c_str());
	}

    return DEVICE_OK;
}

//
// check the error code for the message returned from serial communivation
//
bool MMC100Ctrl::CheckError(unsigned char bErrorCode)
{
    // if the return message is 2 bytes message including CR
    unsigned int nErrorCode = 0;
    ostringstream osMessage;

	osMessage.str("");

    // check 4 error code
    if (bErrorCode == MMC100::MMC100_SP_OVER_RUN)
    {
        // Serial command buffer over run
        nErrorCode = MPError::MPERR_SerialOverRun;       
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
    }
    else if (bErrorCode == MMC100::MMC100_FRAME_ERROR)
    {
        // Receiving serial command time out
        nErrorCode = MPError::MPERR_SerialTimeout;       
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
    }
    else if (bErrorCode == MMC100::MMC100_BUFFER_OVER_RUN)
    {
        // Serial command buffer full
        nErrorCode = MPError::MPERR_SerialBufferFull;       
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
    }
    else if (bErrorCode == MMC100::MMC100_BAD_COMMAND)
    {
        // Invalid serial command
        nErrorCode = MPError::MPERR_SerialInpInvalid;       
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
    }
    else if (bErrorCode == MMC100::MMC100_MOVE_INTERRUPTED)
    {
        // Serial command interrupt motion
        nErrorCode = MPError::MPERR_SerialIntrupMove;       
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
    }
    else if (bErrorCode == 0x00)
    {
        // No response from serial port
        nErrorCode = MPError::MPERR_SerialZeroReturn;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
    }

    if (MMC100::Instance()->GetDebugLogFlag() > 1)
    {
		this->LogMessage(osMessage.str().c_str());
	}

    return (nErrorCode!=0);
}


