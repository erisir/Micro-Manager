//////////////////////////////////////////////////////////////////////////////
// FILE:          MMC100ZStage.cpp
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   MMC100s Controller Driver
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
#include "../../MMCore/MMCore.h"
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceUtils.h"
#include "MMC100Error.h"
#include "MMC100ZStage.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// Z - Stage
///////////////////////////////////////////////////////////////////////////////
//
// Z Stage - single axis stage device.
// Note that this adapter uses two coordinate systems.  There is the adapters own coordinate
// system with the X and Y axis going the 'Micro-Manager standard' direction
// Then, there is the MMC100s native system.  All functions using 'steps' use the MMC100 system
// All functions using Um use the Micro-Manager coordinate system
//

//
// Single axis stage constructor
//
ZStage::ZStage() :
    								m_yInitialized(false)
//m_nAnswerTimeoutMs(1000)
//, stepSizeUm_(1)
{
	InitializeDefaultErrorMessages();

	// Name
	char sZName[120];
	sprintf(sZName, "%s%s", MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZDevNameLabel).c_str(), MM::g_Keyword_Name);
	int ret = CreateProperty(sZName, MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str(), MM::String, true);

	m_nAnswerTimeoutMs = MMC100::Instance()->GetTimeoutInterval();
	m_nAnswerTimeoutTrys = MMC100::Instance()->GetTimeoutTrys();

	std::ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 0)
	{
		osMessage.str("");
		osMessage << "<ZStage::class-constructor> CreateProperty(" << sZName << "=" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str() << "), ReturnCode=" << ret << endl;
		this->LogMessage(osMessage.str().c_str());
	}

	// Description
	char sZDesc[120];
	sprintf(sZDesc, "%s%s", MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZDevDescLabel).c_str(), MM::g_Keyword_Description);
	ret = CreateProperty(sZDesc, "MP-285 Z Stage Driver", MM::String, true);

	// osMessage.clear();
	if (MMC100::Instance()->GetDebugLogFlag() > 0)
	{
		osMessage.str("");
		osMessage << "<ZStage::class-constructor> CreateProperty(" << sZDesc << " = MP-285 Z Stage Driver), ReturnCode=" << ret << endl;
		this->LogMessage(osMessage.str().c_str());
	}
}


//
// Z stage destructor
//
ZStage::~ZStage()
{
	Shutdown();
}

///////////////////////////////////////////////////////////////////////////////
// Stage methods required by the API
///////////////////////////////////////////////////////////////////////////////

//
// Z stage initialization
//
int ZStage::Initialize()
{
	std::ostringstream osMessage;

	if (!MMC100::Instance()->GetDeviceAvailability()) return DEVICE_NOT_CONNECTED;
	//if (MMC100::Instance()->GetNumberOfAxes() < 3) return DEVICE_NOT_CONNECTED;

	//int ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_GetPositionZ).c_str(), "undefined", MM::String, true);  // get position Z
	CPropertyAction* pActOnGetPosZ = new CPropertyAction(this, &ZStage::OnGetPositionZ);
	char sPosZ[20];
	double dPosZ = MMC100::Instance()->GetPositionZ();
	sprintf(sPosZ, "%ld", (long)(dPosZ * (double)MMC100::Instance()->GetUm2UStep()));
	int ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_GetPositionZ).c_str(), sPosZ, MM::Integer, false, pActOnGetPosZ);  // get position Z 

	if (MMC100::Instance()->GetDebugLogFlag() > 0)
	{
		osMessage.str("");
		osMessage << "<ZStage::Initialize> CreateProperty(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_GetPositionZ).c_str() << " = " << sPosZ << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK)  return ret;

	ret = GetPositionUm(dPosZ);
	sprintf(sPosZ, "%ld", (long)(dPosZ * (double)MMC100::Instance()->GetPositionZ()));

	if (MMC100::Instance()->GetDebugLogFlag() > 0)
	{
		osMessage.str("");
		osMessage << "<ZStage::Initialize> GetPosSteps(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_GetPositionZ).c_str() << " = " << sPosZ << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK)  return ret;

	CPropertyAction* pActOnSetPosZ = new CPropertyAction(this, &ZStage::OnSetPositionZ);
	sprintf(sPosZ, "%.2f", dPosZ);
	ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionZ).c_str(), sPosZ, MM::Float, false, pActOnSetPosZ);  // Absolute  vs Relative
	// ret = CreateProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionZ).c_str(), "Undefined", MM::Integer, true);  // Absolute  vs Relative

	if (MMC100::Instance()->GetDebugLogFlag() > 0)
	{
		osMessage.str("");
		osMessage << "<ZStage::Initialize> CreateProperty(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionZ).c_str() << " = " << sPosZ << "), ReturnCode = " << ret;
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK)  return ret;

	ret = UpdateStatus();
	if (ret != DEVICE_OK) return ret;

	m_yInitialized = true;
	return DEVICE_OK;
}

