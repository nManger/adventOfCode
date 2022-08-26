namespace day14
{
    internal class Program
    {
        static long BitArrayToLong(byte[] bitArray)
        {
            long intNum = 0;
            for (int i = 0; i < bitArray.Length; i++)
            {
                //Console.Write(bitArray[i]);
                if (bitArray[i] == 0) continue;
                intNum += (long)Math.Pow(2, 35 - i);
            }
            //Console.Write(" "+ intNum + "\n");
            return intNum;
        }

        static byte[] LongToBitArray(long number)
        {
            //Console.Write(number + " ");
            List<byte> byteStack = new List<byte>();
            long remainder;
            while (number > 0)
            {
                number = Math.DivRem(number, 2, out remainder);
                byteStack.Add((byte) remainder);
            }
            int length = byteStack.Count;
            byte[] result = new byte[36];
            for(int i = 0; i < 36; i++)
            {
                if(i < length)
                {
                    result[35-i] = byteStack[i];
                }
                else
                {
                    result[35 - i] = 0;
                }
                   
            }
            /*
            for(int i = 0; i< result.Length; i++ )
            {
                Console.Write(result[i]);
            }
            Console.WriteLine();
            */
            return result;
        }

        static Dictionary<int, long> DecoderVersion1(string filePath)
        {
            Dictionary<int, long> memory = new();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                char[] currentMask = new char[36];
                while (myReader.Peek() != -1)
                {
                    nextLine = myReader.ReadLine();
                    string[] splitString = nextLine.Split("=", StringSplitOptions.TrimEntries);
                    if (splitString[0] == "mask")
                    {
                        currentMask = splitString[1].ToCharArray();
                    }
                    else
                    {
                        string[] splitAgain = splitString[0].Split(new char[] { '[', ']' },
                            StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

                        long input = long.Parse(splitString[1]);
                        byte[] inputBitArray = LongToBitArray(input);
                        for (int i = 0; i < currentMask.Length; i++)
                        {
                            if (currentMask[i] == 'x' || currentMask[i] == 'X') continue;
                            inputBitArray[i] = (byte)int.Parse(currentMask[i].ToString());
                        }
                        int inputKey = int.Parse(splitAgain[1]);
                        if (memory.ContainsKey(inputKey))
                        {
                            memory[inputKey] = BitArrayToLong(inputBitArray);
                        }
                        else
                        {
                            memory.Add(inputKey, BitArrayToLong(inputBitArray));
                        }

                    }

                }

            }
            return memory;
        }

        static Dictionary<long, long> DecoderVersion2(string filePath)
        {
            Dictionary<long, long> memory = new();
            using (var myReader = new StreamReader(filePath))
            {
                string? nextLine;
                char[] currentMask = new char[36];
                while (myReader.Peek() != -1)
                {
                    nextLine = myReader.ReadLine();
                    string[] splitString = nextLine.Split("=", StringSplitOptions.TrimEntries);
                    if (splitString[0] == "mask")
                    {
                        currentMask = splitString[1].ToCharArray();
                    }
                    else
                    {
                        string[] splitAgain = splitString[0].Split(new char[] { '[', ']' },
                            StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

                        long address = long.Parse(splitAgain[1]);
                        byte[] addressBitArray = LongToBitArray(address);

                        //apply mask (2 represents floating bit)
                        List<int> floatingBitPositions = new List<int>();
                        for (int i = 0; i < currentMask.Length; i++)
                        {
                            if (currentMask[i] == '1' )
                            {
                                addressBitArray[i] = 1;
                            }
                            else if (currentMask[i] == 'x' || currentMask[i] == 'X')
                            {
                                addressBitArray[i] = 2;
                                floatingBitPositions.Add(i);
                            }
                        }
                        List<byte[]> addressList = new List<byte[]> { addressBitArray};
                        for(int i = 0; i < floatingBitPositions.Count; i++)
                        {
                            List<byte[]> copyList = new List<byte[]> {};
                            for(int j=0; j<addressList.Count; j++)
                            {
                                //copy all values in array in addressList[j] and add new array to copy list
                                byte[] tmp = new byte[addressList[j].Length];
                                addressList[j].CopyTo(tmp, 0);
                                copyList.Add(tmp);

                                //set floating bit to 0 in original array and 1 in copy
                                addressList[j][floatingBitPositions[i]] = 0;
                                copyList[j][floatingBitPositions[i]] = 1;

                            }
                            //merge both lists
                            addressList.AddRange(copyList);
                            
                        }

                        long input = long.Parse(splitString[1]);
                        foreach (byte[] addr in addressList)
                        {   
                            long outAddress = BitArrayToLong(addr);

                            if (memory.ContainsKey(outAddress))
                            {
                                memory[outAddress] = input;
                            }
                            else
                            {
                                memory.Add(outAddress, input);
                            }
                        }
                    }

                }

            }
            return memory;
        }

        static void Main(string[] args)
        {
            string fileName = "puzzleInputTest.txt";
            string? filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            Dictionary<int, long> memory = DecoderVersion1(filePath);
            long sumValues = 0;
            foreach (long value in memory.Values)
            {
                sumValues += value;
            }
            Console.WriteLine("Sum of all values in memory (Test 1): {0}",sumValues);

            fileName = "puzzleInput.txt";
            filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);
            
            memory = DecoderVersion1(filePath);
            sumValues = 0;
            foreach (long value in memory.Values)
            {
                sumValues += value;
            }
            Console.WriteLine("Sum of all values in memory (V 1): {0}", sumValues);

            fileName = "puzzleInputTestPart2.txt";
            filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            Dictionary<long,long> memoryV2 = DecoderVersion2(filePath);
            sumValues = 0;
            foreach (KeyValuePair<long,long> pair in memoryV2)
            {
                //Console.WriteLine(pair.Key);
                sumValues += pair.Value;
            }
            Console.WriteLine("Sum of all values in memory (Test 2): {0}", sumValues);

            fileName = "puzzleInput.txt";
            filePath = Path.Combine(Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName, fileName);

            memoryV2 = DecoderVersion2(filePath);
            sumValues = 0;
            foreach (KeyValuePair<long, long> pair in memoryV2)
            {
                //Console.WriteLine(pair.Key);
                sumValues += pair.Value;
            }
            Console.WriteLine("Sum of all values in memory (V 2): {0}", sumValues);

        }
    }
}