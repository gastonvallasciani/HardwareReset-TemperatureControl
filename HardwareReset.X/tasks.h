/* 
 * File:   tasks.h
 * Author: Ariel L
 *
 * Created on November 7, 2017, 10:21 AM
 */

#ifndef TASKS_H
#define	TASKS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
inline void InitHardware();
inline void MainTask();

//TMR2 DELAY
#define _1mSeg 1
#define _10mSeg 10*_1mSeg
#define _100mSeg 10*_10mSeg
#define _1Seg 10*_100mSeg
//


#ifdef	__cplusplus
}
#endif

#endif	/* TASKS_H */

