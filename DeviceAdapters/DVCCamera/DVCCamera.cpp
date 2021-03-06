﻿// DVCCamera.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DVCCamera.h"
#include <iostream>

using namespace std;

/////////////////////////// START HERE /////////////////////////////////

// Constant strings
map<int, string> DVCCamera::m_strMap;

void DVCCamera::initConstStrings() {
	m_strMap[STR_NAME] = "DVCCamera";
	m_strMap[STR_PROP_CAMID] = string(MM::g_Keyword_CameraID);
	m_strMap[STR_CAM_BUSY] = "Camera is busy.";
	m_strMap[STR_INVALID_ROI] = "Invalid ROI.";
	m_strMap[STR_DEVICE_DESC] = "DVC GigE Camera";
	m_strMap[STR_PROP_CAM_DIMENSION] = "Dimension";
	m_strMap[STR_PROP_CAM_WIDTH] = "Camera width";
	m_strMap[STR_PROP_CAM_HEIGHT] = "Camera height";
	m_strMap[STR_PROP_NAME] = string(MM::g_Keyword_CameraName);
	m_strMap[STR_PROP_DESC] = string(MM::g_Keyword_Description);
	m_strMap[STR_PROP_SSN] = "Serial number";
	m_strMap[STR_PROP_DEPTH] = "Depth";
	m_strMap[STR_PROP_GAINDB] = string(MM::g_Keyword_Gain);
	m_strMap[STR_PROP_GAINRANGE] = "Gain range(dB)";
	m_strMap[STR_PROP_BINSIZE] = string(MM::g_Keyword_Binning);
	m_strMap[STR_PROP_EXPOSURE] = string(MM::g_Keyword_Exposure);
	m_strMap[STR_PROP_ACTUAL_FRAME_TIME] = string(
			MM::g_Keyword_ActualInterval_ms);
	m_strMap[STR_PROP_PIXELCLOCK] = "Pixel Clock Frequency(Hz)";
	m_strMap[STR_PROP_SCANRATE] = "Scan rate";
}

const char* g_DVCCameraDeviceName = "DVCCamera";

string DVCCamera::getConstString(int strCode) {
	if (m_strMap.size() == 0)
		initConstStrings();

	string sText;
	map<int, string>::const_iterator it = m_strMap.find(strCode);
	if (it != m_strMap.end())
		sText = it->second;
	return sText;
}

char DVCCamera::m_errorMsg[MM::MaxStrLength];
// All the DVC error code will be returned plus this.
const int g_Err_Offset = 10000;

//// External names used used by the rest of the system
//// to load particular device from the "DemoCamera.dll" library
//const char* g_DVCCameraDeviceName = "DVCCamera";

// singleton instance
DVCCamera* DVCCamera::instance_ = NULL;
//unsigned int DVCCamera::refCount_ = 0;

// Number of user buffers
const int g_UserBufferNumber = 48;

// global driver thread lock
MMThreadLock g_DVCCamDriverLock;

map<int, string> DVCCamera::camTypeMap_;
map<int, string> testMap;

// Properties
const char g_CameraName[] = "Camera Name";
const char g_CameraType[] = "Camera Type";
const char g_Width[] = "Camera Width";
const char g_Height[] = "Camera Height";
const char g_Depth[] = "Bit Depth";
const char g_GaindB[] = "Gain (dB)";
const char g_GaindBRange[] = "Gain Range (dB)";
const char g_Binning[] = "Binning";
const char g_hBinning[] = "hBin";
const char g_absTimestamp[] = "Absolute Timestamp";
const char g_vBinning[] = "vBin";
const char g_ActualFrameTime[] = "Actual Frame Time (ms)";
const char g_PixelClock[] = "Pixel Clock Frequency (Hz)";
const char g_ScanRate[] = "Scan Rate";
const char g_Label[] = "Label";
const char g_MD_ExposeTime[] = "Exposure Time";
const char g_MD_ExposeTimestamp[] = "Exposure Timestamp";
const char g_MD_FrameTimestamp[] = "Frame Timestamp";
const char g_MD_TransferTime[] = "Transfer Time";
const char g_MD_TriggerTimestamp[] = "Trigger Timestamp";

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

/**
 * List all suppoerted hardware devices here
 * Do not discover devices at runtime.  To avoid warnings about missing DLLs, Micro-Manager
 * maintains a list of supported device (MMDeviceList.txt).  This list is generated using 
 * information supplied by this function, so runtime discovery will create problems.
 */MODULE_API void InitializeModuleData() {
	AddAvailableDeviceName(g_DVCCameraDeviceName,
			DVCCamera::getConstString(DVCCamera::STR_DEVICE_DESC).c_str());
}