//
//  Shutdown Z stage
//
int ZStage::Shutdown()
{
	m_yInitialized = false;
	MMC100::Instance()->SetDeviceAvailable(false);
	return DEVICE_OK;
}

//
// Get the device name of the Z stage
//
void ZStage::GetName(char* Name) const
{
	CDeviceUtils::CopyLimitedString(Name, MMC100::Instance()->GetSKStr(MMC100::SKSTR_ZStageDevName).c_str());
}

//
// Set Motion Mode (1: relatice, 0: absolute)
//
int ZStage::SetMotionMode(long lMotionMode)
{
	return DEVICE_OK;
	std::ostringstream osMessage;
	unsigned char sCommand[6] = { 0x00, MMC100::MMC100_TxTerm, 0x0A, 0x00, 0x00, 0x00 };
	unsigned char sResponse[64];
	int ret = DEVICE_OK;

	if (lMotionMode == 0)
		sCommand[0] = 'a';
	else
		sCommand[0] = 'b';

	ret = WriteCommand(sCommand, 3);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::SetMotionMode> = [" << lMotionMode << "," << sCommand[0] << "], Returncode =" << ret;
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK) return ret;

	ret = ReadMessage(sResponse, 2);

	if (ret != DEVICE_OK) return ret;

	MMC100::Instance()->SetMotionMode(lMotionMode);

	return DEVICE_OK;
}

//
// Get Z stage position in um
//
int ZStage::GetPositionUm(double& dZPosUm)
{
	std::ostringstream osMessage;
	long lZPosSteps = 0;


	    osMessage <<"\n\r1POS?\n\r";
	 int  	ret = WriteCommand((unsigned char *)osMessage.str().c_str(),osMessage.str().length());


	if (ret != DEVICE_OK) return ret;

	//unsigned int nBufLen = 256;
	//unsigned char sAnswer[256];
	unsigned char sResponse[256];
	memset(sResponse, 0, 256);
	ret = ReadMessage(sResponse, 24);
	//"         0,X,K,R"
	//(     12000,X,K,R)
	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::GetPositionUm> (ReturnCode = " << ret << ")return msg=("<<sResponse<<")";
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK) return ret;
	/*
	char *delim = ",";
	char *p =strtok((char*)&sResponse, delim);
	p = strtok(NULL, delim);

	 */
	std::string s((char*)&sResponse);
	int sT = s.find(',',0);//10
	if(sT == -1)return MPError::MPERR_SerialZeroReturn;
	int sE = s.find('\n',0);//17 len = 8
	if(sE == -1)return MPError::MPERR_SerialZeroReturn;
	char v[12];
	memset(v,0,12);
	memcpy(v,(char*)(sResponse+sT+1),sE-sT);
	dZPosUm = std::atof(v);
	dZPosUm *= 1000;
	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::GetPositionUm> (z=" << dZPosUm << ")" << "ST=" << sT << "sE=" <<sE<< "v="<<v;
		this->LogMessage(osMessage.str().c_str());
	}

	MMC100::Instance()->SetPositionZ(dZPosUm);

	//char sPosition[20];
	//sprintf(sPosition, "%.2f", dZPosUm);
	//ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_GetPositionZ).c_str(), sPosition);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::GetPositionUm> Z=[" << dZPosUm << /*"," << sPosition <<*/ "], Returncode=" << ret ;
		this->LogMessage(osMessage.str().c_str());
	}

	//f (ret != DEVICE_OK) return ret;

	//ret = UpdateStatus();
	//if (ret != DEVICE_OK) return ret;

	return DEVICE_OK;
}

