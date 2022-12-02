namespace day11
{
    
    internal class Program
    {
        static char[,] readInputData()
        {
            string fileName = "puzzleInput.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<string> rawData = new List<string>();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while ((nextLine = myReader.ReadLine()) != null)
                {
                    rawData.Add(nextLine);
                }
            }

            char[,] floorPlan = new char[rawData.Count+2, rawData[0].Length+2];
            char[] line;
            for (int i = 1; i < rawData.Count+1; i++)
            {
                line = rawData[i-1].ToCharArray();
                for (int j = 1; j < line.Length+1; j++)
                {
                    floorPlan[i, j] = line[j-1];
                }
                floorPlan[i, 0] = '.';
                floorPlan[i, floorPlan.GetLength(1)-1] = '.';
            }

            for(int i=0; i<floorPlan.GetLength(1);i++)
            {
                floorPlan[0,i] = '.';
            }
            return floorPlan;
        }

        static char[,] AdvanceSeatChart(char[,] floorPlan)
        {
            char[,] nextFloorPlan = new char[floorPlan.GetLength(0), floorPlan.GetLength(1)];
            for (int i = 1; i < floorPlan.GetLength(0)-1; i++)
                {
                for (int j = 1; j < floorPlan.GetLength(1) - 1; j++)
                {
                    //if field is floor, copy and continue
                    if (floorPlan[i, j] == '.')
                    {
                        nextFloorPlan[i, j] = '.';
                        continue;
                    }

                    //if field is chair,check if it changes state
                    //count number of neighboring occupied seats
                    int nOccupiedSeats = 0;
                    for (int k = -1; k < 2; k++)
                    {
                        for (int l = -1; l < 2; l++)
                        {
                            if (floorPlan[i + k, j + l] == '#')
                            {
                                if (k == 0 && l == 0) continue; //do not count seat itself
                                nOccupiedSeats++;
                            }
                        }
                    }
                    //if seat is occupied, empty it if adjacent seat count >=4
                    if (floorPlan[i, j] == '#' && nOccupiedSeats >= 4)
                    {
                        nextFloorPlan[i, j] = 'L';
                    }
                    //if seat is empty, occupy it if adjacent seat count == 0
                    else if (floorPlan[i, j] == 'L' && nOccupiedSeats == 0)
                    {
                        nextFloorPlan[i, j] = '#';
                    }
                    //otherwise do not change seat assignment
                    else
                    {
                        nextFloorPlan[i, j] = floorPlan[i, j];
                    }
                }
            }
            return nextFloorPlan;
        }

        static char[,] AdvanceSeatChartV2(char[,] floorPlan)
        {
            char[,] nextFloorPlan = new char[floorPlan.GetLength(0), floorPlan.GetLength(1)];
            for (int i = 1; i < floorPlan.GetLength(0) - 1; i++)
            {
                for (int j = 1; j < floorPlan.GetLength(1) - 1; j++)
                {
                    //if field is floor, copy and continue
                    if (floorPlan[i, j] == '.')
                    {
                        nextFloorPlan[i, j] = '.';
                        continue;
                    }

                    //if field is chair,check if it changes state
                    //count number of visible neighboring occupied seats
                    int nOccupiedSeats = 0;
                    //up
                    int k = 1;
                    while(i - k>0)
                    {
                        if (floorPlan[i-k,j] != '.')
                        {
                            if(floorPlan[i - k, j] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }
                    
                    //down
                    k = 1;
                    while (i + k < floorPlan.GetLength(0)-1)
                    {
                        if (floorPlan[i + k, j] != '.')
                        {
                            if (floorPlan[i + k, j] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //left
                    k = 1;
                    while (j - k > 0)
                    {
                        if (floorPlan[i, j - k] != '.')
                        {
                            if (floorPlan[i, j - k] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //right
                    k = 1;
                    while (j + k < floorPlan.GetLength(1)-1)
                    {
                        if (floorPlan[i, j + k] != '.')
                        {
                            if (floorPlan[i, j + k] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //up-left
                    k = 1;
                    while (i - k > 0 && j - k > 0)
                    {
                        if (floorPlan[i - k, j - k] != '.')
                        {
                            if (floorPlan[i - k, j - k] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //up-right
                    k = 1;
                    while (i - k > 0 && j + k < floorPlan.GetLength(1)-1)
                    {
                        if (floorPlan[i - k, j + k] != '.')
                        {
                            if (floorPlan[i - k, j + k] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //down-left
                    k = 1;
                    while (i + k < floorPlan.GetLength(0)-1 && j - k > 0)
                    {
                        if (floorPlan[i + k, j - k] != '.')
                        {
                            if (floorPlan[i + k, j - k] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //down-right
                    k = 1;
                    while (i + k < floorPlan.GetLength(0)-1 && j + k < floorPlan.GetLength(1)-1)
                    {
                        if (floorPlan[i + k, j + k] != '.')
                        {
                            if (floorPlan[i + k, j + k] == '#') nOccupiedSeats++;
                            break;
                        }
                        k++;
                    }

                    //if seat is occupied, empty it if adjacent seat count >=5
                    if (floorPlan[i, j] == '#' && nOccupiedSeats >= 5)
                    {
                        nextFloorPlan[i, j] = 'L';
                    }
                    //if seat is empty, occupy it if adjacent seat count  == 0
                    else if (floorPlan[i, j] == 'L' && nOccupiedSeats == 0)
                    {
                        nextFloorPlan[i, j] = '#';
                    }
                    //otherwise do not change seat assignment
                    else
                    {
                        nextFloorPlan[i, j] = floorPlan[i, j];
                    }
                }
            }
            return nextFloorPlan;
        }

        static void Main(string[] args)
        {
            char[,] floorPlan = readInputData();
            
            //Loop repeatedly over seating chart until it does not change
            bool seatingChanged = true;
            while(seatingChanged == true)
            {
                seatingChanged = false;
                char[,] newFloorPlan = AdvanceSeatChartV2(floorPlan);
                for (int i = 1; i < newFloorPlan.GetLength(0)-1; i++)
                {
                    for(int j= 1; j < newFloorPlan.GetLength(1)-1; j++)
                    {
                        if (newFloorPlan[i, j] != floorPlan[i, j])
                        {
                            seatingChanged=true;
                        }
                        floorPlan[i, j] = newFloorPlan[i, j];
                        //Console.Write(newFloorPlan[i, j]);
                    }    
                    //Console.WriteLine();
                }
                //Console.WriteLine();
            }

            //Count number of occupied seats
            int nOccupiedSeats = 0;
            for (int i = 0; i < floorPlan.GetLength(0); i++)
            {
                for (int j = 0; j < floorPlan.GetLength(1); j++)
                {
                    if (floorPlan[i, j] == '#') nOccupiedSeats++;
                }
            }
            Console.WriteLine("Number of occupied seats in steady state : {0}",nOccupiedSeats);


        }
    }
}