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
    QHash<ParrotCommand::Tuple, Parrot::Command> hash;
    hash[ParrotCommand::Tuple(0, 1, 0)] = Parrot::CommonNetworkEventDisconnection;
    hash[ParrotCommand::Tuple(0, 2, 0)] = Parrot::CommonSettingsAllSettings;
    hash[ParrotCommand::Tuple(0, 2, 1)] = Parrot::CommonSettingsReset;
    hash[ParrotCommand::Tuple(0, 2, 2)] = Parrot::CommonSettingsProductName;
    hash[ParrotCommand::Tuple(0, 2, 3)] = Parrot::CommonSettingsCountry;
    hash[ParrotCommand::Tuple(0, 2, 4)] = Parrot::CommonSettingsAutoCountry;
    hash[ParrotCommand::Tuple(0, 3, 0)] = Parrot::CommonSettingsStateAllSettingsChanged;
    hash[ParrotCommand::Tuple(0, 3, 1)] = Parrot::CommonSettingsStateResetChanged;
    hash[ParrotCommand::Tuple(0, 3, 2)] = Parrot::CommonSettingsStateProductNameChanged;
    hash[ParrotCommand::Tuple(0, 3, 3)] = Parrot::CommonSettingsStateProductVersionChanged;
    hash[ParrotCommand::Tuple(0, 3, 4)] = Parrot::CommonSettingsStateProductSerialHighChanged;
    hash[ParrotCommand::Tuple(0, 3, 5)] = Parrot::CommonSettingsStateProductSerialLowChanged;
    hash[ParrotCommand::Tuple(0, 3, 6)] = Parrot::CommonSettingsStateCountryChanged;
    hash[ParrotCommand::Tuple(0, 3, 7)] = Parrot::CommonSettingsStateAutoCountryChanged;
    hash[ParrotCommand::Tuple(0, 4, 0)] = Parrot::CommonCommonAllStates;
    hash[ParrotCommand::Tuple(0, 4, 1)] = Parrot::CommonCommonCurrentDate;
    hash[ParrotCommand::Tuple(0, 4, 2)] = Parrot::CommonCommonCurrentTime;
    hash[ParrotCommand::Tuple(0, 4, 3)] = Parrot::CommonCommonReboot;
    hash[ParrotCommand::Tuple(0, 5, 0)] = Parrot::CommonCommonStateAllStatesChanged;
    hash[ParrotCommand::Tuple(0, 5, 1)] = Parrot::CommonCommonStateBatteryStateChanged;
    hash[ParrotCommand::Tuple(0, 5, 2)] = Parrot::CommonCommonStateMassStorageStateListChanged;
    hash[ParrotCommand::Tuple(0, 5, 3)] = Parrot::CommonCommonStateMassStorageInfoStateListChanged;
    hash[ParrotCommand::Tuple(0, 5, 4)] = Parrot::CommonCommonStateCurrentDateChanged;
    hash[ParrotCommand::Tuple(0, 5, 5)] = Parrot::CommonCommonStateCurrentTimeChanged;
    hash[ParrotCommand::Tuple(0, 5, 7)] = Parrot::CommonCommonStateWifiSignalChanged;
    hash[ParrotCommand::Tuple(0, 5, 8)] = Parrot::CommonCommonStateSensorsStatesListChanged;
    hash[ParrotCommand::Tuple(0, 5, 12)] = Parrot::CommonCommonStateMassStorageContent;
    hash[ParrotCommand::Tuple(0, 5, 13)] = Parrot::CommonCommonStateMassStorageContentForCurrentRun;
    hash[ParrotCommand::Tuple(0, 5, 14)] = Parrot::CommonCommonStateVideoRecordingTimestamp;
    hash[ParrotCommand::Tuple(0, 7, 1)] = Parrot::CommonOverHeatStateOverHeatRegulationChanged;
    hash[ParrotCommand::Tuple(0, 8, 0)] = Parrot::CommonControllerisPiloting;
    hash[ParrotCommand::Tuple(0, 9, 0)] = Parrot::CommonWifiSettingsOutdoorSetting;
    hash[ParrotCommand::Tuple(0, 10, 0)] = Parrot::CommonWifiSettingsStateoutdoorSettingsChanged;
    hash[ParrotCommand::Tuple(0, 11, 0)] = Parrot::CommonMavlinkStart;
    hash[ParrotCommand::Tuple(0, 11, 1)] = Parrot::CommonMavlinkPause;
    hash[ParrotCommand::Tuple(0, 11, 2)] = Parrot::CommonMavlinkStop;
    hash[ParrotCommand::Tuple(0, 12, 0)] = Parrot::CommonMavlinkStateMavlinkFilePlayingStateChanged;
    hash[ParrotCommand::Tuple(0, 12, 2)] = Parrot::CommonMavlinkStateMissionItemExecuted;
    hash[ParrotCommand::Tuple(0, 32, 0)] = Parrot::CommonFlightPlanSettingsReturnHomeOnDisconnect;
    hash[ParrotCommand::Tuple(0, 33, 0)] = Parrot::CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged;
    hash[ParrotCommand::Tuple(0, 13, 0)] = Parrot::CommonCalibrationMagnetoCalibration;
    hash[ParrotCommand::Tuple(0, 14, 0)] = Parrot::CommonCalibrationStateMagnetoCalibrationStateChanged;
    hash[ParrotCommand::Tuple(0, 14, 1)] = Parrot::CommonCalibrationStateMagnetoCalibrationRequiredState;
    hash[ParrotCommand::Tuple(0, 14, 2)] = Parrot::CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged;
    hash[ParrotCommand::Tuple(0, 14, 3)] = Parrot::CommonCalibrationStateMagnetoCalibrationStartedChanged;
    hash[ParrotCommand::Tuple(0, 14, 4)] = Parrot::CommonCalibrationStatePitotCalibrationStateChanged;
    hash[ParrotCommand::Tuple(0, 15, 0)] = Parrot::CommonCameraSettingsStateCameraSettingsChanged;
    hash[ParrotCommand::Tuple(0, 17, 0)] = Parrot::CommonFlightPlanStateAvailabilityStateChanged;
    hash[ParrotCommand::Tuple(0, 17, 1)] = Parrot::CommonFlightPlanStateComponentStateListChanged;
    hash[ParrotCommand::Tuple(0, 17, 2)] = Parrot::CommonFlightPlanStateLockStateChanged;
    hash[ParrotCommand::Tuple(0, 19, 0)] = Parrot::CommonFlightPlanEventStartingErrorEvent;
    hash[ParrotCommand::Tuple(0, 19, 1)] = Parrot::CommonFlightPlanEventSpeedBridleEvent;
    hash[ParrotCommand::Tuple(0, 18, 0)] = Parrot::CommonARLibsVersionsStateControllerLibARCommandsVersion;
    hash[ParrotCommand::Tuple(0, 18, 1)] = Parrot::CommonARLibsVersionsStateSkyControllerLibARCommandsVersion;
    hash[ParrotCommand::Tuple(0, 18, 2)] = Parrot::CommonARLibsVersionsStateDeviceLibARCommandsVersion;
    hash[ParrotCommand::Tuple(0, 30, 0)] = Parrot::CommonRunStateRunIdChanged;
    hash[ParrotCommand::Tuple(0, 31, 0)] = Parrot::CommonFactoryReset;
    hash[ParrotCommand::Tuple(1, 0, 0)] = Parrot::Ardrone3PilotingFlatTrim;
    hash[ParrotCommand::Tuple(1, 0, 1)] = Parrot::Ardrone3PilotingTakeOff;
    hash[ParrotCommand::Tuple(1, 0, 2)] = Parrot::Ardrone3PilotingPCMD;
    hash[ParrotCommand::Tuple(1, 0, 3)] = Parrot::Ardrone3PilotingLanding;
    hash[ParrotCommand::Tuple(1, 0, 4)] = Parrot::Ardrone3PilotingEmergency;
    hash[ParrotCommand::Tuple(1, 0, 5)] = Parrot::Ardrone3PilotingNavigateHome;
    hash[ParrotCommand::Tuple(1, 0, 7)] = Parrot::Ardrone3PilotingmoveBy;
    hash[ParrotCommand::Tuple(1, 0, 8)] = Parrot::Ardrone3PilotingUserTakeOff;
    hash[ParrotCommand::Tuple(1, 0, 10)] = Parrot::Ardrone3PilotingmoveTo;
    hash[ParrotCommand::Tuple(1, 0, 11)] = Parrot::Ardrone3PilotingCancelMoveTo;
    hash[ParrotCommand::Tuple(1, 0, 12)] = Parrot::Ardrone3PilotingStartPilotedPOI;
    hash[ParrotCommand::Tuple(1, 0, 13)] = Parrot::Ardrone3PilotingStopPilotedPOI;
    hash[ParrotCommand::Tuple(1, 5, 0)] = Parrot::Ardrone3AnimationsFlip;
    hash[ParrotCommand::Tuple(1, 1, 1)] = Parrot::Ardrone3CameraOrientationV2;
    hash[ParrotCommand::Tuple(1, 1, 2)] = Parrot::Ardrone3CameraVelocity;
    hash[ParrotCommand::Tuple(1, 7, 2)] = Parrot::Ardrone3MediaRecordPictureV2;
    hash[ParrotCommand::Tuple(1, 7, 3)] = Parrot::Ardrone3MediaRecordVideoV2;
    hash[ParrotCommand::Tuple(1, 8, 2)] = Parrot::Ardrone3MediaRecordStatePictureStateChangedV2;
    hash[ParrotCommand::Tuple(1, 8, 3)] = Parrot::Ardrone3MediaRecordStateVideoStateChangedV2;
    hash[ParrotCommand::Tuple(1, 3, 0)] = Parrot::Ardrone3MediaRecordEventPictureEventChanged;
    hash[ParrotCommand::Tuple(1, 3, 1)] = Parrot::Ardrone3MediaRecordEventVideoEventChanged;
    hash[ParrotCommand::Tuple(1, 4, 0)] = Parrot::Ardrone3PilotingStateFlatTrimChanged;
    hash[ParrotCommand::Tuple(1, 4, 1)] = Parrot::Ardrone3PilotingStateFlyingStateChanged;
    hash[ParrotCommand::Tuple(1, 4, 2)] = Parrot::Ardrone3PilotingStateAlertStateChanged;
    hash[ParrotCommand::Tuple(1, 4, 3)] = Parrot::Ardrone3PilotingStateNavigateHomeStateChanged;
    hash[ParrotCommand::Tuple(1, 4, 4)] = Parrot::Ardrone3PilotingStatePositionChanged;
    hash[ParrotCommand::Tuple(1, 4, 5)] = Parrot::Ardrone3PilotingStateSpeedChanged;
    hash[ParrotCommand::Tuple(1, 4, 6)] = Parrot::Ardrone3PilotingStateAttitudeChanged;
    hash[ParrotCommand::Tuple(1, 4, 8)] = Parrot::Ardrone3PilotingStateAltitudeChanged;
    hash[ParrotCommand::Tuple(1, 4, 9)] = Parrot::Ardrone3PilotingStateGpsLocationChanged;
    hash[ParrotCommand::Tuple(1, 4, 12)] = Parrot::Ardrone3PilotingStatemoveToChanged;
    hash[ParrotCommand::Tuple(1, 4, 13)] = Parrot::Ardrone3PilotingStateMotionState;
    hash[ParrotCommand::Tuple(1, 4, 14)] = Parrot::Ardrone3PilotingStatePilotedPOI;
    hash[ParrotCommand::Tuple(1, 4, 15)] = Parrot::Ardrone3PilotingStateReturnHomeBatteryCapacity;
    hash[ParrotCommand::Tuple(1, 34, 0)] = Parrot::Ardrone3PilotingEventmoveByEnd;
    hash[ParrotCommand::Tuple(1, 13, 0)] = Parrot::Ardrone3NetworkWifiScan;
    hash[ParrotCommand::Tuple(1, 13, 1)] = Parrot::Ardrone3NetworkWifiAuthChannel;
    hash[ParrotCommand::Tuple(1, 14, 0)] = Parrot::Ardrone3NetworkStateWifiScanListChanged;
    hash[ParrotCommand::Tuple(1, 14, 1)] = Parrot::Ardrone3NetworkStateAllWifiScanChanged;
    hash[ParrotCommand::Tuple(1, 14, 2)] = Parrot::Ardrone3NetworkStateWifiAuthChannelListChanged;
    hash[ParrotCommand::Tuple(1, 14, 3)] = Parrot::Ardrone3NetworkStateAllWifiAuthChannelChanged;
    hash[ParrotCommand::Tuple(1, 2, 0)] = Parrot::Ardrone3PilotingSettingsMaxAltitude;
    hash[ParrotCommand::Tuple(1, 2, 1)] = Parrot::Ardrone3PilotingSettingsMaxTilt;
    hash[ParrotCommand::Tuple(1, 2, 3)] = Parrot::Ardrone3PilotingSettingsMaxDistance;
    hash[ParrotCommand::Tuple(1, 2, 4)] = Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance;
    hash[ParrotCommand::Tuple(1, 2, 5)] = Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed;
    hash[ParrotCommand::Tuple(1, 2, 6)] = Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed;
    hash[ParrotCommand::Tuple(1, 2, 7)] = Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration;
    hash[ParrotCommand::Tuple(1, 2, 8)] = Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration;
    hash[ParrotCommand::Tuple(1, 2, 9)] = Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed;
    hash[ParrotCommand::Tuple(1, 2, 10)] = Parrot::Ardrone3PilotingSettingsBankedTurn;
    hash[ParrotCommand::Tuple(1, 2, 16)] = Parrot::Ardrone3PilotingSettingsSetMotionDetectionMode;
    hash[ParrotCommand::Tuple(1, 6, 0)] = Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged;
    hash[ParrotCommand::Tuple(1, 6, 1)] = Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged;
    hash[ParrotCommand::Tuple(1, 6, 3)] = Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged;
    hash[ParrotCommand::Tuple(1, 6, 4)] = Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged;
    hash[ParrotCommand::Tuple(1, 6, 5)] = Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed;
    hash[ParrotCommand::Tuple(1, 6, 6)] = Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed;
    hash[ParrotCommand::Tuple(1, 6, 7)] = Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration;
    hash[ParrotCommand::Tuple(1, 6, 8)] = Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration;
    hash[ParrotCommand::Tuple(1, 6, 9)] = Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed;
    hash[ParrotCommand::Tuple(1, 6, 10)] = Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged;
    hash[ParrotCommand::Tuple(1, 6, 16)] = Parrot::Ardrone3PilotingSettingsStateMotionDetection;
    hash[ParrotCommand::Tuple(1, 11, 0)] = Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed;
    hash[ParrotCommand::Tuple(1, 11, 1)] = Parrot::Ardrone3SpeedSettingsMaxRotationSpeed;
    hash[ParrotCommand::Tuple(1, 11, 2)] = Parrot::Ardrone3SpeedSettingsHullProtection;
    hash[ParrotCommand::Tuple(1, 11, 4)] = Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed;
    hash[ParrotCommand::Tuple(1, 12, 0)] = Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged;
    hash[ParrotCommand::Tuple(1, 12, 1)] = Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged;
    hash[ParrotCommand::Tuple(1, 12, 2)] = Parrot::Ardrone3SpeedSettingsStateHullProtectionChanged;
    hash[ParrotCommand::Tuple(1, 12, 4)] = Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged;
    hash[ParrotCommand::Tuple(1, 9, 0)] = Parrot::Ardrone3NetworkSettingsWifiSelection;
    hash[ParrotCommand::Tuple(1, 9, 1)] = Parrot::Ardrone3NetworkSettingswifiSecurity;
    hash[ParrotCommand::Tuple(1, 10, 0)] = Parrot::Ardrone3NetworkSettingsStateWifiSelectionChanged;
    hash[ParrotCommand::Tuple(1, 10, 2)] = Parrot::Ardrone3NetworkSettingsStatewifiSecurity;
    hash[ParrotCommand::Tuple(1, 16, 1)] = Parrot::Ardrone3SettingsStateProductGPSVersionChanged;
    hash[ParrotCommand::Tuple(1, 16, 2)] = Parrot::Ardrone3SettingsStateMotorErrorStateChanged;
    hash[ParrotCommand::Tuple(1, 16, 4)] = Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged;
    hash[ParrotCommand::Tuple(1, 16, 5)] = Parrot::Ardrone3SettingsStateMotorErrorLastErrorChanged;
    hash[ParrotCommand::Tuple(1, 16, 7)] = Parrot::Ardrone3SettingsStateCPUID;
    hash[ParrotCommand::Tuple(1, 19, 0)] = Parrot::Ardrone3PictureSettingsPictureFormatSelection;
    hash[ParrotCommand::Tuple(1, 19, 1)] = Parrot::Ardrone3PictureSettingsAutoWhiteBalanceSelection;
    hash[ParrotCommand::Tuple(1, 19, 2)] = Parrot::Ardrone3PictureSettingsExpositionSelection;
    hash[ParrotCommand::Tuple(1, 19, 3)] = Parrot::Ardrone3PictureSettingsSaturationSelection;
    hash[ParrotCommand::Tuple(1, 19, 4)] = Parrot::Ardrone3PictureSettingsTimelapseSelection;
    hash[ParrotCommand::Tuple(1, 19, 5)] = Parrot::Ardrone3PictureSettingsVideoAutorecordSelection;
    hash[ParrotCommand::Tuple(1, 19, 6)] = Parrot::Ardrone3PictureSettingsVideoStabilizationMode;
    hash[ParrotCommand::Tuple(1, 19, 7)] = Parrot::Ardrone3PictureSettingsVideoRecordingMode;
    hash[ParrotCommand::Tuple(1, 19, 8)] = Parrot::Ardrone3PictureSettingsVideoFramerate;
    hash[ParrotCommand::Tuple(1, 19, 9)] = Parrot::Ardrone3PictureSettingsVideoResolutions;
    hash[ParrotCommand::Tuple(1, 20, 0)] = Parrot::Ardrone3PictureSettingsStatePictureFormatChanged;
    hash[ParrotCommand::Tuple(1, 20, 1)] = Parrot::Ardrone3PictureSettingsStateAutoWhiteBalanceChanged;
    hash[ParrotCommand::Tuple(1, 20, 2)] = Parrot::Ardrone3PictureSettingsStateExpositionChanged;
    hash[ParrotCommand::Tuple(1, 20, 3)] = Parrot::Ardrone3PictureSettingsStateSaturationChanged;
    hash[ParrotCommand::Tuple(1, 20, 4)] = Parrot::Ardrone3PictureSettingsStateTimelapseChanged;
    hash[ParrotCommand::Tuple(1, 20, 5)] = Parrot::Ardrone3PictureSettingsStateVideoAutorecordChanged;
    hash[ParrotCommand::Tuple(1, 20, 6)] = Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged;
    hash[ParrotCommand::Tuple(1, 20, 7)] = Parrot::Ardrone3PictureSettingsStateVideoRecordingModeChanged;
    hash[ParrotCommand::Tuple(1, 20, 8)] = Parrot::Ardrone3PictureSettingsStateVideoFramerateChanged;
    hash[ParrotCommand::Tuple(1, 20, 9)] = Parrot::Ardrone3PictureSettingsStateVideoResolutionsChanged;
    hash[ParrotCommand::Tuple(1, 21, 0)] = Parrot::Ardrone3MediaStreamingVideoEnable;
    hash[ParrotCommand::Tuple(1, 21, 1)] = Parrot::Ardrone3MediaStreamingVideoStreamMode;
    hash[ParrotCommand::Tuple(1, 22, 0)] = Parrot::Ardrone3MediaStreamingStateVideoEnableChanged;
    hash[ParrotCommand::Tuple(1, 22, 1)] = Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged;
    hash[ParrotCommand::Tuple(1, 23, 2)] = Parrot::Ardrone3GPSSettingsSendControllerGPS;
    hash[ParrotCommand::Tuple(1, 23, 3)] = Parrot::Ardrone3GPSSettingsHomeType;
    hash[ParrotCommand::Tuple(1, 23, 4)] = Parrot::Ardrone3GPSSettingsReturnHomeDelay;
    hash[ParrotCommand::Tuple(1, 24, 0)] = Parrot::Ardrone3GPSSettingsStateHomeChanged;
    hash[ParrotCommand::Tuple(1, 24, 2)] = Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged;
    hash[ParrotCommand::Tuple(1, 24, 4)] = Parrot::Ardrone3GPSSettingsStateHomeTypeChanged;
    hash[ParrotCommand::Tuple(1, 24, 5)] = Parrot::Ardrone3GPSSettingsStateReturnHomeDelayChanged;
    hash[ParrotCommand::Tuple(1, 24, 6)] = Parrot::Ardrone3GPSSettingsStateGeofenceCenterChanged;
    hash[ParrotCommand::Tuple(1, 25, 2)] = Parrot::Ardrone3CameraStateOrientationV2;
    hash[ParrotCommand::Tuple(1, 25, 3)] = Parrot::Ardrone3CameraStatedefaultCameraOrientationV2;
    hash[ParrotCommand::Tuple(1, 25, 4)] = Parrot::Ardrone3CameraStateVelocityRange;
    hash[ParrotCommand::Tuple(1, 29, 0)] = Parrot::Ardrone3AntiflickeringelectricFrequency;
    hash[ParrotCommand::Tuple(1, 29, 1)] = Parrot::Ardrone3AntiflickeringsetMode;
    hash[ParrotCommand::Tuple(1, 30, 0)] = Parrot::Ardrone3AntiflickeringStateelectricFrequencyChanged;
    hash[ParrotCommand::Tuple(1, 30, 1)] = Parrot::Ardrone3AntiflickeringStatemodeChanged;
    hash[ParrotCommand::Tuple(1, 31, 0)] = Parrot::Ardrone3GPSStateNumberOfSatelliteChanged;
    hash[ParrotCommand::Tuple(1, 31, 1)] = Parrot::Ardrone3GPSStateHomeTypeAvailabilityChanged;
    hash[ParrotCommand::Tuple(1, 31, 2)] = Parrot::Ardrone3GPSStateHomeTypeChosenChanged;
    hash[ParrotCommand::Tuple(1, 33, 1)] = Parrot::Ardrone3AccessoryStateBattery;
    hash[ParrotCommand::Tuple(1, 35, 0)] = Parrot::Ardrone3SoundStartAlertSound;
    hash[ParrotCommand::Tuple(1, 35, 1)] = Parrot::Ardrone3SoundStopAlertSound;
    hash[ParrotCommand::Tuple(1, 36, 0)] = Parrot::Ardrone3SoundStateAlertSound;

    return hash;
}

