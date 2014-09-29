using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AMD.Util.terminal {
    public class AT_commands {
        #region call
        public const String AT_CALL_ANSWER = "ATA";
        public const String AT_CALL_HANG_UP = "ATH";
        public const String AT_CALL_DIAL = "ATD";
        public const String AT_CALL_SHOW_CALLER_ID = "AT+CLIP="; // Add number
        #endregion

        #region message
        public const String AT_MSG_READ = "AT+CMGR="; // Add number in end to select msg to read
        public const String AT_MSG_DEL = "AT+CMGD="; // Add number and add "AT_MSG_DEL_END after that
        public const String AT_MSG_DEL_END = ",0";
        public const String AT_MSG_DEL_ALL = "AT+CMGD=1,4";
        public const String AT_MSG_FORMAT = "AT+CMGF ="; // Add number
        public const String AT_MSG_SEND = "AT+CMGS\"";
        #endregion

        #region audio
        public const String AT_AUDIO_GET_RINGER_VOLUME = "AT+CRSL?";
        public const String AT_AUDIO_SET_RINGER_VOLUME = "AT+CRSL="; // Add number 0-4
        public const String AT_AUDIO_GET_SPEAKER_VOLUME = "AT+CLVL?";
        public const String AT_AUDIO_SET_SPEAKER_VOLUME = "AT+CLVL="; // Add number 0-100
        public const String AT_AUDIO_GET_CURRENT_ALERT_SOUND = "AT+CALS?";
        public const String AT_AUDIO_SET_CURRENT_ALERT_SOUND = "AT+CALS="; // Add number 0-19
        #endregion

        #region state
        public const String AT_STATE_ON = "AT+CFUN=1";
        public const String AT_STATE_OFF = "AT+CFUN=0";
        #endregion

        #region diagnostic
        public const String AT_DIAG_MODEL_NO = "AT+CGMM";
        public const String AT_DIAG_MANUFACTURER = "AT+CGMI";
        public const String AT_DIAG_GET_ERROR_REPORT = "AT+CEER?";
        #endregion

        #region connection
        public const String AT_CONN_SIGNAL_STRENGTH = "AT+CSQ";
        public const String AT_CONN_ATTACH_STATUS = "AT+CGATT?";
        public const String AT_CONN_NETWORK_REGISTRATION_STATUS = "AT+CREG?";
        public const String AT_CONN_OPERATOR_NAME = "AT+COPS?";
        #endregion

        #region GPS
        public const String AT_GPS_POWER_STATE = "AT+CGPSPWR?";
        public const String AT_GPS_POWER_ON = "AT+CGPSPWR=1";
        public const String AT_GPS_POWER_OFF = "AT+CGPSPWR=0";
        public const String AT_GPS_RST_MODE = "AT+CGPSRST?";
        public const String AT_GPS_RST_COLD = "AT+CGPSRST=0";
        public const String AT_GPS_RST_AUTONOMY = "AT+CGPSRST=1";
        public const String AT_GPS_GET_LOCATION = "AT+CGPSINF=0"; // Response <mode>,<longitude>,<latitude>,<altitude>,<UTCtime>,<TTFF>,<num>,<speed>,<course >
        public const String AT_GPS_GET_STATUS = "AT+CGPSSTATUS?";
        #endregion
    }
}
