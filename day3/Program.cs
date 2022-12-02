List<string> treeMap = new List<string>();

string fileName = "puzzleInput.txt";
string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);
using (var myReader = new StreamReader(filePath))
{
    string? rawLine;
    while ((rawLine = myReader.ReadLine()) is not null)
    {
        treeMap.Add(rawLine);
    }
}

int[,] stepList = { {1,1}, {3,1}, {5,1}, {7,1}, {1,2} };
int[] treesEncountered = new int[5];
for (int i = 0; i < 5; i++)
{
    treesEncountered[i] = walkTrees(treeMap, stepList[i,0], stepList[i, 1]);
    //Console.Write("{0} ",treesEncountered[i]);
}
//Console.WriteLine("\n");
long prodTreesEncountered = 1;
foreach (int i in treesEncountered)
{
    prodTreesEncountered*=i;
}

Console.WriteLine("Number of trees met with nRight=3 and nDown=1: {0}", treesEncountered[1]);
Console.WriteLine("Product of number of trees encountered in all runs: {0}", prodTreesEncountered);

int walkTrees(List<string> treeMap,int nRight, int nDown)
{
    int nTreesMet = 0;
    int posX = 0;
    int mapLenRight = treeMap[0].Length;
    for (int posY = 0; posY < treeMap.Count; posY+=nDown)
    {
        //Console.Write(treeMap[posY][posX%mapLenRight]);
        if (treeMap[posY][posX%mapLenRight] == '#')
        {
            nTreesMet++;
        }
        posX += nRight;
    }
    //Console.Write("\n");
    return nTreesMet;
}