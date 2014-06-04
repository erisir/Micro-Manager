// AVTCamera.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include "AVTCamera.h"
#include <VimbaCPP/Include/VimbaSystem.h>
#include "AVT_Guppy_F146BCamera.h"
using namespace std;
using namespace AVT::VmbAPI;

/////////////////////////// START HERE /////////////////////////////////

// All the DVC error code will be returned plus this.
const int g_Err_Offset = 10000;

const char* g_AVTCameraDeviceName = "AVTCamera";
// singleton instance
AVTCamera* AVTCamera::instance_ = 0;

// global Andor driver thread lock
MMThreadLock g_AVTCamDriverLock;

// Properties
const char g_CameraName[] = "Camera Name";
const char g_DeInterlace[] = "De-interlace Algorithm";
const char g_Binning[] = "Binning";
const char g_Label[] = "Label";
const char g_PixelFormat[] = "PixelFormat";

///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////

/**
 * List all suppoerted hardware devices here
 * Do not discover devices at runtime.  To avoid warnings about missing DLLs, Micro-Manager
 * maintains a list of supported device (MMDeviceList.txt).  This list is generated using 
 * information supplied by this function, so runtime discovery will create problems.
 */
MODULE_API void InitializeModuleData()
{
	VimbaSystem& sys = VimbaSystem::GetInstance();  // Get a reference to the VimbaSystem singleton
	sys.Startup();              					// Initialize the Vimba API
	CameraPtrVector cameras;                        // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects

	std::string strID;                              // The ID of the cam
	std::string strName;                            // The name of the cam


	if ( VmbErrorSuccess == sys.GetCameras( cameras ) )
	{
		for ( CameraPtrVector :: iterator iter = cameras.begin ();cameras.end() != iter;++iter )
		{
			(*iter)->GetID( strID );
			(*iter)->GetName( strName );
			AddAvailableDeviceName(strName.c_str(), "AVT  Camera");
		}
	}
	sys.Shutdown();
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
	if (deviceName == 0)
		return 0;

	VimbaSystem& sys = VimbaSystem::GetInstance();  // Get a reference to the VimbaSystem singleton
	sys.Startup();               // Initialize the Vimba API
	CameraPtrVector cameras;                        // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects

	std::string strID;                              // The ID of the cam
	std::string strName;                            // The name of the cam

	if ( VmbErrorSuccess == sys.GetCameras( cameras ) )
	{
		for ( CameraPtrVector :: iterator iter = cameras.begin ();cameras.end() != iter;++iter )
		{
			(*iter)->GetID( strID );
			(*iter)->GetName( strName );
			if (strcmp(deviceName, strName.c_str()) == 0){
				sys.Shutdown();
				return new AVTCamera(strID.c_str());
			}
		}
	}
	sys.Shutdown();

	return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
	delete pDevice;
}

AVTCamera::AVTCamera(const char * CameraId)
{
	InitializeDefaultErrorMessages();
	// add custom messages
	SetErrorText(ERR_BUSY_ACQUIRING, "Camera Busy.  Stop camera activity first.");
	SetErrorText(ERR_NO_AVAIL_AMPS, "No available amplifiers.");
	SetErrorText(ERR_TRIGGER_NOT_SUPPORTED, "Trigger Not supported.");
	SetErrorText(ERR_INVALID_VSPEED, "Invalid Vertical Shift Speed.");
	SetErrorText(ERR_INVALID_PREAMPGAIN, "Invalid Pre-Amp Gain.");
	SetErrorText(ERR_CAMERA_DOES_NOT_EXIST, "No Camera Found.  Make sure it is connected and switched on, and try again.");
	SetErrorText(ERR_SOFTWARE_TRIGGER_IN_USE, "Only one camera can use software trigger.");
	SetErrorText(ERR_SETROI_ERROR, "setting roi error");

	// Find cameras
	binSize_ = 1;
	depth_ = 8;
	initialized_ = false;
	fullFrameX_ = 0;
	fullFrameY_ = 0;
	fullFrameBufferSize_ = 0;
	timeout_ = 1000;
	exposure_ = 20;
	fullFrameBuffer_ = 0;
	m_CameraId = CameraId;
	VimbaSystem::GetInstance().Startup();
	OpenCamera();
	cam_ = new AVT_Guppy_F146BCamera(m_pCamera);

}

AVTCamera::~AVTCamera()
{
	if (initialized_) {
		Shutdown();
	}
	instance_ = NULL;
}


