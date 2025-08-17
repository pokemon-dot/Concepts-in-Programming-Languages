#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BIGINT_SIZE 309

typedef struct BIG_INT
{
    int *num;
    int sign; 
    int length;
}BigInt;

BigInt* addBigInts(BigInt *b1, BigInt *b2);
BigInt* subtractBigInts(BigInt *b1, BigInt *b2);
BigInt* multBigInts(BigInt *b1, BigInt *b2);

BigInt stringToBigInt(char *s)
{
    BigInt b;
    int size = strlen(s);
    if(s[0] == '-')
    {
        b.sign = 1;
    }
    else
    {
        b.sign = 0;
    }
    b.length = size - b.sign;
    b.num = (int*)malloc(sizeof(int) * (b.length));
    int *i;
    i = b.num;
    size--;
    while(size >= b.sign)
    {
        *(i) = s[size] - '0';
        i++;
        size--;
    }
    return b;
}

void printBigInt(BigInt *b)
{
    int i;
    if(b->sign == 1)
    {
        printf("-");
    }
    for(int i=b->length-1 ; i>=0 ; i--)
    {
        printf("%d",b->num[i]);
    }
}

int compare(BigInt *b1, BigInt *b2)
{
    int i, res;
    if(b1->length == b2->length)
    {
        i = (b1->length) - 1;
        while(i>0 && b1->num[i] == b2->num[i])
        {
            i--;
        }
        if(i<0)
        {
            res = 0;
        }
        else
        {
            res = b1->num[i] - b2->num[i];
        }
    }
    else
    {
        res = b1->length - b2->length;
    }
    return res;
}

//Add function
BigInt* addBigInts(BigInt *b1, BigInt *b2)
{
    int n1 = b1->length;
    int n2 = b2->length;
    int carry = 0;
    BigInt *sum = (BigInt*)malloc(sizeof(BigInt));
    if(n2>n1)
    {
        sum->num = (int*)malloc(sizeof(int)*(n2+1));
    }
    else
    {
        sum->num = (int*)malloc(sizeof(int)*(n1+1));
    }
    //two cases : 
    //case 1 -> if the signs are same
    if(b1->sign == b2->sign)
    {
        sum->sign = b1->sign;
        carry = 0;
        int i = 0, digit = 0;
        while(i<n1 && i<n2)
        {
            digit = b1->num[i] + b2->num[i] + carry;
            carry = digit/10;
            digit = digit%10;
            sum->num[i] = digit;
            i++;
        }
        while(i<n1)
        {
            digit = b1->num[i] + carry;
            carry = digit/10;
            digit = digit%10;
            sum->num[i] = digit;
            i++;
        }
        while(i<n2)
        {
            digit = b2->num[i] + carry;
            carry = digit/10;
            digit = digit%10;
            sum->num[i] = digit;
            i++;
        }
        while(carry)
        {
            sum->num[i] = (carry%10);
            carry = carry/10;
            i++;
        }
        sum->length = i;
    }
    //case 2 -> different signs
    else
    {
        if(b1->sign == 1)
        {
            b1->sign = 0;
            sum = subtractBigInts(b2,b1);
            b1->sign = 1;
        }
        else
        {
            b2->sign = 0;
            sum = subtractBigInts(b1,b2);
            b2->sign = 1;
        }
    }
    return sum;
}

//to subtract the magnitude only
BigInt* subtract(BigInt *b1, BigInt *b2)
{
    BigInt *diff = (BigInt*)malloc(sizeof(BigInt));
    diff->num = (int*)malloc(sizeof(int)*(b1->length));
    int borrow = 0;
    int i=0,digit;
    while(i < b2->length)
    {
        digit = (b1->num[i]) - (b2->num[i]) - borrow;
        if(digit<0)
        {
            borrow = 1;
            digit = digit + 10;
        }
        else
        {
            borrow = 0;
        }
        diff->num[i] = digit;
        i++;
    }
    while(i<b1->length)
    {
        digit = (b1->num[i]) - borrow;
        if(digit < 0)
        {
            borrow = 1;
            digit = digit + 10;
        }
        else
        {
            borrow = 0;
        }
        diff->num[i] = digit;
        i++;
    }
    diff->length = i;
    return diff;
}

