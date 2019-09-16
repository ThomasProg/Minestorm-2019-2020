#include "stats.h"

void stat_init(t_stat* stat, 
		unsigned int min, unsigned int current, unsigned int max)
{
	stat->min = min;
	stat->current = current;
	stat->max = max;
}

void stat_add(t_stat* stat, unsigned int value)
{
	if (stat->current + value < stat->max)
		stat->current += value;
	else
		stat->current = stat->max;
}

void stat_remove(t_stat* stat, unsigned int value)
{
	if (stat->current < stat->min + value)
		stat->current -= value;
	else
		stat->current = stat->min;
}