int AVTCamera::OpenCamera()
{
	return VimbaSystem::GetInstance().OpenCameraByID(m_CameraId,VmbAccessModeFull, m_pCamera );
}
int AVTCamera::Initialize()
{
	VmbErrorType err;
	if (initialized_)
		return DEVICE_OK;

	VmbInt64_t height,width;
	err = cam_->SetPixelFormat(AVT_Guppy_F146BCamera::PixelFormat_Mono12);
	err = cam_->GetHeightMax(height);
	err = cam_->GetWidthMax(width);
	err = cam_->SetOffsetX(0);
	err = cam_->SetOffsetY(0);
	err = cam_->SetHeight(height);
	err = cam_->SetWidth(width);
	fullFrameX_ =(int) width;
	fullFrameY_ =(int) height;
	imageCounter_ = 0;
	m_isbusy = false;
	depth_ = 16;
	binSize_ = 1;
	roi_.x = 0;
	roi_.y = 0;
	roi_.xSize = fullFrameX_;
	roi_.ySize = fullFrameY_;
	fullFrameBufferSize_ = fullFrameX_ * fullFrameY_ * (depth_ / 8 ) ;
	fullFrameBuffer_ = new unsigned char[fullFrameBufferSize_];
	memset(fullFrameBuffer_,10,fullFrameBufferSize_);
	ResizeImageBuffer();
	CPropertyAction* pActOnSetPixelFormat = new CPropertyAction(this, &AVTCamera::OnSetPixelFormat);
	char format[20];
	sprintf(format, "%d", 8);
	CreateProperty(g_PixelFormat, format, MM::String, false, pActOnSetPixelFormat);

	CPropertyAction* pActOnBinning = new CPropertyAction(this, &AVTCamera::OnBinning);
	char binning[20];
	sprintf(binning, "%d", 1);
	CreateProperty(g_Binning, binning, MM::String, false, pActOnBinning);
	return DEVICE_OK;
}

int AVTCamera::ResizeImageBuffer()
{
	//	// resize internal buffers
	//	// NOTE: we are assuming 16-bit pixel type
	const int bpp = (int)ceil(depth_/8.0);
	img_.Resize(roi_.xSize / binSize_, roi_.ySize / binSize_, bpp);
	//	if(fullFrameBuffer_)
	//		delete fullFrameBuffer_;
	//	fullFrameBufferSize_ = roi_.xSize  * roi_.ySize * (depth_ / 8 ) ;
	//	fullFrameBuffer_ = new unsigned char[fullFrameBufferSize_];
	//	memset(fullFrameBuffer_,10,fullFrameBufferSize_);

	return DEVICE_OK;
}

/**
 * Deactivate the camera, reverse the initialization process.
 */
int AVTCamera::Shutdown()
{
	if (initialized_)
	{
		StopCamera();
		m_pCamera->Close();
		VimbaSystem::GetInstance().Shutdown();
		delete fullFrameBuffer_;
	}

	initialized_ = false;
	return DEVICE_OK;
}

void AVTCamera::StopCamera()
{
	LogMessage("StopCamera");
	if(m_isbusy)
		StopSequenceAcquisition();
	m_pCamera->Close();
}

//added to use RTA
/**
 * Acquires a single frame.
 * Micro-Manager expects that this function blocks the calling thread until the exposure phase is over.
 * This wait is implemented by sleeping ActualInterval_ms_ - ReadoutTime_ + 0.99 ms.
 * Note that this is likely not long enough when using internal triggering.
 */
int AVTCamera::SnapImage()
{
	std::ostringstream osMessage;
	if (m_isbusy)   // If we are in the middle of a SequenceAcquisition
		return ERR_BUSY_ACQUIRING;
	FramePtr frame;
	m_pCamera->AcquireSingleImage(frame,1000);
	// See if it is not corrupt
	VmbFrameStatusType eReceiveStatus;
	VmbErrorType err;
	err = frame->GetReceiveStatus( eReceiveStatus );
	if ( VmbErrorSuccess == err && VmbFrameStatusComplete == eReceiveStatus )
	{
		VmbUchar_t *pBuffer;
		frame->GetImage(pBuffer);
		VmbUint32_t bufferSize_ = 0;
		frame->GetBufferSize(bufferSize_);
		memcpy(fullFrameBuffer_,pBuffer,bufferSize_);
	}
	else{
		osMessage.str("");
		osMessage << "snap image error:err code["<< err<<"]eReceiveStatus[" <<eReceiveStatus<<"]";
		this->LogMessage(osMessage.str().c_str());
	}

	return DEVICE_OK;
}

void AVTCamera::GetName(char* name) const 
{
	CDeviceUtils::CopyLimitedString(name, g_AVTCameraDeviceName);
}

const unsigned char* AVTCamera::GetImageBuffer()
{
	return fullFrameBuffer_;
}

int AVTCamera::SetBinning(int bin)
{
	binSize_ = bin;
	return DEVICE_OK;
}

void AVTCamera::SetExposure(double exp)
{
	exposure_ = exp;
	if(m_isbusy)
		StopSequenceAcquisition();
	cam_->SetExposureTime(exp*1000);
}

