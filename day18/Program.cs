using System.Text.RegularExpressions;

namespace day18
{
    internal class Program
    {
        public static List<string> ReadInput(string fileName)
        {

            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<string> input = new List<string>();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while ((nextLine = myReader.ReadLine()) != null)
                {
                    input.Add(nextLine);
                }
            }
            return input;
        }

        static void Main(string[] args)
        {
            List<string> input = ReadInput("puzzleInput.txt");

            long sumResults = 0;
            long result;
            Part2Math math = new();
            
            for(int i = 0; i < input.Count; i++)
            {
                result = math.EvaluateExpression(input[i]);
                sumResults +=result;
                //Console.WriteLine(" {0} = {1}", input[i],result);
            }
            Console.WriteLine("Sum of all results: {0}",sumResults);
            
        }
    }
}