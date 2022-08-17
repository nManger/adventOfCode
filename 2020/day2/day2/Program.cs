using day2;

List<string> passwordList = new List<string>();
List<PwPolicyPart1> passwordPolicyList = new List<PwPolicyPart1>();
List<PwPolicyPart2> passwordPolicyList2 = new List<PwPolicyPart2>();

string fileName = "puzzleInput.txt";
string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);
using (var myReader = new StreamReader(filePath))
{
    string? rawLine;
    while ((rawLine = myReader.ReadLine()) is not null)
    {
        string[] splitLine = rawLine.Split(new char[] { ' ',':' ,'-'},StringSplitOptions.RemoveEmptyEntries);
        //Fill policy list for part 1
        PwPolicyPart1 tempPwPolicy = new PwPolicyPart1(splitLine[2].ToCharArray()[0],
            int.Parse(splitLine[0]), int.Parse(splitLine[1]));
        passwordPolicyList.Add(tempPwPolicy);

        //Fill policy list for part 2
        PwPolicyPart2 tempPwPolicy2 = new PwPolicyPart2(splitLine[2].ToCharArray()[0],
            int.Parse(splitLine[0]), int.Parse(splitLine[1]));
        passwordPolicyList2.Add(tempPwPolicy2);

        //Fill password list
        passwordList.Add(splitLine[3]);
    }
}

passwordCheck<PwPolicyPart1> myPwChecker = new passwordCheck<PwPolicyPart1>(passwordList,passwordPolicyList);
Console.WriteLine("Number of valid Passwords (Part 1): {0}",myPwChecker.checkAllPasswords());

passwordCheck<PwPolicyPart2> myPwChecker2 = new passwordCheck<PwPolicyPart2>(passwordList, passwordPolicyList2);
Console.WriteLine("Number of valid Passwords (Part 2): {0}", myPwChecker2.checkAllPasswords());