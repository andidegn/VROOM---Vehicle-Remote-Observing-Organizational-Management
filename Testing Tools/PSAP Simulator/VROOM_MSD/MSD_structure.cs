using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace VROOM_MSD
{
    class MSD_structure
    {
        public Byte version { get; private set; }
        public Byte msg_identifier { get; private set; }
        public Byte control { get; private set; }
        public Byte vehicle_class { get; private set; }
        public String VIN { get; private set; }
        public Byte fuel_type { get; private set; }
        public UInt32 UTC_sec { get; private set; }
        public Int32 latutude { get; private set; }
        public Int32 longitude { get; private set; }
        public Byte direction { get; private set; }
        public String optional { get; private set; }

        private DateTime UTC_time_stamp;
        private Dictionary<String,byte[]> MSD_Data_bin;
        private Byte[] MSD;

        public MSD_structure()
        {
            MSD_Data_bin = new Dictionary<String, byte[]>();
            UTC_time_stamp = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
        }

        public void AddNewMSD(String key, Byte[] vroom_file_data)
        {
            MSD_Data_bin[key] = vroom_file_data;
        }

        public void DeleteMSD(String key)
        {
            MSD_Data_bin.Remove(key);
        }

        public String GetMSDHexString(String key)
        {
            StringBuilder sb = new StringBuilder();

            foreach (byte item in MSD_Data_bin[key])
            {
                sb.Append(String.Format("0x{0:X2} ", item));
            }

            return sb.ToString();
        }

        public void DecodeMSD(String key)
        {
            int i = 0;
            int offset = 0;

            MSD = MSD_Data_bin[key];

            version = MSD[offset++];
            msg_identifier = MSD[offset++];
            control = MSD[offset++];
            vehicle_class = MSD[offset++];

            VIN = GetString(offset, 20);
            offset += 20;

            fuel_type = MSD[offset++];

            for (i = 3; i >= 0; i--)
            {
                UTC_sec = BitConverter.ToUInt32(MSD, (i + offset));
                latutude = BitConverter.ToInt32(MSD, (i + offset + 4));
                longitude = BitConverter.ToInt32(MSD, (i + offset + 8));
            }
            offset += 12;

            direction = MSD[offset++];

            optional = GetString(offset, 102);
        }

        public String GetControlByteString()
        {
            return Convert.ToString(control, 2).PadLeft(8, '0');
        }

        private String GetString(int offset, int number_of_bytes)
        {
            StringBuilder sb = new StringBuilder();

            for (int i = offset; i < number_of_bytes; i++)
                sb.Append(Convert.ToChar(MSD[i]));

            return sb.ToString();
        }

        public String GetVehicleClass()
        {
            byte classification = (byte)((vehicle_class & 0xF0) >> 4);
            byte category = (byte)(vehicle_class & 0x0F);
            string vehicle_class_str;

            switch (classification)
            {
                case 1: vehicle_class_str = String.Format("L{0}", category); break;
                case 2: vehicle_class_str = String.Format("M{0}", category); break;
                case 3: vehicle_class_str = String.Format("N{0}", category); break;
                case 4: vehicle_class_str = String.Format("O{0}", category); break;
                case 5: vehicle_class_str = String.Format("T{0}", category); break;
                case 6: vehicle_class_str = String.Format("G{0}", category); break;
                case 7: vehicle_class_str = String.Format("SA{0}", category); break;
                default: vehicle_class_str = "Error"; break;
            }

            return vehicle_class_str;
        }

        public String GetFuelType()
        {
            string fuel_type_str;
            switch (fuel_type)
            {
                case 0: fuel_type_str = "Other"; break;
                case 1: fuel_type_str = "Gasoline"; break;
                case 2: fuel_type_str = "Diesel"; break;
                case 3: fuel_type_str = "Natural Gas"; break;
                case 4: fuel_type_str = "Propane"; break;
                case 5: fuel_type_str = "Electric"; break;
                case 6: fuel_type_str = "Hydrogen"; break;
                default: fuel_type_str = "Error"; break;
            }

            return fuel_type_str;
        }

        public Boolean ControlAutoAlarm()
        {
            return (((control >> 7) & 1) == 1);
        }

        public Boolean ControlManualAlarm()
        {
            return (((control >> 6) & 1) == 1);
        }

        public Boolean ControlTestCall()
        {
            return (((control >> 5) & 1) == 1);
        }

        public Boolean ControlPositionTrusted()
        {
            return (((control >> 4) & 1) == 1);
        }

        public Double GetLatitudeDD()
        {
            return latutude / 3600000.0;
        }

        public Double GetLongitudeDD()
        {
            return longitude / 3600000.0;
        }

        public DateTime GetTimeStamp()
        {
           return UTC_time_stamp.AddSeconds(UTC_sec).ToLocalTime();
        }
    }
}