QHash<ParrotCommand::Tuple, Parrot::Command> ParrotCommand::m_commandForTuple = initCommandForTuple();

QHash<Parrot::Command, ParrotCommand::Tuple> initTupleForCommand()
{
    QHash<Parrot::Command, ParrotCommand::Tuple> hash;
    hash[Parrot::CommonNetworkEventDisconnection] = ParrotCommand::Tuple(0, 1, 0);
    hash[Parrot::CommonSettingsAllSettings] = ParrotCommand::Tuple(0, 2, 0);
    hash[Parrot::CommonSettingsReset] = ParrotCommand::Tuple(0, 2, 1);
    hash[Parrot::CommonSettingsProductName] = ParrotCommand::Tuple(0, 2, 2);
    hash[Parrot::CommonSettingsCountry] = ParrotCommand::Tuple(0, 2, 3);
    hash[Parrot::CommonSettingsAutoCountry] = ParrotCommand::Tuple(0, 2, 4);
    hash[Parrot::CommonSettingsStateAllSettingsChanged] = ParrotCommand::Tuple(0, 3, 0);
    hash[Parrot::CommonSettingsStateResetChanged] = ParrotCommand::Tuple(0, 3, 1);
    hash[Parrot::CommonSettingsStateProductNameChanged] = ParrotCommand::Tuple(0, 3, 2);
    hash[Parrot::CommonSettingsStateProductVersionChanged] = ParrotCommand::Tuple(0, 3, 3);
    hash[Parrot::CommonSettingsStateProductSerialHighChanged] = ParrotCommand::Tuple(0, 3, 4);
    hash[Parrot::CommonSettingsStateProductSerialLowChanged] = ParrotCommand::Tuple(0, 3, 5);
    hash[Parrot::CommonSettingsStateCountryChanged] = ParrotCommand::Tuple(0, 3, 6);
    hash[Parrot::CommonSettingsStateAutoCountryChanged] = ParrotCommand::Tuple(0, 3, 7);
    hash[Parrot::CommonCommonAllStates] = ParrotCommand::Tuple(0, 4, 0);
    hash[Parrot::CommonCommonCurrentDate] = ParrotCommand::Tuple(0, 4, 1);
    hash[Parrot::CommonCommonCurrentTime] = ParrotCommand::Tuple(0, 4, 2);
    hash[Parrot::CommonCommonReboot] = ParrotCommand::Tuple(0, 4, 3);
    hash[Parrot::CommonCommonStateAllStatesChanged] = ParrotCommand::Tuple(0, 5, 0);
    hash[Parrot::CommonCommonStateBatteryStateChanged] = ParrotCommand::Tuple(0, 5, 1);
    hash[Parrot::CommonCommonStateMassStorageStateListChanged] = ParrotCommand::Tuple(0, 5, 2);
    hash[Parrot::CommonCommonStateMassStorageInfoStateListChanged] = ParrotCommand::Tuple(0, 5, 3);
    hash[Parrot::CommonCommonStateCurrentDateChanged] = ParrotCommand::Tuple(0, 5, 4);
    hash[Parrot::CommonCommonStateCurrentTimeChanged] = ParrotCommand::Tuple(0, 5, 5);
    hash[Parrot::CommonCommonStateWifiSignalChanged] = ParrotCommand::Tuple(0, 5, 7);
    hash[Parrot::CommonCommonStateSensorsStatesListChanged] = ParrotCommand::Tuple(0, 5, 8);
    hash[Parrot::CommonCommonStateMassStorageContent] = ParrotCommand::Tuple(0, 5, 12);
    hash[Parrot::CommonCommonStateMassStorageContentForCurrentRun] = ParrotCommand::Tuple(0, 5, 13);
    hash[Parrot::CommonCommonStateVideoRecordingTimestamp] = ParrotCommand::Tuple(0, 5, 14);
    hash[Parrot::CommonOverHeatStateOverHeatRegulationChanged] = ParrotCommand::Tuple(0, 7, 1);
    hash[Parrot::CommonControllerisPiloting] = ParrotCommand::Tuple(0, 8, 0);
    hash[Parrot::CommonWifiSettingsOutdoorSetting] = ParrotCommand::Tuple(0, 9, 0);
    hash[Parrot::CommonWifiSettingsStateoutdoorSettingsChanged] = ParrotCommand::Tuple(0, 10, 0);
    hash[Parrot::CommonMavlinkStart] = ParrotCommand::Tuple(0, 11, 0);
    hash[Parrot::CommonMavlinkPause] = ParrotCommand::Tuple(0, 11, 1);
    hash[Parrot::CommonMavlinkStop] = ParrotCommand::Tuple(0, 11, 2);
    hash[Parrot::CommonMavlinkStateMavlinkFilePlayingStateChanged] = ParrotCommand::Tuple(0, 12, 0);
    hash[Parrot::CommonMavlinkStateMissionItemExecuted] = ParrotCommand::Tuple(0, 12, 2);
    hash[Parrot::CommonFlightPlanSettingsReturnHomeOnDisconnect] = ParrotCommand::Tuple(0, 32, 0);
    hash[Parrot::CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged] = ParrotCommand::Tuple(0, 33, 0);
    hash[Parrot::CommonCalibrationMagnetoCalibration] = ParrotCommand::Tuple(0, 13, 0);
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationStateChanged] = ParrotCommand::Tuple(0, 14, 0);
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationRequiredState] = ParrotCommand::Tuple(0, 14, 1);
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged] = ParrotCommand::Tuple(0, 14, 2);
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationStartedChanged] = ParrotCommand::Tuple(0, 14, 3);
    hash[Parrot::CommonCalibrationStatePitotCalibrationStateChanged] = ParrotCommand::Tuple(0, 14, 4);
    hash[Parrot::CommonCameraSettingsStateCameraSettingsChanged] = ParrotCommand::Tuple(0, 15, 0);
    hash[Parrot::CommonFlightPlanStateAvailabilityStateChanged] = ParrotCommand::Tuple(0, 17, 0);
    hash[Parrot::CommonFlightPlanStateComponentStateListChanged] = ParrotCommand::Tuple(0, 17, 1);
    hash[Parrot::CommonFlightPlanStateLockStateChanged] = ParrotCommand::Tuple(0, 17, 2);
    hash[Parrot::CommonFlightPlanEventStartingErrorEvent] = ParrotCommand::Tuple(0, 19, 0);
    hash[Parrot::CommonFlightPlanEventSpeedBridleEvent] = ParrotCommand::Tuple(0, 19, 1);
    hash[Parrot::CommonARLibsVersionsStateControllerLibARCommandsVersion] = ParrotCommand::Tuple(0, 18, 0);
    hash[Parrot::CommonARLibsVersionsStateSkyControllerLibARCommandsVersion] = ParrotCommand::Tuple(0, 18, 1);
    hash[Parrot::CommonARLibsVersionsStateDeviceLibARCommandsVersion] = ParrotCommand::Tuple(0, 18, 2);
    hash[Parrot::CommonRunStateRunIdChanged] = ParrotCommand::Tuple(0, 30, 0);
    hash[Parrot::CommonFactoryReset] = ParrotCommand::Tuple(0, 31, 0);
    hash[Parrot::Ardrone3PilotingFlatTrim] = ParrotCommand::Tuple(1, 0, 0);
    hash[Parrot::Ardrone3PilotingTakeOff] = ParrotCommand::Tuple(1, 0, 1);
    hash[Parrot::Ardrone3PilotingPCMD] = ParrotCommand::Tuple(1, 0, 2);
    hash[Parrot::Ardrone3PilotingLanding] = ParrotCommand::Tuple(1, 0, 3);
    hash[Parrot::Ardrone3PilotingEmergency] = ParrotCommand::Tuple(1, 0, 4);
    hash[Parrot::Ardrone3PilotingNavigateHome] = ParrotCommand::Tuple(1, 0, 5);
    hash[Parrot::Ardrone3PilotingmoveBy] = ParrotCommand::Tuple(1, 0, 7);
    hash[Parrot::Ardrone3PilotingUserTakeOff] = ParrotCommand::Tuple(1, 0, 8);
    hash[Parrot::Ardrone3PilotingmoveTo] = ParrotCommand::Tuple(1, 0, 10);
    hash[Parrot::Ardrone3PilotingCancelMoveTo] = ParrotCommand::Tuple(1, 0, 11);
    hash[Parrot::Ardrone3PilotingStartPilotedPOI] = ParrotCommand::Tuple(1, 0, 12);
    hash[Parrot::Ardrone3PilotingStopPilotedPOI] = ParrotCommand::Tuple(1, 0, 13);
    hash[Parrot::Ardrone3AnimationsFlip] = ParrotCommand::Tuple(1, 5, 0);
    hash[Parrot::Ardrone3CameraOrientationV2] = ParrotCommand::Tuple(1, 1, 1);
    hash[Parrot::Ardrone3CameraVelocity] = ParrotCommand::Tuple(1, 1, 2);
    hash[Parrot::Ardrone3MediaRecordPictureV2] = ParrotCommand::Tuple(1, 7, 2);
    hash[Parrot::Ardrone3MediaRecordVideoV2] = ParrotCommand::Tuple(1, 7, 3);
    hash[Parrot::Ardrone3MediaRecordStatePictureStateChangedV2] = ParrotCommand::Tuple(1, 8, 2);
    hash[Parrot::Ardrone3MediaRecordStateVideoStateChangedV2] = ParrotCommand::Tuple(1, 8, 3);
    hash[Parrot::Ardrone3MediaRecordEventPictureEventChanged] = ParrotCommand::Tuple(1, 3, 0);
    hash[Parrot::Ardrone3MediaRecordEventVideoEventChanged] = ParrotCommand::Tuple(1, 3, 1);
    hash[Parrot::Ardrone3PilotingStateFlatTrimChanged] = ParrotCommand::Tuple(1, 4, 0);
    hash[Parrot::Ardrone3PilotingStateFlyingStateChanged] = ParrotCommand::Tuple(1, 4, 1);
    hash[Parrot::Ardrone3PilotingStateAlertStateChanged] = ParrotCommand::Tuple(1, 4, 2);
    hash[Parrot::Ardrone3PilotingStateNavigateHomeStateChanged] = ParrotCommand::Tuple(1, 4, 3);
    hash[Parrot::Ardrone3PilotingStatePositionChanged] = ParrotCommand::Tuple(1, 4, 4);
    hash[Parrot::Ardrone3PilotingStateSpeedChanged] = ParrotCommand::Tuple(1, 4, 5);
    hash[Parrot::Ardrone3PilotingStateAttitudeChanged] = ParrotCommand::Tuple(1, 4, 6);
    hash[Parrot::Ardrone3PilotingStateAltitudeChanged] = ParrotCommand::Tuple(1, 4, 8);
    hash[Parrot::Ardrone3PilotingStateGpsLocationChanged] = ParrotCommand::Tuple(1, 4, 9);
    hash[Parrot::Ardrone3PilotingStatemoveToChanged] = ParrotCommand::Tuple(1, 4, 12);
    hash[Parrot::Ardrone3PilotingStateMotionState] = ParrotCommand::Tuple(1, 4, 13);
    hash[Parrot::Ardrone3PilotingStatePilotedPOI] = ParrotCommand::Tuple(1, 4, 14);
    hash[Parrot::Ardrone3PilotingStateReturnHomeBatteryCapacity] = ParrotCommand::Tuple(1, 4, 15);
    hash[Parrot::Ardrone3PilotingEventmoveByEnd] = ParrotCommand::Tuple(1, 34, 0);
    hash[Parrot::Ardrone3NetworkWifiScan] = ParrotCommand::Tuple(1, 13, 0);
    hash[Parrot::Ardrone3NetworkWifiAuthChannel] = ParrotCommand::Tuple(1, 13, 1);
    hash[Parrot::Ardrone3NetworkStateWifiScanListChanged] = ParrotCommand::Tuple(1, 14, 0);
    hash[Parrot::Ardrone3NetworkStateAllWifiScanChanged] = ParrotCommand::Tuple(1, 14, 1);
    hash[Parrot::Ardrone3NetworkStateWifiAuthChannelListChanged] = ParrotCommand::Tuple(1, 14, 2);
    hash[Parrot::Ardrone3NetworkStateAllWifiAuthChannelChanged] = ParrotCommand::Tuple(1, 14, 3);
    hash[Parrot::Ardrone3PilotingSettingsMaxAltitude] = ParrotCommand::Tuple(1, 2, 0);
    hash[Parrot::Ardrone3PilotingSettingsMaxTilt] = ParrotCommand::Tuple(1, 2, 1);
    hash[Parrot::Ardrone3PilotingSettingsMaxDistance] = ParrotCommand::Tuple(1, 2, 3);
    hash[Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance] = ParrotCommand::Tuple(1, 2, 4);
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed] = ParrotCommand::Tuple(1, 2, 5);
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed] = ParrotCommand::Tuple(1, 2, 6);
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration] = ParrotCommand::Tuple(1, 2, 7);
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration] = ParrotCommand::Tuple(1, 2, 8);
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed] = ParrotCommand::Tuple(1, 2, 9);
    hash[Parrot::Ardrone3PilotingSettingsBankedTurn] = ParrotCommand::Tuple(1, 2, 10);
    hash[Parrot::Ardrone3PilotingSettingsSetMotionDetectionMode] = ParrotCommand::Tuple(1, 2, 16);
    hash[Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged] = ParrotCommand::Tuple(1, 6, 0);
    hash[Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged] = ParrotCommand::Tuple(1, 6, 1);
    hash[Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged] = ParrotCommand::Tuple(1, 6, 3);
    hash[Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged] = ParrotCommand::Tuple(1, 6, 4);
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed] = ParrotCommand::Tuple(1, 6, 5);
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed] = ParrotCommand::Tuple(1, 6, 6);
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration] = ParrotCommand::Tuple(1, 6, 7);
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration] = ParrotCommand::Tuple(1, 6, 8);
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed] = ParrotCommand::Tuple(1, 6, 9);
    hash[Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged] = ParrotCommand::Tuple(1, 6, 10);
    hash[Parrot::Ardrone3PilotingSettingsStateMotionDetection] = ParrotCommand::Tuple(1, 6, 16);
    hash[Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed] = ParrotCommand::Tuple(1, 11, 0);
    hash[Parrot::Ardrone3SpeedSettingsMaxRotationSpeed] = ParrotCommand::Tuple(1, 11, 1);
    hash[Parrot::Ardrone3SpeedSettingsHullProtection] = ParrotCommand::Tuple(1, 11, 2);
    hash[Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed] = ParrotCommand::Tuple(1, 11, 4);
    hash[Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged] = ParrotCommand::Tuple(1, 12, 0);
    hash[Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged] = ParrotCommand::Tuple(1, 12, 1);
    hash[Parrot::Ardrone3SpeedSettingsStateHullProtectionChanged] = ParrotCommand::Tuple(1, 12, 2);
    hash[Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged] = ParrotCommand::Tuple(1, 12, 4);
    hash[Parrot::Ardrone3NetworkSettingsWifiSelection] = ParrotCommand::Tuple(1, 9, 0);
    hash[Parrot::Ardrone3NetworkSettingswifiSecurity] = ParrotCommand::Tuple(1, 9, 1);
    hash[Parrot::Ardrone3NetworkSettingsStateWifiSelectionChanged] = ParrotCommand::Tuple(1, 10, 0);
    hash[Parrot::Ardrone3NetworkSettingsStatewifiSecurity] = ParrotCommand::Tuple(1, 10, 2);
    hash[Parrot::Ardrone3SettingsStateProductGPSVersionChanged] = ParrotCommand::Tuple(1, 16, 1);
    hash[Parrot::Ardrone3SettingsStateMotorErrorStateChanged] = ParrotCommand::Tuple(1, 16, 2);
    hash[Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged] = ParrotCommand::Tuple(1, 16, 4);
    hash[Parrot::Ardrone3SettingsStateMotorErrorLastErrorChanged] = ParrotCommand::Tuple(1, 16, 5);
    hash[Parrot::Ardrone3SettingsStateCPUID] = ParrotCommand::Tuple(1, 16, 7);
    hash[Parrot::Ardrone3PictureSettingsPictureFormatSelection] = ParrotCommand::Tuple(1, 19, 0);
    hash[Parrot::Ardrone3PictureSettingsAutoWhiteBalanceSelection] = ParrotCommand::Tuple(1, 19, 1);
    hash[Parrot::Ardrone3PictureSettingsExpositionSelection] = ParrotCommand::Tuple(1, 19, 2);
    hash[Parrot::Ardrone3PictureSettingsSaturationSelection] = ParrotCommand::Tuple(1, 19, 3);
    hash[Parrot::Ardrone3PictureSettingsTimelapseSelection] = ParrotCommand::Tuple(1, 19, 4);
    hash[Parrot::Ardrone3PictureSettingsVideoAutorecordSelection] = ParrotCommand::Tuple(1, 19, 5);
    hash[Parrot::Ardrone3PictureSettingsVideoStabilizationMode] = ParrotCommand::Tuple(1, 19, 6);
    hash[Parrot::Ardrone3PictureSettingsVideoRecordingMode] = ParrotCommand::Tuple(1, 19, 7);
    hash[Parrot::Ardrone3PictureSettingsVideoFramerate] = ParrotCommand::Tuple(1, 19, 8);
    hash[Parrot::Ardrone3PictureSettingsVideoResolutions] = ParrotCommand::Tuple(1, 19, 9);
    hash[Parrot::Ardrone3PictureSettingsStatePictureFormatChanged] = ParrotCommand::Tuple(1, 20, 0);
    hash[Parrot::Ardrone3PictureSettingsStateAutoWhiteBalanceChanged] = ParrotCommand::Tuple(1, 20, 1);
    hash[Parrot::Ardrone3PictureSettingsStateExpositionChanged] = ParrotCommand::Tuple(1, 20, 2);
    hash[Parrot::Ardrone3PictureSettingsStateSaturationChanged] = ParrotCommand::Tuple(1, 20, 3);
    hash[Parrot::Ardrone3PictureSettingsStateTimelapseChanged] = ParrotCommand::Tuple(1, 20, 4);
    hash[Parrot::Ardrone3PictureSettingsStateVideoAutorecordChanged] = ParrotCommand::Tuple(1, 20, 5);
    hash[Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged] = ParrotCommand::Tuple(1, 20, 6);
    hash[Parrot::Ardrone3PictureSettingsStateVideoRecordingModeChanged] = ParrotCommand::Tuple(1, 20, 7);
    hash[Parrot::Ardrone3PictureSettingsStateVideoFramerateChanged] = ParrotCommand::Tuple(1, 20, 8);
    hash[Parrot::Ardrone3PictureSettingsStateVideoResolutionsChanged] = ParrotCommand::Tuple(1, 20, 9);
    hash[Parrot::Ardrone3MediaStreamingVideoEnable] = ParrotCommand::Tuple(1, 21, 0);
    hash[Parrot::Ardrone3MediaStreamingVideoStreamMode] = ParrotCommand::Tuple(1, 21, 1);
    hash[Parrot::Ardrone3MediaStreamingStateVideoEnableChanged] = ParrotCommand::Tuple(1, 22, 0);
    hash[Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged] = ParrotCommand::Tuple(1, 22, 1);
    hash[Parrot::Ardrone3GPSSettingsSendControllerGPS] = ParrotCommand::Tuple(1, 23, 2);
    hash[Parrot::Ardrone3GPSSettingsHomeType] = ParrotCommand::Tuple(1, 23, 3);
    hash[Parrot::Ardrone3GPSSettingsReturnHomeDelay] = ParrotCommand::Tuple(1, 23, 4);
    hash[Parrot::Ardrone3GPSSettingsStateHomeChanged] = ParrotCommand::Tuple(1, 24, 0);
    hash[Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged] = ParrotCommand::Tuple(1, 24, 2);
    hash[Parrot::Ardrone3GPSSettingsStateHomeTypeChanged] = ParrotCommand::Tuple(1, 24, 4);
    hash[Parrot::Ardrone3GPSSettingsStateReturnHomeDelayChanged] = ParrotCommand::Tuple(1, 24, 5);
    hash[Parrot::Ardrone3GPSSettingsStateGeofenceCenterChanged] = ParrotCommand::Tuple(1, 24, 6);
    hash[Parrot::Ardrone3CameraStateOrientationV2] = ParrotCommand::Tuple(1, 25, 2);
    hash[Parrot::Ardrone3CameraStatedefaultCameraOrientationV2] = ParrotCommand::Tuple(1, 25, 3);
    hash[Parrot::Ardrone3CameraStateVelocityRange] = ParrotCommand::Tuple(1, 25, 4);
    hash[Parrot::Ardrone3AntiflickeringelectricFrequency] = ParrotCommand::Tuple(1, 29, 0);
    hash[Parrot::Ardrone3AntiflickeringsetMode] = ParrotCommand::Tuple(1, 29, 1);
    hash[Parrot::Ardrone3AntiflickeringStateelectricFrequencyChanged] = ParrotCommand::Tuple(1, 30, 0);
    hash[Parrot::Ardrone3AntiflickeringStatemodeChanged] = ParrotCommand::Tuple(1, 30, 1);
    hash[Parrot::Ardrone3GPSStateNumberOfSatelliteChanged] = ParrotCommand::Tuple(1, 31, 0);
    hash[Parrot::Ardrone3GPSStateHomeTypeAvailabilityChanged] = ParrotCommand::Tuple(1, 31, 1);
    hash[Parrot::Ardrone3GPSStateHomeTypeChosenChanged] = ParrotCommand::Tuple(1, 31, 2);
    hash[Parrot::Ardrone3AccessoryStateBattery] = ParrotCommand::Tuple(1, 33, 1);
    hash[Parrot::Ardrone3SoundStartAlertSound] = ParrotCommand::Tuple(1, 35, 0);
    hash[Parrot::Ardrone3SoundStopAlertSound] = ParrotCommand::Tuple(1, 35, 1);
    hash[Parrot::Ardrone3SoundStateAlertSound] = ParrotCommand::Tuple(1, 36, 0);

    return hash;
}

