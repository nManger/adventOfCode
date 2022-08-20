string fileName = "puzzleInput.txt";
string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

// read Dataset line by line
List<string> groupAnswers = new List<string> { "" };
List<int> peoplePerGroup = new List<int> { 0 };
using (var myReader = new StreamReader(filePath))
{
    string? nextLine = "";
    int nGroups = 0;
    while ((nextLine = myReader.ReadLine()) != null)
    {
        if (nextLine == "")
        {
            groupAnswers.Add(nextLine);
            peoplePerGroup.Add(0);
            nGroups++;
        }
        else
        {
            groupAnswers[nGroups] += nextLine;
            peoplePerGroup[nGroups]++;
        }
    }
}

//Find number of questions anyone answered yes to
int diffAnswersCount = 0;
List<char> diffAnswers = new List<char>();
foreach (string groupAnswer in groupAnswers)
{
    foreach(char answer in groupAnswer)
    {
        if (!diffAnswers.Contains(answer)) diffAnswers.Add(answer);
    }
    diffAnswersCount += diffAnswers.Count;
    //Console.WriteLine("Number of different answers = {0}",diffAnswers.Count);
    diffAnswers.Clear();
}
Console.WriteLine("Total number of questions anyone answered = {0}", diffAnswersCount);

//Find number of questions everyone answered yes to

int[] numAnswersPerQuestion = new int[26];
int allAnsweredCount = 0;
for(int i = 0; i < groupAnswers.Count; i++)
{
    //Count how often each answer was given in group
    string groupAnswer = groupAnswers[i];
    foreach (char answer in groupAnswer)
    {
        numAnswersPerQuestion[(int)answer-97]++;
    }
    //Compare number of given answers to number of people in group, if equal increment allAnsweredCount
    //Then reset answer counter array
    int numPeopleInGroup = peoplePerGroup[i];
    for(int j = 0; j < 26; j++)
    {
        if (numPeopleInGroup == numAnswersPerQuestion[j])
        {
            allAnsweredCount++;
        }
        numAnswersPerQuestion[j] = 0;
    }
}
Console.WriteLine("Total number of questions everyone answered = {0}", allAnsweredCount);
