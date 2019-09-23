#include <stdlib.h>

#include "string_utilities.h"

// Return the number of digits in a decimal number
// (using static here to avoid multiple definitions of this function)
static int get_digits_count(int num)
{
    if (num == 0)
        return 1;

    int result = 0;
	while (num != 0)
    {
		num /= 10;
        result++;
    }
	return result;
}

// y = base^exp
static int power_of(int base, int exp)
{
    int result = 1;
    while (exp > 0)
    {
        result *= base;
        exp--;
    }
    return result;
}

char* int_to_alpha(int num)
{
    int str_len = get_digits_count(num);
    if (num < 0)
        str_len += 1; // +1 to store the '-' symbol

    // Alloc string
    char* result = malloc((str_len + 1) * sizeof(char));
    result[str_len] = '\0'; // Put termination char at the end
    int index = 0; // Store the advancement inside the string

    // Specifically add '-' for negative number
    if (num < 0)
    {
        result[index] = '-';
        index++;
    }

    // Same algorithm than day_02/ex_03/my_put_number.c
    int power_of_10 = power_of(10, get_digits_count(num)-1);
    while (power_of_10 > 0)
    {
        int unit = (num / power_of_10) % 10;

        if (num < 0)
            unit = -unit;

        result[index] = '0' + unit;
        index++;
        power_of_10 /= 10;
    }

    return result;
}
