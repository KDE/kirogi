/*
 * Copyright 2019 Eike Hein <hein@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "parrotprotocol.h"
#include "debug.h"

#include <QDataStream>

QHash<ParrotCommand::Tuple, Parrot::Command> initCommandForTuple()
{
    return {
        {ParrotCommand::Tuple(0, 1, 0), Parrot::CommonNetworkEventDisconnection},
        {ParrotCommand::Tuple(0, 2, 0), Parrot::CommonSettingsAllSettings},
        {ParrotCommand::Tuple(0, 2, 1), Parrot::CommonSettingsReset},
        {ParrotCommand::Tuple(0, 2, 2), Parrot::CommonSettingsProductName},
        {ParrotCommand::Tuple(0, 2, 3), Parrot::CommonSettingsCountry},
        {ParrotCommand::Tuple(0, 2, 4), Parrot::CommonSettingsAutoCountry},
        {ParrotCommand::Tuple(0, 3, 0), Parrot::CommonSettingsStateAllSettingsChanged},
        {ParrotCommand::Tuple(0, 3, 1), Parrot::CommonSettingsStateResetChanged},
        {ParrotCommand::Tuple(0, 3, 2), Parrot::CommonSettingsStateProductNameChanged},
        {ParrotCommand::Tuple(0, 3, 3), Parrot::CommonSettingsStateProductVersionChanged},
        {ParrotCommand::Tuple(0, 3, 4), Parrot::CommonSettingsStateProductSerialHighChanged},
        {ParrotCommand::Tuple(0, 3, 5), Parrot::CommonSettingsStateProductSerialLowChanged},
        {ParrotCommand::Tuple(0, 3, 6), Parrot::CommonSettingsStateCountryChanged},
        {ParrotCommand::Tuple(0, 3, 7), Parrot::CommonSettingsStateAutoCountryChanged},
        {ParrotCommand::Tuple(0, 4, 0), Parrot::CommonCommonAllStates},
        {ParrotCommand::Tuple(0, 4, 1), Parrot::CommonCommonCurrentDate},
        {ParrotCommand::Tuple(0, 4, 2), Parrot::CommonCommonCurrentTime},
        {ParrotCommand::Tuple(0, 4, 3), Parrot::CommonCommonReboot},
        {ParrotCommand::Tuple(0, 5, 0), Parrot::CommonCommonStateAllStatesChanged},
        {ParrotCommand::Tuple(0, 5, 1), Parrot::CommonCommonStateBatteryStateChanged},
        {ParrotCommand::Tuple(0, 5, 2), Parrot::CommonCommonStateMassStorageStateListChanged},
        {ParrotCommand::Tuple(0, 5, 3), Parrot::CommonCommonStateMassStorageInfoStateListChanged},
        {ParrotCommand::Tuple(0, 5, 4), Parrot::CommonCommonStateCurrentDateChanged},
        {ParrotCommand::Tuple(0, 5, 5), Parrot::CommonCommonStateCurrentTimeChanged},
        {ParrotCommand::Tuple(0, 5, 7), Parrot::CommonCommonStateWifiSignalChanged},
        {ParrotCommand::Tuple(0, 5, 8), Parrot::CommonCommonStateSensorsStatesListChanged},
        {ParrotCommand::Tuple(0, 5, 12), Parrot::CommonCommonStateMassStorageContent},
        {ParrotCommand::Tuple(0, 5, 13), Parrot::CommonCommonStateMassStorageContentForCurrentRun},
        {ParrotCommand::Tuple(0, 5, 14), Parrot::CommonCommonStateVideoRecordingTimestamp},
        {ParrotCommand::Tuple(0, 7, 1), Parrot::CommonOverHeatStateOverHeatRegulationChanged},
        {ParrotCommand::Tuple(0, 8, 0), Parrot::CommonControllerisPiloting},
        {ParrotCommand::Tuple(0, 9, 0), Parrot::CommonWifiSettingsOutdoorSetting},
        {ParrotCommand::Tuple(0, 10, 0), Parrot::CommonWifiSettingsStateoutdoorSettingsChanged},
        {ParrotCommand::Tuple(0, 11, 0), Parrot::CommonMavlinkStart},
        {ParrotCommand::Tuple(0, 11, 1), Parrot::CommonMavlinkPause},
        {ParrotCommand::Tuple(0, 11, 2), Parrot::CommonMavlinkStop},
        {ParrotCommand::Tuple(0, 12, 0), Parrot::CommonMavlinkStateMavlinkFilePlayingStateChanged},
        {ParrotCommand::Tuple(0, 12, 2), Parrot::CommonMavlinkStateMissionItemExecuted},
        {ParrotCommand::Tuple(0, 32, 0), Parrot::CommonFlightPlanSettingsReturnHomeOnDisconnect},
        {ParrotCommand::Tuple(0, 33, 0), Parrot::CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged},
        {ParrotCommand::Tuple(0, 13, 0), Parrot::CommonCalibrationMagnetoCalibration},
        {ParrotCommand::Tuple(0, 14, 0), Parrot::CommonCalibrationStateMagnetoCalibrationStateChanged},
        {ParrotCommand::Tuple(0, 14, 1), Parrot::CommonCalibrationStateMagnetoCalibrationRequiredState},
        {ParrotCommand::Tuple(0, 14, 2), Parrot::CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged},
        {ParrotCommand::Tuple(0, 14, 3), Parrot::CommonCalibrationStateMagnetoCalibrationStartedChanged},
        {ParrotCommand::Tuple(0, 14, 4), Parrot::CommonCalibrationStatePitotCalibrationStateChanged},
        {ParrotCommand::Tuple(0, 15, 0), Parrot::CommonCameraSettingsStateCameraSettingsChanged},
        {ParrotCommand::Tuple(0, 17, 0), Parrot::CommonFlightPlanStateAvailabilityStateChanged},
        {ParrotCommand::Tuple(0, 17, 1), Parrot::CommonFlightPlanStateComponentStateListChanged},
        {ParrotCommand::Tuple(0, 17, 2), Parrot::CommonFlightPlanStateLockStateChanged},
        {ParrotCommand::Tuple(0, 19, 0), Parrot::CommonFlightPlanEventStartingErrorEvent},
        {ParrotCommand::Tuple(0, 19, 1), Parrot::CommonFlightPlanEventSpeedBridleEvent},
        {ParrotCommand::Tuple(0, 18, 0), Parrot::CommonARLibsVersionsStateControllerLibARCommandsVersion},
        {ParrotCommand::Tuple(0, 18, 1), Parrot::CommonARLibsVersionsStateSkyControllerLibARCommandsVersion},
        {ParrotCommand::Tuple(0, 18, 2), Parrot::CommonARLibsVersionsStateDeviceLibARCommandsVersion},
        {ParrotCommand::Tuple(0, 30, 0), Parrot::CommonRunStateRunIdChanged},
        {ParrotCommand::Tuple(0, 31, 0), Parrot::CommonFactoryReset},
        {ParrotCommand::Tuple(1, 0, 0), Parrot::Ardrone3PilotingFlatTrim},
        {ParrotCommand::Tuple(1, 0, 1), Parrot::Ardrone3PilotingTakeOff},
        {ParrotCommand::Tuple(1, 0, 2), Parrot::Ardrone3PilotingPCMD},
        {ParrotCommand::Tuple(1, 0, 3), Parrot::Ardrone3PilotingLanding},
        {ParrotCommand::Tuple(1, 0, 4), Parrot::Ardrone3PilotingEmergency},
        {ParrotCommand::Tuple(1, 0, 5), Parrot::Ardrone3PilotingNavigateHome},
        {ParrotCommand::Tuple(1, 0, 7), Parrot::Ardrone3PilotingmoveBy},
        {ParrotCommand::Tuple(1, 0, 8), Parrot::Ardrone3PilotingUserTakeOff},
        {ParrotCommand::Tuple(1, 0, 10), Parrot::Ardrone3PilotingmoveTo},
        {ParrotCommand::Tuple(1, 0, 11), Parrot::Ardrone3PilotingCancelMoveTo},
        {ParrotCommand::Tuple(1, 0, 12), Parrot::Ardrone3PilotingStartPilotedPOI},
        {ParrotCommand::Tuple(1, 0, 13), Parrot::Ardrone3PilotingStopPilotedPOI},
        {ParrotCommand::Tuple(1, 5, 0), Parrot::Ardrone3AnimationsFlip},
        {ParrotCommand::Tuple(1, 1, 1), Parrot::Ardrone3CameraOrientationV2},
        {ParrotCommand::Tuple(1, 1, 2), Parrot::Ardrone3CameraVelocity},
        {ParrotCommand::Tuple(1, 7, 2), Parrot::Ardrone3MediaRecordPictureV2},
        {ParrotCommand::Tuple(1, 7, 3), Parrot::Ardrone3MediaRecordVideoV2},
        {ParrotCommand::Tuple(1, 8, 2), Parrot::Ardrone3MediaRecordStatePictureStateChangedV2},
        {ParrotCommand::Tuple(1, 8, 3), Parrot::Ardrone3MediaRecordStateVideoStateChangedV2},
        {ParrotCommand::Tuple(1, 3, 0), Parrot::Ardrone3MediaRecordEventPictureEventChanged},
        {ParrotCommand::Tuple(1, 3, 1), Parrot::Ardrone3MediaRecordEventVideoEventChanged},
        {ParrotCommand::Tuple(1, 4, 0), Parrot::Ardrone3PilotingStateFlatTrimChanged},
        {ParrotCommand::Tuple(1, 4, 1), Parrot::Ardrone3PilotingStateFlyingStateChanged},
        {ParrotCommand::Tuple(1, 4, 2), Parrot::Ardrone3PilotingStateAlertStateChanged},
        {ParrotCommand::Tuple(1, 4, 3), Parrot::Ardrone3PilotingStateNavigateHomeStateChanged},
        {ParrotCommand::Tuple(1, 4, 4), Parrot::Ardrone3PilotingStatePositionChanged},
        {ParrotCommand::Tuple(1, 4, 5), Parrot::Ardrone3PilotingStateSpeedChanged},
        {ParrotCommand::Tuple(1, 4, 6), Parrot::Ardrone3PilotingStateAttitudeChanged},
        {ParrotCommand::Tuple(1, 4, 8), Parrot::Ardrone3PilotingStateAltitudeChanged},
        {ParrotCommand::Tuple(1, 4, 9), Parrot::Ardrone3PilotingStateGpsLocationChanged},
        {ParrotCommand::Tuple(1, 4, 12), Parrot::Ardrone3PilotingStatemoveToChanged},
        {ParrotCommand::Tuple(1, 4, 13), Parrot::Ardrone3PilotingStateMotionState},
        {ParrotCommand::Tuple(1, 4, 14), Parrot::Ardrone3PilotingStatePilotedPOI},
        {ParrotCommand::Tuple(1, 4, 15), Parrot::Ardrone3PilotingStateReturnHomeBatteryCapacity},
        {ParrotCommand::Tuple(1, 34, 0), Parrot::Ardrone3PilotingEventmoveByEnd},
        {ParrotCommand::Tuple(1, 13, 0), Parrot::Ardrone3NetworkWifiScan},
        {ParrotCommand::Tuple(1, 13, 1), Parrot::Ardrone3NetworkWifiAuthChannel},
        {ParrotCommand::Tuple(1, 14, 0), Parrot::Ardrone3NetworkStateWifiScanListChanged},
        {ParrotCommand::Tuple(1, 14, 1), Parrot::Ardrone3NetworkStateAllWifiScanChanged},
        {ParrotCommand::Tuple(1, 14, 2), Parrot::Ardrone3NetworkStateWifiAuthChannelListChanged},
        {ParrotCommand::Tuple(1, 14, 3), Parrot::Ardrone3NetworkStateAllWifiAuthChannelChanged},
        {ParrotCommand::Tuple(1, 2, 0), Parrot::Ardrone3PilotingSettingsMaxAltitude},
        {ParrotCommand::Tuple(1, 2, 1), Parrot::Ardrone3PilotingSettingsMaxTilt},
        {ParrotCommand::Tuple(1, 2, 3), Parrot::Ardrone3PilotingSettingsMaxDistance},
        {ParrotCommand::Tuple(1, 2, 4), Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance},
        {ParrotCommand::Tuple(1, 2, 5), Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed},
        {ParrotCommand::Tuple(1, 2, 6), Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed},
        {ParrotCommand::Tuple(1, 2, 7), Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration},
        {ParrotCommand::Tuple(1, 2, 8), Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration},
        {ParrotCommand::Tuple(1, 2, 9), Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed},
        {ParrotCommand::Tuple(1, 2, 10), Parrot::Ardrone3PilotingSettingsBankedTurn},
        {ParrotCommand::Tuple(1, 2, 16), Parrot::Ardrone3PilotingSettingsSetMotionDetectionMode},
        {ParrotCommand::Tuple(1, 6, 0), Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged},
        {ParrotCommand::Tuple(1, 6, 1), Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged},
        {ParrotCommand::Tuple(1, 6, 3), Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged},
        {ParrotCommand::Tuple(1, 6, 4), Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged},
        {ParrotCommand::Tuple(1, 6, 5), Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed},
        {ParrotCommand::Tuple(1, 6, 6), Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed},
        {ParrotCommand::Tuple(1, 6, 7), Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration},
        {ParrotCommand::Tuple(1, 6, 8), Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration},
        {ParrotCommand::Tuple(1, 6, 9), Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed},
        {ParrotCommand::Tuple(1, 6, 10), Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged},
        {ParrotCommand::Tuple(1, 6, 16), Parrot::Ardrone3PilotingSettingsStateMotionDetection},
        {ParrotCommand::Tuple(1, 11, 0), Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed},
        {ParrotCommand::Tuple(1, 11, 1), Parrot::Ardrone3SpeedSettingsMaxRotationSpeed},
        {ParrotCommand::Tuple(1, 11, 2), Parrot::Ardrone3SpeedSettingsHullProtection},
        {ParrotCommand::Tuple(1, 11, 4), Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed},
        {ParrotCommand::Tuple(1, 12, 0), Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged},
        {ParrotCommand::Tuple(1, 12, 1), Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged},
        {ParrotCommand::Tuple(1, 12, 2), Parrot::Ardrone3SpeedSettingsStateHullProtectionChanged},
        {ParrotCommand::Tuple(1, 12, 4), Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged},
        {ParrotCommand::Tuple(1, 9, 0), Parrot::Ardrone3NetworkSettingsWifiSelection},
        {ParrotCommand::Tuple(1, 9, 1), Parrot::Ardrone3NetworkSettingswifiSecurity},
        {ParrotCommand::Tuple(1, 10, 0), Parrot::Ardrone3NetworkSettingsStateWifiSelectionChanged},
        {ParrotCommand::Tuple(1, 10, 2), Parrot::Ardrone3NetworkSettingsStatewifiSecurity},
        {ParrotCommand::Tuple(1, 16, 1), Parrot::Ardrone3SettingsStateProductGPSVersionChanged},
        {ParrotCommand::Tuple(1, 16, 2), Parrot::Ardrone3SettingsStateMotorErrorStateChanged},
        {ParrotCommand::Tuple(1, 16, 4), Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged},
        {ParrotCommand::Tuple(1, 16, 5), Parrot::Ardrone3SettingsStateMotorErrorLastErrorChanged},
        {ParrotCommand::Tuple(1, 16, 7), Parrot::Ardrone3SettingsStateCPUID},
        {ParrotCommand::Tuple(1, 19, 0), Parrot::Ardrone3PictureSettingsPictureFormatSelection},
        {ParrotCommand::Tuple(1, 19, 1), Parrot::Ardrone3PictureSettingsAutoWhiteBalanceSelection},
        {ParrotCommand::Tuple(1, 19, 2), Parrot::Ardrone3PictureSettingsExpositionSelection},
        {ParrotCommand::Tuple(1, 19, 3), Parrot::Ardrone3PictureSettingsSaturationSelection},
        {ParrotCommand::Tuple(1, 19, 4), Parrot::Ardrone3PictureSettingsTimelapseSelection},
        {ParrotCommand::Tuple(1, 19, 5), Parrot::Ardrone3PictureSettingsVideoAutorecordSelection},
        {ParrotCommand::Tuple(1, 19, 6), Parrot::Ardrone3PictureSettingsVideoStabilizationMode},
        {ParrotCommand::Tuple(1, 19, 7), Parrot::Ardrone3PictureSettingsVideoRecordingMode},
        {ParrotCommand::Tuple(1, 19, 8), Parrot::Ardrone3PictureSettingsVideoFramerate},
        {ParrotCommand::Tuple(1, 19, 9), Parrot::Ardrone3PictureSettingsVideoResolutions},
        {ParrotCommand::Tuple(1, 20, 0), Parrot::Ardrone3PictureSettingsStatePictureFormatChanged},
        {ParrotCommand::Tuple(1, 20, 1), Parrot::Ardrone3PictureSettingsStateAutoWhiteBalanceChanged},
        {ParrotCommand::Tuple(1, 20, 2), Parrot::Ardrone3PictureSettingsStateExpositionChanged},
        {ParrotCommand::Tuple(1, 20, 3), Parrot::Ardrone3PictureSettingsStateSaturationChanged},
        {ParrotCommand::Tuple(1, 20, 4), Parrot::Ardrone3PictureSettingsStateTimelapseChanged},
        {ParrotCommand::Tuple(1, 20, 5), Parrot::Ardrone3PictureSettingsStateVideoAutorecordChanged},
        {ParrotCommand::Tuple(1, 20, 6), Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged},
        {ParrotCommand::Tuple(1, 20, 7), Parrot::Ardrone3PictureSettingsStateVideoRecordingModeChanged},
        {ParrotCommand::Tuple(1, 20, 8), Parrot::Ardrone3PictureSettingsStateVideoFramerateChanged},
        {ParrotCommand::Tuple(1, 20, 9), Parrot::Ardrone3PictureSettingsStateVideoResolutionsChanged},
        {ParrotCommand::Tuple(1, 21, 0), Parrot::Ardrone3MediaStreamingVideoEnable},
        {ParrotCommand::Tuple(1, 21, 1), Parrot::Ardrone3MediaStreamingVideoStreamMode},
        {ParrotCommand::Tuple(1, 22, 0), Parrot::Ardrone3MediaStreamingStateVideoEnableChanged},
        {ParrotCommand::Tuple(1, 22, 1), Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged},
        {ParrotCommand::Tuple(1, 23, 2), Parrot::Ardrone3GPSSettingsSendControllerGPS},
        {ParrotCommand::Tuple(1, 23, 3), Parrot::Ardrone3GPSSettingsHomeType},
        {ParrotCommand::Tuple(1, 23, 4), Parrot::Ardrone3GPSSettingsReturnHomeDelay},
        {ParrotCommand::Tuple(1, 24, 0), Parrot::Ardrone3GPSSettingsStateHomeChanged},
        {ParrotCommand::Tuple(1, 24, 2), Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged},
        {ParrotCommand::Tuple(1, 24, 4), Parrot::Ardrone3GPSSettingsStateHomeTypeChanged},
        {ParrotCommand::Tuple(1, 24, 5), Parrot::Ardrone3GPSSettingsStateReturnHomeDelayChanged},
        {ParrotCommand::Tuple(1, 24, 6), Parrot::Ardrone3GPSSettingsStateGeofenceCenterChanged},
        {ParrotCommand::Tuple(1, 25, 2), Parrot::Ardrone3CameraStateOrientationV2},
        {ParrotCommand::Tuple(1, 25, 3), Parrot::Ardrone3CameraStatedefaultCameraOrientationV2},
        {ParrotCommand::Tuple(1, 25, 4), Parrot::Ardrone3CameraStateVelocityRange},
        {ParrotCommand::Tuple(1, 29, 0), Parrot::Ardrone3AntiflickeringelectricFrequency},
        {ParrotCommand::Tuple(1, 29, 1), Parrot::Ardrone3AntiflickeringsetMode},
        {ParrotCommand::Tuple(1, 30, 0), Parrot::Ardrone3AntiflickeringStateelectricFrequencyChanged},
        {ParrotCommand::Tuple(1, 30, 1), Parrot::Ardrone3AntiflickeringStatemodeChanged},
        {ParrotCommand::Tuple(1, 31, 0), Parrot::Ardrone3GPSStateNumberOfSatelliteChanged},
        {ParrotCommand::Tuple(1, 31, 1), Parrot::Ardrone3GPSStateHomeTypeAvailabilityChanged},
        {ParrotCommand::Tuple(1, 31, 2), Parrot::Ardrone3GPSStateHomeTypeChosenChanged},
        {ParrotCommand::Tuple(1, 33, 1), Parrot::Ardrone3AccessoryStateBattery},
        {ParrotCommand::Tuple(1, 35, 0), Parrot::Ardrone3SoundStartAlertSound},
        {ParrotCommand::Tuple(1, 35, 1), Parrot::Ardrone3SoundStopAlertSound},
        {ParrotCommand::Tuple(1, 36, 0), Parrot::Ardrone3SoundStateAlertSound}
    };

}

QHash<ParrotCommand::Tuple, Parrot::Command> ParrotCommand::m_commandForTuple = initCommandForTuple();

QHash<Parrot::Command, ParrotCommand::Tuple> initTupleForCommand()
{
    return {
        {Parrot::CommonNetworkEventDisconnection, ParrotCommand::Tuple(0, 1, 0)},
        {Parrot::CommonSettingsAllSettings, ParrotCommand::Tuple(0, 2, 0)},
        {Parrot::CommonSettingsReset, ParrotCommand::Tuple(0, 2, 1)},
        {Parrot::CommonSettingsProductName, ParrotCommand::Tuple(0, 2, 2)},
        {Parrot::CommonSettingsCountry, ParrotCommand::Tuple(0, 2, 3)},
        {Parrot::CommonSettingsAutoCountry, ParrotCommand::Tuple(0, 2, 4)},
        {Parrot::CommonSettingsStateAllSettingsChanged, ParrotCommand::Tuple(0, 3, 0)},
        {Parrot::CommonSettingsStateResetChanged, ParrotCommand::Tuple(0, 3, 1)},
        {Parrot::CommonSettingsStateProductNameChanged, ParrotCommand::Tuple(0, 3, 2)},
        {Parrot::CommonSettingsStateProductVersionChanged, ParrotCommand::Tuple(0, 3, 3)},
        {Parrot::CommonSettingsStateProductSerialHighChanged, ParrotCommand::Tuple(0, 3, 4)},
        {Parrot::CommonSettingsStateProductSerialLowChanged, ParrotCommand::Tuple(0, 3, 5)},
        {Parrot::CommonSettingsStateCountryChanged, ParrotCommand::Tuple(0, 3, 6)},
        {Parrot::CommonSettingsStateAutoCountryChanged, ParrotCommand::Tuple(0, 3, 7)},
        {Parrot::CommonCommonAllStates, ParrotCommand::Tuple(0, 4, 0)},
        {Parrot::CommonCommonCurrentDate, ParrotCommand::Tuple(0, 4, 1)},
        {Parrot::CommonCommonCurrentTime, ParrotCommand::Tuple(0, 4, 2)},
        {Parrot::CommonCommonReboot, ParrotCommand::Tuple(0, 4, 3)},
        {Parrot::CommonCommonStateAllStatesChanged, ParrotCommand::Tuple(0, 5, 0)},
        {Parrot::CommonCommonStateBatteryStateChanged, ParrotCommand::Tuple(0, 5, 1)},
        {Parrot::CommonCommonStateMassStorageStateListChanged, ParrotCommand::Tuple(0, 5, 2)},
        {Parrot::CommonCommonStateMassStorageInfoStateListChanged, ParrotCommand::Tuple(0, 5, 3)},
        {Parrot::CommonCommonStateCurrentDateChanged, ParrotCommand::Tuple(0, 5, 4)},
        {Parrot::CommonCommonStateCurrentTimeChanged, ParrotCommand::Tuple(0, 5, 5)},
        {Parrot::CommonCommonStateWifiSignalChanged, ParrotCommand::Tuple(0, 5, 7)},
        {Parrot::CommonCommonStateSensorsStatesListChanged, ParrotCommand::Tuple(0, 5, 8)},
        {Parrot::CommonCommonStateMassStorageContent, ParrotCommand::Tuple(0, 5, 12)},
        {Parrot::CommonCommonStateMassStorageContentForCurrentRun, ParrotCommand::Tuple(0, 5, 13)},
        {Parrot::CommonCommonStateVideoRecordingTimestamp, ParrotCommand::Tuple(0, 5, 14)},
        {Parrot::CommonOverHeatStateOverHeatRegulationChanged, ParrotCommand::Tuple(0, 7, 1)},
        {Parrot::CommonControllerisPiloting, ParrotCommand::Tuple(0, 8, 0)},
        {Parrot::CommonWifiSettingsOutdoorSetting, ParrotCommand::Tuple(0, 9, 0)},
        {Parrot::CommonWifiSettingsStateoutdoorSettingsChanged, ParrotCommand::Tuple(0, 10, 0)},
        {Parrot::CommonMavlinkStart, ParrotCommand::Tuple(0, 11, 0)},
        {Parrot::CommonMavlinkPause, ParrotCommand::Tuple(0, 11, 1)},
        {Parrot::CommonMavlinkStop, ParrotCommand::Tuple(0, 11, 2)},
        {Parrot::CommonMavlinkStateMavlinkFilePlayingStateChanged, ParrotCommand::Tuple(0, 12, 0)},
        {Parrot::CommonMavlinkStateMissionItemExecuted, ParrotCommand::Tuple(0, 12, 2)},
        {Parrot::CommonFlightPlanSettingsReturnHomeOnDisconnect, ParrotCommand::Tuple(0, 32, 0)},
        {Parrot::CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged, ParrotCommand::Tuple(0, 33, 0)},
        {Parrot::CommonCalibrationMagnetoCalibration, ParrotCommand::Tuple(0, 13, 0)},
        {Parrot::CommonCalibrationStateMagnetoCalibrationStateChanged, ParrotCommand::Tuple(0, 14, 0)},
        {Parrot::CommonCalibrationStateMagnetoCalibrationRequiredState, ParrotCommand::Tuple(0, 14, 1)},
        {Parrot::CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged, ParrotCommand::Tuple(0, 14, 2)},
        {Parrot::CommonCalibrationStateMagnetoCalibrationStartedChanged, ParrotCommand::Tuple(0, 14, 3)},
        {Parrot::CommonCalibrationStatePitotCalibrationStateChanged, ParrotCommand::Tuple(0, 14, 4)},
        {Parrot::CommonCameraSettingsStateCameraSettingsChanged, ParrotCommand::Tuple(0, 15, 0)},
        {Parrot::CommonFlightPlanStateAvailabilityStateChanged, ParrotCommand::Tuple(0, 17, 0)},
        {Parrot::CommonFlightPlanStateComponentStateListChanged, ParrotCommand::Tuple(0, 17, 1)},
        {Parrot::CommonFlightPlanStateLockStateChanged, ParrotCommand::Tuple(0, 17, 2)},
        {Parrot::CommonFlightPlanEventStartingErrorEvent, ParrotCommand::Tuple(0, 19, 0)},
        {Parrot::CommonFlightPlanEventSpeedBridleEvent, ParrotCommand::Tuple(0, 19, 1)},
        {Parrot::CommonARLibsVersionsStateControllerLibARCommandsVersion, ParrotCommand::Tuple(0, 18, 0)},
        {Parrot::CommonARLibsVersionsStateSkyControllerLibARCommandsVersion, ParrotCommand::Tuple(0, 18, 1)},
        {Parrot::CommonARLibsVersionsStateDeviceLibARCommandsVersion, ParrotCommand::Tuple(0, 18, 2)},
        {Parrot::CommonRunStateRunIdChanged, ParrotCommand::Tuple(0, 30, 0)},
        {Parrot::CommonFactoryReset, ParrotCommand::Tuple(0, 31, 0)},
        {Parrot::Ardrone3PilotingFlatTrim, ParrotCommand::Tuple(1, 0, 0)},
        {Parrot::Ardrone3PilotingTakeOff, ParrotCommand::Tuple(1, 0, 1)},
        {Parrot::Ardrone3PilotingPCMD, ParrotCommand::Tuple(1, 0, 2)},
        {Parrot::Ardrone3PilotingLanding, ParrotCommand::Tuple(1, 0, 3)},
        {Parrot::Ardrone3PilotingEmergency, ParrotCommand::Tuple(1, 0, 4)},
        {Parrot::Ardrone3PilotingNavigateHome, ParrotCommand::Tuple(1, 0, 5)},
        {Parrot::Ardrone3PilotingmoveBy, ParrotCommand::Tuple(1, 0, 7)},
        {Parrot::Ardrone3PilotingUserTakeOff, ParrotCommand::Tuple(1, 0, 8)},
        {Parrot::Ardrone3PilotingmoveTo, ParrotCommand::Tuple(1, 0, 10)},
        {Parrot::Ardrone3PilotingCancelMoveTo, ParrotCommand::Tuple(1, 0, 11)},
        {Parrot::Ardrone3PilotingStartPilotedPOI, ParrotCommand::Tuple(1, 0, 12)},
        {Parrot::Ardrone3PilotingStopPilotedPOI, ParrotCommand::Tuple(1, 0, 13)},
        {Parrot::Ardrone3AnimationsFlip, ParrotCommand::Tuple(1, 5, 0)},
        {Parrot::Ardrone3CameraOrientationV2, ParrotCommand::Tuple(1, 1, 1)},
        {Parrot::Ardrone3CameraVelocity, ParrotCommand::Tuple(1, 1, 2)},
        {Parrot::Ardrone3MediaRecordPictureV2, ParrotCommand::Tuple(1, 7, 2)},
        {Parrot::Ardrone3MediaRecordVideoV2, ParrotCommand::Tuple(1, 7, 3)},
        {Parrot::Ardrone3MediaRecordStatePictureStateChangedV2, ParrotCommand::Tuple(1, 8, 2)},
        {Parrot::Ardrone3MediaRecordStateVideoStateChangedV2, ParrotCommand::Tuple(1, 8, 3)},
        {Parrot::Ardrone3MediaRecordEventPictureEventChanged, ParrotCommand::Tuple(1, 3, 0)},
        {Parrot::Ardrone3MediaRecordEventVideoEventChanged, ParrotCommand::Tuple(1, 3, 1)},
        {Parrot::Ardrone3PilotingStateFlatTrimChanged, ParrotCommand::Tuple(1, 4, 0)},
        {Parrot::Ardrone3PilotingStateFlyingStateChanged, ParrotCommand::Tuple(1, 4, 1)},
        {Parrot::Ardrone3PilotingStateAlertStateChanged, ParrotCommand::Tuple(1, 4, 2)},
        {Parrot::Ardrone3PilotingStateNavigateHomeStateChanged, ParrotCommand::Tuple(1, 4, 3)},
        {Parrot::Ardrone3PilotingStatePositionChanged, ParrotCommand::Tuple(1, 4, 4)},
        {Parrot::Ardrone3PilotingStateSpeedChanged, ParrotCommand::Tuple(1, 4, 5)},
        {Parrot::Ardrone3PilotingStateAttitudeChanged, ParrotCommand::Tuple(1, 4, 6)},
        {Parrot::Ardrone3PilotingStateAltitudeChanged, ParrotCommand::Tuple(1, 4, 8)},
        {Parrot::Ardrone3PilotingStateGpsLocationChanged, ParrotCommand::Tuple(1, 4, 9)},
        {Parrot::Ardrone3PilotingStatemoveToChanged, ParrotCommand::Tuple(1, 4, 12)},
        {Parrot::Ardrone3PilotingStateMotionState, ParrotCommand::Tuple(1, 4, 13)},
        {Parrot::Ardrone3PilotingStatePilotedPOI, ParrotCommand::Tuple(1, 4, 14)},
        {Parrot::Ardrone3PilotingStateReturnHomeBatteryCapacity, ParrotCommand::Tuple(1, 4, 15)},
        {Parrot::Ardrone3PilotingEventmoveByEnd, ParrotCommand::Tuple(1, 34, 0)},
        {Parrot::Ardrone3NetworkWifiScan, ParrotCommand::Tuple(1, 13, 0)},
        {Parrot::Ardrone3NetworkWifiAuthChannel, ParrotCommand::Tuple(1, 13, 1)},
        {Parrot::Ardrone3NetworkStateWifiScanListChanged, ParrotCommand::Tuple(1, 14, 0)},
        {Parrot::Ardrone3NetworkStateAllWifiScanChanged, ParrotCommand::Tuple(1, 14, 1)},
        {Parrot::Ardrone3NetworkStateWifiAuthChannelListChanged, ParrotCommand::Tuple(1, 14, 2)},
        {Parrot::Ardrone3NetworkStateAllWifiAuthChannelChanged, ParrotCommand::Tuple(1, 14, 3)},
        {Parrot::Ardrone3PilotingSettingsMaxAltitude, ParrotCommand::Tuple(1, 2, 0)},
        {Parrot::Ardrone3PilotingSettingsMaxTilt, ParrotCommand::Tuple(1, 2, 1)},
        {Parrot::Ardrone3PilotingSettingsMaxDistance, ParrotCommand::Tuple(1, 2, 3)},
        {Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance, ParrotCommand::Tuple(1, 2, 4)},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed, ParrotCommand::Tuple(1, 2, 5)},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed, ParrotCommand::Tuple(1, 2, 6)},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration, ParrotCommand::Tuple(1, 2, 7)},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration, ParrotCommand::Tuple(1, 2, 8)},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed, ParrotCommand::Tuple(1, 2, 9)},
        {Parrot::Ardrone3PilotingSettingsBankedTurn, ParrotCommand::Tuple(1, 2, 10)},
        {Parrot::Ardrone3PilotingSettingsSetMotionDetectionMode, ParrotCommand::Tuple(1, 2, 16)},
        {Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged, ParrotCommand::Tuple(1, 6, 0)},
        {Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged, ParrotCommand::Tuple(1, 6, 1)},
        {Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged, ParrotCommand::Tuple(1, 6, 3)},
        {Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged, ParrotCommand::Tuple(1, 6, 4)},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed, ParrotCommand::Tuple(1, 6, 5)},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed, ParrotCommand::Tuple(1, 6, 6)},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration, ParrotCommand::Tuple(1, 6, 7)},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration, ParrotCommand::Tuple(1, 6, 8)},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed, ParrotCommand::Tuple(1, 6, 9)},
        {Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged, ParrotCommand::Tuple(1, 6, 10)},
        {Parrot::Ardrone3PilotingSettingsStateMotionDetection, ParrotCommand::Tuple(1, 6, 16)},
        {Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, ParrotCommand::Tuple(1, 11, 0)},
        {Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, ParrotCommand::Tuple(1, 11, 1)},
        {Parrot::Ardrone3SpeedSettingsHullProtection, ParrotCommand::Tuple(1, 11, 2)},
        {Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, ParrotCommand::Tuple(1, 11, 4)},
        {Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged, ParrotCommand::Tuple(1, 12, 0)},
        {Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged, ParrotCommand::Tuple(1, 12, 1)},
        {Parrot::Ardrone3SpeedSettingsStateHullProtectionChanged, ParrotCommand::Tuple(1, 12, 2)},
        {Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged, ParrotCommand::Tuple(1, 12, 4)},
        {Parrot::Ardrone3NetworkSettingsWifiSelection, ParrotCommand::Tuple(1, 9, 0)},
        {Parrot::Ardrone3NetworkSettingswifiSecurity, ParrotCommand::Tuple(1, 9, 1)},
        {Parrot::Ardrone3NetworkSettingsStateWifiSelectionChanged, ParrotCommand::Tuple(1, 10, 0)},
        {Parrot::Ardrone3NetworkSettingsStatewifiSecurity, ParrotCommand::Tuple(1, 10, 2)},
        {Parrot::Ardrone3SettingsStateProductGPSVersionChanged, ParrotCommand::Tuple(1, 16, 1)},
        {Parrot::Ardrone3SettingsStateMotorErrorStateChanged, ParrotCommand::Tuple(1, 16, 2)},
        {Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged, ParrotCommand::Tuple(1, 16, 4)},
        {Parrot::Ardrone3SettingsStateMotorErrorLastErrorChanged, ParrotCommand::Tuple(1, 16, 5)},
        {Parrot::Ardrone3SettingsStateCPUID, ParrotCommand::Tuple(1, 16, 7)},
        {Parrot::Ardrone3PictureSettingsPictureFormatSelection, ParrotCommand::Tuple(1, 19, 0)},
        {Parrot::Ardrone3PictureSettingsAutoWhiteBalanceSelection, ParrotCommand::Tuple(1, 19, 1)},
        {Parrot::Ardrone3PictureSettingsExpositionSelection, ParrotCommand::Tuple(1, 19, 2)},
        {Parrot::Ardrone3PictureSettingsSaturationSelection, ParrotCommand::Tuple(1, 19, 3)},
        {Parrot::Ardrone3PictureSettingsTimelapseSelection, ParrotCommand::Tuple(1, 19, 4)},
        {Parrot::Ardrone3PictureSettingsVideoAutorecordSelection, ParrotCommand::Tuple(1, 19, 5)},
        {Parrot::Ardrone3PictureSettingsVideoStabilizationMode, ParrotCommand::Tuple(1, 19, 6)},
        {Parrot::Ardrone3PictureSettingsVideoRecordingMode, ParrotCommand::Tuple(1, 19, 7)},
        {Parrot::Ardrone3PictureSettingsVideoFramerate, ParrotCommand::Tuple(1, 19, 8)},
        {Parrot::Ardrone3PictureSettingsVideoResolutions, ParrotCommand::Tuple(1, 19, 9)},
        {Parrot::Ardrone3PictureSettingsStatePictureFormatChanged, ParrotCommand::Tuple(1, 20, 0)},
        {Parrot::Ardrone3PictureSettingsStateAutoWhiteBalanceChanged, ParrotCommand::Tuple(1, 20, 1)},
        {Parrot::Ardrone3PictureSettingsStateExpositionChanged, ParrotCommand::Tuple(1, 20, 2)},
        {Parrot::Ardrone3PictureSettingsStateSaturationChanged, ParrotCommand::Tuple(1, 20, 3)},
        {Parrot::Ardrone3PictureSettingsStateTimelapseChanged, ParrotCommand::Tuple(1, 20, 4)},
        {Parrot::Ardrone3PictureSettingsStateVideoAutorecordChanged, ParrotCommand::Tuple(1, 20, 5)},
        {Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged, ParrotCommand::Tuple(1, 20, 6)},
        {Parrot::Ardrone3PictureSettingsStateVideoRecordingModeChanged, ParrotCommand::Tuple(1, 20, 7)},
        {Parrot::Ardrone3PictureSettingsStateVideoFramerateChanged, ParrotCommand::Tuple(1, 20, 8)},
        {Parrot::Ardrone3PictureSettingsStateVideoResolutionsChanged, ParrotCommand::Tuple(1, 20, 9)},
        {Parrot::Ardrone3MediaStreamingVideoEnable, ParrotCommand::Tuple(1, 21, 0)},
        {Parrot::Ardrone3MediaStreamingVideoStreamMode, ParrotCommand::Tuple(1, 21, 1)},
        {Parrot::Ardrone3MediaStreamingStateVideoEnableChanged, ParrotCommand::Tuple(1, 22, 0)},
        {Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged, ParrotCommand::Tuple(1, 22, 1)},
        {Parrot::Ardrone3GPSSettingsSendControllerGPS, ParrotCommand::Tuple(1, 23, 2)},
        {Parrot::Ardrone3GPSSettingsHomeType, ParrotCommand::Tuple(1, 23, 3)},
        {Parrot::Ardrone3GPSSettingsReturnHomeDelay, ParrotCommand::Tuple(1, 23, 4)},
        {Parrot::Ardrone3GPSSettingsStateHomeChanged, ParrotCommand::Tuple(1, 24, 0)},
        {Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged, ParrotCommand::Tuple(1, 24, 2)},
        {Parrot::Ardrone3GPSSettingsStateHomeTypeChanged, ParrotCommand::Tuple(1, 24, 4)},
        {Parrot::Ardrone3GPSSettingsStateReturnHomeDelayChanged, ParrotCommand::Tuple(1, 24, 5)},
        {Parrot::Ardrone3GPSSettingsStateGeofenceCenterChanged, ParrotCommand::Tuple(1, 24, 6)},
        {Parrot::Ardrone3CameraStateOrientationV2, ParrotCommand::Tuple(1, 25, 2)},
        {Parrot::Ardrone3CameraStatedefaultCameraOrientationV2, ParrotCommand::Tuple(1, 25, 3)},
        {Parrot::Ardrone3CameraStateVelocityRange, ParrotCommand::Tuple(1, 25, 4)},
        {Parrot::Ardrone3AntiflickeringelectricFrequency, ParrotCommand::Tuple(1, 29, 0)},
        {Parrot::Ardrone3AntiflickeringsetMode, ParrotCommand::Tuple(1, 29, 1)},
        {Parrot::Ardrone3AntiflickeringStateelectricFrequencyChanged, ParrotCommand::Tuple(1, 30, 0)},
        {Parrot::Ardrone3AntiflickeringStatemodeChanged, ParrotCommand::Tuple(1, 30, 1)},
        {Parrot::Ardrone3GPSStateNumberOfSatelliteChanged, ParrotCommand::Tuple(1, 31, 0)},
        {Parrot::Ardrone3GPSStateHomeTypeAvailabilityChanged, ParrotCommand::Tuple(1, 31, 1)},
        {Parrot::Ardrone3GPSStateHomeTypeChosenChanged, ParrotCommand::Tuple(1, 31, 2)},
        {Parrot::Ardrone3AccessoryStateBattery, ParrotCommand::Tuple(1, 33, 1)},
        {Parrot::Ardrone3SoundStartAlertSound, ParrotCommand::Tuple(1, 35, 0)},
        {Parrot::Ardrone3SoundStopAlertSound, ParrotCommand::Tuple(1, 35, 1)},
        {Parrot::Ardrone3SoundStateAlertSound, ParrotCommand::Tuple(1, 36, 0)}
    };
}

QHash<Parrot::Command, ParrotCommand::Tuple> ParrotCommand::m_tupleForCommand = initTupleForCommand();

QHash<Parrot::Command, QVector<Parrot::ArgumentType>> initSignatureForCommand()
{
    return {
        {Parrot::CommonNetworkEventDisconnection, {Parrot::TYPE_ENUM}},
        {Parrot::CommonSettingsProductName, {Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsCountry, {Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsAutoCountry, {Parrot::TYPE_U8}},
        {Parrot::CommonSettingsStateProductNameChanged, {Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsStateProductVersionChanged, {Parrot::TYPE_STRING, Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsStateProductSerialHighChanged, {Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsStateProductSerialLowChanged, {Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsStateCountryChanged, {Parrot::TYPE_STRING}},
        {Parrot::CommonSettingsStateAutoCountryChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonCommonCurrentDate, {Parrot::TYPE_STRING}},
        {Parrot::CommonCommonCurrentTime, {Parrot::TYPE_STRING}},
        {Parrot::CommonCommonStateBatteryStateChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonCommonStateMassStorageStateListChanged, {Parrot::TYPE_U8, Parrot::TYPE_STRING}},
        {Parrot::CommonCommonStateMassStorageInfoStateListChanged, {Parrot::TYPE_U8, Parrot::TYPE_U32, Parrot::TYPE_U32, Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::CommonCommonStateCurrentDateChanged, {Parrot::TYPE_STRING}},
        {Parrot::CommonCommonStateCurrentTimeChanged, {Parrot::TYPE_STRING}},
        {Parrot::CommonCommonStateWifiSignalChanged, {Parrot::TYPE_I16}},
        {Parrot::CommonCommonStateSensorsStatesListChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::CommonCommonStateMassStorageContent, {Parrot::TYPE_U8, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16}},
        {Parrot::CommonCommonStateMassStorageContentForCurrentRun, {Parrot::TYPE_U8, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16}},
        {Parrot::CommonCommonStateVideoRecordingTimestamp, {Parrot::TYPE_U64, Parrot::TYPE_U64}},
        {Parrot::CommonOverHeatStateOverHeatRegulationChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonControllerisPiloting, {Parrot::TYPE_U8}},
        {Parrot::CommonWifiSettingsOutdoorSetting, {Parrot::TYPE_U8}},
        {Parrot::CommonWifiSettingsStateoutdoorSettingsChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonMavlinkStart, {Parrot::TYPE_STRING, Parrot::TYPE_ENUM}},
        {Parrot::CommonMavlinkStateMavlinkFilePlayingStateChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_STRING, Parrot::TYPE_ENUM}},
        {Parrot::CommonMavlinkStateMissionItemExecuted, {Parrot::TYPE_U32}},
        {Parrot::CommonFlightPlanSettingsReturnHomeOnDisconnect, {Parrot::TYPE_U8}},
        {Parrot::CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged, {Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::CommonCalibrationMagnetoCalibration, {Parrot::TYPE_U8}},
        {Parrot::CommonCalibrationStateMagnetoCalibrationStateChanged, {Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::CommonCalibrationStateMagnetoCalibrationRequiredState, {Parrot::TYPE_U8}},
        {Parrot::CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged, {Parrot::TYPE_ENUM}},
        {Parrot::CommonCalibrationStateMagnetoCalibrationStartedChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonCalibrationStatePitotCalibrationStateChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::CommonCameraSettingsStateCameraSettingsChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::CommonFlightPlanStateAvailabilityStateChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonFlightPlanStateComponentStateListChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::CommonFlightPlanStateLockStateChanged, {Parrot::TYPE_U8}},
        {Parrot::CommonARLibsVersionsStateControllerLibARCommandsVersion, {Parrot::TYPE_STRING}},
        {Parrot::CommonARLibsVersionsStateSkyControllerLibARCommandsVersion, {Parrot::TYPE_STRING}},
        {Parrot::CommonARLibsVersionsStateDeviceLibARCommandsVersion, {Parrot::TYPE_STRING}},
        {Parrot::CommonRunStateRunIdChanged, {Parrot::TYPE_STRING}},
        {Parrot::Ardrone3PilotingPCMD, {Parrot::TYPE_U8, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_U32}},
        {Parrot::Ardrone3PilotingNavigateHome, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingmoveBy, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingUserTakeOff, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingmoveTo, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_ENUM, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingStartPilotedPOI, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE}},
        {Parrot::Ardrone3AnimationsFlip, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3CameraOrientationV2, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3CameraVelocity, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3MediaRecordVideoV2, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaRecordStatePictureStateChangedV2, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaRecordStateVideoStateChangedV2, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaRecordEventPictureEventChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaRecordEventVideoEventChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStateFlyingStateChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStateAlertStateChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStateNavigateHomeStateChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStatePositionChanged, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE}},
        {Parrot::Ardrone3PilotingStateSpeedChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingStateAttitudeChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingStateAltitudeChanged, {Parrot::TYPE_DOUBLE}},
        {Parrot::Ardrone3PilotingStateGpsLocationChanged, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_I8}},
        {Parrot::Ardrone3PilotingStatemoveToChanged, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_ENUM, Parrot::TYPE_FLOAT, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStateMotionState, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStatePilotedPOI, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingStateReturnHomeBatteryCapacity, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PilotingEventmoveByEnd, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3NetworkWifiScan, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3NetworkStateWifiScanListChanged, {Parrot::TYPE_STRING, Parrot::TYPE_I16, Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::Ardrone3NetworkStateWifiAuthChannelListChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingSettingsMaxAltitude, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsMaxTilt, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsMaxDistance, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsBankedTurn, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingSettingsSetMotionDetectionMode, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3PilotingSettingsStateMotionDetection, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3SpeedSettingsHullProtection, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3SpeedSettingsStateHullProtectionChanged, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3NetworkSettingsWifiSelection, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::Ardrone3NetworkSettingswifiSecurity, {Parrot::TYPE_ENUM, Parrot::TYPE_STRING, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3NetworkSettingsStateWifiSelectionChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::Ardrone3NetworkSettingsStatewifiSecurity, {Parrot::TYPE_ENUM, Parrot::TYPE_STRING, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3SettingsStateProductGPSVersionChanged, {Parrot::TYPE_STRING, Parrot::TYPE_STRING}},
        {Parrot::Ardrone3SettingsStateMotorErrorStateChanged, {Parrot::TYPE_U8, Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged, {Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U32}},
        {Parrot::Ardrone3SettingsStateMotorErrorLastErrorChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3SettingsStateCPUID, {Parrot::TYPE_STRING}},
        {Parrot::Ardrone3PictureSettingsPictureFormatSelection, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsAutoWhiteBalanceSelection, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsExpositionSelection, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PictureSettingsSaturationSelection, {Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PictureSettingsTimelapseSelection, {Parrot::TYPE_U8, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PictureSettingsVideoAutorecordSelection, {Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::Ardrone3PictureSettingsVideoStabilizationMode, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsVideoRecordingMode, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsVideoFramerate, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsVideoResolutions, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsStatePictureFormatChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsStateAutoWhiteBalanceChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsStateExpositionChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PictureSettingsStateSaturationChanged, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PictureSettingsStateTimelapseChanged, {Parrot::TYPE_U8, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3PictureSettingsStateVideoAutorecordChanged, {Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsStateVideoRecordingModeChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsStateVideoFramerateChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3PictureSettingsStateVideoResolutionsChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaStreamingVideoEnable, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3MediaStreamingVideoStreamMode, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaStreamingStateVideoEnableChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3GPSSettingsSendControllerGPS, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE}},
        {Parrot::Ardrone3GPSSettingsHomeType, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3GPSSettingsReturnHomeDelay, {Parrot::TYPE_U16}},
        {Parrot::Ardrone3GPSSettingsStateHomeChanged, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE}},
        {Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3GPSSettingsStateHomeTypeChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3GPSSettingsStateReturnHomeDelayChanged, {Parrot::TYPE_U16}},
        {Parrot::Ardrone3GPSSettingsStateGeofenceCenterChanged, {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE}},
        {Parrot::Ardrone3CameraStateOrientationV2, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3CameraStatedefaultCameraOrientationV2, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3CameraStateVelocityRange, {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT}},
        {Parrot::Ardrone3AntiflickeringelectricFrequency, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3AntiflickeringsetMode, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3AntiflickeringStateelectricFrequencyChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3AntiflickeringStatemodeChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3GPSStateNumberOfSatelliteChanged, {Parrot::TYPE_U8}},
        {Parrot::Ardrone3GPSStateHomeTypeAvailabilityChanged, {Parrot::TYPE_ENUM, Parrot::TYPE_U8}},
        {Parrot::Ardrone3GPSStateHomeTypeChosenChanged, {Parrot::TYPE_ENUM}},
        {Parrot::Ardrone3AccessoryStateBattery, {Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8}},
        {Parrot::Ardrone3SoundStateAlertSound, {Parrot::TYPE_ENUM}}
    };
}

QHash<Parrot::Command, QVector<Parrot::ArgumentType>> ParrotCommand::m_signatureForCommand = initSignatureForCommand();

ParrotCommand::ParrotCommand(Parrot::Command command, const QVariantList &arguments)
    : tuple(tupleForCommand(command))
{
    if (!arguments.isEmpty() && m_signatureForCommand.contains(command)) {
        const QVector<Parrot::ArgumentType> &signature = m_signatureForCommand.value(command);

        QDataStream s(&data, QIODevice::WriteOnly);
        s.setByteOrder(QDataStream::LittleEndian);

        for (int i = 0; i < signature.count(); ++i) {
            const Parrot::ArgumentType argType = signature.at(i);

            switch (argType) {
                case Parrot::TYPE_U8: {
                    s << arguments.at(i).value<quint8>();
                    break;
                }
                case Parrot::TYPE_I8: {
                    s << arguments.at(i).value<qint8>();
                    break;
                }
                case Parrot::TYPE_U16: {
                    s << arguments.at(i).value<quint16>();
                    break;
                }
                case Parrot::TYPE_I16: {
                    s << arguments.at(i).value<qint16>();
                    break;
                }
                case Parrot::TYPE_U32: {
                    s << arguments.at(i).value<quint32>();
                    break;
                }
                case Parrot::TYPE_I32: {
                    s << arguments.at(i).value<qint32>();
                    break;
                }
                case Parrot::TYPE_U64: {
                    s << arguments.at(i).value<quint64>();
                    break;
                }
                case Parrot::TYPE_I64: {
                    s << arguments.at(i).value<qint64>();
                    break;
                }
                case Parrot::TYPE_FLOAT: {
                    s.setFloatingPointPrecision(QDataStream::SinglePrecision);
                    s << arguments.at(i).value<float>();
                    break;
                }
                case Parrot::TYPE_DOUBLE: {
                    s.setFloatingPointPrecision(QDataStream::DoublePrecision);
                    s << arguments.at(i).value<qreal>();
                    break;
                }
                case Parrot::TYPE_STRING: {
                    s << arguments.at(i).toString().toLatin1(); // FIXME UTF-8?
                    break;
                }
                case Parrot::TYPE_ENUM: {
                    s << arguments.at(i).value<qint32>();
                    break;
                }
            }
        }
    }
}

ParrotCommand::ParrotCommand(Parrot::Command command, const QByteArray &arguments)
    : tuple(tupleForCommand(command))
    , data(arguments)
{
}

ParrotCommand::ParrotCommand(const QByteArray &_data)
{
    QDataStream s(_data);
    s.setByteOrder(QDataStream::LittleEndian);

    s >> tuple.productId >> tuple.classId >> tuple.commandId;

    const int dataSize = _data.size() - Parrot::HEADER_COMMAND;

    if (dataSize > 0) {
        data = QByteArray(dataSize, 0);
        s.readRawData(data.data(), dataSize);
    }
}

ParrotCommand::ParrotCommand()
{
}

ParrotCommand::~ParrotCommand()
{
}

QByteArray ParrotCommand::pack() const
{
    QByteArray command;

    QDataStream s(&command, QIODevice::WriteOnly);
    s.setByteOrder(QDataStream::LittleEndian);

    s << tuple.productId << tuple.classId << tuple.commandId;

    if (!data.isNull()) {
        s.writeRawData(data.constData(), data.size()); // FIXME Doesn't null-terminate strings anymore.
    }

    return command;
}

Parrot::Command ParrotCommand::id() const
{
    return commandForTuple(tuple);
}

Parrot::Command ParrotCommand::commandForTuple(const Tuple &tuple)
{
    if (m_commandForTuple.contains(tuple)) {
        return m_commandForTuple.value(tuple);
    }

    return Parrot::UnknownCommand;
}

ParrotCommand::Tuple ParrotCommand::tupleForCommand(Parrot::Command command)
{
    if (m_tupleForCommand.contains(command)) {
        return m_tupleForCommand.value(command);
    }

    return Tuple();
}

ParrotFrame::ParrotFrame(quint8 _dataType, quint8 _bufferId, quint8 _seq, const QByteArray& _data)
    : dataType(_dataType)
    , bufferId(_bufferId)
    , seq(_seq)
    , size(Parrot::HEADER_FRAME + (_data.size() > Parrot::HEADER_COMMAND
        ? _data.size() + 1 : _data.size())) // FIXME HACK for null-terminating strings.
    , data(_data)
{
}

ParrotFrame::ParrotFrame(const QByteArray &_data, int start = 0)
{
    QDataStream s(_data);
    s.setByteOrder(QDataStream::LittleEndian);

    s.skipRawData(start);

    s >> dataType >> bufferId >> seq >> size;

    const int dataSize = size - Parrot::HEADER_FRAME;
    data = QByteArray(dataSize, 0);
    s.readRawData(data.data(), dataSize);
}

ParrotFrame::~ParrotFrame()
{
}

QByteArray ParrotFrame::pack() const
{
    QByteArray frame;
    frame.reserve(data.size() + Parrot::HEADER_FRAME);

    QDataStream s(&frame, QIODevice::WriteOnly);
    s.setByteOrder(QDataStream::LittleEndian);

    s << dataType << bufferId << seq << size;

    if (!data.isNull()) {
        const int dataSize = data.size() > Parrot::HEADER_COMMAND ? data.size() + 1 : data.size(); // FIXME HACK for null-terminating strings.
        s.writeRawData(data.constData(), dataSize);
    }

    return frame;
}

ParrotCommand ParrotFrame::command() const
{
    return ParrotCommand(data);
}
