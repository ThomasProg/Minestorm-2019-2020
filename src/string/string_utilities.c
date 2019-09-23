#include <stdlib.h>
#include "string_utilities.h"

unsigned int string_length(const char* str)
{
	unsigned int i = 0;
	while (str[i] != '\0')
		i++;

	return i;
}

char* string_duplicate(char const* str)
{
	unsigned int length = string_length(str);
	char* newString = malloc((length + 1) * sizeof(char));

	for (unsigned int i = 0; i < length; i++)
		newString[i] = str[i];

	newString[length] = '\0';

	return newString;
}

char* string_append(char const* str1, char const* str2)
{
	unsigned int length1 = string_length(str1);
	unsigned int length2 = string_length(str2);
	unsigned int length = length1 + length2;

	char* newString = malloc((length + 1) * sizeof(char));

	for (unsigned int i = 0; i < length1; i++)
		newString[i] = str1[i];

	for (unsigned int i = 0; i < length2; i++)
		newString[i + length1] = str2[i];

	newString[length] = '\0';

	return newString;
}
