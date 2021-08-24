#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int _base = 1000000000;

class BigInteger
{
private :
    std::vector<int> num;
    int sign = 1;
    static int sgn(int x)
    {
        return x >= 0 ? 1 : -1;
    }

    static void minus(std::vector<int>& res, const std::vector<int>& v1, const std::vector<int>& v2)
    {
        int carry = 0;
        for(size_t i = 0; i < v2.size() || carry; ++i)
        {
            res[i] = v1[i] - carry - (i < v2.size() ? v2[i] : 0);
            carry = res[i] < 0;
            if(carry)
                res[i] += _base;
        }
        while(res.size() > 1 && res.back() == 0)
            res.pop_back();
    }

    static void plus(std::vector<int>& res, std::vector<int> v1, std::vector<int> v2)
    {
        int carry = 0;
        for (size_t i = 0; (i < std::max(v1.size(), v2.size())) || carry; ++i)
        {
            if (i == v1.size())
                res.push_back(0);
            res[i] = v1[i] + carry + (i < v2.size() ? v2[i] : 0);
            carry = res[i] >= _base;
            if (carry)
                res[i] -= _base;
        }
    }

    static void mul(BigInteger& a, const BigInteger& b)
    {
        a.num.resize(a.num.size() + b.num.size());
        for(size_t i = 0; i < a.num.size(); ++i)
        {
            for (long long j = 0, carry = 0; j < b.num.size() || carry; ++j) {
                long long curr = a.num[i + j] + a.num[i] * 1ll * (j < b.num.size() ? b.num[j] : 0) + carry;
                a.num[i + j] = int(curr % _base);
                carry = curr / _base;
            }
        }
        while(a.num.size() > 1 && a.num.back() == 0)
            a.num.pop_back();
    }

    static BigInteger divide(BigInteger& a, const BigInteger& b)
    {
        int k = a.num.size();
        int l = b.num.size();
        if(k < l)
        {
            a = 0;
            return 0;
        }
        BigInteger d, r, q;//intermediate dividend, remainder, quotient
        //Initialization
	r.num.clear();
	for(int i = a.num.size() - l + 1; i < a.num.size(); ++i)
	{
		r.num.push_back(a.num[i]);
	}

        q = 0;

        for(int i = 0; i <= k - l; ++i)
        {
	    
            d = _base;
            d *= r;
            d += a.num[a.num.size() - i - l];
            BigInteger d_copy = d;
            int beta = 0;
            while(d_copy >= b)
            {
                ++beta;
                d_copy -= b;
            }
            d_copy = b;
            d_copy *= beta;
            r = d;
            r -= d_copy;

            q *= _base;
            q += beta;
        }
	std::reverse(q.num.begin(), q.num.end());
        a = q;
        return r;
    }

public:

    BigInteger()
    {
        num.resize(1);
        num[0] = 0;
        sign = 0;
    }

    BigInteger(int x)//TODO rework
    {
        sign = sgn(x);
        x *= sign;
        if(x == 0)
            num.push_back(0);

        while(x != 0)
        {
            num.push_back(x % 10);
            x /= 10;
        }
    }

    BigInteger(const BigInteger& bi)
    {
        sign = bi.sign;
        num = bi.num;
    }

    ~BigInteger() {}

    //Predicates
    friend bool operator>(const BigInteger& bi1, const BigInteger& bi2)
    {
        if(bi1.sign > bi2.sign)
            return true;
        if(bi1.sign < bi2.sign)
            return false;
        bool min = (bi1.sign == -1 && bi2.sign == -1);

        if(bi1.num.size() > bi2.num.size())
        {
            if(min)
                return false;
            else
                return true;
        }
        if(bi1.num.size() < bi2.num.size())
        {
            if(min)
                return true;
            else
                return false;
        }

        std::vector<int>::const_reverse_iterator riter1, riter2;
        riter2 = bi2.num.rbegin();

        for(riter1 = bi1.num.rbegin(); riter1 != bi1.num.rend(); ++riter1, ++riter2)
        {
            if(*riter1 > *riter2)
            {
                if(min)
                    return false;
                else
                    return true;
            }
            else if (*riter1 < *riter2)
            {
                if(min)
                    return true;
                else
                    return false;
            }
        }

        return false;
    }

    friend bool operator==(const BigInteger& bi1, const BigInteger& bi2)
    {
        return (!(bi1 > bi2)) && (!(bi2 > bi1));
    }

    friend bool operator<(const BigInteger& bi1, const BigInteger& bi2)
    {
        return !((bi1 > bi2) || (bi1 == bi2));
    }

    friend bool operator!=(const BigInteger& bi1, const BigInteger& bi2)
    {
        return !(bi1 == bi2);
    }

