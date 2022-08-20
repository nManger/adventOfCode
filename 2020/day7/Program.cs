using day7;

string fileName = "puzzleInput.txt";
string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

// read Dataset line by line

List<BagRule> bagRules = new List<BagRule>();
using (var myReader = new StreamReader(filePath))
{
    string? nextLine;
    while(( nextLine = myReader.ReadLine()) != null)
    {
        string[] splitLine = nextLine.Split("contain", StringSplitOptions.None);
        string[] splitAgain = splitLine[1].Split(new string[]{ "bags","bag",".",","},
                              StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
        
        string outerBag = splitLine[0].Split("bags", StringSplitOptions.TrimEntries)[0];
        string[] innerBagTypes;
        int[] innerBagQuantities;
        if (splitAgain[0] == "no other")
        {
            innerBagTypes = new string[0];
            innerBagQuantities = new int[0];
        }
        else
        {
            innerBagTypes = new string[splitAgain.Length];
            innerBagQuantities = new int[splitAgain.Length];
            string[] split;
            for(int i= 0; i < splitAgain.Length; i++)
            {
                split = splitAgain[i].Split(" ", 2);
                innerBagQuantities[i] = int.Parse(split[0]);
                innerBagTypes[i] = split[1];
            }
        }

        BagRule newRule = new BagRule(outerBag, innerBagTypes, innerBagQuantities);
        //Console.WriteLine(newRule.ToString());
        bagRules.Add(newRule);

    }

}

List<string> outerBags = findOuterBags(bagRules, "shiny gold");
Console.WriteLine("Number of possible outer bags: {0}",outerBags.Count);

Console.WriteLine("Number of bags contained in shiny gold bag: {0}", findNumInnerBags(bagRules, "shiny gold")-1);

List<string> findOuterBags(List<BagRule> bagRules, string bagColor)
{
    List<string> outerBags = new List<string>();
    List<string> newElements = new List<string>();
    foreach (BagRule rule in bagRules)
    {
        if (rule.InnerBagTypes.Contains(bagColor))
        {
            newElements.Add(rule.OuterBagType);
            newElements.AddRange(findOuterBags(bagRules, rule.OuterBagType));
            outerBags = outerBags.Union(newElements).ToList();
        }
    }
    return outerBags;
}

int findNumInnerBags(List<BagRule> bagRules, string bagColor)
{
    int nInnerBags = 1;
    foreach(BagRule rule in bagRules)
    {
        if(rule.OuterBagType == bagColor)
        {
            
            for(int i=0; i<rule.InnerBagQuantities.Length; i++)
            {
                nInnerBags+= rule.InnerBagQuantities[i] * findNumInnerBags(bagRules, rule.InnerBagTypes[i]);
            }
            
            break;
        }
    }
    return nInnerBags;
}

