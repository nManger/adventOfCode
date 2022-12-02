using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace day18
{
    public abstract class AocMath
    {
        //Evaluate expression without parentheses, only containing + or * operations
        protected abstract string EvaluateInnerExpression(string substring);

        //Wrapper for EvaluateInnerExpression which takes a Match instance as argument
        protected string EvaluateInnerExpression(Match match)
        {
            //get the matched string and remove parentheses
            string substring = match.ToString()[1..^1];
            return EvaluateInnerExpression(substring);
        }

        //Evaluate given expression by iteratively finding all inner parentheses enclosed substrings left in the expression
        //and replacing them with the result of the expression within the substring
        public long EvaluateExpression(string expression)
        {
            //remove all whitespace
            expression = expression.Replace(" ", string.Empty);

            //while expression still contains parentheses, find innermost parentheses pair and evaluate expression inside
            while (expression.Contains('('))
            {
                string pattern = @"\((\d+(\+|\*))+\d+\)";
                Regex rx = new Regex(pattern);
                expression = rx.Replace(expression, new MatchEvaluator(EvaluateInnerExpression));
            }

            //evaluate outermost expression not containing any parentheses
            expression = EvaluateInnerExpression(expression);

            return long.Parse(expression);
        }
    }

    //Part 1 math: + and * operators have same precedence level
    public class Part1Math : AocMath
    {
        protected override string EvaluateInnerExpression(string substring)
        {
            long result = 0;
            //set operator to + so first number is assigned ( = added to 0)
            char currOperator = '+';

            int i;
            do
            {
                //find next number (delimited by next operator or end of string)
                //and add or multiply to result depending on currOperator
                i = Regex.Match(substring, "(\\*|\\+|$)").Index;
                if (currOperator == '+')
                {
                    result += long.Parse(substring[..i]);
                }
                else if (currOperator == '*')
                {
                    result *= long.Parse(substring[..i]);
                }
                else
                {
                    throw new ArgumentException("Operator " + currOperator.ToString() + " not implemented.");
                }


                if (i == substring.Length)
                {
                    //we have read last number, empty string
                    substring = string.Empty;
                }
                else
                {
                    //save operator to be used on next number
                    currOperator = substring[i];
                    //delete substring up to and including next operator
                    substring = substring[(i + 1)..];
                }

            } while (substring.Length > 0);

            return result.ToString();
        }


    }

    //Part 2 math: + has higher precedence level than *
    public class Part2Math : AocMath
    {
        /// <summary>
        /// Perform addition
        /// </summary>
        /// <param name="match">Match instance containing string matching regex</param>
        /// <returns></returns>
        protected string AddAll(string substring)
        {
            long result = 0;
            int i,j;

            i = 0;
            //find next operator or EoL, parse number right before and add to result
            while(i < substring.Length)
            {
                j = Regex.Match(substring[i..], "(\\+|$)").Index;
                result += long.Parse(substring[i..(i+j)]);
                i+=j+1;
            }
            return result.ToString();
        }

        //Wrapper for AddAll taking a Match object
        protected string AddAll(Match match)
        {
            string substring = match.ToString();
            return AddAll(substring);
        }

        protected string MultiplyAll(string substring)
        {
            long result = 1;//Pure multiplication, so set initial value to 1
            int i, j;

            i = 0;
            //find next operator or EoL, parse number right before and add to result
            while (i < substring.Length)
            {
                j = Regex.Match(substring[i..], "(\\*|$)").Index;
                result *= long.Parse(substring[i..(i+j)]);
                i += j+1;
            }
            return result.ToString();
        }
        protected override string EvaluateInnerExpression(string substring)
        {
            //first search for all additions and evaluate them
            string pattern = "(\\d+\\+)+\\d+";
            Regex rx = new Regex(pattern);
            substring = rx.Replace(substring, AddAll);

            //then evaluate remaining multiplications
            substring = MultiplyAll(substring);

            return substring;
        }
    }
}