QHash<Parrot::Command, ParrotCommand::Tuple> ParrotCommand::m_tupleForCommand = initTupleForCommand();

QHash<Parrot::Command, QVector<Parrot::ArgumentType>> initSignatureForCommand()
{
    QHash<Parrot::Command, QVector<Parrot::ArgumentType>> hash;
    hash[Parrot::CommonNetworkEventDisconnection] = {Parrot::TYPE_ENUM};
    hash[Parrot::CommonSettingsProductName] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsCountry] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsAutoCountry] = {Parrot::TYPE_U8};
    hash[Parrot::CommonSettingsStateProductNameChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsStateProductVersionChanged] = {Parrot::TYPE_STRING, Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsStateProductSerialHighChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsStateProductSerialLowChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsStateCountryChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonSettingsStateAutoCountryChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonCommonCurrentDate] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonCommonCurrentTime] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonCommonStateBatteryStateChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonCommonStateMassStorageStateListChanged] = {Parrot::TYPE_U8, Parrot::TYPE_STRING};
    hash[Parrot::CommonCommonStateMassStorageInfoStateListChanged] = {Parrot::TYPE_U8, Parrot::TYPE_U32, Parrot::TYPE_U32, Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::CommonCommonStateCurrentDateChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonCommonStateCurrentTimeChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonCommonStateWifiSignalChanged] = {Parrot::TYPE_I16};
    hash[Parrot::CommonCommonStateSensorsStatesListChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::CommonCommonStateMassStorageContent] = {Parrot::TYPE_U8, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16};
    hash[Parrot::CommonCommonStateMassStorageContentForCurrentRun] = {Parrot::TYPE_U8, Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U16};
    hash[Parrot::CommonCommonStateVideoRecordingTimestamp] = {Parrot::TYPE_U64, Parrot::TYPE_U64};
    hash[Parrot::CommonOverHeatStateOverHeatRegulationChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonControllerisPiloting] = {Parrot::TYPE_U8};
    hash[Parrot::CommonWifiSettingsOutdoorSetting] = {Parrot::TYPE_U8};
    hash[Parrot::CommonWifiSettingsStateoutdoorSettingsChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonMavlinkStart] = {Parrot::TYPE_STRING, Parrot::TYPE_ENUM};
    hash[Parrot::CommonMavlinkStateMavlinkFilePlayingStateChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_STRING, Parrot::TYPE_ENUM};
    hash[Parrot::CommonMavlinkStateMissionItemExecuted] = {Parrot::TYPE_U32};
    hash[Parrot::CommonFlightPlanSettingsReturnHomeOnDisconnect] = {Parrot::TYPE_U8};
    hash[Parrot::CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged] = {Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::CommonCalibrationMagnetoCalibration] = {Parrot::TYPE_U8};
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationStateChanged] = {Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationRequiredState] = {Parrot::TYPE_U8};
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::CommonCalibrationStateMagnetoCalibrationStartedChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonCalibrationStatePitotCalibrationStateChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::CommonCameraSettingsStateCameraSettingsChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::CommonFlightPlanStateAvailabilityStateChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonFlightPlanStateComponentStateListChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::CommonFlightPlanStateLockStateChanged] = {Parrot::TYPE_U8};
    hash[Parrot::CommonARLibsVersionsStateControllerLibARCommandsVersion] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonARLibsVersionsStateSkyControllerLibARCommandsVersion] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonARLibsVersionsStateDeviceLibARCommandsVersion] = {Parrot::TYPE_STRING};
    hash[Parrot::CommonRunStateRunIdChanged] = {Parrot::TYPE_STRING};
    hash[Parrot::Ardrone3PilotingPCMD] = {Parrot::TYPE_U8, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_U32};
    hash[Parrot::Ardrone3PilotingNavigateHome] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingmoveBy] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingUserTakeOff] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingmoveTo] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_ENUM, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingStartPilotedPOI] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE};
    hash[Parrot::Ardrone3AnimationsFlip] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3CameraOrientationV2] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3CameraVelocity] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3MediaRecordVideoV2] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaRecordStatePictureStateChangedV2] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaRecordStateVideoStateChangedV2] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaRecordEventPictureEventChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaRecordEventVideoEventChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStateFlyingStateChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStateAlertStateChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStateNavigateHomeStateChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStatePositionChanged] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE};
    hash[Parrot::Ardrone3PilotingStateSpeedChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingStateAttitudeChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingStateAltitudeChanged] = {Parrot::TYPE_DOUBLE};
    hash[Parrot::Ardrone3PilotingStateGpsLocationChanged] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_I8, Parrot::TYPE_I8, Parrot::TYPE_I8};
    hash[Parrot::Ardrone3PilotingStatemoveToChanged] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_ENUM, Parrot::TYPE_FLOAT, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStateMotionState] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStatePilotedPOI] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingStateReturnHomeBatteryCapacity] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PilotingEventmoveByEnd] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3NetworkWifiScan] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3NetworkStateWifiScanListChanged] = {Parrot::TYPE_STRING, Parrot::TYPE_I16, Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3NetworkStateWifiAuthChannelListChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingSettingsMaxAltitude] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsMaxTilt] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsMaxDistance] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsBankedTurn] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingSettingsSetMotionDetectionMode] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PilotingSettingsStateMotionDetection] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3SpeedSettingsMaxRotationSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3SpeedSettingsHullProtection] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3SpeedSettingsStateHullProtectionChanged] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3NetworkSettingsWifiSelection] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3NetworkSettingswifiSecurity] = {Parrot::TYPE_ENUM, Parrot::TYPE_STRING, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3NetworkSettingsStateWifiSelectionChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3NetworkSettingsStatewifiSecurity] = {Parrot::TYPE_ENUM, Parrot::TYPE_STRING, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3SettingsStateProductGPSVersionChanged] = {Parrot::TYPE_STRING, Parrot::TYPE_STRING};
    hash[Parrot::Ardrone3SettingsStateMotorErrorStateChanged] = {Parrot::TYPE_U8, Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged] = {Parrot::TYPE_U16, Parrot::TYPE_U16, Parrot::TYPE_U32};
    hash[Parrot::Ardrone3SettingsStateMotorErrorLastErrorChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3SettingsStateCPUID] = {Parrot::TYPE_STRING};
    hash[Parrot::Ardrone3PictureSettingsPictureFormatSelection] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsAutoWhiteBalanceSelection] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsExpositionSelection] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PictureSettingsSaturationSelection] = {Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PictureSettingsTimelapseSelection] = {Parrot::TYPE_U8, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PictureSettingsVideoAutorecordSelection] = {Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PictureSettingsVideoStabilizationMode] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsVideoRecordingMode] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsVideoFramerate] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsVideoResolutions] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsStatePictureFormatChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsStateAutoWhiteBalanceChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsStateExpositionChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PictureSettingsStateSaturationChanged] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PictureSettingsStateTimelapseChanged] = {Parrot::TYPE_U8, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3PictureSettingsStateVideoAutorecordChanged] = {Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsStateVideoRecordingModeChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsStateVideoFramerateChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3PictureSettingsStateVideoResolutionsChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaStreamingVideoEnable] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3MediaStreamingVideoStreamMode] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaStreamingStateVideoEnableChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3GPSSettingsSendControllerGPS] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE};
    hash[Parrot::Ardrone3GPSSettingsHomeType] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3GPSSettingsReturnHomeDelay] = {Parrot::TYPE_U16};
    hash[Parrot::Ardrone3GPSSettingsStateHomeChanged] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE};
    hash[Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3GPSSettingsStateHomeTypeChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3GPSSettingsStateReturnHomeDelayChanged] = {Parrot::TYPE_U16};
    hash[Parrot::Ardrone3GPSSettingsStateGeofenceCenterChanged] = {Parrot::TYPE_DOUBLE, Parrot::TYPE_DOUBLE};
    hash[Parrot::Ardrone3CameraStateOrientationV2] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3CameraStatedefaultCameraOrientationV2] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3CameraStateVelocityRange] = {Parrot::TYPE_FLOAT, Parrot::TYPE_FLOAT};
    hash[Parrot::Ardrone3AntiflickeringelectricFrequency] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3AntiflickeringsetMode] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3AntiflickeringStateelectricFrequencyChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3AntiflickeringStatemodeChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3GPSStateNumberOfSatelliteChanged] = {Parrot::TYPE_U8};
    hash[Parrot::Ardrone3GPSStateHomeTypeAvailabilityChanged] = {Parrot::TYPE_ENUM, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3GPSStateHomeTypeChosenChanged] = {Parrot::TYPE_ENUM};
    hash[Parrot::Ardrone3AccessoryStateBattery] = {Parrot::TYPE_U8, Parrot::TYPE_U8, Parrot::TYPE_U8};
    hash[Parrot::Ardrone3SoundStateAlertSound] = {Parrot::TYPE_ENUM};

    return hash;
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
