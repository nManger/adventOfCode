namespace day9
{
    public class FindXmaxWeakness
    {
        protected const int PREAMBLELENGTH = 25;
        private List<long> XmaxNumberList { get; set; }

        public FindXmaxWeakness(List<long> xmasNumberList)
        {
            XmaxNumberList = xmasNumberList;
        }

        public long FindIrregularNumber()
        {
            long currentNumber = 0;
            bool pairFound = false;
            for (int i = PREAMBLELENGTH; i < XmaxNumberList.Count; i++)
            {
                //Find combination of two numbers representing current number
                pairFound = false;
                currentNumber = XmaxNumberList[i];
                for (int j= i- PREAMBLELENGTH; j<i ; j++)
                {
                    
                    for( int k= j; k<i ; k++)
                    {
                        if (XmaxNumberList[j] + XmaxNumberList[k] == currentNumber)
                        {
                            pairFound = true;
                            break;
                        }
                        if(pairFound) break;
                    }
                }
                if(!pairFound) break;
            }

            return currentNumber;
        }

        public long[] FindEncryptionWeakness()
        {
            long irregNumber = this.FindIrregularNumber();
            long[] numRange = new long[2];
            
            for(int i = 0; i < XmaxNumberList.Count; i++)
            {
                long sum = 0;
                int j = i;
                numRange[0] = XmaxNumberList[i];
                numRange[1] = XmaxNumberList[i];
                while (sum < irregNumber)
                {
                    sum += XmaxNumberList[j];
                    if (XmaxNumberList[j] < numRange[0])
                    {
                        numRange[0] = XmaxNumberList[j];
                    }
                    else if (XmaxNumberList[j] > numRange[1])
                    {
                        numRange[1] = XmaxNumberList[j];
                    }
                    j++;
                }
                if(sum == irregNumber)
                {
                    break;
                }
            }
            return numRange;
        }
    }
    
    internal class Program
    {
        static void Main(string[] args)
        {
            //Parse puzzle input into list
            string fileName = "puzzleInput.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<long> xmasNumberList = new List<long>();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while ((nextLine = myReader.ReadLine()) != null) { 
                    xmasNumberList.Add(long.Parse(nextLine));
                }
            }

            FindXmaxWeakness xmas = new FindXmaxWeakness(xmasNumberList);

            long irregularNumber = xmas.FindIrregularNumber();

            Console.WriteLine("First irregular number found: {0}", irregularNumber);

            long[] numRange = xmas.FindEncryptionWeakness();
            Console.WriteLine("minRange = {0}, maxRange = {1}, encryption weakness = {2}",
                numRange[0], numRange[1], (numRange[0] + numRange[1]));
        }
    }
}