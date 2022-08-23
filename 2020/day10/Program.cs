namespace day10
{
    internal class Program
    {
        static int CalcJoltDiff(int min, int max)
        {
            return max - min;
        }

        static long FindNumCombinations(List<int> adapterList, int currentStep)
        {
            //We reached the end of a chain: return 1 found chain
            if (currentStep == adapterList.Count-1)
            {
                return 1;
            }

            //Loop over next adapters until joltage difference is too big
            //Call function on itself for all valid adapters to continue chain
            //Add up all valid found chains and return number to previous level
            long numCombinations = 0;
            int i = 1;
            while ( (currentStep+i < adapterList.Count) && (adapterList[currentStep + i] - adapterList[currentStep] <=3) )
            {
                numCombinations += FindNumCombinations(adapterList, currentStep+i);
                i++;
            }
            return numCombinations;
        }

        public static long FindAllArrangements(List<int>adapterList)
        {
            long numArrangements = 1;
            int i = 0;
            while(i<adapterList.Count-1)
            {
                //find next point in list where difference between successive adapters is 3
                //Calculate number of paths for the sublist of items between current index and this point
                
                if (adapterList[i + 1] < adapterList[i] + 3)
                {
                    int j;
                    for (j = i + 1; j < adapterList.Count-1; j++)
                    {
                        if(adapterList[j+1] == adapterList[j]+3) break;
                    }
                    List<int> subList = adapterList.GetRange(i, j-i+1);
                    //foreach(int item in subList) Console.Write(item+" ");
                    //Console.WriteLine();
                    //Console.WriteLine(FindNumCombinations(subList, 0));

                    //Multiply number of paths for each sublist to get total number of paths
                    numArrangements *= FindNumCombinations(subList, 0);
                    
                    //set outer index to end of current sublist
                    i = j+1;
                }
                else // if difference to next adapter is 3 skip search and continue search with next item in list
                {
                    i++;
                }
                 
            }
            return numArrangements;

        }

        static void Main(string[] args)
        {
            //Parse puzzle input into list
            string fileName = "puzzleInput.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<int> adapterList = new List<int>();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while ((nextLine = myReader.ReadLine()) != null)
                {
                    adapterList.Add(int.Parse(nextLine));
                }
            }

            adapterList.Sort();
            adapterList.Insert(0, 0); //add difference from outlet (always 0) to first adapter

            int[] joltDiffCount = new int[] { 0, 0, 0 };
            int joltDiff = 0;
            for (int i = 1; i < adapterList.Count; i++)
            {

                joltDiff = CalcJoltDiff(adapterList[i - 1], adapterList[i]);

                if (joltDiff > 0 && joltDiff <= 3)
                {
                    joltDiffCount[joltDiff - 1]++;
                }
                else
                {
                    throw new Exception("Adapter list invalid: Jolt difference not in range 1 to 3");
                }
            }
            joltDiffCount[2]++; //Add difference (always 3) from last adapter to built-in adapter 
            Console.WriteLine("1 jolts: {0}, 2 jolts: {1}, 3 jolts: {2}",
                joltDiffCount[0], joltDiffCount[1], joltDiffCount[2]);
            Console.WriteLine("num 1 jolts times num 3 jolts: {0}", joltDiffCount[0] * joltDiffCount[2]);

            adapterList.Add(adapterList[adapterList.Count-1]+3);
            long numberChains = FindAllArrangements(adapterList);
            Console.WriteLine("Number of possible chains: {0}", numberChains);
            //for (int i = 0; i < adapterList.Count; i++) Console.Write(adapterList[i] + " ");
        }
    }
}