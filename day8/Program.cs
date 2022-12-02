namespace day8
{
    class Command
    {
        public readonly string command;
        public readonly int offset;
        public bool executed;

        public Command(string command, int offset)
        {
            this.command = command;
            this.offset = offset;
            this.executed = false;
        }

        public override string ToString()
        {
            return "command: " + this.command + " offset: " + this.offset.ToString()

                + " has executed: " + this.executed.ToString();
        }
    }
    internal class Program
    {
        public static int[] ProgramChecker(List<Command> commands)
        {
            int accumulator = 0;
            int i = 0;
            
            while (i < commands.Count)
            {
                //Console.WriteLine(i.ToString() + " " + commands[i].ToString());

                //if command has already been visited, break out of loop
                if (commands[i].executed == true) break;

                //mark command as executed, then execute
                commands[i].executed = true;
                switch (commands[i].command)
                {
                    case "acc":
                        accumulator += commands[i].offset;
                        i++;
                        break;
                    case "jmp":
                        i += commands[i].offset;
                        break;
                    case "nop":
                        i++;
                        break;
                    default:
                        throw new Exception("Command " + commands[i].command + " not recognized");
                }
            }
            if (i>= commands.Count)
            {
                return new int[] { 0, i, accumulator };
            }
            else
            {
                return new int[] { 1, i, accumulator };
            }
        }


        //make a deep copy of the List to modify
        //and change statement i from jmp to nop or vice versa.
        static List<Command> copyCommandList(List<Command> commands, int i)
        {
            List<Command> copyCommands = new();
            for (int j = 0; j < commands.Count; j++)
            {
                if (j == i)
                {
                    if (commands[i].command == "nop")
                    {
                        copyCommands.Add(new Command("jmp", commands[j].offset));
                    }
                    else if (commands[i].command == "jmp")
                    {
                        copyCommands.Add(new Command("nop", commands[j].offset));
                    }
                    else
                    {
                        throw new Exception(" acc statements are not to be changed!!!!!!!!!!!!!!");
                    }

                }
                else
                {
                    copyCommands.Add(new Command(commands[j].command, commands[j].offset));
                }
            }
            return copyCommands;
        }

        static void Main(string[] args)
        {
            //read puzzle input
            string fileName = "puzzleInput.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            List<Command> commands = new List<Command>();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while ((nextLine = myReader.ReadLine()) != null)
                {
                    string[] splitLine = nextLine.Split(' ', StringSplitOptions.TrimEntries);
                    commands.Add(new Command(splitLine[0], int.Parse(splitLine[1])));
                }
            }

            int[] programTest = ProgramChecker(commands);
            
            //Console.WriteLine();
            Console.WriteLine("# repeated statement = {0}, acc = {1} ", programTest[1], programTest[2]);

            int[] result = new int[3];
            int i;
            for (i=0; i<commands.Count; i++)
            {
                // acc statements are not changed, skip those
                if (commands[i].command == "acc") continue;

                List<Command> copyCommands = copyCommandList(commands, i);
                result = ProgramChecker(copyCommands);

                if (result[0] == 0) break;
             
            }

            Console.WriteLine();
            Console.WriteLine("# changed statement = {0}, acc = {1} ", i, result[2]);
        }
    }
}