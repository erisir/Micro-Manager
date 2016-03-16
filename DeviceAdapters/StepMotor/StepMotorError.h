///////////////////////////////////////////////////////////////////////////////
// FILE:          StepMotorError.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   StepMotor Device Adapter Error Codes & Messages
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
// AUTHOR:        Lon Chu (lonchu@yahoo.com) created on June 2011
//

#ifndef _StepMotorERROR_H_
#define _StepMotorERROR_H_

#include <string>
#include <map>

#define C51DEVICE_OK 0x00
#define C51DEVICE_BUSY 0x00 +'A'
#define C51OUT_OF_LOW_LIMIT 0x00 +'B'
#define C51OUT_OF_HIGH_LIMIT 0x00 +'C'
#define C51CHECK_SUM_ERROR 0x00  +'D'
#define C51BAD_COMMAND	    0x00 +'E'

/*协议格式

@ 起始
U 命令 ,moveUP等
X
X
X
X 以上四位为命令参数，需要转码
X 校验位，使用异或校验
*/
//command string		以下
#define _SetZeroPosition 0x00 +'F'
#define _MoveUp	        0x00 +'G'
#define _MoveDown	    0x00 +'H'
#define _SetRunningDelay 0x00 +'I'
#define _SetStartDelay 	0x00 +'J'
#define _FindLimit		0x00 +'K'
#define _ReleasePower	0x00 +'L'
#define _QueryPosition   0x00 +'M'
#define _QueryStage   	0x00 +'N'
#define _SetPosition	    0x00 + 'O'
#define _SetUM2Step	    0x00 + 'P'
#define _GetUM2Step	    0x00 + 'Q'
#define _SetAngel2Step   0x00 + 'R'
#define _GetAngel2Step   0x00 + 'S'
#define _SetDivMode		0x00 + 'T'

#define _QueryAngel	    0x00 + 'U'
#define _SetZeroAngel    0x00 +'V'
#define _SetAngel        0x00 +'W'
class MPError
{
public:
    ~MPError();

    typedef int MPErr;
    enum _MPErr
    {
        MPERR_OK                        = 0,        // OK
        MPERR_SerialOverRun             = 1,        // Serial command buffer over run
        MPERR_SerialTimeout             = 2,        // Receiving serial command time out
        MPERR_SerialBufferFull          = 3,        // Serial command buffer full
        MPERR_SerialInpInvalid          = 4,        // Invalid serial command
        MPERR_SerialIntrupMove          = 5,        // Serial command interrupt motion
        MPERR_SerialZeroReturn          = 6,        // No response from serial port
        MPERR_SerialUnknownError        = 7,        // Unknown serial command
        MPERR_GENERIC                   = 8,        // Unspecified StepMotor adapter errors
        MPERR_FileOpenFailed            = 9,         // Fail to open file
        MPERR_OutOfLimit				= 10
    };

    static MPError* Instance();
    std::string GetErrorText(int nErrorCode) const;

private:
    MPError();
    static bool m_yInstanceFlag;
    static MPError* m_pMPError;
    std::map<int, std::string> m_sErrorText;        // error messages
};

#endif  // _StepMotorERROR_H_
