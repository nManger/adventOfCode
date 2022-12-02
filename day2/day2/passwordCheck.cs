using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace day2
{
    public class passwordCheck<T> where T : passwordPolicy
    {
        private List<string> _passwordList;
        private List<T> _passwordPolicyList;
        public passwordCheck(List<string> passwordList,List<T> passwordPolicyList)
        {
            _passwordList = passwordList ?? throw new ArgumentNullException(nameof(passwordList));
            _passwordPolicyList = passwordPolicyList ?? throw new ArgumentNullException(nameof(passwordPolicyList));
            

            if (passwordPolicyList.Count != passwordList.Count)
            {
                throw new ArgumentException("List of passwords and password policies contain unequal number of entries.");
            }

        }
        /*
         * Check all passwords in passwordList against the respective policy in passwordPolicyList
         * and return number of valid passwords
         */
        public int checkAllPasswords()
        {
            int nValidPw = 0;
            for (int i = 0; i < _passwordList.Count; i++)
            {
                if(_passwordPolicyList[i].checkPasswordValidity(_passwordList[i]))
                {
                    nValidPw++;
                }
            }
            return nValidPw;
        }

    }


    public abstract class passwordPolicy
    {
        protected char _requiredChar;
        protected int _minUse;
        protected int _maxUse;

        public passwordPolicy(char requiredChar, int min, int max)
        {
            _requiredChar = requiredChar;   
            _minUse = min;
            _maxUse = max;
        }

        public abstract bool checkPasswordValidity(string password);
        

    }

    public class PwPolicyPart1 : passwordPolicy
    {
        public PwPolicyPart1(char requiredChar, int min, int max) : base (requiredChar, min, max)
        { }
        public override bool checkPasswordValidity(string password)
        {
            int i = 0;
            foreach (char c in password)
            {
                if (c == _requiredChar)
                {
                    i++;
                }
            }
            if ((i >= _minUse) && (i <= _maxUse))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    public class PwPolicyPart2 : passwordPolicy
    {
        public PwPolicyPart2(char requiredChar, int min, int max) : base(requiredChar, min, max)
        { }
        public override bool checkPasswordValidity(string password)
        {
            //Check if character positions can exist in password string
            //if ((_minUse < 1) || (_minUse >= password.Length)) return false; 
            //if ((_maxUse < 1) || (_maxUse >= password.Length)) return false;

            //Check if only one of the positions specified by _minUse,_maxUse contains _requiredChar
            if ((password[_minUse-1] == _requiredChar) ^ (password[_maxUse-1] == _requiredChar))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}


