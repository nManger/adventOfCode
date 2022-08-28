using System.Diagnostics;

namespace day17
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

            // Part 1
            List<string> input = ReadInput("puzzleInputTest.txt");

            Stopwatch sw = new Stopwatch();
            sw.Start();
            Cubes3D testGame = new Cubes3D(input);
            testGame.simulateGameOfLife(6);
            sw.Stop();

            Console.WriteLine("Active cubes after 6 cycle boot (3D test): {0}, Time elapsed: {1}",
                testGame.CountActiveCubes(),sw.ElapsedMilliseconds);
            Console.WriteLine();

            input = ReadInput("puzzleInput.txt");

            sw.Reset();
            sw.Start();
            Cubes3D game = new Cubes3D(input);
            game.simulateGameOfLife(6);
            sw.Stop();

            Console.WriteLine("Active cubes after 6 cycle boot (3D): {0}, Time elapsed: {1}",
                game.CountActiveCubes(),sw.ElapsedMilliseconds);
            Console.WriteLine();

            //Part 2
            input = ReadInput("puzzleInputTest.txt");

            sw.Reset();
            sw.Start();
            Cubes4D testGame2 = new Cubes4D(input);
            testGame2.simulateGameOfLife(6);
            sw.Stop();

            Console.WriteLine("Active cubes after 6 cycle boot (4D test): {0}, Time elapsed: {1}",
                testGame2.CountActiveCubes(),sw.ElapsedMilliseconds);
            Console.WriteLine();

            input = ReadInput("puzzleInput.txt");

            sw.Reset();
            sw.Start();
            Cubes4D game2 = new Cubes4D(input);
            game2.simulateGameOfLife(6);
            sw.Stop();

            Console.WriteLine("Active cubes after 6 cycle boot (4D): {0}, Time elapsed: {1}",
                game2.CountActiveCubes(), sw.ElapsedMilliseconds);
            Console.WriteLine();


        }
    }
}