//
// Move to Z stage to relative distance from current position in um
//
int ZStage::SetRelativePositionUm(double dZPosUm)
{
	int ret = DEVICE_OK;
	ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage << "<ZStage::SetRelativePositionUm> (z=" << dZPosUm << ")";
		this->LogMessage(osMessage.str().c_str());
	}
	double dZPosmm = dZPosUm/1000;
	char sPosZ[20];
	sprintf(sPosZ, "%.6f", dZPosmm);

	osMessage.str("");
	osMessage <<"\n\r1MVR"<<sPosZ<<"\n\r";

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		this->LogMessage(osMessage.str().c_str());
	}
	ret = WriteCommand((unsigned char *)osMessage.str().c_str(),osMessage.str().length());
	double delta = dZPosUm;
	if(delta<0)delta *=-1;
		CDeviceUtils::SleepMs((long)(delta/2));//wait for return; 2um/ms
	double currentZPosition =MMC100::Instance()->GetPositionZ()+ dZPosUm;
	MMC100::Instance()->SetPositionZ(currentZPosition);

	double dPosZ = 0.;

	ret = GetPositionUm(dPosZ);

	if (ret != DEVICE_OK) return ret;

	return ret;
}


//
// Move to Z stage position in um
//
int ZStage::SetPositionUm(double dZPosUm)
{
	int ret = DEVICE_OK;
	ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage << "<ZStage::SetPositionUm> (z=" << dZPosUm << ")";
		this->LogMessage(osMessage.str().c_str());
	}
	osMessage.str("");

	double dZPosmm = dZPosUm/1000;
	char sPosZ[20];
	sprintf(sPosZ, "%.6f", dZPosmm);
	osMessage <<"\n\r1MVA"<<sPosZ<<"\n\r";

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		this->LogMessage(osMessage.str().c_str());
	}
	ret = WriteCommand((unsigned char *)osMessage.str().c_str(),osMessage.str().length());


	if (ret != DEVICE_OK) return ret;
	double currentPos = MMC100::Instance()->GetPositionZ();
	double delta = currentPos - dZPosUm;
	if(delta<0)delta *=-1;
	CDeviceUtils::SleepMs((long)(delta/2));//wait for return; 2um/ms

	MMC100::Instance()->SetPositionZ(dZPosUm);

	double dPosZ = 0.;

	ret = GetPositionUm(dPosZ);

	if (ret != DEVICE_OK) return ret;

	return ret;
}

//
// Get Z stage position in steps
//
int ZStage::GetPositionSteps(long& lZPosSteps)
{
	// get current position
	unsigned char sCommand[6] = { 0x63, MMC100::MMC100_TxTerm, 0x0A, 0x00, 0x00, 0x00 };
	int ret = WriteCommand(sCommand, 3);

	if (ret != DEVICE_OK)  return ret;

	unsigned char sResponse[64];
	memset(sResponse, 0, 64);

	bool yCommError = false;
	int nTrys = 0;

	while (!yCommError && nTrys < MMC100::Instance()->GetTimeoutTrys())
	{
		long lXPosSteps = (long) (MMC100::Instance()->GetPositionX() * (double) MMC100::Instance()->GetUm2UStep());
		long lYPosSteps = (long) (MMC100::Instance()->GetPositionY() * (double) MMC100::Instance()->GetUm2UStep());

		ret = ReadMessage(sResponse, 14);

		ostringstream osMessage;
		char sCommStat[30];
		int nError = CheckError(sResponse[0]);
		yCommError = (sResponse[0] == 0) ? false : nError != 0;
		if (yCommError)
		{
			if (nError == MPError::MPERR_SerialZeroReturn && nTrys < MMC100::Instance()->GetTimeoutTrys()) { nTrys++; yCommError = false; }

			if (MMC100::Instance()->GetDebugLogFlag() > 1)
			{
				osMessage.str("");
				osMessage << "<XYStage::GetPositionSteps> Response = (" << nError << "," << nTrys << ")" ;
			}

			sprintf(sCommStat, "Error Code ==> <%2x>", sResponse[0]);
		}
		else
		{
			lXPosSteps = *((long*)(&sResponse[0]));
			lYPosSteps = *((long*)(&sResponse[4]));
			lZPosSteps = *((long*)(&sResponse[8]));
			//MMC100::Instance()->SetPositionX(lXPosSteps);
			//MMC100::Instance()->SetPositionY(lYPosSteps);
			//MMC100::Instance()->SetPositionZ(lZPosSteps);
			strcpy(sCommStat, "Success");

			if (MMC100::Instance()->GetDebugLogFlag() > 1)
			{
				osMessage.str("");
				osMessage << "<ZStage::GetPositionSteps> Response(X = <" << lXPosSteps << ">, Y = <" << lYPosSteps << ">, Z = <"<< lZPosSteps << ">), ReturnCode=" << ret;
			}

			nTrys = MMC100::Instance()->GetTimeoutTrys();

		}

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			this->LogMessage(osMessage.str().c_str());
		}

		//ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), sCommStat);

	}

	if (ret != DEVICE_OK) return ret;

	return DEVICE_OK;
}



