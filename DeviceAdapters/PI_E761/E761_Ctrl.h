/*
 * E761_Ctrl.h
 *
 *  Created on: Nov 9, 2012
 *      Author: Zephyre
 */

#ifndef E761_CTRL_H_
#define E761_CTRL_H_

#include "../../MMDevice/DeviceBase.h"
#include "../../MMDevice/ImgBuffer.h"
#include "../../MMDevice/DeviceThreads.h"

//////
// PI_E761_Control class
/////
class E761_Ctrl: public CGenericBase<E761_Ctrl> {
public:
	enum _MPStr {
		STR_CtrlDevName = 0,
		STR_XYStageDevName,
		STR_ZStageDevName,
		STR_Version,
		STR_PROP_NAME,
		STR_PROP_DESC,
		STR_PROP_BOARDID,
		STR_XYStageDesc,
		STR_ZStageDesc,
		STR_CtrlDesc,
		STR_PROP_SERVO
	};

	static E761_Ctrl* getInstance();
	void GetName(char* name) const;
	static std::string getConstString(int strCode);
	int Initialize();
	bool isInitialized() {
		return m_initialized;
	}
	bool debugLogFlag() {
		return m_debugLogFlag;
	}
	bool Busy() {
		return false;
	}
	int Shutdown();

	int OnBoardId(MM::PropertyBase* pProp, MM::ActionType eAct);

	static int initConstStrings();
	int getDeviceId() {
		return m_devId;
	}
	void getAxisName(char* px, char* py, char* pz);

protected:
	virtual ~E761_Ctrl();

private:
	bool m_initialized;			// controller initialized flag
	bool m_debugLogFlag;			// Whether to log debug information
	long m_boardId;	//	PI_E761 board ID for initialization
	int m_devId;
	static E761_Ctrl* m_pInstance;
	static std::map<int, std::string> m_strMap;
	char m_axisNames[32];

	E761_Ctrl();
};

//////
// PI_E761_Control class
/////
class E761_XYStage: public CXYStageBase<E761_XYStage> {
public:
	static E761_XYStage* getInstance();
	int Initialize();
	bool Busy() {
		return false;
	}
	int Shutdown();
	void GetName(char* name) const;
	int Home() {
		return DEVICE_OK;
	}
	int SetOrigin() {
		return DEVICE_OK;
	}
	int Stop() {
		return DEVICE_OK;
	}
	int IsXYStageSequenceable(bool& seq) const {
		seq = false;
		return DEVICE_OK;
	}
	int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax) {
		xMin = 0;
		xMax = 100;
		yMin = 0;
		yMax = 100;
		return DEVICE_OK;
	}
	int GetStepLimits(long& /*xMin*/, long& /*xMax*/, long& /*yMin*/,
			long& /*yMax*/) {
		return DEVICE_OK/*DEVICE_UNSUPPORTED_COMMAND*/;
	}
	double GetStepSizeXUm() {
		return 0.001;
	}
	double GetStepSizeYUm() {
		return 0.001;
	}
	int SetPositionSteps(long lXPosSteps, long lYPosSteps);
	int GetPositionSteps(long& x, long& y);

protected:
	~E761_XYStage();

private:
	bool m_initialized;			// controller initialized flag
	static E761_XYStage* m_pInstance;

	E761_XYStage();
};

//////
// PI_E761_Control class
/////
class E761_ZStage: public CStageBase<E761_ZStage> {
public:
	static E761_ZStage* getInstance();
	bool Busy() {
		return false;
	}
	int Initialize();
	int Shutdown();
	void GetName(char* name) const;
	int SetPositionUm(double pos);
	int GetPositionUm(double& pos);
	int SetPositionSteps(long steps);
	int GetPositionSteps(long& steps);
	int SetOrigin() {
		return DEVICE_OK;
	}
	int GetLimits(double& lower, double& upper) {
		lower = 0;
		upper = 10;
		return DEVICE_OK;
	}
	int IsStageSequenceable(bool& seq) const {
		seq = false;
		return DEVICE_OK;
	}
	bool IsContinuousFocusDrive() const {
		return true;
	}
	int OnServoMode(MM::PropertyBase* pProp, MM::ActionType eAct);

protected:
	~E761_ZStage();

private:
	bool m_initialized;			// controller initialized flag
	static E761_ZStage* m_pInstance;
	double stepSizeUm;
	bool m_servoMode;

	E761_ZStage();
};
#endif /* E761_CTRL_H_ */