MODULE_API MM::Device* CreateDevice(const char* deviceName) {
	if (deviceName == 0)
		return 0;

	if (strcmp(deviceName, g_DVCCameraDeviceName) == 0)
		return DVCCamera::GetInstance();

	// ...supplied name not recognized
	return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice) {
	delete pDevice;
}

DriverGuard::DriverGuard(const DVCCamera*) {
	g_DVCCamDriverLock.Lock();
	//if (cam != 0 && cam->GetNumberOfWorkableCameras() > 1)
	//{
	// // must be defined as 32bit in order to compile on 64bit systems since GetCurrentCamera
	//    // only takes 32bit -kdb
	//    at_32 currentCamera;
	//    GetCurrentCamera(&currentCamera);
	//    if (currentCamera != cam->GetMyCameraID())
	//    {
	//    int ret = SetCurrentCamera(cam->GetMyCameraID());
	//    if (ret != DRV_SUCCESS)
	//        printf("Error switching active camera");
	//    }
	//}
}

DriverGuard::~DriverGuard() {
	g_DVCCamDriverLock.Unlock();
}

void DVCCamera::GetName(char* name) const {
	CDeviceUtils::CopyLimitedString(name, g_DVCCameraDeviceName);
}

///////////////////////////////////////////////////////////////////////////////
// Continuous acquisition
//

DVCCamera::DVCCamera() :
		initialized_(false), busy_(false), m_camId(1), camType_(0), fullFrameX_(
				0), fullFrameY_(0), depth_(0), bufNumber_(48), expMs_(0.0), sequenceRunning_(
				false), fullFrameBuffer_(NULL), binSize_(1), sequenceLength_(0), intervalMs_(
				0), imageCounter_(0), startTime_(0), thd_(NULL), sequencePaused_(
				false), stopOnOverflow_(true) {
	InitializeDefaultErrorMessages();

	QueryPerformanceFrequency(&perfFreq_);
	//seqThread_ = new AcqSequenceThread(this);

	// Initialize the camera type map
	camTypeMap_.insert(pair<int, string>(0, "Unknown camera"));
	camTypeMap_.insert(
			pair<int, string>(1, "Sony 085 12 bit monochrome: 1312M"));
	camTypeMap_.insert(pair<int, string>(2, "Sony 085 12 bit color: 1312C"));
	camTypeMap_.insert(
			pair<int, string>(3, "Sony 085 10 bit monochrome: 1310M"));
	camTypeMap_.insert(pair<int, string>(4, "Sony 085 10 bit color: 1310C"));
	camTypeMap_.insert(
			pair<int, string>(5, "Sony 285 12 bit monochrome: 1412M"));
	camTypeMap_.insert(pair<int, string>(6, "Sony 285 12 bit color: 1412C"));
	camTypeMap_.insert(
			pair<int, string>(7, "Sony 428 10 bit monochrome: 700M"));
	camTypeMap_.insert(pair<int, string>(8, "Sony 428 10 bit color: 700C"));
	camTypeMap_.insert(
			pair<int, string>(9, "Kodak 2020 12 bit monochrome: 2000M"));
	camTypeMap_.insert(pair<int, string>(10, "Kodak 2020 12 bit color: 2000C"));
	camTypeMap_.insert(
			pair<int, string>(11, "Kodak 4021 12 bit monochrome: 4000M"));
	camTypeMap_.insert(pair<int, string>(12, "Kodak 4021 12 bit color: 4000C"));
	camTypeMap_.insert(
			pair<int, string>(13, "Kodak 340 12 bit monochrome: 340M"));
	camTypeMap_.insert(
			pair<int, string>(14, "Kodak 340 12 bit monochrome: 340C"));

	int ret;
	char propName[MM::MaxStrLength];

	// Camera Id
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_CAMID).c_str());
	char strBoardId[32];
	_snprintf_s(strBoardId, sizeof(strBoardId), _TRUNCATE, "%d", m_camId);
	CPropertyAction* pActOnCamId = new CPropertyAction(this,
			&DVCCamera::OnCamId);
	ret = CreateProperty(propName, strBoardId, MM::Integer, false, pActOnCamId,
			true);

	instance_ = this;
}

DVCCamera::~DVCCamera() {
	Shutdown();
}

