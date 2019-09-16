#ifndef _STATS_H_
#define _STATS_H_

typedef struct s_stat
{
	unsigned int min, current, max;
} t_stat;

void stat_init(t_stat* stat, 
		unsigned int min, unsigned int current, unsigned int max);

void stat_add(t_stat* stat, unsigned int value);

#endif
