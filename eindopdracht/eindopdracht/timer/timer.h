/*
 * timer.h
 *
 * Created: 22-3-2023 10:56:17
 *  Author: coend
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#ifndef BIT
#define BIT(x)		( 1<<x )
#endif

void timer_init();
void timer_set_compare_value(int value);
int timer_get_hz();



#endif /* TIMER_H_ */