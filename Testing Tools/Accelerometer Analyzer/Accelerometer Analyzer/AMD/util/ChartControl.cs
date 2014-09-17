using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace Accelerometer_Analyzer.AMD.util {
    class ChartControl {
        public static void ChartSetup(Chart chart, String seriesName, int borderWidth, Color color, SeriesChartType chartType, ChartValueType xValueType) {
            Series newSeries = new Series(seriesName);
            newSeries.ChartType = chartType;
            newSeries.BorderWidth = borderWidth;
            newSeries.Color = color;
            newSeries.XValueType = xValueType;
            chart.Series.Add(newSeries);

        }
    }
}
