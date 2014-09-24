using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AMD.Util.terminal {
    public class AT_commands {
        #region call
        public const String AT_ANSWER = "ATA";
        public const String AT_HANG_UP = "ATH";
        public const String AT_DIAL = "ATD";
        public const String AT_SHOW_NUM = "AT+CNUM";
        #endregion

        #region diagnostic
        public const String AT_MODEL_NO = "AT+CGMM";
        public const String AT_MANUFACTURER = "AT+CGMI";
        #endregion

        #region connection
        public const String AT_ATTACH_STATUS = "AT+CGATT?";
        #endregion
    }
}