int DVCCamera::OnCamId(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);
	if (eAct == MM::BeforeGet) {
		pProp->Set(m_camId);
	} else if (eAct == MM::AfterSet) {
		pProp->Get(m_camId);
	}
	return DEVICE_OK;
}

DVCCamera* DVCCamera::GetInstance() {
	if (instance_ == NULL)
		instance_ = new DVCCamera();

//	refCount_++;
	return instance_;
}

int DVCCamera::processErr() {
	int err = dvcGetLastErr();
	const char* msg = dvcGetLastErrMsg();
	if (msg != NULL)
		_snprintf_s(m_errorMsg, MM::MaxStrLength, _TRUNCATE,
				"DVCCamera error: code: %d, reason: %s", err, msg);
	return DVCCAM_ERROR_CODE;
}

int DVCCamera::getErrorMsg() {
	return getErrorMsg(NULL);
}

int DVCCamera::getErrorMsg(const char* msg) {
	int err = dvcGetLastErr();
	const char* errStr = dvcGetLastErrMsg();
	if (errStr != NULL) {
		if (msg != NULL)
			_snprintf_s(m_errorMsg, MM::MaxStrLength, _TRUNCATE,
					"DVCCamera error: code: %d, reason: %s. Custom message: %s",
					err, errStr, msg);
		else
			_snprintf_s(m_errorMsg, MM::MaxStrLength, _TRUNCATE,
					"DVCCamera error: code: %d, reason: %s.", err, errStr);
	} else if (msg != NULL)
		_snprintf_s(m_errorMsg, MM::MaxStrLength, _TRUNCATE,
				"DVCCamera error. Custom message: %s", msg);
	else
		strcpy_s(m_errorMsg, MM::MaxStrLength, "DVCCamera error: Unknown.");

	return DVCCAM_ERROR_CODE;
}

int DVCCamera::Initialize() {
	if (initialized_)
		return DEVICE_OK;

	int ret;

	CameraListArrayStruct cams;
	if (!dvcGetListOfCameras(&cams))
		LogMessage("No DVC camera found!");
	for (int i = 0; i < (int) cams.nCameras; i++) {
		camMap_.insert(pair<int, HANDLE>(i + 1, (HANDLE) NULL));
	}

	const HANDLE hCam = dvcOpenCamera(m_camId);
	if (hCam == NULL)
		return processErr();
	camMap_[m_camId] = hCam;

	thd_ = new DVCCamera::AcqSequenceThread(this, hCam);
	dvcAllocateUserBuffers(hCam, &userBuffers_, g_UserBufferNumber);

	if (!dvcResetCamera(hCam))
		return processErr();

	char propName[MM::MaxStrLength];
	char str[MM::MaxStrLength];

	// Name
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_NAME).c_str());
	ret = CreateProperty(propName, DVCCamera::getConstString(STR_NAME).c_str(),
			MM::String, true);

	// Description
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_DESC).c_str());
	dvcGetCameraName(hCam, str, MM::MaxStrLength);
	ret = CreateProperty(propName, str, MM::String, true);

	// Camera serial number
	int camSSN;
	if (!dvcGetCameraSerialNumber(hCam, &camSSN)) {
		return processErr();
	}
	_itoa_s(camSSN, str, MM::MaxStrLength, 10);
	CreateProperty(DVCCamera::getConstString(STR_PROP_SSN).c_str(), str,
			MM::String, true);

	// Gain
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_GAINDB).c_str());
	CPropertyAction* pAct = new CPropertyAction(this, &DVCCamera::OnGain);
	CreateProperty(propName, "", MM::Float, false, pAct);

	// Gain range
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_GAINRANGE).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnGainRange);
	CreateProperty(propName, "", MM::String, true, pAct);

	// Binning size
	int hb, vb;
	if (!dvcGetHVBin(hCam, &hb, &vb))
		return processErr();
	binSize_ = hb;
	if (!dvcSetHVBin(hCam, binSize_, binSize_))
		return processErr();
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_BINSIZE).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnBinning);
	CreateProperty(propName, "", MM::Integer, false, pAct);

	// Exposure
	expMs_ = dvcGetExposeMsec(hCam);
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_EXPOSURE).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnExposure);
	CreateProperty(propName, "", MM::Float, false, pAct);

	// Camera dimension
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_CAM_DIMENSION).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnCameraDimension);
	CreateProperty(propName, "", MM::String, true, pAct);

	// Camera width
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_CAM_WIDTH).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnCameraWidth);
	CreateProperty(propName, "", MM::Integer, true, pAct);

	// Camera height
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_CAM_HEIGHT).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnCameraHeight);
	CreateProperty(propName, "", MM::Integer, true, pAct);

	fullFrameX_ = dvcGetCCDWidth(hCam);
	fullFrameY_ = dvcGetCCDHeight(hCam);
	roi_.x = 0;
	roi_.y = 0;
	roi_.xSize = fullFrameX_;
	roi_.ySize = fullFrameY_;

	// Depth
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_DEPTH).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnDepth);
	CreateProperty(propName, "", MM::Integer, true, pAct);
	depth_ = dvcGetNBits(hCam);

	fullFrameBuffer_ = new unsigned short[fullFrameX_ * fullFrameY_];
	ResizeImageBuffer();

	// Frame time
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_ACTUAL_FRAME_TIME).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnActualFrameTime);
	CreateProperty(propName, "", MM::Float, true, pAct);

	// Pixel clock
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_PIXELCLOCK).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnPixelClock);
	CreateProperty(propName, "", MM::Float, true, pAct);

	// Scan rate
	_snprintf_s(propName, MM::MaxStrLength, _TRUNCATE,
			getConstString(STR_PROP_SCANRATE).c_str());
	pAct = new CPropertyAction(this, &DVCCamera::OnScanRate);
	CreateProperty(propName, "", MM::String, false, pAct);
	scanRates_.push_back("20 MHz");
	scanRates_.push_back("40 MHz");
	SetAllowedValues(propName, scanRates_);

	// Set Mirror X to 1
	SetProperty(MM::g_Keyword_Transpose_MirrorX, "1");

	if (!dvcAllocateUserBuffers(hCam, &dvcBuf, bufNumber_))
		return processErr();

	if (!dvcSetUserBuffers(hCam, &dvcBuf))
		return processErr();

	return DEVICE_OK;
}

