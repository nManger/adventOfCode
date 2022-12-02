namespace day13
{
    internal class Program
    {
        static List<string> ReadInputData()
        {
            string fileName = "puzzleInput.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<string> rawData = new();
            using (var myReader = new StreamReader(filePath))
            {
                // First line is your arrival time
                string nextLine = myReader.ReadLine();
                rawData.Add(nextLine);

                // Second line is ','-separated list of bus numbers
                nextLine = myReader.ReadLine();
                string[] busTimes = nextLine.Split(',');
                foreach (string busTime in busTimes) rawData.Add(busTime);
            }
            return rawData;
        }

        static long inv(long a, long m)
        {
            long m0 = m, t, q;
            long x0 = 0, x1 = 1;

            if (m == 1)
                return 0;

            // Apply extended Euclid Algorithm
            // calculate gcd and a*x+b*y = gcd(a,b) (y is not calculated)
            while (a > 1)
            {
                // q is quotient
                q = a / m;

                t = m;
                // m is remainder now, process same as euclid's algorithm
                m = a % m; a = t;

                t = x0;

                x0 = x1 - q * x0;

                x1 = t;
            }

            // Make x1 positive
            if (x1 < 0)
                x1 += m0;

            return x1;
        }

        static long FindMinTimestamp(int[] num, int[] rem, int k)
        {
            // Compute product
            // of all numbers
            long prod = 1;
            for (int i = 0; i < k; i++)
                prod *= num[i];

            // Initialize result
            long result = 0;

            // Apply above formula
            for (int i = 0; i < k; i++)
            {
                long pp = prod / num[i];
                result += rem[i] * inv(pp, num[i]) * pp;
            }

            return result % prod;
        }

        static void Main(string[] args)
        {
            List<string> rawData = ReadInputData();
            
            int myArrivalTime = int.Parse(rawData[0]);

            int earliestBus = 0;
            int timeToEarliestBus = myArrivalTime;
            for(int i = 1; i < rawData.Count; i++)
            {
                // entries x are ignored for now
                if (rawData[i] == "x") continue;

                int busNumber = int.Parse(rawData[i]);
                int nextBusArrival = (myArrivalTime/busNumber +1)*busNumber;
                int timeToNextBus = nextBusArrival-myArrivalTime;
                if(timeToNextBus < timeToEarliestBus)
                {
                    earliestBus = busNumber;
                    timeToEarliestBus = timeToNextBus;
                }
            }
            Console.WriteLine("ID of earliest bus: {0}, Wait time: {1}, ID x Wait time: {2}",
                earliestBus,timeToEarliestBus,earliestBus*timeToEarliestBus);

            //prepare data for part 2: List all bus IDs and corresponding arrival delays vs time t
            List<int> busIDs = new List<int>();
            List<int> delayVsFirstBus = new List<int>();
            int t;
            for(int i = 1; i < rawData.Count; i++)
            {
                // entries x are ignored
                if (rawData[i] == "x") continue;

                t = int.Parse(rawData[i]);
                busIDs.Add(t);
                delayVsFirstBus.Add(t-(i-1));
            }
            long minTimestamp = FindMinTimestamp(busIDs.ToArray(), delayVsFirstBus.ToArray(), busIDs.Count);

            for(int i=0; i<busIDs.Count; i++)
            {
                Console.WriteLine(busIDs[i] + " " + delayVsFirstBus[i]);
                 
            }

            Console.WriteLine("Fist timestamp at which busses arrive with specified delays: {0}", minTimestamp);

        }
    }
}