    friend bool operator<=(const BigInteger& bi1, const BigInteger& bi2)
    {
        return !(bi1 > bi2);
    }

    friend bool operator>=(const BigInteger& bi1, const BigInteger& bi2)
    {
        return !(bi1 < bi2);
    }
   //

   explicit operator bool()
   {
        return *this != 0;
   }

    BigInteger& operator-()
    {
        sign *= -1;
        return *this;
    }

    //Assignment operators
    BigInteger& operator=(const BigInteger& bi)
    {
        num = bi.num;
        sign = bi.sign;
        return *this;
    }

    BigInteger& operator+=(const BigInteger& bi)
    {
        if(sign == bi.sign)
        {
            plus(this->num, this->num, bi.num);
        }
        else
        {
            if(sign == -1)
            {
                sign = 1;
                if(*this < bi)
                {
                    minus(this->num, bi.num, this->num);
                }
                else
                {
                    minus(this->num, this->num, bi.num);
                    if(*this != 0)
                        sign = -1;
                }
            }
            else
            {
                sign = -1;
                if(*this > bi)
                {
                    minus(this->num, bi.num, this->num);
                }
                else
                {
                    minus(this->num, this->num, bi.num);
                    sign = 1;
                }
            }
        }
        return * this;
    }

    BigInteger& operator-=(const BigInteger& bi)
    {
        if(sign == 1 && bi.sign == 1)
        {
            if(*this < bi)
            {
                sign = -1;
                minus(this->num, bi.num, this->num);
            }
            else
            {
               minus(this->num, this->num, bi.num);
            }
        }
        else if(sign == -1 && bi.sign == -1)
        {
            if(*this > bi)
            {
                sign = 1;
                minus(this->num, bi.num, this->num);
            }
            else if(*this < bi)
            {
                sign = -1;
                minus(this->num, this->num, bi.num);
            }
            else if(*this == bi)
            {
                *this = 0;
                sign = 1;
            }
        }
        else
        {
            if(sign == -1 && bi.sign == 1)
            {
                sign = 1;
                *this += bi;
                sign = -1;
            }
            else
            {
                plus(this->num, this->num, bi.num);
            }
        }
        return *this;
    }

    BigInteger& operator*=(const BigInteger& bi)
    {
        mul(*this, bi);
        return *this;
    }

    BigInteger& operator/=(const BigInteger& bi)
    {
        BigInteger r;
        r = divide(*this, bi);
        sign *= bi.sign;
        return *this;
    }
    BigInteger& operator%=(const BigInteger& bi)
    {
        BigInteger a_copy = *this;
        BigInteger r  = divide(a_copy, bi);
        a_copy *= bi;
        *this -= a_copy;
        return *this;
    }
    //Prefix
    BigInteger& operator++()
    {
        *this += 1;
        return *this;
    }
    BigInteger& operator--()
    {
        *this -= 1;
        return *this;
    }
    //Postfix
    BigInteger operator++(int)
    {
        BigInteger tmp = *this;
        *this += 1;
        return tmp;
    }
    BigInteger operator--(int)
    {
        BigInteger tmp = *this;
        *this -= 1;
        return tmp;
    }

    std::string toString() {
        std::string s;
        std::vector<int>::reverse_iterator riter;
        if (sign == -1)
            s.push_back('-');
	s += std::to_string(num.back());
        for (int i = num.size() - 2; i >= 0; --i) {
            int tmp = num[i];
            std::string tmp_s = std::to_string(tmp);
            if (tmp != 0 && tmp_s.size() < 9) {
                tmp_s = "0" + tmp_s;
            }
            s += tmp_s;
        }
        return s;
    }

    friend std::istream& operator>>(std::istream& is, BigInteger& bi)
    {
        std::string s;
        is >> s;
        bi.num.clear();
        bi.sign = 1;
        if(s[0] == '-')
        {
            bi.sign = -1;
            s.erase(0, 1);
        }
        bi.num.clear();
        for(int i = s.size(); i > 0; i -= 9)
        {
            if(i < 9)
            {
                bi.num.push_back(atoi(s.substr(0, i).c_str()));
            }
            else
            {
                bi.num.push_back(atoi(s.substr(i - 9, 9).c_str()));
            }
        }
        return is;
    }

};

BigInteger operator+(const BigInteger& a, const BigInteger& b)
{
    BigInteger c = a;
    c += b;
    return c;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b)
{
    BigInteger c = a;
    c -= b;
    return c;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b)
{
    BigInteger c = a;
    c *= a;
    return c;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b)
{
    BigInteger c = a;
    c /= b;
    return c;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b)
{
    BigInteger c = a;
    c %= b;
    return c;
}



std::ostream& operator<<(std::ostream& os, BigInteger& bi)
{
    os << bi.toString();
    return os;
}
