using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace day5
{
    
    internal class BoardingPass
    {
        private string _seatCode;
        private int _seatRow;
        private int _seatColumn;
        private int _seatID;

        public BoardingPass(string seatCode)
        {
            _seatCode = seatCode;
        }
        public string SeatCode { get; set; }
        public int SeatRow { get; private set; }
        public int SeatColumn { get; private set; }
        public int SeatID { get; private set; }

        public int DecodeSeatPosition()
        {
            string rowCode = _seatCode.Substring(0, 7);
            string columnCode = _seatCode.Substring(7);

            int rowMin = 0;
            int rowMax = 127;
            int rowMid;
            for(int i = 0; i < rowCode.Length; i++)
            {
                rowMid = (rowMax + rowMin) / 2;
                if (rowCode[i] =='F')
                {
                    rowMax = rowMid;
                }
                else
                {
                    rowMin = rowMid+1;
                }
            }
            this.SeatRow = rowMin;

            int colMax = 7;
            int colMin = 0;
            int colMid;
            for(int i = 0; i < columnCode.Length; i++)
            {
                colMid = (colMax + colMin) / 2;
                if (columnCode[i] == 'L')
                {
                    colMax = colMid;
                }
                else
                {
                    colMin = colMid+1;
                }
            }
            this.SeatColumn = colMin;
            //Console.WriteLine(rowMin.ToString() + " " + colMin.ToString());


            this.SeatID = this.SeatRow*8 + this.SeatColumn;
            return this.SeatID;
        }

    }
}