int DVCCamera::OnCameraDimension(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	if (eAct == MM::BeforeGet) {
		HANDLE hCam = camMap_[m_camId];
		int width = dvcGetCCDWidth(hCam);
		int height = dvcGetCCDHeight(hCam);
		char str[MM::MaxStrLength];
		_snprintf_s(str, sizeof(str), _TRUNCATE,
				"Width: %d pixels, height: %d pixels", width, height);
		pProp->Set(str);
	}
	return DEVICE_OK;
}

int DVCCamera::OnCameraWidth(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	if (eAct == MM::BeforeGet) {
		HANDLE hCam = camMap_[m_camId];
		long width = dvcGetCCDWidth(hCam);
		pProp->Set(width);
	}
	return DEVICE_OK;
}

int DVCCamera::OnCameraHeight(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	if (eAct == MM::BeforeGet) {
		HANDLE hCam = camMap_[m_camId];
		long width = dvcGetCCDHeight(hCam);
		pProp->Set(width);
	}
	return DEVICE_OK;
}

int DVCCamera::OnDepth(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	if (eAct == MM::BeforeGet) {
		HANDLE hCam = camMap_[m_camId];
		long dep = dvcGetNBits(hCam);
		pProp->Set(dep);
	}
	return DEVICE_OK;
}

int DVCCamera::OnGain(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		double gain;
		if (!dvcGetGaindB(hCam, &gain))
			return processErr();
		pProp->Set(gain);
	} else if (eAct == MM::AfterSet) {
		double gain;
		pProp->Get(gain);
		if (!dvcSetGaindB(hCam, gain))
			return processErr();
	}
	return DEVICE_OK;
}

int DVCCamera::OnGainRange(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		double min, max, granularity;
		if (!dvcGetGaindBRange(hCam, &min, &max, &granularity))
			return processErr();

		char str[MM::MaxStrLength];
		_snprintf_s(str, MM::MaxStrLength, _TRUNCATE,
				"Range: %.1f~%.1fdB, granularity: %.1f", min, max, granularity);
		pProp->Set(str);

		SetPropertyLimits(getConstString(STR_PROP_GAINDB).c_str(), min, max);
	}
	return DEVICE_OK;
}

int DVCCamera::OnActualFrameTime(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		double lineTime, frameTime;
		if (!dvcGetLineFrameTime(hCam, &lineTime, &frameTime))
			return getErrorMsg("OnActualFrameTime");
		pProp->Set(frameTime);
	}
	return DEVICE_OK;
}

