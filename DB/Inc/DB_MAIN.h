/*
 * DB_MAIN.h
 *
 *  Created on: Dec 25, 2022
 *      Author: dolev
 */

#ifndef INC_DB_MAIN_H_
#define INC_DB_MAIN_H_

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "main.h"
#include "lwip.h"
#include "lwip/udp.h"
#include "lwip/timeouts.h"
#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZERO_TICKS_TO_WAIT		0
#define MS_TO_WAIT				10
#define TIME_TO_GET_EVENT		100
#define BUSY_WAIT				300
#define LED_BUSY_WAIT			5
#define LED_DELAY				1000

#define EVENT_PRIORITY 			1
#define DISP_PRIORITY 			4
#define TFTP_PRIORITY 			6

#define CRI_QUEUE_SIZE  		10
#define NOTWAR_QUEUE_SIZE		20
#define EVENT_MSG_LEN			150
#define LWIP_STACK_SIZE 		1000

#define POLICE_EVENT 			80
#define AMBULANCE_EVENT 		65
#define FIRE_EVENT 				70
#define MAINTENANCE_EVENT 		77
#define WARNING 				110
#define NOTICE 					119
#define CRITICAL 				99
#define EVENET_MSG_OFFSET 		0
#define POLICE_MSG_OFFSET 		7
#define AMBULANCE_MSG_OFFSET 	10
#define FIRE_MSG_OFFSET 		5
#define MAINTENANCE_MSG_OFFSET 	12
#define BUF_INIT_VALUE 			0
#define MAX_EVENT_AMOUNT 		50

// UART DEBUGGER HANDLER for printf/scanf implementation
extern UART_HandleTypeDef huart3;

// GENERAL DEFINITION FOR scanf
#define END_OF_STRING 0
#define BACKSPACE 8
#define LINE_FEED 10
#define CARRIAGE_RETURN 13


// UART DEFINITION
#define UART_DEBUG &huart3

#define RED_LED LD3_Pin

//extern dispMessage_t pack[MAX_EVENT_AMOUNT];

typedef enum{
	POLICE = 49,
	FIRE = 50,
	AMBULANCE = 51,
	MAINTENANCE =52
} toolsDepartment;


typedef enum {
	Critical = 49,
	Warning = 50,
	Notice = 51
} toolsSeverity;

typedef struct
{
	uint8_t handlingDepartment;
	uint8_t eventSeverity;
	char eventAddress [80];
} dispMessage_t;

// main function running all
void ex_main();

/* ----- task declaration ----- */
void vDispatcher(void *pvParameters);
void vLwipTask(void *pvParameters);
void vPoliceManage (void *pvParameters);
void vAmbulanceManage (void *pvParameters);
void vFireManage (void *pvParameters);
void vMaintenanceManage (void *pvParameters);
void vLightLedTask (void *pvParameters);




/* Define the structure type that will be passed on the queue. */

extern dispMessage_t events[MAX_EVENT_AMOUNT];

static const dispMessage_t EventsToSend [10]=
{
		{1,1, "address 1 is bla bla bla"},
		{2,2,"address 2 where RTG located"},
		{2,1,"address 3 is SOLAREDGE HEADQUARTERS"},
		{3,3,"address 4 is Balfur Street"},
		{2,2,"address 5 is Sinai Street"},
		{3,2,"address 6 "},
		{3,2,"address 7 is anywhere"},
		{1,2,"address 8 elsewhere"},
		{4,3,"address 9 doesn't exist"},
		{4,2,"address 10 is at some other point"}
};

//void ex_main(void);

#endif /* INC_DB_MAIN_H_ */
