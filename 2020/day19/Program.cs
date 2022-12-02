namespace day19
{
    internal class Program
    {
        public static void ReadInput(string fileName,
                                     out Dictionary<char, char[]> messageRules, out List<string> messages)
        {

            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            messageRules = new();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                while (!string.IsNullOrEmpty(nextLine = myReader.ReadLine()))
                {
                    string[] split = nextLine.Split(':');
                    char[] rules = (from x in split[1].ToCharArray() where char.IsDigit(x) select x).ToArray();

                    messageRules.Add(split[0][0], rules);
                }

                messages = new();
                while (!string.IsNullOrEmpty(nextLine = myReader.ReadLine()))
                {
                    messages.Add(nextLine);
                }
            }
      
        }



        static void Main(string[] args)
        {
            
        }
    }
}