int DVCCamera::OnPixelClock(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		double freq = dvcGetPixelClock(hCam);
		pProp->Set(freq);
	}
	return DEVICE_OK;
}

int DVCCamera::OnScanRate(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		int scan;
		if (!dvcGetScanRate(hCam, &scan))
			return getErrorMsg("OnScanRate: BeforeGet");
		pProp->Set(scanRates_[scan].c_str());
	} else if (eAct == MM::AfterSet) {
		string scan;
		pProp->Get(scan);

		for (int i = 0; i < scanRates_.size(); i++) {
			if (scan.compare(scanRates_[i]) == 0) {
				if (!dvcSetScanRate(hCam, i))
					return getErrorMsg("OnScanRate: AfterSet");
				return DEVICE_OK;
			}
		}
	}
	return DEVICE_OK;
}

int DVCCamera::getReadoutTime(double& time) {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	double line, frame;
	if (!dvcGetLineFrameTime(hCam, &line, &frame)) {
		return processErr();
	}
	time = expMs_ + frame;
	return DEVICE_OK;
}

//added to use RTA
/**
 * Acquires a single frame.
 * Micro-Manager expects that this function blocks the calling thread until the exposure phase is over.
 * This wait is implemented by sleeping ActualInterval_ms_ - ReadoutTime_ + 0.99 ms.
 * Note that this is likely not long enough when using internal triggering.
 */
int DVCCamera::SnapImage() {
	DriverGuard dg(this);

	if (sequenceRunning_)   // If we are in the middle of a SequenceAcquisition
		return getErrorMsg(getConstString(STR_CAM_BUSY).c_str());

	HANDLE hCam = camMap_[m_camId];
	if (!dvcTakePicture(hCam)) {
		return processErr();
	}
	return DEVICE_OK;
}

/**
 * Deactivate the camera, reverse the initialization process.
 */
int DVCCamera::Shutdown() {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	if (initialized_) {
		SetToIdle();
		dvcSetUserBuffers(hCam, NULL);
		dvcReleaseUserBuffers(&dvcBuf);
		dvcCloseCamera(hCam);
		delete fullFrameBuffer_;
		initialized_ = false;
	}
	camMap_.erase(m_camId);
// clear the instance pointer
	instance_ = NULL;
	return DEVICE_OK;
}

bool DVCCamera::IsAcquiring() {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];
	return (dvcGetStatus(hCam) != DVC_STATUS_STOPPED);
}

int DVCCamera::SetBinning(int bin) {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];

	if (sequenceRunning_)
		return getErrorMsg(getConstString(STR_CAM_BUSY).c_str());

	//added to use RTA
	SetToIdle();
	if (!dvcSetHVBin(hCam, bin, bin))
		return getErrorMsg("SetBinning");
	binSize_ = bin;
	ResizeImageBuffer();
	return DEVICE_OK;
}

int DVCCamera::SetToIdle() {
	DriverGuard dg(this);

	if (!initialized_ || !IsAcquiring())
		return DEVICE_OK;

	HANDLE hCam = camMap_[m_camId];
	if (!dvcStopSequence(hCam))
		return processErr();

	int status = DVC_STATUS_RUNNING;
	while (status != DVC_STATUS_STOPPED) {
		status = dvcGetStatus(hCam);
	}

	return DEVICE_OK;
}

const unsigned char* DVCCamera::GetImageBuffer() {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	int nBytes = dvcGetImageBytes(hCam);

	int w = dvcGetXDim(hCam);
	int h = dvcGetYDim(hCam);

	BOOL ret = dvcReadImage(hCam, fullFrameBuffer_, 0, 0, w, h);
	if (!ret) {
		logDeviceError();
		return NULL;
	}
	return (unsigned char*) fullFrameBuffer_;
}

int DVCCamera::logDeviceError() {
	DriverGuard dg(this);
	int err = dvcGetLastErr();
	char* msg = dvcGetLastErrMsg();
	char str[1024];
	snprintf(str, 1024, "Error code %d: %s", err, msg);
	LogMessage(str, false);
	return err;
}

int DVCCamera::OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		int hb, vb;
		if (!dvcGetHVBin(hCam, &hb, &vb))
			return getErrorMsg("OnBinning: BeforeGet");
		binSize_ = hb;
		pProp->Set((long) binSize_);
	} else if (eAct == MM::AfterSet) {
		long bin;
		pProp->Get(bin);
		SetBinning((int) bin);
	}
	return DEVICE_OK;
}

