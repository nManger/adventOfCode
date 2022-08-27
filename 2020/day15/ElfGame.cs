using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace day15
{
    internal class ElfGame
    {
        private Dictionary<int, int> _numberLastOccurence;
        private int[] _startingNumbers; 
        public ElfGame(int[] startingNumbers)
        {
            this._numberLastOccurence = new Dictionary<int, int>();
            this._startingNumbers = startingNumbers;
        }
        public void ResetGame()
        {
            this._numberLastOccurence = new Dictionary<int, int>();
        }
        public void ResetGame(int[] newStartingNumbers)
        {
            this._numberLastOccurence = new Dictionary<int, int>();
            this._startingNumbers = newStartingNumbers;
        }

        public int SimulateGame(int stopTurn)
        {
            //play starting rounds
            for(int i = 0; i < this._startingNumbers.Length-1; i++)
            {
                this._numberLastOccurence.Add(this._startingNumbers[i], i+1);
            }    

            int number = this._startingNumbers[this._startingNumbers.Length-1];
            for(int i = this._startingNumbers.Length+1; i < stopTurn+1; i++)
            {
                number = this.NextTurn(i, number);
            }    
            return number;
        }

        private int NextTurn(int turn,int lastNumber)
        {
            int nextNumber = 0;
            //if number has already been spoken
            if(this._numberLastOccurence.ContainsKey(lastNumber))
            {
                nextNumber = turn - 1 - this._numberLastOccurence[lastNumber];
                this._numberLastOccurence[lastNumber] = turn -1;
            }
            else //if number is new
            {
                this._numberLastOccurence.Add(lastNumber,turn-1);
            }
            //Console.WriteLine("Turn {0}: last number = {1}, next number = {2}",turn,lastNumber,nextNumber);
            return nextNumber;
        }

    }

}