double AVTCamera::GetExposure() const
{
	return exposure_;
}

int AVTCamera::SetROI(unsigned uX, unsigned uY, unsigned uXSize, unsigned uYSize)
{
	if(m_isbusy)
		StopSequenceAcquisition();

	ROI oldRoi = roi_;
	//F146B Roi设置说明：uX/uY/uYSize 为偶数,uXSize为4的倍数

	uX -= uX%2;
	uY -= uY%2;
	uYSize -= uYSize%2;
	uXSize -= uXSize%4;

	binSize_ = 1;

	roi_.x = uX * binSize_;
	roi_.xSize = uXSize * binSize_;
	roi_.y = uY * binSize_;
	roi_.ySize = uYSize * binSize_;

	if (roi_.x + roi_.xSize > fullFrameX_ || roi_.y + roi_.ySize > fullFrameY_)
	{
		roi_ = oldRoi;
		return ERR_INVALID_ROI;
	}
	std::ostringstream osMessage;
	osMessage.str("");
	VmbErrorType ret;

	ret = cam_->SetOffsetY(roi_.y);
	osMessage << "SetOffsetY Value[" <<roi_.y<<"]"<<"ret=["<<ret<<"]\r\n";
	if(VmbErrorSuccess != ret){
		ret = cam_->SetHeight(roi_.ySize);
		osMessage << "SetHeight Value[" <<roi_.ySize<<"]"<<"ret=["<<ret<<"]\r\n";
		ret = cam_->SetOffsetY(roi_.y);
		osMessage << "SetOffsetY Value[" <<roi_.y<<"]"<<"ret=["<<ret<<"]\r\n";
	}else{
		ret = cam_->SetHeight(roi_.ySize);
		osMessage << "SetHeight Value[" <<roi_.ySize<<"]"<<"ret=["<<ret<<"]\r\n";
	}

	ret = cam_->SetOffsetX(roi_.x);
	osMessage << "SetOffsetX Value[" <<roi_.x<<"]"<<"ret=["<<ret<<"]\r\n";
	if(VmbErrorSuccess != ret){
		ret = cam_->SetWidth(roi_.xSize);
		osMessage << "SetWidth Value[" <<roi_.xSize<<"]"<<"ret=["<<ret<<"]\r\n";
		ret = cam_->SetOffsetX(roi_.x);
		osMessage << "SetOffsetX Value[" <<roi_.x<<"]"<<"ret=["<<ret<<"]\r\n";
	}else{
		ret = cam_->SetWidth(roi_.xSize);
		osMessage << "SetWidth Value[" <<roi_.xSize<<"]"<<"ret=["<<ret<<"]\r\n";
	}
	this->LogMessage(osMessage.str());
	ResizeImageBuffer();
	OnPropertiesChanged();
	return DEVICE_OK;
}

int AVTCamera::GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize, unsigned& uYSize)
{
	uX = roi_.x / binSize_;
	uY = roi_.y / binSize_;
	uXSize = roi_.xSize / binSize_;
	uYSize = roi_.ySize / binSize_;

	return DEVICE_OK;
}

int AVTCamera::ClearROI()
{
	SetROI(0,0, fullFrameX_, fullFrameY_);
	return SetROI(0,0, fullFrameX_, fullFrameY_);
}

int AVTCamera::OnSetPixelFormat(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		long tmp = format_;
		pProp->Set(tmp);
	}
	else if (eAct == MM::AfterSet)
	{
		long tmp;
		pProp->Get(tmp);
		format_ = tmp;
		if(m_isbusy)
			StopSequenceAcquisition();
		if(format_ == 8 ){
			cam_->SetPixelFormat(AVT_Guppy_F146BCamera::PixelFormat_Mono8);
			depth_ = 8;
		}
		if(format_ == 12 ){
			cam_->SetPixelFormat(AVT_Guppy_F146BCamera::PixelFormat_Mono12);
			depth_ = 16;
		}
		if(format_ == 16 ){
			cam_->SetPixelFormat(AVT_Guppy_F146BCamera::PixelFormat_Mono16);
			depth_ = 16;
		}
		ResizeImageBuffer();
	}
	return DEVICE_OK;
}
int AVTCamera::OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	if (eAct == MM::BeforeGet)
	{
		long tmp = binSize_;
		pProp->Set(tmp);
	}
	else if (eAct == MM::AfterSet)
	{
		long tmp;
		pProp->Get(tmp);
		binSize_ = tmp;
	}
	return DEVICE_OK;
}

int AVTCamera::PrepareSequenceAcqusition()
{
	if (IsCapturing())
		return DEVICE_CAMERA_BUSY_ACQUIRING;

	int ret = GetCoreCallback()->PrepareForAcq(this);
	if (ret != DEVICE_OK)
		return ret;

	return DEVICE_OK;
}