int DVCCamera::OnExposure(MM::PropertyBase* pProp, MM::ActionType eAct) {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];
	if (eAct == MM::BeforeGet) {
		expMs_ = dvcGetExposeMsec(hCam);
		pProp->Set(expMs_);
	} else if (eAct == MM::AfterSet) {
		double exp;
		pProp->Get(exp);

		double interval;
		if (!dvcGetExposeInterval(hCam, -1, -1, -1, &interval))
			return processErr();

		if (fabs(exp - expMs_) < interval)
			return DEVICE_OK;
		SetExposure(exp);
	}
	return DEVICE_OK;
}

void DVCCamera::SetExposure(double exp) {
	bool acquiring = sequenceRunning_;
	if (acquiring)
		StopSequenceAcquisition(true);

//if (sequenceRunning_)
//	return ERR_BUSY_ACQUIRING;

	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];
	expMs_ = dvcSetExposeMsec(hCam, exp);

	if (acquiring)
		StartSequenceAcquisition(sequenceLength_ - imageCounter_, intervalMs_,
				stopOnOverflow_);
}

double DVCCamera::GetExposure() const {
	DriverGuard dg(this);
	map<int, HANDLE>::const_iterator it = camMap_.find(m_camId);
	return dvcGetExposeMsec((it->second));
}

int DVCCamera::GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize,
		unsigned& uYSize) {
	uX = roi_.x / binSize_;
	uY = roi_.y / binSize_;
	uXSize = roi_.xSize / binSize_;
	uYSize = roi_.ySize / binSize_;

	return DEVICE_OK;
}

int DVCCamera::SetROI(unsigned uX, unsigned uY, unsigned uXSize,
		unsigned uYSize) {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];

	if (Busy())
		return getErrorMsg(getConstString(STR_CAM_BUSY).c_str());

//added to use RTA
	SetToIdle();

	ROI oldRoi = roi_;

	roi_.x = uX * binSize_;
	roi_.y = uY * binSize_;
	roi_.xSize = uXSize * binSize_;
	roi_.ySize = uYSize * binSize_;

	if (roi_.x + roi_.xSize > fullFrameX_
			|| roi_.y + roi_.ySize > fullFrameY_) {
		roi_ = oldRoi;
		_snprintf_s(m_errorMsg, MM::MaxStrLength, _TRUNCATE,
				"%s: (%d, %d, %d, %d", getConstString(STR_INVALID_ROI).c_str(),
				roi_.x, roi_.y, roi_.xSize, roi_.ySize);
		return DVCCAM_ERROR_CODE;
	}

// adjust image extent to conform to the bin size
	roi_.xSize -= (roi_.xSize % binSize_);
	roi_.ySize -= (roi_.ySize % binSize_);

	RECT rc;
	rc.left = roi_.x;
	rc.right = roi_.x + roi_.xSize;
	rc.top = roi_.y;
	rc.bottom = roi_.y + roi_.ySize;

	if (!dvcSetRoi(hCam, 1, &rc)) {
		roi_ = oldRoi;
		return logDeviceError();
	}
	// Sometimes, the requested ROI cannot be set, in which case
	// the returned rc contains actual ROI information.
	roi_.x = rc.left;
	roi_.y = rc.top;
	roi_.xSize = rc.right - rc.left;
	roi_.ySize = rc.bottom - rc.top;

	ResizeImageBuffer();

	return DEVICE_OK;
}

int DVCCamera::ClearROI() {
	DriverGuard dg(this);
	HANDLE hCam = camMap_[m_camId];

	if (sequenceRunning_)
		return getErrorMsg(getConstString(STR_CAM_BUSY).c_str());

//added to use RTA
	SetToIdle();

	roi_.x = 0;
	roi_.y = 0;
	roi_.xSize = fullFrameX_;
	roi_.ySize = fullFrameY_;

// adjust image extent to conform to the bin size
	roi_.xSize -= roi_.xSize % binSize_;
	roi_.ySize -= roi_.ySize % binSize_;

	RECT rc;
	rc.left = roi_.x;
	rc.right = roi_.x + roi_.xSize;
	rc.top = roi_.y;
	rc.bottom = roi_.y + roi_.ySize;
	if (!dvcSetRoi(hCam, 0, &rc))
		return logDeviceError();

	ResizeImageBuffer();
	OnPropertiesChanged();
	return DEVICE_OK;
}

