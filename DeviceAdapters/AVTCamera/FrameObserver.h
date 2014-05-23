﻿/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        FrameObserver.h

  Description: The frame observer that is used for notifications from VimbaCPP
               regarding the arrival of a newly acquired frame.

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

#ifndef AVT_VMBAPI_EXAMPLES_FRAMEOBSERVER
#define AVT_VMBAPI_EXAMPLES_FRAMEOBSERVER

#include <queue>
#include <VimbaCPP/Include/VimbaCPP.h>

namespace AVT {
namespace VmbAPI {

#define WM_FRAME_READY WM_USER + 1

class FrameObserver : virtual public IFrameObserver
{
public:
	// We pass the camera that will deliver the frames to the constructor
	FrameObserver(  CameraPtr pCamera ) : IFrameObserver( pCamera ) {
	}
//	AVTCamera *cam_ ;
	// This is our callback routine that will be executed on every received frame
	virtual void FrameReceived( const FramePtr pFrame );
	// After the view has been notified about a new frame it can pick it up
	FramePtr GetFrame();

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

}} // namespace AVT::VmbAPI::Examples

#endif
