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

#pragma once

#include <QObject>

class Parrot : public QObject
{
public:
    Parrot()
        : QObject()
    {
    }
    ~Parrot()
    {
    }

    enum HeaderSizes { HEADER_COMMAND = 4, HEADER_FRAME = 7 };
    Q_ENUM(HeaderSizes)

    enum DataType { ACK = 1, DATA = 2, LOW_LATENCY_DATA = 3, DATA_WITH_ACK = 4 };
    Q_ENUM(DataType)

    enum BufferId {
        BUFFER_PING = 0,
        BUFFER_PONG = 1,
        BUFFER_C2D_NON_ACK_DATA = 10,
        BUFFER_C2D_ACK_DATA = 11,
        BUFFER_C2D_EMERGENCY_DATA = 12,
        BUFFER_D2C_ACK_DATA = 126,
        BUFFER_D2C_NON_ACK_DATA = 127,
        BUFFER_D2C_ACK_FOR_C2D_ACK_DATA = BUFFER_C2D_ACK_DATA + 128
    };
    Q_ENUM(BufferId)

    enum ArgumentType { TYPE_U8 = 0, TYPE_I8 = 1, TYPE_U16 = 2, TYPE_I16 = 3, TYPE_U32 = 4, TYPE_I32 = 5, TYPE_U64 = 6, TYPE_I64 = 7, TYPE_FLOAT = 8, TYPE_DOUBLE = 9, TYPE_STRING = 10, TYPE_ENUM = 11 };
    Q_ENUM(ArgumentType)

