using System.Text.RegularExpressions;
using static System.Net.Mime.MediaTypeNames;

string fileName = "puzzleInput.txt";
string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

// read Dataset line by line
List<string> rawDataset = new List<string>();
using (var myReader = new StreamReader(filePath))
{
    string? nextLine = "";
    while ((nextLine = myReader.ReadLine()) != null)
    {
        rawDataset.Add(nextLine);
    }
}

List<Dictionary<string,string>> modDataset = new List<Dictionary<string, string>>();
bool newSet = true;
string[] splitString;
string[] pair;
int nDatasets = 0;
/*
 * Go through raw dataset containing one line per list item
 * if newSet is true: Make new Dictionary out of this line and append to modDataset
 * if newSet is false: Append content of line to last created dictionary in modDataset
 * If line is empty: last created dictionary complete, next line creates new dictionary
 */
for (int i = 0; i < rawDataset.Count(); i++)
{
    if (rawDataset[i] == "")
    {
        newSet = true;
        nDatasets++;
        continue;
    }

    splitString = rawDataset[i].Split(' ');
    if(newSet)
    {
        Dictionary<string, string> newDict = new Dictionary<string, string>();
        foreach(string substr in splitString)
        {
            pair= substr.Split(':');
            newDict.Add(pair[0], pair[1]);
        }
        modDataset.Add(newDict);
        newSet = false;
    }
    else
    {
        foreach (string substr in splitString)
        {
            pair = substr.Split(':');
            modDataset[nDatasets].Add(pair[0], pair[1]);
        }
    }
}


int numValidPassports = 0;
foreach (Dictionary<string,string> item in modDataset)
{ 
    if (checkAllFieldsPresent(item))
    {
        numValidPassports++;
    }
}

Console.WriteLine("Number of valid passports (Part 1): {0}", numValidPassports);

numValidPassports = 0;
foreach (Dictionary<string, string> item in modDataset)
{
    if (checkAllFieldsPresent(item) && checkAllFieldsValid(item))
    {
        numValidPassports++;
    }
}
Console.WriteLine("Number of valid passports (Part 2): {0}", numValidPassports);

bool checkAllFieldsPresent(Dictionary<string, string> item) 
{
    string[] neededKeys = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    bool passportValid = true;
    foreach (string key in neededKeys)
    {
        if (!item.ContainsKey(key))
        {
            passportValid = false;
            break;
        }
    }
    return passportValid;
}

bool checkAllFieldsValid(Dictionary<string, string> item)
{
    // Check birth year validity
    if (item.ContainsKey("byr"))
    {
        int yr = int.Parse(item["byr"]);
        if ((yr < 1920) || (yr > 2002))
        {
            //Console.WriteLine("Byr {0} invalid", item["byr"]);
            return false;
        }
    }

    // check year of issuance
    if (item.ContainsKey("iyr"))
    {
        int yr = int.Parse(item["iyr"]);
        if ((yr < 2010) || (yr > 2020))
        {
            //Console.WriteLine("Iyr {0} invalid", item["iyr"]);
            return false;
        }
    }

    // Check expiration year
    if (item.ContainsKey("eyr"))
    {
        int yr = int.Parse(item["eyr"]);
        if ((yr < 2020) || (yr > 2030))
        {
            //Console.WriteLine("Eyr {0} invalid", item["eyr"]);
            return false;
        }
    }

    // Check body height
    if (item.ContainsKey("hgt"))
    {
        // find all numbers in height string and convert to int
        var digits = from c in item["hgt"]
                     where Char.IsDigit(c) select c;
        string text = "";
        foreach (var digit in digits)
        {
            text += digit.ToString();
        }
        int height = int.Parse(text);

        // find all non-digit characters
        var alphas = from c in item["hgt"]
                     where !Char.IsDigit(c)
                     select c;
        text = "";
        foreach(var a in alphas)
        {
            text += a.ToString();
        }

        //Console.WriteLine("Height: {0} {1}",height,text);

        // check height if given in cm
        if (text == "cm")
        {
            if( height <150 || height > 193)
            {
                //Console.WriteLine("Height {0} invalid", item["hgt"]);
                return false;
            }
        }
        //check height if given in inch
        else if (text == "in")
        {
            if (height < 59 || height > 76)
            {
                //Console.WriteLine("Height {0} invalid", item["hgt"]);
                return false;
            }
        }
        //heights not containing cm or in (==inch) are not allowed
        else
        {
            //Console.WriteLine("Height {0} invalid", item["hgt"]);
            return false;
        }
    }

    if (item.ContainsKey("hcl"))
    {
        string pattern = @"^#[a-f0-9]{6}$";
        Regex rx = new Regex(pattern);
        
        MatchCollection matches = rx.Matches(item["hcl"]);
        if (matches.Count != 1)
        {
            //Console.WriteLine("Haircolor {0} invalid", item["hcl"]);
            return false;
        }
    }

    if (item.ContainsKey("ecl"))
    {
        string[] permittedColors = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
        if (!permittedColors.Contains(item["ecl"]))
        {
            //Console.WriteLine("Eyecolor {0} invalid", item["ecl"]);
            return false;
        }
    }

    if (item.ContainsKey("pid"))
    {
        string pattern = @"^\d{9}$";
        Regex rx = new Regex(pattern);

        MatchCollection matches = rx.Matches(item["pid"]);
        if (matches.Count != 1)
        {
            //Console.WriteLine("PID {0} invalid", item["pid"]);
            return false;
        }
    }

    return true;
}