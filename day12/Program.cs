
namespace day12
{
    internal class Program
    {
        static List<Tuple<char,int>> ReadInputData()
        {
            string fileName = "puzzleInput.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<Tuple<char, int>> directionsList = new ();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while ((nextLine = myReader.ReadLine()) != null)
                {
                    directionsList.Add(new Tuple<char,int> (nextLine[0], int.Parse(nextLine.Substring(1))));
                }
            }
            return directionsList;
        }

        static int[] PlotCourse(List<Tuple<char,int>> directionsList)
        {
            //coordinates[0]: +North/-South, coordinates[1]: +East/-West
            int[] coordinates = new int[] { 0, 0 };

            //List of possible directions ordered counter-clockwise
            char[] allDirections = new char[] { 'E', 'N', 'W', 'S' };
            int directionIndex = 0;

            //Go through directionsList to plot course
            foreach (Tuple<char,int> nextDirection in directionsList)
            {
                switch (nextDirection.Item1)
                {
                    case 'N': coordinates[0] += nextDirection.Item2; break;
                    case 'S': coordinates[0] -= nextDirection.Item2; break;
                    case 'E': coordinates[1] += nextDirection.Item2; break;
                    case 'W': coordinates[1] -= nextDirection.Item2; break;
                    case 'L':
                        {
                            
                            switch(nextDirection.Item2)
                            {
                                case 90:  directionIndex = (directionIndex + 1) % 4; break;
                                case 180: directionIndex = (directionIndex + 2) % 4; break;
                                case 270: directionIndex = (directionIndex + 3) % 4; break;
                            }
                            break;
                        }
                    case 'R':
                        {

                            switch (nextDirection.Item2)
                            {
                                case 90:  directionIndex = (directionIndex - 1 + 4) % 4; break;
                                case 180: directionIndex = (directionIndex - 2 + 4) % 4; break;
                                case 270: directionIndex = (directionIndex - 3 + 4) % 4; break;
                            }
                            break;
                        }
                    case 'F':
                        {
                            switch (allDirections[directionIndex])
                            {
                                case 'N': coordinates[0] += nextDirection.Item2; break;
                                case 'S': coordinates[0] -= nextDirection.Item2; break;
                                case 'E': coordinates[1] += nextDirection.Item2; break;
                                case 'W': coordinates[1] -= nextDirection.Item2; break;
                            }
                            break;
                        }
                }
            }

            return coordinates;
        }

        static int[] PlotCourseWithWaypoint(List<Tuple<char, int>> directionsList)
        {
            //ship coordinates
            //coordinates[0]: +North/-South, coordinates[1]: +East/-West
            int[] coordinates = new int[] { 0, 0 };
            //waypoint coordinates relative to ship
            int[] waypointCoordinates = new int[] { 1, 10 };

            //List of possible directions ordered counter-clockwise
            char[] allDirections = new char[] { 'E', 'N', 'W', 'S' };
            int directionIndex = 0;

            foreach (Tuple<char, int> nextDirection in directionsList)
            {
                switch (nextDirection.Item1)
                {
                    case 'N':
                        waypointCoordinates[0] += nextDirection.Item2; break;
                    case 'S': 
                        waypointCoordinates[0] -= nextDirection.Item2; break;
                    case 'E': 
                        waypointCoordinates[1] += nextDirection.Item2; break;
                    case 'W':
                        waypointCoordinates[1] -= nextDirection.Item2; break;
                    case 'L':// rotate waypoint counter-clockwise
                        {
                            //number of 90 deg steps
                            int steps = nextDirection.Item2 / 90;
                            for(int i=0; i<steps;i++)
                            {
                                int tmp = waypointCoordinates[0];
                                waypointCoordinates[0] = waypointCoordinates[1];
                                waypointCoordinates[1] = -1 * tmp;
                            }
                            break;
                        }
                    case 'R': // rotate waypoint clockwise
                        {
                            //number of 90 deg steps
                            int steps = nextDirection.Item2 / 90;
                            for (int i = 0; i < steps; i++)
                            {
                                int tmp = waypointCoordinates[0];
                                waypointCoordinates[0] = -1 * waypointCoordinates[1];
                                waypointCoordinates[1] = tmp;
                            }
                            break;
                            break;
                        }
                    case 'F':
                        {
                            coordinates[0] += waypointCoordinates[0] * nextDirection.Item2;
                            coordinates[1] += waypointCoordinates[1] * nextDirection.Item2;
                            break;
                        }
                }
            }
            

            return coordinates;
        }
        static void Main(string[] args)
        {
            List < Tuple<char, int> > directionsList = ReadInputData();
            int[] coordinates = PlotCourse(directionsList);
            Console.WriteLine("Final position: {0} N, {1} E", coordinates[0],coordinates[1]);
            Console.WriteLine("Manhattan distance to (0,0): {0}", Math.Abs(coordinates[0]) + Math.Abs(coordinates[1]));

            Console.WriteLine();
            coordinates = PlotCourseWithWaypoint(directionsList);
            Console.WriteLine("Final position: {0} N, {1} E", coordinates[0], coordinates[1]);
            Console.WriteLine("Manhattan distance to (0,0): {0}", Math.Abs(coordinates[0]) + Math.Abs(coordinates[1]));

        }
    }
}