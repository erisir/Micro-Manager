/*=============================================================================
  Copyright (C) 2013 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  Please do not modify this file, because it was created automatically by a 
  code generator tool (AVT VimbaClassGenerator). So any manual modifications 
  will be lost if you run the tool again.

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifndef AVT_GUPPY_F146BCAMERA_H
#define AVT_GUPPY_F146BCAMERA_H

#include "VimbaCPP/Include/VimbaCPP.h"
using namespace AVT::VmbAPI;
class AVT_Guppy_F146BCamera
{
public:
	CameraPtr m_pCamera;
	typedef SP_DECL( AVT_Guppy_F146BCamera ) Ptr;

	typedef enum _AcquisitionModeEnum
	{
		AcquisitionMode_Continuous = 0,
		AcquisitionMode_SingleFrame = 1,
		AcquisitionMode_MultiFrame = 2
	} AcquisitionModeEnum;

	typedef enum _BlackLevelSelectorEnum
	{
		BlackLevelSelector_All = 0,
		BlackLevelSelector_IIDCBlackLevel = 1
	} BlackLevelSelectorEnum;

	typedef enum _DeviceScanTypeEnum
	{
		DeviceScanType_Areascan = 0
	} DeviceScanTypeEnum;

	typedef enum _ExposureAutoEnum
	{
		ExposureAuto_Off = 0,
		ExposureAuto_Once = 1,
		ExposureAuto_Continuous = 2
	} ExposureAutoEnum;

	typedef enum _ExposureAutoAlgEnum
	{
		ExposureAutoAlg_Mean = 0
	} ExposureAutoAlgEnum;

	typedef enum _ExposureAutoTimebaseEnum
	{
		ExposureAutoTimebase_tb1us = 1,
		ExposureAutoTimebase_tb2us = 2,
		ExposureAutoTimebase_tb5us = 5,
		ExposureAutoTimebase_tb10us = 10,
		ExposureAutoTimebase_tb20us = 20,
		ExposureAutoTimebase_tb50us = 50,
		ExposureAutoTimebase_tb100us = 100,
		ExposureAutoTimebase_tb200us = 200,
		ExposureAutoTimebase_tb500us = 500,
		ExposureAutoTimebase_tb1000us = 1000
	} ExposureAutoTimebaseEnum;

	typedef enum _ExposureModeEnum
	{
		ExposureMode_Timed = 0,
		ExposureMode_TriggerWidth = 1
	} ExposureModeEnum;

	typedef enum _FileOpenModeEnum
	{
		FileOpenMode_Read = 0,
		FileOpenMode_Write = 1,
		FileOpenMode_ReadWrite = 3
	} FileOpenModeEnum;

	typedef enum _FileOperationSelectorEnum
	{
		FileOperationSelector_Open = 0,
		FileOperationSelector_Close = 1,
		FileOperationSelector_Read = 2,
		FileOperationSelector_Write = 3
	} FileOperationSelectorEnum;

	typedef enum _FileOperationStatusEnum
	{
		FileOperationStatus_Success = 0,
		FileOperationStatus_Failure = 1
	} FileOperationStatusEnum;

	typedef enum _FileSelectorEnum
	{
		FileSelector_ShadingData = 0,
		FileSelector_LUTLuminance = 1,
		FileSelector_LUTLuminance2 = 2,
		FileSelector_LUTLuminance3 = 3,
		FileSelector_LUTLuminance4 = 4,
		FileSelector_LUTLuminance5 = 5,
		FileSelector_LUTLuminance6 = 6,
		FileSelector_LUTLuminance7 = 7,
		FileSelector_LUTLuminance8 = 8,
		FileSelector_LUTLuminance9 = 9,
		FileSelector_LUTLuminance10 = 10,
		FileSelector_LUTLuminance11 = 11,
		FileSelector_LUTLuminance12 = 12,
		FileSelector_LUTLuminance13 = 13,
		FileSelector_LUTLuminance14 = 14,
		FileSelector_LUTLuminance15 = 15,
		FileSelector_LUTLuminance16 = 16
	} FileSelectorEnum;

	typedef enum _FileStatusEnum
	{
		FileStatus_Closed = 0,
		FileStatus_Open = 1
	} FileStatusEnum;

	typedef enum _GainAutoEnum
	{
		GainAuto_Off = 0,
		GainAuto_Once = 1,
		GainAuto_Continuous = 2
	} GainAutoEnum;

	typedef enum _GainSelectorEnum
	{
		GainSelector_All = 0,
		GainSelector_IIDCGain = 1
	} GainSelectorEnum;

	typedef enum _IIDCIsoChannelAutoEnum
	{
		IIDCIsoChannelAuto_Off = 0,
		IIDCIsoChannelAuto_On = 1
	} IIDCIsoChannelAutoEnum;

	typedef enum _IIDCModeEnum
	{
		IIDCMode_Mode0 = 0,
		IIDCMode_Mode1 = 1,
		IIDCMode_Mode2 = 2,
		IIDCMode_Mode3 = 3,
		IIDCMode_Mode4 = 4,
		IIDCMode_Mode5 = 5,
		IIDCMode_Mode6 = 6,
		IIDCMode_Mode7 = 7
	} IIDCModeEnum;

	typedef enum _IIDCPacketSizeAutoEnum
	{
		IIDCPacketSizeAuto_Off = 0,
		IIDCPacketSizeAuto_Maximize = 1
	} IIDCPacketSizeAutoEnum;

	typedef enum _IIDCPhyspeedEnum
	{
		IIDCPhyspeed_S100 = 100,
		IIDCPhyspeed_S200 = 200,
		IIDCPhyspeed_S400 = 400
	} IIDCPhyspeedEnum;

	typedef enum _LUTSelectorEnum
	{
		LUTSelector_Luminance = 0,
		LUTSelector_Luminance2 = 1,
		LUTSelector_Luminance3 = 2,
		LUTSelector_Luminance4 = 3,
		LUTSelector_Luminance5 = 4,
		LUTSelector_Luminance6 = 5,
		LUTSelector_Luminance7 = 6,
		LUTSelector_Luminance8 = 7,
		LUTSelector_Luminance9 = 8,
		LUTSelector_Luminance10 = 9,
		LUTSelector_Luminance11 = 10,
		LUTSelector_Luminance12 = 11,
		LUTSelector_Luminance13 = 12,
		LUTSelector_Luminance14 = 13,
		LUTSelector_Luminance15 = 14,
		LUTSelector_Luminance16 = 15
	} LUTSelectorEnum;

	typedef enum _LineFormatEnum
	{
		LineFormat_TTL = 0,
		LineFormat_OptoCoupled = 1
	} LineFormatEnum;

	typedef enum _LineModeEnum
	{
		LineMode_Input = 0,
		LineMode_Output = 1
	} LineModeEnum;

	typedef enum _LineRoutingEnum
	{
		LineRouting_Off = 0,
		LineRouting_Trigger = 2
	} LineRoutingEnum;

	typedef enum _LineSelectorEnum
	{
		LineSelector_Line0 = 0,
		LineSelector_Line1 = 1,
		LineSelector_Line2 = 2,
		LineSelector_Line3 = 3,
		LineSelector_Line4 = 8,
		LineSelector_Line5 = 9,
		LineSelector_Line6 = 10,
		LineSelector_Line7 = 11
	} LineSelectorEnum;

	typedef enum _LineSourceEnum
	{
		LineSource_Off = 0,
		LineSource_Direct = 1,
		LineSource_ExposureActive = 2,
		LineSource_FrameValid = 6,
		LineSource_Busy = 7,
		LineSource_FollowInput = 8,
		LineSource_PWM = 9,
		LineSource_FrameTriggerWait = 10
	} LineSourceEnum;

	typedef enum _PixelColorFilterEnum
	{
		PixelColorFilter_None = 0,
		PixelColorFilter_BayerRG = 1,
		PixelColorFilter_BayerGB = 2,
		PixelColorFilter_BayerGR = 3,
		PixelColorFilter_BayerBG = 4
	} PixelColorFilterEnum;

	typedef enum _PixelColorFilterAutoEnum
	{
		PixelColorFilterAuto_Manual = 0,
		PixelColorFilterAuto_Auto = 1
	} PixelColorFilterAutoEnum;

	typedef enum _PixelFormatEnum
	{
		PixelFormat_Mono8 = 17301505,
		PixelFormat_Mono12Packed = 17563654,
		PixelFormat_Mono16 = 17825799
	} PixelFormatEnum;

	typedef enum _SensorTapsEnum
	{
		SensorTaps_One = 1,
		SensorTaps_Two = 2
	} SensorTapsEnum;

	typedef enum _SerialPortBaudRateEnum
	{
		SerialPortBaudRate_Baud300 = 0,
		SerialPortBaudRate_Baud600 = 1,
		SerialPortBaudRate_Baud1200 = 2,
		SerialPortBaudRate_Baud2400 = 3,
		SerialPortBaudRate_Baud4800 = 4,
		SerialPortBaudRate_Baud9600 = 5,
		SerialPortBaudRate_Baud19200 = 6,
		SerialPortBaudRate_Baud38400 = 7,
		SerialPortBaudRate_Baud57600 = 8,
		SerialPortBaudRate_Baud115200 = 9,
		SerialPortBaudRate_Baud230400 = 10
	} SerialPortBaudRateEnum;

	typedef enum _SerialPortCharLengthEnum
	{
		SerialPortCharLength_Length7Bits = 7,
		SerialPortCharLength_Length8Bits = 8
	} SerialPortCharLengthEnum;

	typedef enum _SerialPortModeSelectorEnum
	{
		SerialPortModeSelector_Off = 0,
		SerialPortModeSelector_Transmit = 1,
		SerialPortModeSelector_Receive = 2,
		SerialPortModeSelector_Both = 3
	} SerialPortModeSelectorEnum;

	typedef enum _SerialPortParityEnum
	{
		SerialPortParity_None = 0,
		SerialPortParity_Odd = 1,
		SerialPortParity_Even = 2
	} SerialPortParityEnum;

	typedef enum _SerialPortSelectorEnum
	{
		SerialPortSelector_IIDC = 0
	} SerialPortSelectorEnum;

	typedef enum _SerialPortStopBitsEnum
	{
		SerialPortStopBits_StopBits1 = 0,
		SerialPortStopBits_StopBits1_5 = 1,
		SerialPortStopBits_StopBits2 = 2
	} SerialPortStopBitsEnum;

	typedef enum _StreamAcquisitionModeSelectorEnum
	{
		StreamAcquisitionModeSelector_Default = 0
	} StreamAcquisitionModeSelectorEnum;

	typedef enum _StreamTypeEnum
	{
		StreamType_IIDC = 4
	} StreamTypeEnum;

	typedef enum _TestImageSelectorEnum
	{
		TestImageSelector_Off = 0,
		TestImageSelector_Img1 = 1,
		TestImageSelector_Img2 = 2,
		TestImageSelector_Img3 = 3,
		TestImageSelector_Img4 = 4
	} TestImageSelectorEnum;

	typedef enum _TriggerActivationEnum
	{
		TriggerActivation_FallingEdge = 0,
		TriggerActivation_RisingEdge = 1,
		TriggerActivation_LevelLow = 2,
		TriggerActivation_LevelHigh = 3
	} TriggerActivationEnum;

	typedef enum _TriggerModeEnum
	{
		TriggerMode_Off = 0,
		TriggerMode_On = 1
	} TriggerModeEnum;

	typedef enum _TriggerSelectorEnum
	{
		TriggerSelector_ExposureStart = 0,
		TriggerSelector_ExposureActive = 1,
		TriggerSelector_AcquisitionStart = 15
	} TriggerSelectorEnum;

	typedef enum _TriggerSourceEnum
	{
		TriggerSource_InputLines = 0,
		TriggerSource_Software = 7
	} TriggerSourceEnum;

	typedef enum _UserSetOperationStatusEnum
	{
		UserSetOperationStatus_Success = 0,
		UserSetOperationStatus_Failure = 1
	} UserSetOperationStatusEnum;

	typedef enum _UserSetSelectorEnum
	{
		UserSetSelector_Default = 0,
		UserSetSelector_UserSet1 = 1,
		UserSetSelector_UserSet2 = 2,
		UserSetSelector_UserSet3 = 3
	} UserSetSelectorEnum;


	AVT_Guppy_F146BCamera (CameraPtr Camera);


	// Category /AcquisitionControl
	VmbErrorType GetAcquisitionFrameCount (VmbInt64_t & value);
	VmbErrorType SetAcquisitionFrameCount (VmbInt64_t value);
	VmbErrorType GetAcquisitionFrameCountFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAcquisitionFrameRate (double & value);
	VmbErrorType SetAcquisitionFrameRate (double value);
	VmbErrorType GetAcquisitionFrameRateFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAcquisitionFrameRateLimit (double & value);
	VmbErrorType GetAcquisitionFrameRateLimitFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAcquisitionMode (AcquisitionModeEnum & value);
	VmbErrorType SetAcquisitionMode (AcquisitionModeEnum value);
	VmbErrorType GetAcquisitionModeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType AcquisitionStart ();
	VmbErrorType GetAcquisitionStartFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType AcquisitionStop ();
	VmbErrorType GetAcquisitionStopFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureAuto (ExposureAutoEnum & value);
	VmbErrorType SetExposureAuto (ExposureAutoEnum value);
	VmbErrorType GetExposureAutoFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureMode (ExposureModeEnum & value);
	VmbErrorType SetExposureMode (ExposureModeEnum value);
	VmbErrorType GetExposureModeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureOffset (double & value);
	VmbErrorType GetExposureOffsetFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureTime (double & value);
	VmbErrorType SetExposureTime (double value);
	VmbErrorType GetExposureTimeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetTriggerActivation (TriggerActivationEnum & value);
	VmbErrorType SetTriggerActivation (TriggerActivationEnum value);
	VmbErrorType GetTriggerActivationFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetTriggerDelay (double & value);
	VmbErrorType SetTriggerDelay (double value);
	VmbErrorType GetTriggerDelayFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetTriggerMode (TriggerModeEnum & value);
	VmbErrorType SetTriggerMode (TriggerModeEnum value);
	VmbErrorType GetTriggerModeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetTriggerSelector (TriggerSelectorEnum & value);
	VmbErrorType SetTriggerSelector (TriggerSelectorEnum value);
	VmbErrorType GetTriggerSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetTriggerSource (TriggerSourceEnum & value);
	VmbErrorType SetTriggerSource (TriggerSourceEnum value);
	VmbErrorType GetTriggerSourceFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /AcquisitionControl/ExposureAutoControl
	VmbErrorType GetExposureAutoAlg (ExposureAutoAlgEnum & value);
	VmbErrorType GetExposureAutoAlgFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureAutoMax (double & value);
	VmbErrorType SetExposureAutoMax (double value);
	VmbErrorType GetExposureAutoMaxFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureAutoMin (double & value);
	VmbErrorType SetExposureAutoMin (double value);
	VmbErrorType GetExposureAutoMinFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetExposureAutoTimebase (ExposureAutoTimebaseEnum & value);
	VmbErrorType SetExposureAutoTimebase (ExposureAutoTimebaseEnum value);
	VmbErrorType GetExposureAutoTimebaseFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /AnalogControl
	VmbErrorType GetBlackLevel (double & value);
	VmbErrorType SetBlackLevel (double value);
	VmbErrorType GetBlackLevelFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetBlackLevelSelector (BlackLevelSelectorEnum & value);
	VmbErrorType GetBlackLevelSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetGain (double & value);
	VmbErrorType SetGain (double value);
	VmbErrorType GetGainFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetGainAuto (GainAutoEnum & value);
	VmbErrorType SetGainAuto (GainAutoEnum value);
	VmbErrorType GetGainAutoFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetGainSelector (GainSelectorEnum & value);
	VmbErrorType GetGainSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetGamma (double & value);
	VmbErrorType SetGamma (double value);
	VmbErrorType GetGammaFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /AutofunctionControl
	VmbErrorType GetAutofunctionAOIEnable (bool & value);
	VmbErrorType SetAutofunctionAOIEnable (bool value);
	VmbErrorType GetAutofunctionAOIEnableFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAutofunctionAOIHeight (VmbInt64_t & value);
	VmbErrorType SetAutofunctionAOIHeight (VmbInt64_t value);
	VmbErrorType GetAutofunctionAOIHeightFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAutofunctionAOIOffsetX (VmbInt64_t & value);
	VmbErrorType SetAutofunctionAOIOffsetX (VmbInt64_t value);
	VmbErrorType GetAutofunctionAOIOffsetXFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAutofunctionAOIOffsetY (VmbInt64_t & value);
	VmbErrorType SetAutofunctionAOIOffsetY (VmbInt64_t value);
	VmbErrorType GetAutofunctionAOIOffsetYFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAutofunctionAOIShowArea (bool & value);
	VmbErrorType SetAutofunctionAOIShowArea (bool value);
	VmbErrorType GetAutofunctionAOIShowAreaFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAutofunctionAOIWidth (VmbInt64_t & value);
	VmbErrorType SetAutofunctionAOIWidth (VmbInt64_t value);
	VmbErrorType GetAutofunctionAOIWidthFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetAutofunctionTargetIntensity (VmbInt64_t & value);
	VmbErrorType SetAutofunctionTargetIntensity (VmbInt64_t value);
	VmbErrorType GetAutofunctionTargetIntensityFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /DataStreamModule
	VmbErrorType GetDriverBuffersCount (VmbInt64_t & value);
	VmbErrorType SetDriverBuffersCount (VmbInt64_t value);
	VmbErrorType GetDriverBuffersCountFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetStreamAcquisitionModeSelector (StreamAcquisitionModeSelectorEnum & value);
	VmbErrorType GetStreamAcquisitionModeSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetStreamAnnounceBufferMinimum (VmbInt64_t & value);
	VmbErrorType GetStreamAnnounceBufferMinimumFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetStreamAnnouncedBufferCount (VmbInt64_t & value);
	VmbErrorType GetStreamAnnouncedBufferCountFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetStreamID (std::string & value);
	VmbErrorType GetStreamIDFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetStreamIsGrabbing (bool & value);
	VmbErrorType GetStreamIsGrabbingFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetStreamType (StreamTypeEnum & value);
	VmbErrorType GetStreamTypeFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /DeviceControl
	VmbErrorType GetDeviceAccessRegisterAddress (VmbInt64_t & value);
	VmbErrorType SetDeviceAccessRegisterAddress (VmbInt64_t value);
	VmbErrorType GetDeviceAccessRegisterAddressFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceAccessRegisterValue (VmbInt64_t & value);
	VmbErrorType SetDeviceAccessRegisterValue (VmbInt64_t value);
	VmbErrorType GetDeviceAccessRegisterValueFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceFirmwareVersion (std::string & value);
	VmbErrorType GetDeviceFirmwareVersionFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceID (std::string & value);
	VmbErrorType GetDeviceIDFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceMicrocontrollerVersion (std::string & value);
	VmbErrorType GetDeviceMicrocontrollerVersionFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceModelName (std::string & value);
	VmbErrorType GetDeviceModelNameFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceSFNCVersionMajor (VmbInt64_t & value);
	VmbErrorType GetDeviceSFNCVersionMajorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceSFNCVersionMinor (VmbInt64_t & value);
	VmbErrorType GetDeviceSFNCVersionMinorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceSFNCVersionSubMinor (VmbInt64_t & value);
	VmbErrorType GetDeviceSFNCVersionSubMinorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceScanType (DeviceScanTypeEnum & value);
	VmbErrorType GetDeviceScanTypeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceSerialNumber (VmbInt64_t & value);
	VmbErrorType GetDeviceSerialNumberFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetDeviceVendorName (std::string & value);
	VmbErrorType GetDeviceVendorNameFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFirmwareVerBuild (VmbInt64_t & value);
	VmbErrorType GetFirmwareVerBuildFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFirmwareVerMajor (VmbInt64_t & value);
	VmbErrorType GetFirmwareVerMajorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFirmwareVerMinor (VmbInt64_t & value);
	VmbErrorType GetFirmwareVerMinorFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /DigitalIOControl
	VmbErrorType GetIntEnaDelayEnable (bool & value);
	VmbErrorType SetIntEnaDelayEnable (bool value);
	VmbErrorType GetIntEnaDelayEnableFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIntEnaDelayTime (double & value);
	VmbErrorType SetIntEnaDelayTime (double value);
	VmbErrorType GetIntEnaDelayTimeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineFormat (LineFormatEnum & value);
	VmbErrorType GetLineFormatFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineInverter (bool & value);
	VmbErrorType SetLineInverter (bool value);
	VmbErrorType GetLineInverterFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineMode (LineModeEnum & value);
	VmbErrorType GetLineModeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineRouting (LineRoutingEnum & value);
	VmbErrorType SetLineRouting (LineRoutingEnum value);
	VmbErrorType GetLineRoutingFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineSelector (LineSelectorEnum & value);
	VmbErrorType SetLineSelector (LineSelectorEnum value);
	VmbErrorType GetLineSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineSource (LineSourceEnum & value);
	VmbErrorType GetLineSourceFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLineStatus (bool & value);
	VmbErrorType GetLineStatusFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /DigitalIOControl/SerialPortControl
	VmbErrorType GetSerialPortBaudRate (SerialPortBaudRateEnum & value);
	VmbErrorType SetSerialPortBaudRate (SerialPortBaudRateEnum value);
	VmbErrorType GetSerialPortBaudRateFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortBuffer (AVT::VmbAPI::UcharVector & value);
	VmbErrorType SetSerialPortBuffer (const AVT::VmbAPI::UcharVector & value);
	VmbErrorType GetSerialPortBufferFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortCharLength (SerialPortCharLengthEnum & value);
	VmbErrorType SetSerialPortCharLength (SerialPortCharLengthEnum value);
	VmbErrorType GetSerialPortCharLengthFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortModeSelector (SerialPortModeSelectorEnum & value);
	VmbErrorType SetSerialPortModeSelector (SerialPortModeSelectorEnum value);
	VmbErrorType GetSerialPortModeSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortParity (SerialPortParityEnum & value);
	VmbErrorType SetSerialPortParity (SerialPortParityEnum value);
	VmbErrorType GetSerialPortParityFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortReceiveFramingError (bool & value);
	VmbErrorType GetSerialPortReceiveFramingErrorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortReceiveOverrunError (bool & value);
	VmbErrorType GetSerialPortReceiveOverrunErrorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortReceiveParityError (bool & value);
	VmbErrorType GetSerialPortReceiveParityErrorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortReceiveReady (bool & value);
	VmbErrorType GetSerialPortReceiveReadyFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortRemainingReceiveSize (VmbInt64_t & value);
	VmbErrorType GetSerialPortRemainingReceiveSizeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortSelector (SerialPortSelectorEnum & value);
	VmbErrorType SetSerialPortSelector (SerialPortSelectorEnum value);
	VmbErrorType GetSerialPortSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortStopBits (SerialPortStopBitsEnum & value);
	VmbErrorType SetSerialPortStopBits (SerialPortStopBitsEnum value);
	VmbErrorType GetSerialPortStopBitsFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortTransmitReady (bool & value);
	VmbErrorType GetSerialPortTransmitReadyFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortTransmitSize (VmbInt64_t & value);
	VmbErrorType GetSerialPortTransmitSizeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSerialPortValidReceiveSize (VmbInt64_t & value);
	VmbErrorType GetSerialPortValidReceiveSizeFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /FileAccessControl
	VmbErrorType GetFileAccessBuffer (AVT::VmbAPI::UcharVector & value);
	VmbErrorType SetFileAccessBuffer (const AVT::VmbAPI::UcharVector & value);
	VmbErrorType GetFileAccessBufferFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileAccessLength (VmbInt64_t & value);
	VmbErrorType SetFileAccessLength (VmbInt64_t value);
	VmbErrorType GetFileAccessLengthFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileAccessOffset (VmbInt64_t & value);
	VmbErrorType GetFileAccessOffsetFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileOpenMode (FileOpenModeEnum & value);
	VmbErrorType SetFileOpenMode (FileOpenModeEnum value);
	VmbErrorType GetFileOpenModeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType FileOperationExecute ();
	VmbErrorType GetFileOperationExecuteFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileOperationResult (VmbInt64_t & value);
	VmbErrorType GetFileOperationResultFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileOperationSelector (FileOperationSelectorEnum & value);
	VmbErrorType SetFileOperationSelector (FileOperationSelectorEnum value);
	VmbErrorType GetFileOperationSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileOperationStatus (FileOperationStatusEnum & value);
	VmbErrorType GetFileOperationStatusFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileSelector (FileSelectorEnum & value);
	VmbErrorType SetFileSelector (FileSelectorEnum value);
	VmbErrorType GetFileSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileSize (VmbInt64_t & value);
	VmbErrorType GetFileSizeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetFileStatus (FileStatusEnum & value);
	VmbErrorType GetFileStatusFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /ImageFormatControl
	VmbErrorType GetHeight (VmbInt64_t & value);
	VmbErrorType SetHeight (VmbInt64_t value);
	VmbErrorType GetHeightFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetHeightMax (VmbInt64_t & value);
	VmbErrorType GetHeightMaxFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCMode (IIDCModeEnum & value);
	VmbErrorType SetIIDCMode (IIDCModeEnum value);
	VmbErrorType GetIIDCModeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetOffsetX (VmbInt64_t & value);
	VmbErrorType SetOffsetX (VmbInt64_t value);
	VmbErrorType GetOffsetXFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetOffsetY (VmbInt64_t & value);
	VmbErrorType SetOffsetY (VmbInt64_t value);
	VmbErrorType GetOffsetYFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetPixelColorFilter (PixelColorFilterEnum & value);
	VmbErrorType GetPixelColorFilterFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetPixelColorFilterAuto (PixelColorFilterAutoEnum & value);
	VmbErrorType GetPixelColorFilterAutoFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetPixelFormat (PixelFormatEnum & value);
	VmbErrorType SetPixelFormat (PixelFormatEnum value);
	VmbErrorType GetPixelFormatFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSensorBits (VmbInt64_t & value);
	VmbErrorType GetSensorBitsFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSensorHeight (VmbInt64_t & value);
	VmbErrorType GetSensorHeightFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSensorTaps (SensorTapsEnum & value);
	VmbErrorType GetSensorTapsFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetSensorWidth (VmbInt64_t & value);
	VmbErrorType GetSensorWidthFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetTestImageSelector (TestImageSelectorEnum & value);
	VmbErrorType SetTestImageSelector (TestImageSelectorEnum value);
	VmbErrorType GetTestImageSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetWidth (VmbInt64_t & value);
	VmbErrorType SetWidth (VmbInt64_t value);
	VmbErrorType GetWidthFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetWidthMax (VmbInt64_t & value);
	VmbErrorType GetWidthMaxFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /LUTControl
	VmbErrorType GetLUTBitDepthIn (VmbInt64_t & value);
	VmbErrorType GetLUTBitDepthInFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLUTBitDepthOut (VmbInt64_t & value);
	VmbErrorType GetLUTBitDepthOutFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLUTCount (VmbInt64_t & value);
	VmbErrorType GetLUTCountFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLUTEnable (bool & value);
	VmbErrorType SetLUTEnable (bool value);
	VmbErrorType GetLUTEnableFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLUTSelector (LUTSelectorEnum & value);
	VmbErrorType SetLUTSelector (LUTSelectorEnum value);
	VmbErrorType GetLUTSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetLUTSizeBytes (VmbInt64_t & value);
	VmbErrorType GetLUTSizeBytesFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /TransportLayerControl
	VmbErrorType GetIIDCBusNumber (VmbInt64_t & value);
	VmbErrorType GetIIDCBusNumberFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCCameraAcceptDelay (VmbInt64_t & value);
	VmbErrorType SetIIDCCameraAcceptDelay (VmbInt64_t value);
	VmbErrorType GetIIDCCameraAcceptDelayFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCFreeBandwidth (VmbInt64_t & value);
	VmbErrorType GetIIDCFreeBandwidthFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCIsoChannel (VmbInt64_t & value);
	VmbErrorType GetIIDCIsoChannelFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCIsoChannelAuto (IIDCIsoChannelAutoEnum & value);
	VmbErrorType SetIIDCIsoChannelAuto (IIDCIsoChannelAutoEnum value);
	VmbErrorType GetIIDCIsoChannelAutoFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCPacketCount (VmbInt64_t & value);
	VmbErrorType GetIIDCPacketCountFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCPacketSize (VmbInt64_t & value);
	VmbErrorType SetIIDCPacketSize (VmbInt64_t value);
	VmbErrorType GetIIDCPacketSizeFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCPacketSizeAuto (IIDCPacketSizeAutoEnum & value);
	VmbErrorType SetIIDCPacketSizeAuto (IIDCPacketSizeAutoEnum value);
	VmbErrorType GetIIDCPacketSizeAutoFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetIIDCPhyspeed (IIDCPhyspeedEnum & value);
	VmbErrorType SetIIDCPhyspeed (IIDCPhyspeedEnum value);
	VmbErrorType GetIIDCPhyspeedFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetPayloadSize (VmbInt64_t & value);
	VmbErrorType GetPayloadSizeFeature (AVT::VmbAPI::FeaturePtr & feature);


	// Category /UserSetControl
	VmbErrorType UserSetLoad ();
	VmbErrorType GetUserSetLoadFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType UserSetMakeDefault ();
	VmbErrorType GetUserSetMakeDefaultFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetUserSetOperationResult (VmbInt64_t & value);
	VmbErrorType GetUserSetOperationResultFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetUserSetOperationStatus (UserSetOperationStatusEnum & value);
	VmbErrorType GetUserSetOperationStatusFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType UserSetSave ();
	VmbErrorType GetUserSetSaveFeature (AVT::VmbAPI::FeaturePtr & feature);

	VmbErrorType GetUserSetSelector (UserSetSelectorEnum & value);
	VmbErrorType SetUserSetSelector (UserSetSelectorEnum value);
	VmbErrorType GetUserSetSelectorFeature (AVT::VmbAPI::FeaturePtr & feature);


private:
	AVT::VmbAPI::FeaturePtr m_AcquisitionFrameCountFeature;
	AVT::VmbAPI::FeaturePtr m_AcquisitionFrameRateFeature;
	AVT::VmbAPI::FeaturePtr m_AcquisitionFrameRateLimitFeature;
	AVT::VmbAPI::FeaturePtr m_AcquisitionModeFeature;
	AVT::VmbAPI::FeaturePtr m_AcquisitionStartFeature;
	AVT::VmbAPI::FeaturePtr m_AcquisitionStopFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureAutoFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureModeFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureOffsetFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureTimeFeature;
	AVT::VmbAPI::FeaturePtr m_TriggerActivationFeature;
	AVT::VmbAPI::FeaturePtr m_TriggerDelayFeature;
	AVT::VmbAPI::FeaturePtr m_TriggerModeFeature;
	AVT::VmbAPI::FeaturePtr m_TriggerSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_TriggerSourceFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureAutoAlgFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureAutoMaxFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureAutoMinFeature;
	AVT::VmbAPI::FeaturePtr m_ExposureAutoTimebaseFeature;
	AVT::VmbAPI::FeaturePtr m_BlackLevelFeature;
	AVT::VmbAPI::FeaturePtr m_BlackLevelSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_GainFeature;
	AVT::VmbAPI::FeaturePtr m_GainAutoFeature;
	AVT::VmbAPI::FeaturePtr m_GainSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_GammaFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionAOIEnableFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionAOIHeightFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionAOIOffsetXFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionAOIOffsetYFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionAOIShowAreaFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionAOIWidthFeature;
	AVT::VmbAPI::FeaturePtr m_AutofunctionTargetIntensityFeature;
	AVT::VmbAPI::FeaturePtr m_DriverBuffersCountFeature;
	AVT::VmbAPI::FeaturePtr m_StreamAcquisitionModeSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_StreamAnnounceBufferMinimumFeature;
	AVT::VmbAPI::FeaturePtr m_StreamAnnouncedBufferCountFeature;
	AVT::VmbAPI::FeaturePtr m_StreamIDFeature;
	AVT::VmbAPI::FeaturePtr m_StreamIsGrabbingFeature;
	AVT::VmbAPI::FeaturePtr m_StreamTypeFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceAccessRegisterAddressFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceAccessRegisterValueFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceFirmwareVersionFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceIDFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceMicrocontrollerVersionFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceModelNameFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceSFNCVersionMajorFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceSFNCVersionMinorFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceSFNCVersionSubMinorFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceScanTypeFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceSerialNumberFeature;
	AVT::VmbAPI::FeaturePtr m_DeviceVendorNameFeature;
	AVT::VmbAPI::FeaturePtr m_FirmwareVerBuildFeature;
	AVT::VmbAPI::FeaturePtr m_FirmwareVerMajorFeature;
	AVT::VmbAPI::FeaturePtr m_FirmwareVerMinorFeature;
	AVT::VmbAPI::FeaturePtr m_IntEnaDelayEnableFeature;
	AVT::VmbAPI::FeaturePtr m_IntEnaDelayTimeFeature;
	AVT::VmbAPI::FeaturePtr m_LineFormatFeature;
	AVT::VmbAPI::FeaturePtr m_LineInverterFeature;
	AVT::VmbAPI::FeaturePtr m_LineModeFeature;
	AVT::VmbAPI::FeaturePtr m_LineRoutingFeature;
	AVT::VmbAPI::FeaturePtr m_LineSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_LineSourceFeature;
	AVT::VmbAPI::FeaturePtr m_LineStatusFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortBaudRateFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortBufferFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortCharLengthFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortModeSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortParityFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortReceiveFramingErrorFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortReceiveOverrunErrorFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortReceiveParityErrorFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortReceiveReadyFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortRemainingReceiveSizeFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortStopBitsFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortTransmitReadyFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortTransmitSizeFeature;
	AVT::VmbAPI::FeaturePtr m_SerialPortValidReceiveSizeFeature;
	AVT::VmbAPI::FeaturePtr m_FileAccessBufferFeature;
	AVT::VmbAPI::FeaturePtr m_FileAccessLengthFeature;
	AVT::VmbAPI::FeaturePtr m_FileAccessOffsetFeature;
	AVT::VmbAPI::FeaturePtr m_FileOpenModeFeature;
	AVT::VmbAPI::FeaturePtr m_FileOperationExecuteFeature;
	AVT::VmbAPI::FeaturePtr m_FileOperationResultFeature;
	AVT::VmbAPI::FeaturePtr m_FileOperationSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_FileOperationStatusFeature;
	AVT::VmbAPI::FeaturePtr m_FileSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_FileSizeFeature;
	AVT::VmbAPI::FeaturePtr m_FileStatusFeature;
	AVT::VmbAPI::FeaturePtr m_HeightFeature;
	AVT::VmbAPI::FeaturePtr m_HeightMaxFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCModeFeature;
	AVT::VmbAPI::FeaturePtr m_OffsetXFeature;
	AVT::VmbAPI::FeaturePtr m_OffsetYFeature;
	AVT::VmbAPI::FeaturePtr m_PixelColorFilterFeature;
	AVT::VmbAPI::FeaturePtr m_PixelColorFilterAutoFeature;
	AVT::VmbAPI::FeaturePtr m_PixelFormatFeature;
	AVT::VmbAPI::FeaturePtr m_SensorBitsFeature;
	AVT::VmbAPI::FeaturePtr m_SensorHeightFeature;
	AVT::VmbAPI::FeaturePtr m_SensorTapsFeature;
	AVT::VmbAPI::FeaturePtr m_SensorWidthFeature;
	AVT::VmbAPI::FeaturePtr m_TestImageSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_WidthFeature;
	AVT::VmbAPI::FeaturePtr m_WidthMaxFeature;
	AVT::VmbAPI::FeaturePtr m_LUTBitDepthInFeature;
	AVT::VmbAPI::FeaturePtr m_LUTBitDepthOutFeature;
	AVT::VmbAPI::FeaturePtr m_LUTCountFeature;
	AVT::VmbAPI::FeaturePtr m_LUTEnableFeature;
	AVT::VmbAPI::FeaturePtr m_LUTSelectorFeature;
	AVT::VmbAPI::FeaturePtr m_LUTSizeBytesFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCBusNumberFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCCameraAcceptDelayFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCFreeBandwidthFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCIsoChannelFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCIsoChannelAutoFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCPacketCountFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCPacketSizeFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCPacketSizeAutoFeature;
	AVT::VmbAPI::FeaturePtr m_IIDCPhyspeedFeature;
	AVT::VmbAPI::FeaturePtr m_PayloadSizeFeature;
	AVT::VmbAPI::FeaturePtr m_UserSetLoadFeature;
	AVT::VmbAPI::FeaturePtr m_UserSetMakeDefaultFeature;
	AVT::VmbAPI::FeaturePtr m_UserSetOperationResultFeature;
	AVT::VmbAPI::FeaturePtr m_UserSetOperationStatusFeature;
	AVT::VmbAPI::FeaturePtr m_UserSetSaveFeature;
	AVT::VmbAPI::FeaturePtr m_UserSetSelectorFeature;
};

#endif
