using day5;
string fileName = "puzzleInput.txt";
string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

// read Dataset line by line
List<string> seatPositions = new List<string>();
using (var myReader = new StreamReader(filePath))
{
    string? nextLine = "";
    while ((nextLine = myReader.ReadLine()) != null)
    {
        seatPositions.Add(nextLine);
    }
}

List<int> seatIDs = new List<int>();
for (int i = 0; i < seatPositions.Count; i++)
{
    BoardingPass pass = new BoardingPass(seatPositions[i]);
    seatIDs.Add(pass.DecodeSeatPosition());
    //Console.WriteLine("Seat row = {0}, Seat column = {1}, Seat ID = {2}", pass.SeatRow,pass.SeatColumn,pass.SeatID);
}

int maxID = 0;
for (int i = 0; i < seatIDs.Count; i++)
{
    if (seatIDs[i] > maxID)
    {
        maxID = seatIDs[i];
    }
}

Console.WriteLine("Maximum Seat ID: {0}",maxID);

seatIDs.Sort();
for (int i = 0; i < seatIDs.Count-1; i++)
{
    if (seatIDs[i] + 1 != seatIDs[i + 1])
    {
        Console.WriteLine("My seat: {0}", seatIDs[i] + 1);
        break;
    }
}