/**
 * Required by the MM::Camera API
 * Please implement this yourself and do not rely on the base class implementation
 * The Base class implementation is deprecated and will be removed shortly
 */
int AVTCamera::StartSequenceAcquisition(double interval) {
	imageCounter_ =0;
	return StartSequenceAcquisition(LONG_MAX, interval, false);
}

/**
 * Stop and wait for the Sequence thread finished
 */
int AVTCamera::StopSequenceAcquisition()
{
	VmbErrorType ret;

	ret = m_pCamera->StopContinuousImageAcquisition();
	std::ostringstream os;
	os << "\t\t********StopContinuousImageAcquisition: ret[" << ret<<"]";
	LogMessage(os.str().c_str());
	m_isbusy = false;
	return ret;
}

/**
 * Simple implementation of Sequence Acquisition
 * A sequence acquisition should run on its own thread and transport new images
 * coming of the camera into the MMCore circular buffer.
 */
int AVTCamera::StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow)
{
	if (IsCapturing())
		return DEVICE_CAMERA_BUSY_ACQUIRING;
	imageCounter_ =0;
	m_numImages = numImages;
	m_isbusy = true;
	m_pFrameObserver = new FrameObserver(this,m_pCamera );
	VmbErrorType ret;
	ret = m_pCamera->StartContinuousImageAcquisition(3,
			IFrameObserverPtr(m_pFrameObserver));
	std::ostringstream os;
	os << "\t\t********StartContinuousImageAcquisition: ret[" << ret<<"]";
	LogMessage(os.str().c_str());
	return DEVICE_OK;
}

/*
 * Inserts Image and MetaData into MMCore circular Buffer
 */
int AVTCamera::InsertImage()
{

	MM::MMTime timeStamp = this->GetCurrentMMTime();
	char label[MM::MaxStrLength];
	this->GetLabel(label);

	// Important:  metadata about the image are generated here:
	Metadata md;
	md.put("Camera", label);
	md.put(MM::g_Keyword_Metadata_StartTime, CDeviceUtils::ConvertToString(sequenceStartTime_.getMsec()));
	md.put(MM::g_Keyword_Elapsed_Time_ms, CDeviceUtils::ConvertToString((timeStamp - sequenceStartTime_).getMsec()));
	md.put(MM::g_Keyword_Metadata_ROI_X, CDeviceUtils::ConvertToString( (long)roi_.x));
	md.put(MM::g_Keyword_Metadata_ROI_Y, CDeviceUtils::ConvertToString( (long) roi_.y));

	char buf[MM::MaxStrLength];
	GetProperty(MM::g_Keyword_Binning, buf);
	md.put(MM::g_Keyword_Binning, buf);


	const unsigned char* pI;
	pI = GetImageBuffer();

	unsigned int w = GetImageWidth();
	unsigned int h = GetImageHeight();
	unsigned int b = GetImageBytesPerPixel();
	int ret = GetCoreCallback()->InsertImage(this, pI, w, h, b, md.Serialize().c_str());
	if (ret == DEVICE_BUFFER_OVERFLOW)
	{
		// do not stop on overflow - just reset the buffer
		GetCoreCallback()->ClearImageBuffer(this);
		LogMessage("DEVICE_BUFFER_OVERFLOW");
		return GetCoreCallback()->InsertImage(this, pI, w, h, b, md.Serialize().c_str(), false);
	} else
		return ret;
}


int  AVTCamera::ThreadRun (FramePtr frame)
{
	GenerateImage(frame);
	imageCounter_++;
	if(imageCounter_ >= m_numImages){
		LogMessage("m_numImages Enough, StopSequenceAcquisition");
		return StopSequenceAcquisition();
	}
	return  InsertImage();
};
int AVTCamera::GenerateImage(FramePtr frame)
{
	VmbUchar_t *pBuffer;
	VmbUint32_t bufferSize_ = 0;

	frame->GetImage(pBuffer);
	frame->GetImageSize(bufferSize_);
	memcpy(fullFrameBuffer_,pBuffer,bufferSize_);
	m_pCamera->QueueFrame( frame );
	return DEVICE_OK;
}


FrameObserver::FrameObserver(AVTCamera* pCam,  CameraPtr pCamera ): IFrameObserver( pCamera )
{
	cam_ = pCam;
}
void FrameObserver::FrameReceived( const FramePtr pFrame )
{
	bool bQueueDirectly = true;
	VmbFrameStatusType eReceiveStatus;

	if ( VmbErrorSuccess == pFrame->GetReceiveStatus( eReceiveStatus ))
	{
		cam_->ThreadRun(pFrame);
	}else{
		m_pCamera->QueueFrame( pFrame );

	}
}