//
// Move x-y stage to a relative distance from current position in uSteps
//
int ZStage::SetRelativePositionSteps(long lZPosSteps)
{
	int ret = DEVICE_OK;
	ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		//osMessage.str("");
		osMessage << "<ZStage::SetRelativePositionSteps> (z=" << lZPosSteps << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	// set relative motion mode
	if (MMC100::Instance()->GetMotionMode() == 0)
	{
		ret = SetMotionMode(1);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage.str("");
			osMessage << "<ZStage::SetRelativePositionSteps> (" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_MotionMode).c_str() << " = <RELATIVE>), ReturnCode = " << ret;
			this->LogMessage(osMessage.str().c_str());
		}

		if (ret != DEVICE_OK) return ret;
	}

	ret = _SetPositionSteps(0L, 0L, lZPosSteps);

	if (ret != DEVICE_OK) return ret;

	return DEVICE_OK;
}

//
// move z stage to absolute position in uSsteps
//
int ZStage::SetPositionSteps(long lZPosSteps)
{
	int ret = DEVICE_OK;
	ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		//osMessage.str("");
		osMessage << "<ZStage::SetPositionSteps> (z=" << lZPosSteps << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	// set absolute motion mode
	if (MMC100::Instance()->GetMotionMode() != 0)
	{
		ret = SetMotionMode(0);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage.str("");
			osMessage << "<ZStage::SetPositionSteps> (" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_MotionMode).c_str() << " = <ABSOLUTE>), ReturnCode = " << ret;
			this->LogMessage(osMessage.str().c_str());
		}

		if (ret != DEVICE_OK) return ret;
	}

	long lPositionX = (long)MMC100::Instance()->GetPositionX() * (long)MMC100::Instance()->GetUm2UStep();
	long lPositionY = (long)MMC100::Instance()->GetPositionY() * (long)MMC100::Instance()->GetUm2UStep();

	ret = _SetPositionSteps(lPositionX, lPositionY, lZPosSteps);

	if (ret != DEVICE_OK) return ret;

	return DEVICE_OK;
}

//
// move x-y-z stage in uSsteps
//
int ZStage::_SetPositionSteps(long lXPosSteps, long lYPosSteps, long lZPosSteps)
{
	int ret = DEVICE_OK;
	ostringstream osMessage;

	// get current position X-Y
	unsigned char sCommand[16];
	memset(sCommand, 0, 16);
	sCommand[0]  = 0x6D;
	sCommand[13] = MMC100::MMC100_TxTerm;
	sCommand[14] = 0x0A;

	long* plPositionX = (long*)(&sCommand[1]);
	*plPositionX = lXPosSteps;

	long* plPositionY = (long*)(&sCommand[5]);
	*plPositionY = lYPosSteps;

	long* plPositionZ = (long*)(&sCommand[9]);
	*plPositionZ = lZPosSteps;

	ret = WriteCommand(sCommand, 15);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		//osMessage.str("");
		osMessage << "<ZStage::_SetPositionSteps> Command(<0x6D>, X = <" << *plPositionX << ">,<" << *plPositionY << ">,<" << *plPositionZ << ">), ReturnCode=" << ret;
		LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK)  return ret;

	double dVelocity = (double)MMC100::Instance()->GetVelocity() * (double)MMC100::Instance()->GetUm2UStep();
	double dSec = 0.;
	if (MMC100::GetMotionMode == 0)
	{
		long lOldZPosSteps = (long)MMC100::Instance()->GetPositionZ();
		dSec = (double)labs(lZPosSteps-lOldZPosSteps) / dVelocity;
	}
	else
	{
		dSec = (double)labs(lZPosSteps) / dVelocity;
	}
	long lSleep = (long)(dSec * 120.);

	CDeviceUtils::SleepMs(lSleep);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::_SetPositionSteps> Sleep..." << lSleep << " millisec...";
		LogMessage(osMessage.str().c_str());
	}

	bool yCommError = true;

	while (yCommError)
	{
		unsigned char sResponse[64];
		memset(sResponse, 0, 64);

		ret = ReadMessage(sResponse, 2);

		//char sCommStat[30];
		yCommError = CheckError(sResponse[0]) != MPError::MPERR_OK;
		//if (yCommError)
		//{
		//    sprintf(sCommStat, "Error Code ==> <%2x>", sResponse[0]);
		//}
		// else
		//{
		//     strcpy(sCommStat, "Success");
		//}

		//ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), sCommStat);
	}

	if (ret != DEVICE_OK) return ret;

	return DEVICE_OK;
}