    enum Command {
        UnknownCommand = 0,
        Ardrone3AccessoryStateBattery = 1,
        Ardrone3AnimationsFlip = 2,
        Ardrone3AntiflickeringStateelectricFrequencyChanged = 3,
        Ardrone3AntiflickeringStatemodeChanged = 4,
        Ardrone3AntiflickeringelectricFrequency = 5,
        Ardrone3AntiflickeringsetMode = 6,
        Ardrone3CameraOrientationV2 = 7,
        Ardrone3CameraStateOrientationV2 = 8,
        Ardrone3CameraStateVelocityRange = 9,
        Ardrone3CameraStatedefaultCameraOrientationV2 = 10,
        Ardrone3CameraVelocity = 11,
        Ardrone3GPSSettingsHomeType = 12,
        Ardrone3GPSSettingsReturnHomeDelay = 13,
        Ardrone3GPSSettingsSendControllerGPS = 14,
        Ardrone3GPSSettingsStateGPSFixStateChanged = 15,
        Ardrone3GPSSettingsStateGeofenceCenterChanged = 16,
        Ardrone3GPSSettingsStateHomeChanged = 17,
        Ardrone3GPSSettingsStateHomeTypeChanged = 18,
        Ardrone3GPSSettingsStateReturnHomeDelayChanged = 19,
        Ardrone3GPSStateHomeTypeAvailabilityChanged = 20,
        Ardrone3GPSStateHomeTypeChosenChanged = 21,
        Ardrone3GPSStateNumberOfSatelliteChanged = 22,
        Ardrone3MediaRecordEventPictureEventChanged = 23,
        Ardrone3MediaRecordEventVideoEventChanged = 24,
        Ardrone3MediaRecordPictureV2 = 25,
        Ardrone3MediaRecordStatePictureStateChangedV2 = 26,
        Ardrone3MediaRecordStateVideoStateChangedV2 = 27,
        Ardrone3MediaRecordVideoV2 = 28,
        Ardrone3MediaStreamingStateVideoEnableChanged = 29,
        Ardrone3MediaStreamingStateVideoStreamModeChanged = 30,
        Ardrone3MediaStreamingVideoEnable = 31,
        Ardrone3MediaStreamingVideoStreamMode = 32,
        Ardrone3NetworkSettingsStateWifiSelectionChanged = 33,
        Ardrone3NetworkSettingsStatewifiSecurity = 34,
        Ardrone3NetworkSettingsWifiSelection = 35,
        Ardrone3NetworkSettingswifiSecurity = 36,
        Ardrone3NetworkStateAllWifiAuthChannelChanged = 37,
        Ardrone3NetworkStateAllWifiScanChanged = 38,
        Ardrone3NetworkStateWifiAuthChannelListChanged = 39,
        Ardrone3NetworkStateWifiScanListChanged = 40,
        Ardrone3NetworkWifiAuthChannel = 41,
        Ardrone3NetworkWifiScan = 42,
        Ardrone3PictureSettingsAutoWhiteBalanceSelection = 43,
        Ardrone3PictureSettingsExpositionSelection = 44,
        Ardrone3PictureSettingsPictureFormatSelection = 45,
        Ardrone3PictureSettingsSaturationSelection = 46,
        Ardrone3PictureSettingsStateAutoWhiteBalanceChanged = 47,
        Ardrone3PictureSettingsStateExpositionChanged = 48,
        Ardrone3PictureSettingsStatePictureFormatChanged = 49,
        Ardrone3PictureSettingsStateSaturationChanged = 50,
        Ardrone3PictureSettingsStateTimelapseChanged = 51,
        Ardrone3PictureSettingsStateVideoAutorecordChanged = 52,
        Ardrone3PictureSettingsStateVideoFramerateChanged = 53,
        Ardrone3PictureSettingsStateVideoRecordingModeChanged = 54,
        Ardrone3PictureSettingsStateVideoResolutionsChanged = 55,
        Ardrone3PictureSettingsStateVideoStabilizationModeChanged = 56,
        Ardrone3PictureSettingsTimelapseSelection = 57,
        Ardrone3PictureSettingsVideoAutorecordSelection = 58,
        Ardrone3PictureSettingsVideoFramerate = 59,
        Ardrone3PictureSettingsVideoRecordingMode = 60,
        Ardrone3PictureSettingsVideoResolutions = 61,
        Ardrone3PictureSettingsVideoStabilizationMode = 62,
        Ardrone3PilotingCancelMoveTo = 63,
        Ardrone3PilotingEmergency = 64,
        Ardrone3PilotingEventmoveByEnd = 65,
        Ardrone3PilotingFlatTrim = 66,
        Ardrone3PilotingLanding = 67,
        Ardrone3PilotingNavigateHome = 68,
        Ardrone3PilotingPCMD = 69,
        Ardrone3PilotingSettingsBankedTurn = 70,
        Ardrone3PilotingSettingsMaxAltitude = 71,
        Ardrone3PilotingSettingsMaxDistance = 72,
        Ardrone3PilotingSettingsMaxTilt = 73,
        Ardrone3PilotingSettingsNoFlyOverMaxDistance = 74,
        Ardrone3PilotingSettingsSetMotionDetectionMode = 75,
        Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalAcceleration = 76,
        Ardrone3PilotingSettingsStateAutonomousFlightMaxHorizontalSpeed = 77,
        Ardrone3PilotingSettingsStateAutonomousFlightMaxRotationSpeed = 78,
        Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalAcceleration = 79,
        Ardrone3PilotingSettingsStateAutonomousFlightMaxVerticalSpeed = 80,
        Ardrone3PilotingSettingsStateBankedTurnChanged = 81,
        Ardrone3PilotingSettingsStateMaxAltitudeChanged = 82,
        Ardrone3PilotingSettingsStateMaxDistanceChanged = 83,
        Ardrone3PilotingSettingsStateMaxTiltChanged = 84,
        Ardrone3PilotingSettingsStateMotionDetection = 85,
        Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged = 86,
        Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalAcceleration = 87,
        Ardrone3PilotingSettingssetAutonomousFlightMaxHorizontalSpeed = 88,
        Ardrone3PilotingSettingssetAutonomousFlightMaxRotationSpeed = 89,
        Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalAcceleration = 90,
        Ardrone3PilotingSettingssetAutonomousFlightMaxVerticalSpeed = 91,
        Ardrone3PilotingStartPilotedPOI = 92,
        Ardrone3PilotingStateAlertStateChanged = 93,
        Ardrone3PilotingStateAltitudeChanged = 94,
        Ardrone3PilotingStateAttitudeChanged = 95,
        Ardrone3PilotingStateFlatTrimChanged = 96,
        Ardrone3PilotingStateFlyingStateChanged = 97,
        Ardrone3PilotingStateGpsLocationChanged = 98,
        Ardrone3PilotingStateMotionState = 99,
        Ardrone3PilotingStateNavigateHomeStateChanged = 100,
        Ardrone3PilotingStatePilotedPOI = 101,
        Ardrone3PilotingStatePositionChanged = 102,
        Ardrone3PilotingStateReturnHomeBatteryCapacity = 103,
        Ardrone3PilotingStateSpeedChanged = 104,
        Ardrone3PilotingStatemoveToChanged = 105,
        Ardrone3PilotingStopPilotedPOI = 106,
        Ardrone3PilotingTakeOff = 107,
        Ardrone3PilotingUserTakeOff = 108,
        Ardrone3PilotingmoveBy = 109,
        Ardrone3PilotingmoveTo = 110,
        Ardrone3SettingsStateCPUID = 111,
        Ardrone3SettingsStateMotorErrorLastErrorChanged = 112,
        Ardrone3SettingsStateMotorErrorStateChanged = 113,
        Ardrone3SettingsStateMotorFlightsStatusChanged = 114,
        Ardrone3SettingsStateProductGPSVersionChanged = 115,
        Ardrone3SoundStartAlertSound = 116,
        Ardrone3SoundStateAlertSound = 117,
        Ardrone3SoundStopAlertSound = 118,
        Ardrone3SpeedSettingsHullProtection = 119,
        Ardrone3SpeedSettingsMaxPitchRollRotationSpeed = 120,
        Ardrone3SpeedSettingsMaxRotationSpeed = 121,
        Ardrone3SpeedSettingsMaxVerticalSpeed = 122,
        Ardrone3SpeedSettingsStateHullProtectionChanged = 123,
        Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged = 124,
        Ardrone3SpeedSettingsStateMaxRotationSpeedChanged = 125,
        Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged = 126,
        CommonARLibsVersionsStateControllerLibARCommandsVersion = 127,
        CommonARLibsVersionsStateDeviceLibARCommandsVersion = 128,
        CommonARLibsVersionsStateSkyControllerLibARCommandsVersion = 129,
        CommonCalibrationMagnetoCalibration = 130,
        CommonCalibrationStateMagnetoCalibrationAxisToCalibrateChanged = 131,
        CommonCalibrationStateMagnetoCalibrationRequiredState = 132,
        CommonCalibrationStateMagnetoCalibrationStartedChanged = 133,
        CommonCalibrationStateMagnetoCalibrationStateChanged = 134,
        CommonCalibrationStatePitotCalibrationStateChanged = 135,
        CommonCameraSettingsStateCameraSettingsChanged = 136,
        CommonCommonAllStates = 137,
        CommonCommonCurrentDate = 138,
        CommonCommonCurrentTime = 139,
        CommonCommonReboot = 140,
        CommonCommonStateAllStatesChanged = 141,
        CommonCommonStateBatteryStateChanged = 142,
        CommonCommonStateCurrentDateChanged = 143,
        CommonCommonStateCurrentTimeChanged = 144,
        CommonCommonStateMassStorageContent = 145,
        CommonCommonStateMassStorageContentForCurrentRun = 146,
        CommonCommonStateMassStorageInfoStateListChanged = 147,
        CommonCommonStateMassStorageStateListChanged = 148,
        CommonCommonStateSensorsStatesListChanged = 149,
        CommonCommonStateVideoRecordingTimestamp = 150,
        CommonCommonStateWifiSignalChanged = 151,
        CommonControllerisPiloting = 152,
        CommonFactoryReset = 153,
        CommonFlightPlanEventSpeedBridleEvent = 154,
        CommonFlightPlanEventStartingErrorEvent = 155,
        CommonFlightPlanSettingsReturnHomeOnDisconnect = 156,
        CommonFlightPlanSettingsStateReturnHomeOnDisconnectChanged = 157,
        CommonFlightPlanStateAvailabilityStateChanged = 158,
        CommonFlightPlanStateComponentStateListChanged = 159,
        CommonFlightPlanStateLockStateChanged = 160,
        CommonMavlinkPause = 161,
        CommonMavlinkStart = 162,
        CommonMavlinkStateMavlinkFilePlayingStateChanged = 163,
        CommonMavlinkStateMissionItemExecuted = 164,
        CommonMavlinkStop = 165,
        CommonNetworkEventDisconnection = 166,
        CommonOverHeatStateOverHeatRegulationChanged = 167,
        CommonRunStateRunIdChanged = 168,
        CommonSettingsAllSettings = 169,
        CommonSettingsAutoCountry = 170,
        CommonSettingsCountry = 171,
        CommonSettingsProductName = 172,
        CommonSettingsReset = 173,
        CommonSettingsStateAllSettingsChanged = 174,
        CommonSettingsStateAutoCountryChanged = 175,
        CommonSettingsStateCountryChanged = 176,
        CommonSettingsStateProductNameChanged = 177,
        CommonSettingsStateProductSerialHighChanged = 178,
        CommonSettingsStateProductSerialLowChanged = 179,
        CommonSettingsStateProductVersionChanged = 180,
        CommonSettingsStateResetChanged = 181,
        CommonWifiSettingsOutdoorSetting = 182,
        CommonWifiSettingsStateoutdoorSettingsChanged = 183,
        Bla = 184
    };
    Q_ENUM(Command)
};

