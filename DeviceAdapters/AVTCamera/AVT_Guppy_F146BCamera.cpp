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

#include "AVT_Guppy_F146BCamera.h"

AVT_Guppy_F146BCamera::AVT_Guppy_F146BCamera (CameraPtr Camera)
{
	m_pCamera = Camera;
}


// Category /AcquisitionControl
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionFrameCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAcquisitionFrameCount (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionFrameCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionFrameCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AcquisitionFrameCount", m_AcquisitionFrameCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionFrameCountFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionFrameCountFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionFrameRate (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAcquisitionFrameRate (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionFrameRateFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionFrameRateFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AcquisitionFrameRate", m_AcquisitionFrameRateFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionFrameRateFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionFrameRateFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionFrameRateLimit (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionFrameRateLimitFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionFrameRateLimitFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionFrameRateLimitFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AcquisitionFrameRateLimit", m_AcquisitionFrameRateLimitFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionFrameRateLimitFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionFrameRateLimitFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionMode (AcquisitionModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (AcquisitionModeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAcquisitionMode (AcquisitionModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AcquisitionMode", m_AcquisitionModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionModeFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::AcquisitionStart ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionStartFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionStartFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionStartFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AcquisitionStart", m_AcquisitionStartFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionStartFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionStartFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::AcquisitionStop ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAcquisitionStopFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAcquisitionStopFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AcquisitionStopFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AcquisitionStop", m_AcquisitionStopFeature);
        if (result != VmbErrorSuccess)
        {
            m_AcquisitionStopFeature.reset();
            return result;
        }
    }
    feature = m_AcquisitionStopFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureAuto (ExposureAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureAutoEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetExposureAuto (ExposureAutoEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureAuto", m_ExposureAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureMode (ExposureModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureModeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetExposureMode (ExposureModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureMode", m_ExposureModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureModeFeature.reset();
            return result;
        }
    }
    feature = m_ExposureModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureOffset (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureOffsetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureOffsetFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureOffsetFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureOffset", m_ExposureOffsetFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureOffsetFeature.reset();
            return result;
        }
    }
    feature = m_ExposureOffsetFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureTime (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureTimeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetExposureTime (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureTimeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureTimeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureTimeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureTimeAbs", m_ExposureTimeFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureTimeFeature.reset();
            return result;
        }
    }
    feature = m_ExposureTimeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetTriggerActivation (TriggerActivationEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerActivationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerActivationEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetTriggerActivation (TriggerActivationEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerActivationFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetTriggerActivationFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerActivationFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("TriggerActivation", m_TriggerActivationFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerActivationFeature.reset();
            return result;
        }
    }
    feature = m_TriggerActivationFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetTriggerDelay (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerDelayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetTriggerDelay (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerDelayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetTriggerDelayFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerDelayFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("TriggerDelay", m_TriggerDelayFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerDelayFeature.reset();
            return result;
        }
    }
    feature = m_TriggerDelayFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetTriggerMode (TriggerModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerModeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetTriggerMode (TriggerModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetTriggerModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("TriggerMode", m_TriggerModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerModeFeature.reset();
            return result;
        }
    }
    feature = m_TriggerModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetTriggerSelector (TriggerSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetTriggerSelector (TriggerSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetTriggerSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("TriggerSelector", m_TriggerSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerSelectorFeature.reset();
            return result;
        }
    }
    feature = m_TriggerSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetTriggerSource (TriggerSourceEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TriggerSourceEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetTriggerSource (TriggerSourceEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTriggerSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetTriggerSourceFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TriggerSourceFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("TriggerSource", m_TriggerSourceFeature);
        if (result != VmbErrorSuccess)
        {
            m_TriggerSourceFeature.reset();
            return result;
        }
    }
    feature = m_TriggerSourceFeature;
    return VmbErrorSuccess;
}


// Category /AcquisitionControl/ExposureAutoControl
VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoAlg (ExposureAutoAlgEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoAlgFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureAutoAlgEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoAlgFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoAlgFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureAutoAlg", m_ExposureAutoAlgFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoAlgFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoAlgFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoMax (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetExposureAutoMax (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureAutoMax", m_ExposureAutoMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoMaxFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoMaxFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoMin (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMinFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetExposureAutoMin (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoMinFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoMinFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoMinFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureAutoMin", m_ExposureAutoMinFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoMinFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoMinFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoTimebase (ExposureAutoTimebaseEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoTimebaseFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (ExposureAutoTimebaseEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetExposureAutoTimebase (ExposureAutoTimebaseEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetExposureAutoTimebaseFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetExposureAutoTimebaseFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_ExposureAutoTimebaseFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("ExposureAutoTimebase", m_ExposureAutoTimebaseFeature);
        if (result != VmbErrorSuccess)
        {
            m_ExposureAutoTimebaseFeature.reset();
            return result;
        }
    }
    feature = m_ExposureAutoTimebaseFeature;
    return VmbErrorSuccess;
}


// Category /AnalogControl
VmbErrorType AVT_Guppy_F146BCamera::GetBlackLevel (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBlackLevelFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetBlackLevel (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBlackLevelFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetBlackLevelFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_BlackLevelFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("BlackLevel", m_BlackLevelFeature);
        if (result != VmbErrorSuccess)
        {
            m_BlackLevelFeature.reset();
            return result;
        }
    }
    feature = m_BlackLevelFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetBlackLevelSelector (BlackLevelSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetBlackLevelSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (BlackLevelSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetBlackLevelSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_BlackLevelSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("BlackLevelSelector", m_BlackLevelSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_BlackLevelSelectorFeature.reset();
            return result;
        }
    }
    feature = m_BlackLevelSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetGain (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetGain (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetGainFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("Gain", m_GainFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainFeature.reset();
            return result;
        }
    }
    feature = m_GainFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetGainAuto (GainAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (GainAutoEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetGainAuto (GainAutoEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetGainAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("GainAuto", m_GainAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainAutoFeature.reset();
            return result;
        }
    }
    feature = m_GainAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetGainSelector (GainSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGainSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (GainSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetGainSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GainSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("GainSelector", m_GainSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_GainSelectorFeature.reset();
            return result;
        }
    }
    feature = m_GainSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetGamma (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGammaFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetGamma (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetGammaFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetGammaFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_GammaFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("Gamma", m_GammaFeature);
        if (result != VmbErrorSuccess)
        {
            m_GammaFeature.reset();
            return result;
        }
    }
    feature = m_GammaFeature;
    return VmbErrorSuccess;
}


// Category /AutofunctionControl
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIEnable (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionAOIEnable (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIEnableFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionAOIEnableFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionAOIEnable", m_AutofunctionAOIEnableFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionAOIEnableFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionAOIEnableFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIHeight (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionAOIHeight (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIHeightFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionAOIHeightFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionAOIHeight", m_AutofunctionAOIHeightFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionAOIHeightFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionAOIHeightFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIOffsetX (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIOffsetXFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionAOIOffsetX (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIOffsetXFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIOffsetXFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionAOIOffsetXFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionAOIOffsetX", m_AutofunctionAOIOffsetXFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionAOIOffsetXFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionAOIOffsetXFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIOffsetY (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIOffsetYFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionAOIOffsetY (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIOffsetYFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIOffsetYFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionAOIOffsetYFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionAOIOffsetY", m_AutofunctionAOIOffsetYFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionAOIOffsetYFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionAOIOffsetYFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIShowArea (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIShowAreaFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionAOIShowArea (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIShowAreaFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIShowAreaFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionAOIShowAreaFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionAOIShowArea", m_AutofunctionAOIShowAreaFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionAOIShowAreaFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionAOIShowAreaFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIWidth (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionAOIWidth (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionAOIWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionAOIWidthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionAOIWidthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionAOIWidth", m_AutofunctionAOIWidthFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionAOIWidthFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionAOIWidthFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionTargetIntensity (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionTargetIntensityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetAutofunctionTargetIntensity (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetAutofunctionTargetIntensityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetAutofunctionTargetIntensityFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_AutofunctionTargetIntensityFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("AutofunctionTargetIntensity", m_AutofunctionTargetIntensityFeature);
        if (result != VmbErrorSuccess)
        {
            m_AutofunctionTargetIntensityFeature.reset();
            return result;
        }
    }
    feature = m_AutofunctionTargetIntensityFeature;
    return VmbErrorSuccess;
}


// Category /DataStreamModule
VmbErrorType AVT_Guppy_F146BCamera::GetDriverBuffersCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDriverBuffersCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetDriverBuffersCount (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDriverBuffersCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDriverBuffersCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DriverBuffersCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DriverBuffersCount", m_DriverBuffersCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_DriverBuffersCountFeature.reset();
            return result;
        }
    }
    feature = m_DriverBuffersCountFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetStreamAcquisitionModeSelector (StreamAcquisitionModeSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamAcquisitionModeSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (StreamAcquisitionModeSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetStreamAcquisitionModeSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamAcquisitionModeSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("StreamAcquisitionModeSelector", m_StreamAcquisitionModeSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamAcquisitionModeSelectorFeature.reset();
            return result;
        }
    }
    feature = m_StreamAcquisitionModeSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetStreamAnnounceBufferMinimum (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamAnnounceBufferMinimumFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetStreamAnnounceBufferMinimumFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamAnnounceBufferMinimumFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("StreamAnnounceBufferMinimum", m_StreamAnnounceBufferMinimumFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamAnnounceBufferMinimumFeature.reset();
            return result;
        }
    }
    feature = m_StreamAnnounceBufferMinimumFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetStreamAnnouncedBufferCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamAnnouncedBufferCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetStreamAnnouncedBufferCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamAnnouncedBufferCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("StreamAnnouncedBufferCount", m_StreamAnnouncedBufferCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamAnnouncedBufferCountFeature.reset();
            return result;
        }
    }
    feature = m_StreamAnnouncedBufferCountFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetStreamID (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetStreamIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("StreamID", m_StreamIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamIDFeature.reset();
            return result;
        }
    }
    feature = m_StreamIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetStreamIsGrabbing (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamIsGrabbingFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetStreamIsGrabbingFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamIsGrabbingFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("StreamIsGrabbing", m_StreamIsGrabbingFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamIsGrabbingFeature.reset();
            return result;
        }
    }
    feature = m_StreamIsGrabbingFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetStreamType (StreamTypeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetStreamTypeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (StreamTypeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetStreamTypeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_StreamTypeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("StreamType", m_StreamTypeFeature);
        if (result != VmbErrorSuccess)
        {
            m_StreamTypeFeature.reset();
            return result;
        }
    }
    feature = m_StreamTypeFeature;
    return VmbErrorSuccess;
}


// Category /DeviceControl
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceAccessRegisterAddress (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceAccessRegisterAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetDeviceAccessRegisterAddress (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceAccessRegisterAddressFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceAccessRegisterAddressFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceAccessRegisterAddressFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceAccessRegisterAddress", m_DeviceAccessRegisterAddressFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceAccessRegisterAddressFeature.reset();
            return result;
        }
    }
    feature = m_DeviceAccessRegisterAddressFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceAccessRegisterValue (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceAccessRegisterValueFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetDeviceAccessRegisterValue (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceAccessRegisterValueFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceAccessRegisterValueFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceAccessRegisterValueFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceAccessRegisterValue", m_DeviceAccessRegisterValueFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceAccessRegisterValueFeature.reset();
            return result;
        }
    }
    feature = m_DeviceAccessRegisterValueFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceFirmwareVersion (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceFirmwareVersionFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceFirmwareVersionFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceFirmwareVersionFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceFirmwareVersion", m_DeviceFirmwareVersionFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceFirmwareVersionFeature.reset();
            return result;
        }
    }
    feature = m_DeviceFirmwareVersionFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceID (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceIDFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceIDFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceIDFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceID", m_DeviceIDFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceIDFeature.reset();
            return result;
        }
    }
    feature = m_DeviceIDFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceMicrocontrollerVersion (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceMicrocontrollerVersionFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceMicrocontrollerVersionFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceMicrocontrollerVersionFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceMicrocontrollerVersion", m_DeviceMicrocontrollerVersionFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceMicrocontrollerVersionFeature.reset();
            return result;
        }
    }
    feature = m_DeviceMicrocontrollerVersionFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceModelName (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceModelNameFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceModelNameFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceModelNameFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceModelName", m_DeviceModelNameFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceModelNameFeature.reset();
            return result;
        }
    }
    feature = m_DeviceModelNameFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSFNCVersionMajor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceSFNCVersionMajorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSFNCVersionMajorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceSFNCVersionMajorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceSFNCVersionMajor", m_DeviceSFNCVersionMajorFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceSFNCVersionMajorFeature.reset();
            return result;
        }
    }
    feature = m_DeviceSFNCVersionMajorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSFNCVersionMinor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceSFNCVersionMinorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSFNCVersionMinorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceSFNCVersionMinorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceSFNCVersionMinor", m_DeviceSFNCVersionMinorFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceSFNCVersionMinorFeature.reset();
            return result;
        }
    }
    feature = m_DeviceSFNCVersionMinorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSFNCVersionSubMinor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceSFNCVersionSubMinorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSFNCVersionSubMinorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceSFNCVersionSubMinorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceSFNCVersionSubMinor", m_DeviceSFNCVersionSubMinorFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceSFNCVersionSubMinorFeature.reset();
            return result;
        }
    }
    feature = m_DeviceSFNCVersionSubMinorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceScanType (DeviceScanTypeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceScanTypeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (DeviceScanTypeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceScanTypeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceScanTypeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceScanType", m_DeviceScanTypeFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceScanTypeFeature.reset();
            return result;
        }
    }
    feature = m_DeviceScanTypeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSerialNumber (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceSerialNumberFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceSerialNumberFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceSerialNumberFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceSerialNumber", m_DeviceSerialNumberFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceSerialNumberFeature.reset();
            return result;
        }
    }
    feature = m_DeviceSerialNumberFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetDeviceVendorName (std::string & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetDeviceVendorNameFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetDeviceVendorNameFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_DeviceVendorNameFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("DeviceVendorName", m_DeviceVendorNameFeature);
        if (result != VmbErrorSuccess)
        {
            m_DeviceVendorNameFeature.reset();
            return result;
        }
    }
    feature = m_DeviceVendorNameFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFirmwareVerBuild (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFirmwareVerBuildFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFirmwareVerBuildFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FirmwareVerBuildFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FirmwareVerBuild", m_FirmwareVerBuildFeature);
        if (result != VmbErrorSuccess)
        {
            m_FirmwareVerBuildFeature.reset();
            return result;
        }
    }
    feature = m_FirmwareVerBuildFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFirmwareVerMajor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFirmwareVerMajorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFirmwareVerMajorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FirmwareVerMajorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FirmwareVerMajor", m_FirmwareVerMajorFeature);
        if (result != VmbErrorSuccess)
        {
            m_FirmwareVerMajorFeature.reset();
            return result;
        }
    }
    feature = m_FirmwareVerMajorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFirmwareVerMinor (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFirmwareVerMinorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFirmwareVerMinorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FirmwareVerMinorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FirmwareVerMinor", m_FirmwareVerMinorFeature);
        if (result != VmbErrorSuccess)
        {
            m_FirmwareVerMinorFeature.reset();
            return result;
        }
    }
    feature = m_FirmwareVerMinorFeature;
    return VmbErrorSuccess;
}


// Category /DigitalIOControl
VmbErrorType AVT_Guppy_F146BCamera::GetIntEnaDelayEnable (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIntEnaDelayEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIntEnaDelayEnable (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIntEnaDelayEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIntEnaDelayEnableFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IntEnaDelayEnableFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IntEnaDelayEnable", m_IntEnaDelayEnableFeature);
        if (result != VmbErrorSuccess)
        {
            m_IntEnaDelayEnableFeature.reset();
            return result;
        }
    }
    feature = m_IntEnaDelayEnableFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIntEnaDelayTime (double & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIntEnaDelayTimeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIntEnaDelayTime (double value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIntEnaDelayTimeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIntEnaDelayTimeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IntEnaDelayTimeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IntEnaDelayTime", m_IntEnaDelayTimeFeature);
        if (result != VmbErrorSuccess)
        {
            m_IntEnaDelayTimeFeature.reset();
            return result;
        }
    }
    feature = m_IntEnaDelayTimeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineFormat (LineFormatEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineFormatFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (LineFormatEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineFormatFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineFormatFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineFormat", m_LineFormatFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineFormatFeature.reset();
            return result;
        }
    }
    feature = m_LineFormatFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineInverter (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineInverterFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetLineInverter (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineInverterFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineInverterFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineInverterFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineInverter", m_LineInverterFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineInverterFeature.reset();
            return result;
        }
    }
    feature = m_LineInverterFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineMode (LineModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (LineModeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineMode", m_LineModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineModeFeature.reset();
            return result;
        }
    }
    feature = m_LineModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineRouting (LineRoutingEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineRoutingFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (LineRoutingEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetLineRouting (LineRoutingEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineRoutingFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineRoutingFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineRoutingFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineRouting", m_LineRoutingFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineRoutingFeature.reset();
            return result;
        }
    }
    feature = m_LineRoutingFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineSelector (LineSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (LineSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetLineSelector (LineSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineSelector", m_LineSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineSelectorFeature.reset();
            return result;
        }
    }
    feature = m_LineSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineSource (LineSourceEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineSourceFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (LineSourceEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineSourceFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineSourceFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineSource", m_LineSourceFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineSourceFeature.reset();
            return result;
        }
    }
    feature = m_LineSourceFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLineStatus (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLineStatusFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLineStatusFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LineStatusFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LineStatus", m_LineStatusFeature);
        if (result != VmbErrorSuccess)
        {
            m_LineStatusFeature.reset();
            return result;
        }
    }
    feature = m_LineStatusFeature;
    return VmbErrorSuccess;
}


// Category /DigitalIOControl/SerialPortControl
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortBaudRate (SerialPortBaudRateEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortBaudRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SerialPortBaudRateEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortBaudRate (SerialPortBaudRateEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortBaudRateFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortBaudRateFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortBaudRateFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortBaudRate", m_SerialPortBaudRateFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortBaudRateFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortBaudRateFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortBuffer (AVT::VmbAPI::UcharVector & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortBufferFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortBuffer (const AVT::VmbAPI::UcharVector & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortBufferFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortBufferFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortBufferFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortBuffer", m_SerialPortBufferFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortBufferFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortBufferFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortCharLength (SerialPortCharLengthEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortCharLengthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SerialPortCharLengthEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortCharLength (SerialPortCharLengthEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortCharLengthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortCharLengthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortCharLengthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortCharLength", m_SerialPortCharLengthFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortCharLengthFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortCharLengthFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortModeSelector (SerialPortModeSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortModeSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SerialPortModeSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortModeSelector (SerialPortModeSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortModeSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortModeSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortModeSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortModeSelector", m_SerialPortModeSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortModeSelectorFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortModeSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortParity (SerialPortParityEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortParityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SerialPortParityEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortParity (SerialPortParityEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortParityFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortParityFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortParityFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortParity", m_SerialPortParityFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortParityFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortParityFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveFramingError (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortReceiveFramingErrorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveFramingErrorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortReceiveFramingErrorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortReceiveFramingError", m_SerialPortReceiveFramingErrorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortReceiveFramingErrorFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortReceiveFramingErrorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveOverrunError (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortReceiveOverrunErrorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveOverrunErrorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortReceiveOverrunErrorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortReceiveOverrunError", m_SerialPortReceiveOverrunErrorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortReceiveOverrunErrorFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortReceiveOverrunErrorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveParityError (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortReceiveParityErrorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveParityErrorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortReceiveParityErrorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortReceiveParityError", m_SerialPortReceiveParityErrorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortReceiveParityErrorFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortReceiveParityErrorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveReady (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortReceiveReadyFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortReceiveReadyFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortReceiveReadyFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortReceiveReady", m_SerialPortReceiveReadyFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortReceiveReadyFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortReceiveReadyFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortRemainingReceiveSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortRemainingReceiveSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortRemainingReceiveSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortRemainingReceiveSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortRemainingReceiveSize", m_SerialPortRemainingReceiveSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortRemainingReceiveSizeFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortRemainingReceiveSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortSelector (SerialPortSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SerialPortSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortSelector (SerialPortSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortSelector", m_SerialPortSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortSelectorFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortStopBits (SerialPortStopBitsEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortStopBitsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SerialPortStopBitsEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetSerialPortStopBits (SerialPortStopBitsEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortStopBitsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortStopBitsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortStopBitsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortStopBits", m_SerialPortStopBitsFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortStopBitsFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortStopBitsFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortTransmitReady (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortTransmitReadyFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortTransmitReadyFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortTransmitReadyFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortTransmitReady", m_SerialPortTransmitReadyFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortTransmitReadyFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortTransmitReadyFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortTransmitSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortTransmitSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortTransmitSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortTransmitSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortTransmitSize", m_SerialPortTransmitSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortTransmitSizeFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortTransmitSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortValidReceiveSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSerialPortValidReceiveSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSerialPortValidReceiveSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SerialPortValidReceiveSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SerialPortValidReceiveSize", m_SerialPortValidReceiveSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_SerialPortValidReceiveSizeFeature.reset();
            return result;
        }
    }
    feature = m_SerialPortValidReceiveSizeFeature;
    return VmbErrorSuccess;
}


// Category /FileAccessControl
VmbErrorType AVT_Guppy_F146BCamera::GetFileAccessBuffer (AVT::VmbAPI::UcharVector & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileAccessBufferFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetFileAccessBuffer (const AVT::VmbAPI::UcharVector & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileAccessBufferFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileAccessBufferFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileAccessBufferFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileAccessBuffer", m_FileAccessBufferFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileAccessBufferFeature.reset();
            return result;
        }
    }
    feature = m_FileAccessBufferFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileAccessLength (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileAccessLengthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetFileAccessLength (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileAccessLengthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileAccessLengthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileAccessLengthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileAccessLength", m_FileAccessLengthFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileAccessLengthFeature.reset();
            return result;
        }
    }
    feature = m_FileAccessLengthFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileAccessOffset (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileAccessOffsetFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileAccessOffsetFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileAccessOffsetFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileAccessOffset", m_FileAccessOffsetFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileAccessOffsetFeature.reset();
            return result;
        }
    }
    feature = m_FileAccessOffsetFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileOpenMode (FileOpenModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOpenModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (FileOpenModeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetFileOpenMode (FileOpenModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOpenModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileOpenModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileOpenModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileOpenMode", m_FileOpenModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileOpenModeFeature.reset();
            return result;
        }
    }
    feature = m_FileOpenModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::FileOperationExecute ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOperationExecuteFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationExecuteFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileOperationExecuteFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileOperationExecute", m_FileOperationExecuteFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileOperationExecuteFeature.reset();
            return result;
        }
    }
    feature = m_FileOperationExecuteFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationResult (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOperationResultFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationResultFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileOperationResultFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileOperationResult", m_FileOperationResultFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileOperationResultFeature.reset();
            return result;
        }
    }
    feature = m_FileOperationResultFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationSelector (FileOperationSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOperationSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (FileOperationSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetFileOperationSelector (FileOperationSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOperationSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileOperationSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileOperationSelector", m_FileOperationSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileOperationSelectorFeature.reset();
            return result;
        }
    }
    feature = m_FileOperationSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationStatus (FileOperationStatusEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileOperationStatusFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (FileOperationStatusEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileOperationStatusFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileOperationStatusFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileOperationStatus", m_FileOperationStatusFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileOperationStatusFeature.reset();
            return result;
        }
    }
    feature = m_FileOperationStatusFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileSelector (FileSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (FileSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetFileSelector (FileSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileSelector", m_FileSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileSelectorFeature.reset();
            return result;
        }
    }
    feature = m_FileSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileSize", m_FileSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileSizeFeature.reset();
            return result;
        }
    }
    feature = m_FileSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetFileStatus (FileStatusEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetFileStatusFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (FileStatusEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetFileStatusFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_FileStatusFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("FileStatus", m_FileStatusFeature);
        if (result != VmbErrorSuccess)
        {
            m_FileStatusFeature.reset();
            return result;
        }
    }
    feature = m_FileStatusFeature;
    return VmbErrorSuccess;
}


// Category /ImageFormatControl
VmbErrorType AVT_Guppy_F146BCamera::GetHeight (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetHeight (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetHeightFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_HeightFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("Height", m_HeightFeature);
        if (result != VmbErrorSuccess)
        {
            m_HeightFeature.reset();
            return result;
        }
    }
    feature = m_HeightFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetHeightMax (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetHeightMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetHeightMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_HeightMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("HeightMax", m_HeightMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_HeightMaxFeature.reset();
            return result;
        }
    }
    feature = m_HeightMaxFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCMode (IIDCModeEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (IIDCModeEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIIDCMode (IIDCModeEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCModeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCModeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCModeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCMode", m_IIDCModeFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCModeFeature.reset();
            return result;
        }
    }
    feature = m_IIDCModeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetOffsetX (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetXFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetOffsetX (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetXFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetOffsetXFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_OffsetXFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("OffsetX", m_OffsetXFeature);
        if (result != VmbErrorSuccess)
        {
            m_OffsetXFeature.reset();
            return result;
        }
    }
    feature = m_OffsetXFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetOffsetY (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetYFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetOffsetY (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetOffsetYFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetOffsetYFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_OffsetYFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("OffsetY", m_OffsetYFeature);
        if (result != VmbErrorSuccess)
        {
            m_OffsetYFeature.reset();
            return result;
        }
    }
    feature = m_OffsetYFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetPixelColorFilter (PixelColorFilterEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPixelColorFilterFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (PixelColorFilterEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetPixelColorFilterFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PixelColorFilterFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("PixelColorFilter", m_PixelColorFilterFeature);
        if (result != VmbErrorSuccess)
        {
            m_PixelColorFilterFeature.reset();
            return result;
        }
    }
    feature = m_PixelColorFilterFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetPixelColorFilterAuto (PixelColorFilterAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPixelColorFilterAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (PixelColorFilterAutoEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetPixelColorFilterAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PixelColorFilterAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("PixelColorFilterAuto", m_PixelColorFilterAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_PixelColorFilterAutoFeature.reset();
            return result;
        }
    }
    feature = m_PixelColorFilterAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetPixelFormat (PixelFormatEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPixelFormatFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (PixelFormatEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetPixelFormat (PixelFormatEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPixelFormatFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetPixelFormatFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PixelFormatFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("PixelFormat", m_PixelFormatFeature);
        if (result != VmbErrorSuccess)
        {
            m_PixelFormatFeature.reset();
            return result;
        }
    }
    feature = m_PixelFormatFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSensorBits (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorBitsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSensorBitsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorBitsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SensorBits", m_SensorBitsFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorBitsFeature.reset();
            return result;
        }
    }
    feature = m_SensorBitsFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSensorHeight (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorHeightFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSensorHeightFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorHeightFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SensorHeight", m_SensorHeightFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorHeightFeature.reset();
            return result;
        }
    }
    feature = m_SensorHeightFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSensorTaps (SensorTapsEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorTapsFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (SensorTapsEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSensorTapsFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorTapsFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SensorTaps", m_SensorTapsFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorTapsFeature.reset();
            return result;
        }
    }
    feature = m_SensorTapsFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetSensorWidth (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetSensorWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetSensorWidthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_SensorWidthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("SensorWidth", m_SensorWidthFeature);
        if (result != VmbErrorSuccess)
        {
            m_SensorWidthFeature.reset();
            return result;
        }
    }
    feature = m_SensorWidthFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetTestImageSelector (TestImageSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTestImageSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (TestImageSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetTestImageSelector (TestImageSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetTestImageSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetTestImageSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_TestImageSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("TestImageSelector", m_TestImageSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_TestImageSelectorFeature.reset();
            return result;
        }
    }
    feature = m_TestImageSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetWidth (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetWidth (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetWidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetWidthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_WidthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("Width", m_WidthFeature);
        if (result != VmbErrorSuccess)
        {
            m_WidthFeature.reset();
            return result;
        }
    }
    feature = m_WidthFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetWidthMax (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetWidthMaxFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetWidthMaxFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_WidthMaxFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("WidthMax", m_WidthMaxFeature);
        if (result != VmbErrorSuccess)
        {
            m_WidthMaxFeature.reset();
            return result;
        }
    }
    feature = m_WidthMaxFeature;
    return VmbErrorSuccess;
}


// Category /LUTControl
VmbErrorType AVT_Guppy_F146BCamera::GetLUTBitDepthIn (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTBitDepthInFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLUTBitDepthInFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LUTBitDepthInFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LUTBitDepthIn", m_LUTBitDepthInFeature);
        if (result != VmbErrorSuccess)
        {
            m_LUTBitDepthInFeature.reset();
            return result;
        }
    }
    feature = m_LUTBitDepthInFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLUTBitDepthOut (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTBitDepthOutFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLUTBitDepthOutFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LUTBitDepthOutFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LUTBitDepthOut", m_LUTBitDepthOutFeature);
        if (result != VmbErrorSuccess)
        {
            m_LUTBitDepthOutFeature.reset();
            return result;
        }
    }
    feature = m_LUTBitDepthOutFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLUTCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLUTCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LUTCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LUTCount", m_LUTCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_LUTCountFeature.reset();
            return result;
        }
    }
    feature = m_LUTCountFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLUTEnable (bool & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetLUTEnable (bool value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTEnableFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLUTEnableFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LUTEnableFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LUTEnable", m_LUTEnableFeature);
        if (result != VmbErrorSuccess)
        {
            m_LUTEnableFeature.reset();
            return result;
        }
    }
    feature = m_LUTEnableFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLUTSelector (LUTSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (LUTSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetLUTSelector (LUTSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLUTSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LUTSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LUTSelector", m_LUTSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_LUTSelectorFeature.reset();
            return result;
        }
    }
    feature = m_LUTSelectorFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetLUTSizeBytes (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetLUTSizeBytesFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetLUTSizeBytesFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_LUTSizeBytesFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("LUTSizeBytes", m_LUTSizeBytesFeature);
        if (result != VmbErrorSuccess)
        {
            m_LUTSizeBytesFeature.reset();
            return result;
        }
    }
    feature = m_LUTSizeBytesFeature;
    return VmbErrorSuccess;
}


// Category /TransportLayerControl
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCBusNumber (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCBusNumberFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCBusNumberFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCBusNumberFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCBusNumber", m_IIDCBusNumberFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCBusNumberFeature.reset();
            return result;
        }
    }
    feature = m_IIDCBusNumberFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCCameraAcceptDelay (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCCameraAcceptDelayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIIDCCameraAcceptDelay (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCCameraAcceptDelayFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCCameraAcceptDelayFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCCameraAcceptDelayFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCCameraAcceptDelay", m_IIDCCameraAcceptDelayFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCCameraAcceptDelayFeature.reset();
            return result;
        }
    }
    feature = m_IIDCCameraAcceptDelayFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCFreeBandwidth (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCFreeBandwidthFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCFreeBandwidthFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCFreeBandwidthFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCFreeBandwidth", m_IIDCFreeBandwidthFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCFreeBandwidthFeature.reset();
            return result;
        }
    }
    feature = m_IIDCFreeBandwidthFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCIsoChannel (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCIsoChannelFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCIsoChannelFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCIsoChannelFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCIsoChannel", m_IIDCIsoChannelFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCIsoChannelFeature.reset();
            return result;
        }
    }
    feature = m_IIDCIsoChannelFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCIsoChannelAuto (IIDCIsoChannelAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCIsoChannelAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (IIDCIsoChannelAutoEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIIDCIsoChannelAuto (IIDCIsoChannelAutoEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCIsoChannelAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCIsoChannelAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCIsoChannelAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCIsoChannelAuto", m_IIDCIsoChannelAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCIsoChannelAutoFeature.reset();
            return result;
        }
    }
    feature = m_IIDCIsoChannelAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPacketCount (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPacketCountFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPacketCountFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCPacketCountFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCPacketCount", m_IIDCPacketCountFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCPacketCountFeature.reset();
            return result;
        }
    }
    feature = m_IIDCPacketCountFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPacketSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIIDCPacketSize (VmbInt64_t value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPacketSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPacketSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCPacketSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCPacketSize", m_IIDCPacketSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCPacketSizeFeature.reset();
            return result;
        }
    }
    feature = m_IIDCPacketSizeFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPacketSizeAuto (IIDCPacketSizeAutoEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPacketSizeAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (IIDCPacketSizeAutoEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIIDCPacketSizeAuto (IIDCPacketSizeAutoEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPacketSizeAutoFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPacketSizeAutoFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCPacketSizeAutoFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCPacketSizeAuto", m_IIDCPacketSizeAutoFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCPacketSizeAutoFeature.reset();
            return result;
        }
    }
    feature = m_IIDCPacketSizeAutoFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPhyspeed (IIDCPhyspeedEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPhyspeedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (IIDCPhyspeedEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetIIDCPhyspeed (IIDCPhyspeedEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetIIDCPhyspeedFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetIIDCPhyspeedFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_IIDCPhyspeedFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("IIDCPhyspeed", m_IIDCPhyspeedFeature);
        if (result != VmbErrorSuccess)
        {
            m_IIDCPhyspeedFeature.reset();
            return result;
        }
    }
    feature = m_IIDCPhyspeedFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetPayloadSize (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetPayloadSizeFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetPayloadSizeFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_PayloadSizeFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("PayloadSize", m_PayloadSizeFeature);
        if (result != VmbErrorSuccess)
        {
            m_PayloadSizeFeature.reset();
            return result;
        }
    }
    feature = m_PayloadSizeFeature;
    return VmbErrorSuccess;
}


// Category /UserSetControl
VmbErrorType AVT_Guppy_F146BCamera::UserSetLoad ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetLoadFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetUserSetLoadFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetLoadFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("UserSetLoad", m_UserSetLoadFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetLoadFeature.reset();
            return result;
        }
    }
    feature = m_UserSetLoadFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::UserSetMakeDefault ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetMakeDefaultFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetUserSetMakeDefaultFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetMakeDefaultFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("UserSetMakeDefault", m_UserSetMakeDefaultFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetMakeDefaultFeature.reset();
            return result;
        }
    }
    feature = m_UserSetMakeDefaultFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetUserSetOperationResult (VmbInt64_t & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetOperationResultFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->GetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetUserSetOperationResultFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetOperationResultFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("UserSetOperationResult", m_UserSetOperationResultFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetOperationResultFeature.reset();
            return result;
        }
    }
    feature = m_UserSetOperationResultFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetUserSetOperationStatus (UserSetOperationStatusEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetOperationStatusFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (UserSetOperationStatusEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetUserSetOperationStatusFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetOperationStatusFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("UserSetOperationStatus", m_UserSetOperationStatusFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetOperationStatusFeature.reset();
            return result;
        }
    }
    feature = m_UserSetOperationStatusFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::UserSetSave ()
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetSaveFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->RunCommand ();
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetUserSetSaveFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetSaveFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("UserSetSave", m_UserSetSaveFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetSaveFeature.reset();
            return result;
        }
    }
    feature = m_UserSetSaveFeature;
    return VmbErrorSuccess;
}

VmbErrorType AVT_Guppy_F146BCamera::GetUserSetSelector (UserSetSelectorEnum & value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    VmbInt64_t nValue;
    result = pFeature->GetValue (nValue);
    value = (UserSetSelectorEnum) nValue;
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::SetUserSetSelector (UserSetSelectorEnum value)
{
    VmbErrorType result;
    AVT::VmbAPI::FeaturePtr pFeature;
    result = GetUserSetSelectorFeature (pFeature);
    if (result != VmbErrorSuccess)
        return result;
    result = pFeature->SetValue (value);
    return result;
}
VmbErrorType AVT_Guppy_F146BCamera::GetUserSetSelectorFeature (AVT::VmbAPI::FeaturePtr & feature)
{
    if (m_UserSetSelectorFeature.get() == NULL)
    {
        VmbErrorType result;
        result = m_pCamera->GetFeatureByName ("UserSetSelector", m_UserSetSelectorFeature);
        if (result != VmbErrorSuccess)
        {
            m_UserSetSelectorFeature.reset();
            return result;
        }
    }
    feature = m_UserSetSelectorFeature;
    return VmbErrorSuccess;
}

