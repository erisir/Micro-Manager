// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AVTCAMERA_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AVTCAMERA_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef AVTCAMERA_EXPORTS
#define AVTCAMERA_API __declspec(dllexport)
#else
#define AVTCAMERA_API __declspec(dllimport)
#endif

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/DeviceThreads.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceUtils.h"

#include "AVT_Guppy_F146BCamera.h"
#include <queue>
#include <string>
#include <VimbaCPP/Include/VimbaSystem.h>
#include <VimbaCPP/Include/Camera.h>
using namespace std;
using namespace AVT::VmbAPI;

// error codes
#define ERR_BUFFER_ALLOCATION_FAILED 101
#define ERR_INCOMPLETE_SNAP_IMAGE_CYCLE 102
#define ERR_INVALID_ROI 103
#define ERR_INVALID_READOUT_MODE_SETUP 104
#define ERR_CAMERA_DOES_NOT_EXIST 105
#define ERR_BUSY_ACQUIRING 106
#define ERR_INVALID_PREAMPGAIN 107
#define ERR_INVALID_VSPEED 108
#define ERR_TRIGGER_NOT_SUPPORTED 109
#define ERR_OPEN_OR_CLOSE_SHUTTER_IN_ACQUISITION_NOT_ALLOWEDD 110
#define ERR_NO_AVAIL_AMPS 111
#define ERR_SOFTWARE_TRIGGER_IN_USE 112
#define ERR_SETROI_ERROR 113
class  FrameObserver;
class AVTCamera : public CCameraBase<AVTCamera>
{
public:
	AVTCamera(const char * CameraId);
	~AVTCamera();
	const char * m_CameraId;
	// MMDevice API
	int Initialize();
	int Shutdown();
	int OpenCamera();
	void GetName(char* pszName) const;
	bool Busy() { return false; }
	FrameObserver* m_pFrameObserver;
	// MMCamera API
	int SnapImage();
	const unsigned char* GetImageBuffer();
	long GetImageBufferSize() const { return img_.Width() * img_.Height() * GetImageBytesPerPixel(); }
	unsigned int GetImageWidth() const { return img_.Width(); }
	unsigned int GetImageHeight() const { return img_.Height(); }
	unsigned int GetImageBytesPerPixel() const { return img_.Depth(); }
	unsigned int GetBitDepth() const { return depth_; }
	int GetBinning() const { return binSize_; }
	int SetBinning(int binSize);
	double GetExposure() const;
	void SetExposure(double dExp);
	int SetROI(unsigned uX, unsigned uY, unsigned uXSize, unsigned uYSize); 
	int GetROI(unsigned& uX, unsigned& uY, unsigned& uXSize, unsigned& uYSize);
	int ClearROI();
	int IsExposureSequenceable(bool& isSequenceable) const { isSequenceable = false; return DEVICE_OK; }
	int PrepareSequenceAcqusition();
	int StartSequenceAcquisition(double interval);
	int StartSequenceAcquisition(long numImages, double interval_ms, bool stopOnOverflow);
	int StopSequenceAcquisition();
	int InsertImage();
	bool IsCapturing();
	int ThreadRun(FramePtr frame);
	int m_busy;
private:

	friend class  FrameObserver;

	// Update the image buffer information
	int ResizeImageBuffer();

	void StopCamera();

	// Property handlers
	int OnDeInterlace(MM::PropertyBase* pProp, MM::ActionType eAct);
	int OnBinning(MM::PropertyBase* pProp, MM::ActionType eAct);

	std::vector<double> exposureSequence_;
	MM::MMTime sequenceStartTime_;
	long imageCounter_;
	bool sequenceRunning_;
	int GenerateImage(FramePtr frame);
	// Camera information
	ImgBuffer img_;
	int depth_;
	int binSize_;
	std::string camName_;
	int fullFrameX_;
	int fullFrameY_;
	unsigned char* fullFrameBuffer_;
	int fullFrameBufferSize_;
	unsigned long timeout_;
	// Methods for de-interlacing. 0 for none, 1 for normal, 2 for Jiwei's special algorithm
	int deinterlace_;
	double exposure_;
	static unsigned int refCount_;
	static AVTCamera* instance_;
	bool initialized_;
	AVT_Guppy_F146BCamera *cam_;
	CameraPtr m_pCamera;
	bool m_isbusy;
	struct ROI {
		int x;
		int y;
		int xSize;
		int ySize;

		ROI() : x(0), y(0), xSize(0), ySize(0) {}
		~ROI() {}

		bool isEmpty() {return x==0 && y==0 && xSize==0 && ySize == 0;}
	};
	ROI roi_;


};


class FrameObserver : virtual public IFrameObserver
{
	friend class AVTCamera;

public:
	FrameObserver(AVTCamera* pCam,  CameraPtr pCamera );
	//	AVTCamera *cam_ ;
	// This is our callback routine that will be executed on every received frame
	virtual void FrameReceived( const FramePtr pFrame );
	// After the view has been notified about a new frame it can pick it up
	FramePtr GetFrame();
	AVTCamera* cam_;
	// Clears the double buffer frame queue
	void ClearFrameQueue();
	bool hasNewFrame();
private:
	// Since a MFC message cannot contain a whole frame
	// the frame observer stores all FramePtr
	std::queue<FramePtr> m_Frames;
	AVT::VmbAPI::Mutex m_FramesMutex;
	bool m_newFrameArrive;
};