class ParrotCommand
{
public:
    struct Tuple {
        quint8 productId;
        quint8 classId;
        quint16 commandId;

        inline bool operator==(const Tuple &t) const
        {
            return (productId == t.productId && classId == t.classId && commandId == t.commandId);
        }
    };

    explicit ParrotCommand(Parrot::Command command, const QVariantList &arguments = QVariantList());
    explicit ParrotCommand(Parrot::Command command, const QByteArray &arguments = QByteArray());
    explicit ParrotCommand(const QByteArray &data);
    explicit ParrotCommand();
    ~ParrotCommand();

    QByteArray pack() const;
    Parrot::Command id() const;

    Tuple tuple;
    QByteArray data;

private:
    static Parrot::Command commandForTuple(const Tuple &tuple);
    static Tuple tupleForCommand(Parrot::Command command);

    static QHash<Tuple, Parrot::Command> m_commandForTuple;
    static QHash<Parrot::Command, Tuple> m_tupleForCommand;
    static QHash<Parrot::Command, QVector<Parrot::ArgumentType>> m_signatureForCommand;
};

Q_DECLARE_METATYPE(ParrotCommand)

inline uint qHash(const ParrotCommand::Tuple &t)
{
    return t.productId * t.classId * t.commandId;
}

class ParrotFrame
{
public:
    explicit ParrotFrame(quint8 dataType, quint8 bufferId, quint8 seq, const QByteArray &data = QByteArray());
    explicit ParrotFrame(const QByteArray &data, int start);
    ~ParrotFrame();

    QByteArray pack() const;
    ParrotCommand command() const;

    quint8 dataType = 0;
    quint8 bufferId = 0;
    quint8 seq = 0;
    quint32 size = 0;
    QByteArray data;
    int retry = 0; // NOTE: Setting to -1 before sending means infinite retries.
};
