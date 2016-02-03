//
// Copyright 2014, 2015 Razer Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include "IForceFeedbackSystem.h"
#include "IInputDevice.h"
#include "IMotionController.h"
#include "IOSVR.h"
#include "OSVRTypes.h"

#include <osvr/ClientKit/InterfaceC.h>

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <functional>

DECLARE_LOG_CATEGORY_EXTERN(LogOSVRInputDevice, Log, All);

class OSVRButton;

/**
*
*/
class FOSVRInputDevice : public IInputDevice, public IMotionController
{
public:
	FOSVRInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& MessageHandler);
    virtual ~FOSVRInputDevice();
	static void RegisterNewKeys();

    /* IMotionController interface*/

    /**
     * Returns the calibration-space orientation of the requested controller's hand.
     *
     * @param ControllerIndex	The Unreal controller (player) index of the contoller set
     * @param DeviceHand		Which hand, within the controller set for the player, to get the orientation and position for
     * @param OutOrientation	(out) If tracked, the orientation (in calibrated-space) of the controller in the specified hand
     * @param OutPosition		(out) If tracked, the position (in calibrated-space) of the controller in the specified hand
     * @return					True if the device requested is valid and tracked, false otherwise
     */
    virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition) const override;

	/** Tick the interface (e.g. check for new controllers) */
	virtual void Tick(float DeltaTime) override;

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents() override;

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	// IForceFeedbackSystem pass through functions
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values) override;

	void EventReport(const FKey& Key, const FVector& Translation, const FQuat& Orientation);

private:
    std::map<std::string, OSVR_ClientInterface> interfaces;
    std::vector<OSVRButton> osvrButtons;
    OSVR_ClientContext context;
	TSharedRef< FGenericApplicationMessageHandler > MessageHandler;
    OSVR_ClientInterface leftHand;
    OSVR_ClientInterface rightHand;
};