int DVCCamera::ResizeImageBuffer() {
// resize internal buffers
// NOTE: we are assuming 16-bit pixel type
	const int bpp = (int) ceil(depth_ / 8.0);
	img_.Resize(roi_.xSize / binSize_, roi_.ySize / binSize_, bpp);
	return DEVICE_OK;
}

int DVCCamera::AcqSequenceThread::svc() {
	int streamState;
	unsigned long remainings;
	imageCounter_ = 0;
	prevFrameIndex_ = -1;
	do {
		// Get current progress
		int currentFrameIndex;
		{
			DriverGuard dg(pCam_);
			dvcGetUserBufferId(hCam_, &currentFrameIndex);
			//return pCam->processErr();
			dvcGetStreamState(hCam_, &streamState, &remainings);
		}

		if (currentFrameIndex == prevFrameIndex_ || currentFrameIndex == -1) {
			// No image is available yet
			CDeviceUtils::SleepMs(waitTime_);
			continue;
		}

		int capturedFrames;
		if (currentFrameIndex > prevFrameIndex_)
			capturedFrames = currentFrameIndex - prevFrameIndex_;
		else
			capturedFrames = g_UserBufferNumber - prevFrameIndex_
					+ currentFrameIndex;

		for (int i = 0; i < capturedFrames; i++) {
			int id = prevFrameIndex_ + i + 1;
			if (id >= g_UserBufferNumber)
				id -= g_UserBufferNumber;
			pCam_->PushImage(id);
			imageCounter_++;
		}
		prevFrameIndex_ = currentFrameIndex;

		CDeviceUtils::SleepMs(waitTime_);
	} while (remainings > 0 && !stop_ && streamState != DVC_IDLE);

	if (stop_) {
		pCam_->LogMessage("Acquisition interrupted by the user.", false);
		return DEVICE_OK;
	}

	if (imageCounter_ != numImages_) {
		pCam_->LogMessage("Did not get the intended number of images.", false);
		pCam_->StopCameraAcquisition();
		return DEVICE_OK;
	}
	pCam_->StopCameraAcquisition();
	return DEVICE_OK;
}

/**
 * Starts continuous acquisition.
 */
int DVCCamera::StartSequenceAcquisition(long numImages, double interval_ms,
		bool stopOnOverflow) {
	DriverGuard dg(this);

	HANDLE hCam = camMap_[m_camId];

	if (sequenceRunning_)
		return getErrorMsg(getConstString(STR_CAM_BUSY).c_str());

	stopOnOverflow_ = stopOnOverflow;
	sequenceLength_ = numImages;
	intervalMs_ = interval_ms;

	if (IsAcquiring()) {
		SetToIdle();
	}

	ostringstream os;
	os << "Started sequence acquisition: " << numImages << "images  at "
			<< interval_ms << " ms" << endl;
	LogMessage(os.str().c_str());

// start thread
	imageCounter_ = 0;

	os.str("");
	os << "Setting thread length to " << numImages << " Images";
	LogMessage(os.str().c_str(), true);
	thd_->SetLength(numImages);

	thd_->SetWaitTime((int) (dvcGetExposeMsec(hCam) / 5));
	thd_->SetTimeOut(10000);

// prepare the core
	int ret = GetCoreCallback()->PrepareForAcq(this);
	if (ret != DEVICE_OK) {
		return ret;
	}

	LogMessage("Starting acquisition in the camera", true);
	startTime_ = GetCurrentMMTime();
	QueryPerformanceCounter(&perfStartCounter_);
	dvcStartTs_ = dvcElapseTime(0.0);

	if (!dvcSetUserBuffers(hCam, NULL))
		return processErr();
	if (!dvcReleaseUserBuffers(&userBuffers_))
		return processErr();
	if (!dvcAllocateUserBuffers(hCam, &userBuffers_, g_UserBufferNumber))
		return processErr();
	if (!dvcSetUserBuffers(hCam, &userBuffers_))
		return processErr();

	if (!dvcStartSequence(hCam, numImages)) {
		std::ostringstream os;
		os << "DVC driver returned error value: " << ret;
		LogMessage(os.str().c_str(), false);
		return processErr();
	} else {
		thd_->Start(numImages, interval_ms);
		sequenceRunning_ = true;
	}
	return DEVICE_OK;
}

/**
 * Stops Sequence acquisition
 * This is for external use only (if called from the sequence acquisition thread, deadlock will ensue!
 */
int DVCCamera::StopSequenceAcquisition() {
	return StopSequenceAcquisition(false);
}

