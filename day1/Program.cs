List < Int32 > expenseReport = new List<int>();

// Read puzzle input into List 'expenseReport'
string fileName = "puzzleInput.txt";
string filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);
using (var myReader = new StreamReader(filePath))
{
    string rawLine;
    while ((rawLine = myReader.ReadLine()) is not null)
    {
        expenseReport.Add(int.Parse(rawLine));
    }
}


int prod = 0;
prod = summation2(expenseReport);
Console.WriteLine("Part 1 - Result of multiplication : {0}", prod);

prod = summation3(expenseReport);
Console.WriteLine("Part 2 - Result of multiplication : {0}", prod);

int summation2( List<int> expenseReport)
{
    int sum = 0;
    int prod = 0;
    for (int i = 0; i < expenseReport.Count - 1; i++)
    {
        for (int j = i + 1; j < expenseReport.Count; j++)
        {
            sum = expenseReport[i] + expenseReport[j];
            //Console.WriteLine("a = {0}, b= {1}, a+b = {2}", expenseReport[i], expenseReport[j],sum);
            if (sum == 2020)
            {
                prod = expenseReport[i] * expenseReport[j];
                return prod;
            }

        }
    }
    return -1;
}

int summation3(List<int> expenseReport)
{
    int sum = 0;
    int prod = 0;
    for (int i = 0; i < expenseReport.Count - 2; i++)
    {
        for (int j = i + 1; j < expenseReport.Count - 1; j++)
        {
            for(int k = j + 1; k < expenseReport.Count; k++)
            {
                sum = expenseReport[i] + expenseReport[j] + expenseReport[k];
                //Console.WriteLine("a = {0}, b= {1}, a+b = {2}", expenseReport[i], expenseReport[j],sum);
                if (sum == 2020)
                {
                    prod = expenseReport[i] * expenseReport[j] * expenseReport[k];
                    return prod;
                }
            }
            

        }
    }
    return -1;
}