//
// Set current position as origin
//
int ZStage::SetOrigin()
{
	unsigned char sCommand[6] = { 0x6F, MMC100::MMC100_TxTerm, 0x0A, 0x00, 0x00, 0x00 };
	int ret = WriteCommand(sCommand, 3);

	std::ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::SetOrigin> (ReturnCode=" << ret << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret!=DEVICE_OK) return ret;

	unsigned char sResponse[64];

	memset(sResponse, 0, 64);
	ret = ReadMessage(sResponse, 2);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::CheckStatus::SetOrigin> (ReturnCode = " << ret << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK) return ret;

	bool yCommError = CheckError(sResponse[0]) != 0;

	char sCommStat[30];
	if (yCommError)
		sprintf(sCommStat, "Error Code ==> <%2x>", sResponse[0]);
	else
		strcpy(sCommStat, "Success");

	//ret = SetProperty(MMC100::Instance()->GetSKStr(MMC100::SKSTR_CommStateLabel).c_str(), sCommStat);

	if (ret != DEVICE_OK) return ret;

	return DEVICE_OK;
}

//
// stop and interrupt Z stage motion
//
int ZStage::Stop()
{
	unsigned char sCommand[6] = { 0x03, MMC100::MMC100_TxTerm, 0x00, 0x00, 0x00, 0x00 };

	int ret = WriteCommand(sCommand, 2);

	ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::Stop> (ReturnCode = " << ret << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////

//
// Unsupported command from MMC100
//
int ZStage::OnStepSize (MM::PropertyBase* /*pProp*/, MM::ActionType /*eAct*/) 
{
	return DEVICE_OK;
}

int ZStage::OnSpeed(MM::PropertyBase* /*pProp*/, MM::ActionType /*eAct*/)
{
	return DEVICE_OK;
}

int ZStage::OnGetPositionZ(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	std::ostringstream osMessage;
	int ret = DEVICE_OK;
	double dPos = MMC100::Instance()->GetPositionZ();

	osMessage.str("");

	//if (eAct == MM::BeforeGet)
	//{
	//    pProp->Set(dPos);
	//
	//	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	//	{
	//		osMessage << "<MMC100Ctrl::OnGetPositionZ> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionX).c_str() << " = [" << dPos << "], ReturnCode = " << ret;
	//		//this->LogMessage(osMessage.str().c_str());
	//	}
	//}
	//if (eAct == MM::AfterSet)
	//{
	// pProp->Get(dPos);  // not used

	ret = GetPositionUm(dPos);
	dPos *= (double)MMC100::Instance()->GetUm2UStep();
	char sPos[20];
	sprintf(sPos, "%ld", (long)dPos);

	pProp->Set(dPos);

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage << "<MMC100Ctrl::OnGetPositionZ> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionX).c_str() << " = [" << dPos << "," << sPos << "], ReturnCode = " << ret;
		//this->LogMessage(osMessage.str().c_str());
	}

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK) return ret;
	//}

	return DEVICE_OK;
}

int ZStage::OnSetPositionZ(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	std::ostringstream osMessage;
	int ret = DEVICE_OK;
	double dPos = MMC100::Instance()->GetPositionZ();;

	osMessage.str("");

	if (eAct == MM::BeforeGet)
	{
		pProp->Set(dPos);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnSetPositionZ> BeforeGet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionZ).c_str() << " = [" << dPos << "], ReturnCode = " << ret;
			//this->LogMessage(osMessage.str().c_str());
		}
	}
	else if (eAct == MM::AfterSet)
	{
		pProp->Get(dPos);

		if (MMC100::Instance()->GetMotionMode() == 0)
			ret = SetPositionUm(dPos);
		else
			ret = SetRelativePositionUm(dPos);

		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<MMC100Ctrl::OnSetPositionZ> AfterSet(" << MMC100::Instance()->GetSKStr(MMC100::SKSTR_SetPositionZ).c_str() << " = [" << dPos << "], ReturnCode = " << ret;
			//this->LogMessage(osMessage.str().c_str());
		}

	}

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage << ")";
		this->LogMessage(osMessage.str().c_str());
	}

	if (ret != DEVICE_OK) return ret;


	return DEVICE_OK;
}


