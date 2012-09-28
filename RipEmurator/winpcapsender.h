/*
 * winpcapsender.h
 *
 *  Created on: 2010-5-9
 *      Author: Bleastrind
 */

#ifndef WINPCAPSENDER_H_
#define WINPCAPSENDER_H_
extern "C"{

int winpcapsend(int interface,char * buffer,int size,int target, int port);
}
#endif /* WINPCAPSENDER_H_ */