int DVCCamera::StopSequenceAcquisition(bool temporary) {
	{
		DriverGuard dg(this);
		sequencePaused_ = temporary;
		StopCameraAcquisition();
	}

	thd_->Stop();
	thd_->wait();

	return DEVICE_OK;
}

/**
 * Stop Seq sequence acquisition
 * This is the function for internal use and can/should be called from the thread
 */
int DVCCamera::StopCameraAcquisition() {
	{
		DriverGuard dg(this);
		HANDLE hCam = camMap_[m_camId];

		if (!sequenceRunning_)
			return DEVICE_OK;

		LogMessage("Stopped sequence acquisition");
		dvcStopSequence(hCam);

		int state;
		unsigned long remaining;
		dvcGetStreamState(hCam, &state, &remaining);

		while (state != DVC_IDLE) {
			CDeviceUtils::SleepMs(100);
			dvcGetStreamState(hCam, &state, &remaining);
		}
		sequenceRunning_ = false;
	}

	MM::Core* cb = GetCoreCallback();
	if (cb)
		return cb->AcqFinished(this, 0);
	else
		return DEVICE_OK;
}

/**
 * Waits for new image and inserts it in the circular buffer.
 * This method is called by the acquisition thread AcqSequenceThread::svc()
 * in an infinite loop.
 *
 * In case of error or if the sequence is finished StopSequenceAcquisition()
 * is called, which will raise the stop_ flag and cause the thread to exit.
 */
int DVCCamera::PushImage(int userBufferId) {
// create metadata
	char label[MM::MaxStrLength];
	this->GetLabel(label);

	MM::MMTime timestamp = this->GetCurrentMMTime();
	Metadata md;
// Copy the metadata inserted by other processes:
	std::vector<std::string> keys = metadata_.GetKeys();
	for (unsigned int i = 0; i < keys.size(); i++) {
		md.put(keys[i],
				metadata_.GetSingleTag(keys[i].c_str()).GetValue().c_str());
	}

	ImageMetaDataP pMd = userBuffers_.pMeta + userBufferId;
	md.put("Camera", label);
	md.put(MM::g_Keyword_Metadata_StartTime, dvcStartTs_);

//	LARGE_INTEGER perfCounter;
//	QueryPerformanceCounter(&perfCounter);
//	md.put(MM::g_Keyword_Elapsed_Time_ms,
//			(double) (perfCounter.QuadPart - perfStartCounter_.QuadPart)
//					/ (double) perfFreq_.QuadPart);

	md.put(MM::g_Keyword_Elapsed_Time_ms, pMd->dExposeTimeStamp - dvcStartTs_);

	md.put(MM::g_Keyword_Metadata_ImageNumber,
			CDeviceUtils::ConvertToString((long) (pMd->ulStreamCount)));

	md.put(MM::g_Keyword_Binning, binSize_);
	md.PutTag(g_hBinning, label, pMd->hBin);
	md.PutTag(g_vBinning, label, pMd->vBin);
	md.PutTag(g_MD_ExposeTime, label, pMd->dExposeTime);
	md.PutTag(g_MD_ExposeTimestamp, label, pMd->dExposeTimeStamp);
	md.PutTag(g_MD_FrameTimestamp, label, pMd->dFrameTime);
	md.PutTag(g_MD_TransferTime, label, pMd->dTransferTime);
	md.PutTag(g_MD_TriggerTimestamp, label, pMd->dTriggerTimeStamp);

	const int height = GetImageHeight();
	const int width = GetImageWidth();
	const int bytesPerPixel = GetImageBytesPerPixel();

	imageCounter_++;

// This method inserts new image in the circular buffer (residing in MMCore)
	const int retCode = GetCoreCallback()->InsertImage(this,
			(const unsigned char*) (userBuffers_.pBuffers[userBufferId]),
			(unsigned int) width, (unsigned int) height,
			(unsigned int) bytesPerPixel, &md);

	if (!stopOnOverflow_ && retCode == DEVICE_BUFFER_OVERFLOW) {
//		LogMessage("Overflow occured.");
		// do not stop on overflow - just reset the buffer
		GetCoreCallback()->ClearImageBuffer(this);
		return GetCoreCallback()->InsertImage(this,
				(const unsigned char*) (userBuffers_.pBuffers[userBufferId]),
				(unsigned int) width, (unsigned int) height,
				(unsigned int) bytesPerPixel, md.Serialize().c_str());
	}
	userBuffers_.pBufferStatus[userBufferId] = 0;

	return DEVICE_OK;
}
