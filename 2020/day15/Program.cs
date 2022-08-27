namespace day15
{
    internal class Program
    {
        static void Main(string[] args)
        {
            //Test game
            int[] startingNumbers = new int[] { 0, 3, 6 };

            ElfGame testGame = new ElfGame(startingNumbers);
            int lastNumber = testGame.SimulateGame(2020);
            Console.WriteLine("Number spoken on turn 2020: {0}",lastNumber);

            //Part 1 game
            startingNumbers = new int[] { 11, 0, 1, 10, 5, 19 };
            ElfGame game = new ElfGame(startingNumbers);
            lastNumber = game.SimulateGame(2020);
            Console.WriteLine("Number spoken on turn 2020: {0}", lastNumber);

            Console.WriteLine();
            //Test game part 2
            testGame.ResetGame();
            lastNumber = testGame.SimulateGame(30000000);
            Console.WriteLine("Number spoken on turn 30000000: {0}", lastNumber);

            //game Part 2
            game.ResetGame();
            lastNumber = game.SimulateGame(30000000);
            Console.WriteLine("Number spoken on turn 30000000: {0}", lastNumber);
        }
    }
}