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
        public const String AT_CALL_SHOW_NUM = "AT+CNUM";
        #endregion

        #region message
        public const String AT_MSG_READ = "AT+CMGR="; // Add number in end to select msg to read
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

        #region diagnostic
        public const String AT_DIAG_MODEL_NO = "AT+CGMM";
        public const String AT_DIAG_MANUFACTURER = "AT+CGMI";
        public const String AT_DIAG_GET_ERROR_REPORT = "AT+CEER?";
        #endregion

        #region connection
        public const String AT_CONN_SIGNAL_STRENGTH = "AT+CSQ";
        public const String AT_CONN_ATTACH_STATUS = "AT+CGATT?";
        public const String AT_CONN_NETWORK_REGISTRATION_STATUS = "AT+CREG?";
        #endregion
    }
}
