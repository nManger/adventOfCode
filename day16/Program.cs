namespace day16
{
    internal class Program
    {
        static void readInputFile(string fileName, out Dictionary<string, Tuple<int, int>[]> ticketFields,
                                  out int[] myTicket, out List<int[]> otherTickets)
        {
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            
            using (var myReader = new StreamReader(filePath))
            {

                ticketFields = new Dictionary<string, Tuple<int, int>[]>();
                string? nextLine;
                while(!string.IsNullOrEmpty(nextLine = myReader.ReadLine()))
                {
                    string[] split1 = nextLine.Split(':');
                    string[] split2 = split1[1].Split("or",StringSplitOptions.TrimEntries);

                    Tuple<int, int>[] ranges = new Tuple<int, int>[2];
                    int i = 0;
                    foreach(string subString in split2)
                    {
                        string[] part = subString.Split('-');
                        ranges[i] = Tuple.Create(int.Parse(part[0]), int.Parse(part[1]));
                        i++;
                    }
                    ticketFields.Add(split1[0],ranges);
                }

                nextLine = myReader.ReadLine(); //line "your Ticket"
                nextLine = myReader.ReadLine(); //line containing my ticket numbers
                
                string[] subStrings = nextLine.Split(',');
                myTicket = new int[subStrings.Length];
                for(int i = 0; i < subStrings.Length; i++)
                {
                    myTicket[i] = int.Parse(subStrings[i]);
                }

                nextLine = myReader.ReadLine(); //empty lines
                nextLine = myReader.ReadLine(); //line "nearby Tickets"

                otherTickets = new List<int[]>();
                while (!string.IsNullOrEmpty(nextLine = myReader.ReadLine()))
                {
                    subStrings = nextLine.Split(',');
                    int[] ticket  = new int[subStrings.Length];
                    for (int i = 0; i < subStrings.Length; i++)
                    {
                        ticket[i] = int.Parse(subStrings[i]);
                    }
                    otherTickets.Add(ticket);
                }
            }
        }

        static int FindTicketScanningErrorRate(Dictionary<string, Tuple<int, int>[]> ticketFields,
                                               List<int[]> otherTickets)
        {
            int errorRate = 0;
            bool validFieldFound;
            foreach (int[] ticket in otherTickets)
            {
                for(int i=0; i<ticket.Length; i++)
                {
                    validFieldFound = false;
                    foreach (Tuple<int,int>[] fieldRanges in ticketFields.Values)
                    {
                        if ((ticket[i] >= fieldRanges[0].Item1 && ticket[i] <= fieldRanges[0].Item2) || 
                            (ticket[i] >= fieldRanges[1].Item1 && ticket[i] <= fieldRanges[1].Item2))
                        {
                            validFieldFound = true;
                            break;
                        }
                    }
                    if(!validFieldFound)
                    {
                        //Console.WriteLine(ticket[i].ToString());
                        errorRate+= ticket[i];
                    }
                }
            }
            return errorRate;
        }

        static List<int[]> RemoveInvalidTickets(Dictionary<string, Tuple<int, int>[]> ticketFields, List<int[]> otherTickets)
        {
            bool validFieldFound;
            bool ticketIsValid;
            List<int[]> validTickets = new List<int[]>();
            foreach (int[] ticket in otherTickets)
            {
                ticketIsValid = true;
                for (int i = 0; i < ticket.Length; i++)
                {
                    validFieldFound = false;
                    foreach (Tuple<int, int>[] fieldRanges in ticketFields.Values)
                    {
                        if ((ticket[i] >= fieldRanges[0].Item1 && ticket[i] <= fieldRanges[0].Item2) ||
                            (ticket[i] >= fieldRanges[1].Item1 && ticket[i] <= fieldRanges[1].Item2))
                        {
                            validFieldFound = true;
                            break;
                        }
                    }
                    if (!validFieldFound) //if number is not valid in any field, ticket must be invalid
                    {
                        ticketIsValid = false;
                        break;
                    }
                }
                //if all numbers in ticket are valid for at least one range, copy to valid ticket list
                if(ticketIsValid)
                {
                    validTickets.Add(ticket);
                }
            }
            return validTickets;
        }

        static string[] DetermineFieldOrder(Dictionary<string, Tuple<int, int>[]> ticketFields, List<int[]> otherTickets)
        {
            string[] orderedFieldList = new string[otherTickets[0].Length];

            Dictionary<string, List<int>> possibleFieldPositions = new();
            bool fieldCandidate;

            foreach(var ticketField in ticketFields)
            {
                possibleFieldPositions.Add(ticketField.Key, new List<int>());
                for(int i = 0; i < otherTickets[0].Length; i++)
                {
                    fieldCandidate = true;
                    for (int j=0; j< otherTickets.Count; j++)
                    {
                        //if any value in row is not in field range, field does not represent row 
                        if (!((otherTickets[j][i] >= ticketField.Value[0].Item1 && otherTickets[j][i] <= ticketField.Value[0].Item2) ||
                              (otherTickets[j][i] >= ticketField.Value[1].Item1 && otherTickets[j][i] <= ticketField.Value[1].Item2)))
                        {
                            fieldCandidate = false;
                            break;
                        }
                    } 
                    if(fieldCandidate)
                    {
                        possibleFieldPositions[ticketField.Key].Add(i);
                    }
                }
                
            }

            int positionedFields = 0; //track number of positioned fields
            while(positionedFields < orderedFieldList.Length)
            {
                foreach(var field in possibleFieldPositions)
                {
                    //skip fields that still have more than one possible placement
                    //or have already been placed
                    if (field.Value.Count != 1) continue;

                    //fix field position if only one possible position remains
                    orderedFieldList[field.Value[0]] = field.Key;
                    positionedFields++;

                    //remove this position as possibility for all fields (field.Value will become empty)
                    int pos = field.Value[0];
                    foreach (var tmp in possibleFieldPositions) //foreach does shallow copy ?!
                    {
                        if (tmp.Value.Contains(pos))
                        {
                            tmp.Value.Remove(pos);
                        }
                    }
                }
            }
            return orderedFieldList;
        }

        static long ReadMyTicket(string[] orderedFieldList, int[] myTicket)
        {
            long result = 1;
            for(int i = 0; i < myTicket.Length; i++)
            {
                if (orderedFieldList[i].Contains("departure"))
                {
                    result *= myTicket[i];
                }
            }
            return result;
        }
        static void Main(string[] args)
        {
            Dictionary<string, Tuple<int, int>[]> ticketFields;
            int[] myTicket;
            List<int[]> otherTickets;

            readInputFile("puzzleInput.txt",out ticketFields,out myTicket,out otherTickets);

            int errorRate = FindTicketScanningErrorRate(ticketFields, otherTickets);
            Console.WriteLine("Ticket scanning error rate = {0}",errorRate);
            Console.WriteLine();

            //assign valid ticket list to otherTickets, GC will collect & discard all data of invalid tickets
            otherTickets = RemoveInvalidTickets(ticketFields,otherTickets);

            string[] orderedFieldsList = DetermineFieldOrder(ticketFields, otherTickets);
            foreach (string field in orderedFieldsList)
            {
                Console.WriteLine(field);
            }
            Console.WriteLine();
            long ticketValue = ReadMyTicket(orderedFieldsList, myTicket);
            Console.WriteLine("Product of all departure values on my ticket = {0}", ticketValue);
        }
    }
}