///////////////////////////////////////////////////////////////////////////////
// Helper, internal methods
///////////////////////////////////////////////////////////////////////////////

//
// Write a coomand to serial port
//
int ZStage::WriteCommand(unsigned char* sCommand, int nLength)
{
	int ret = DEVICE_OK;
	ostringstream osMessage;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<ZStage::WriteCommand> (Command=" <<sCommand << ")";
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

int ZStage::ReadMessage(unsigned char* sResponse, int nBytesRead)
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

		if (MMC100::Instance()->GetDebugLogFlag() > 1 && lByteRead>0)
		{
			osMessage.str("");
			osMessage << "<MMC100Ctrl::ReadMessage> (ReadFromSerial = [ExpectLen:" << nBytesRead << "],[totalLen" << lRead << "],[thisTimeLen" << lByteRead << "])";
			this->LogMessage(osMessage.str().c_str());
		}

		// concade new string
		lRead += lByteRead;

		if (lRead > 2)
		{
			for(int i=0;i<lRead;i++){
				if(sAnswer[i] == '\n')
				{
					yRead = true;
					break;
				}
			}
		}

		yRead = yRead || (lRead >= (unsigned long)nBytesRead);

		if (yRead) break;

		// check for timeout
		yTimeout = ((double)(GetClockTicksUs() - lStartTime) / 1000) > (double) m_nAnswerTimeoutMs;
		if (!yTimeout) CDeviceUtils::SleepMs(3);
	}

	// block/wait for acknowledge, or until we time out
	// if (!yRead || yTimeout) return DEVICE_SERIAL_TIMEOUT;
	// MMC100::Instance()->ByteCopy(sResponse, sAnswer, nBytesRead);
	// if (checkError(sAnswer[0]) != 0) ret = DEVICE_SERIAL_COMMAND_FAILED;

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		osMessage.str("");
		osMessage << "<MMC100Ctrl::ReadMessage> (ReadFromSerial = <" <<sAnswer;
		this->LogMessage(osMessage.str().c_str());
	}

	for (unsigned long lIndx=0; lIndx < (unsigned long)nBytesRead; lIndx++)
	{
		sResponse[lIndx] = sAnswer[lIndx];
	}
	return DEVICE_OK;
}

//
// check the error code for the message returned from serial communivation
//
int ZStage::CheckError(unsigned char bErrorCode)
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
			osMessage << "<ZStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}
	else if (bErrorCode == MMC100::MMC100_FRAME_ERROR)
	{
		// Receiving serial command time out
		nErrorCode = MPError::MPERR_SerialTimeout;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<ZStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}
	else if (bErrorCode == MMC100::MMC100_BUFFER_OVER_RUN)
	{
		// Serial command buffer full
		nErrorCode = MPError::MPERR_SerialBufferFull;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<ZStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}
	else if (bErrorCode == MMC100::MMC100_BAD_COMMAND)
	{
		// Invalid serial command
		nErrorCode = MPError::MPERR_SerialInpInvalid;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<ZStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}
	else if (bErrorCode == MMC100::MMC100_MOVE_INTERRUPTED)
	{
		// Serial command interrupt motion
		nErrorCode = MPError::MPERR_SerialIntrupMove;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<ZStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}
	else if (bErrorCode == 0x0D)
	{
		// read carriage return
		nErrorCode = MPError::MPERR_OK;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<XYStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}
	else if (bErrorCode == 0x00)
	{
		// No response from serial port
		nErrorCode = MPError::MPERR_SerialZeroReturn;
		if (MMC100::Instance()->GetDebugLogFlag() > 1)
		{
			osMessage << "<ZStage::checkError> ErrorCode=[" << MPError::Instance()->GetErrorText(nErrorCode).c_str() << "])";
		}
	}

	if (MMC100::Instance()->GetDebugLogFlag() > 1)
	{
		this->LogMessage(osMessage.str().c_str());
	}

	return (nErrorCode);
}

