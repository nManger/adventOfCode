using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace day7
{
    internal class BagRule
    {
        public string OuterBagType { get; private set; }
        public string[] InnerBagTypes { get; private set; }
        public int[] InnerBagQuantities { get; private set; }

        public BagRule(string outerBagType, string[] innerBagTypes, int[] innerBagQuantities)
        {
            OuterBagType = outerBagType ?? throw new ArgumentNullException(nameof(outerBagType));
            InnerBagTypes = innerBagTypes ?? throw new ArgumentNullException(nameof(innerBagTypes)); ;
            InnerBagQuantities = innerBagQuantities ?? throw new ArgumentNullException(nameof(innerBagQuantities)); ;

            if (innerBagTypes.Length != innerBagQuantities.Length)
            {
                throw new ArgumentException("Number of inner bag types not equal number of inner bag quantities");
            }
        }

        public override string ToString()
        {
            string outstring = "outer: "+ OuterBagType + ", ";
            if (InnerBagTypes.Length == 0)
            {
                outstring += "no inner bags";
            }
            else
            {
                outstring += "inner: ";
                for (int i = 0; i < InnerBagTypes.Length; i++)
                {
                    outstring += InnerBagQuantities[i].ToString() + " " + InnerBagTypes[i] + " , ";
                } 
            }
            return outstring;
        }
    }
}