//Subtraction
BigInt* subtractBigInts(BigInt *b1, BigInt *b2)
{
    int n1 = b1->length;
    int n2 = b2->length;
    int borrow;
    BigInt *diff = (BigInt*)malloc(sizeof(BigInt));
    if(n2 > n1)
        diff->num = (int*)malloc(sizeof(int)*n2);
    else
        diff->num = (int*)malloc(sizeof(int)*n1);

    //two cases
    //case1 -> signs are not equal
    if(b1->sign != b2->sign)
    {
        if(b2->sign == 1)
        {   
            b2->sign = 0;
            diff = addBigInts(b1,b2);
            b2->sign = 1;
            diff->sign = 0;
        }
        else
        {
            b2->sign = 1;
            diff = addBigInts(b1,b2);
            b2->sign = 0;
            diff->sign = 1;
        }
    }

    //case2 -> signs are equal
    else
    {
        if(compare(b1,b2) > 0)
        {
            diff = subtract(b1,b2);
            diff->sign = b1->sign;
        }
        else
        {
            diff = subtract(b2,b1);
            if(b1->sign == 0)
            {
                diff->sign = 1;
            }
            else
            {
                diff->sign = 0;
            }
        }
    }
    return diff;
}

//Multiplication
BigInt* multBigInts(BigInt *b1, BigInt *b2)
{
    BigInt *mult = (BigInt*)malloc(sizeof(BigInt));
    mult->length = b1->length + b2->length;
    mult->num = (int*)malloc(sizeof(int)*(mult->length));

    int i,j,carry,prod;
    for(i=0 ; i<mult->length ; i++)
    {
        mult->num[i] = 0; 
    }
    for(i=0 ; i<b1->length ; i++)
    {
        carry = 0;
        for(j=0 ; j<b2->length ; j++)
        {
            prod = (b1->num[i]*b2->num[j]) + carry;
            carry = prod/10;
            mult->num[i+j] += (prod%10);
            if(mult->num[i+j] > 9)
            {
                mult->num[i+j+1] += 1;
                (mult->num[i+j]) %= 10;
            }
        }
        while(carry != 0)
        {
            mult->num[i+j] += (carry%10);
            if(mult->num[i+j] > 9)
            {
                mult->num[i+j+1] += 1;
                (mult->num[i+j]) %= 10;
            }
            carry = carry/10;
            j++;
        }
    }

    if(b1->sign == b2->sign)
    {
        mult->sign = 0;
    }
    else
    {
        mult->sign = 1;
    }

    i = mult->length - 1;
    while(mult->num[i] == 0)  
    {
        i--;
        (mult->length)--;
    }
    if(mult->length == 0)
    {
        mult->sign = 0;
        mult->length = 1;
    }
    return mult;
}

int main()
{
    BigInt num1,num2;
    char *s1, *s2;
    s1 = (char*)malloc(sizeof(char)*BIGINT_SIZE);
    s2 = (char*)malloc(sizeof(char)*BIGINT_SIZE);

    printf("Enter first number : ");
    scanf("%s",s1);

    printf("Enter second number : ");
    scanf("%s",s2);

    num1 = stringToBigInt(s1);
    num2 = stringToBigInt(s2);

    BigInt *ans = addBigInts(&num1,&num2);
    printf("Sum of two numbers : ");
    printBigInt(ans);

    printf("\n");

    ans = subtractBigInts(&num1,&num2);
    printf("Difference of two numbers : ");
    printBigInt(ans);

    printf("\n");
    
    ans = multBigInts(&num1,&num2);
    printf("Multiplication of two numbers : ");
    printBigInt(ans);
    printf("\n");
    return 0;
}
