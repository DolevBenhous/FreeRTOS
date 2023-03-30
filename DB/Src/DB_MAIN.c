/*
 * DB_MAIN.c
 *
 *  Created on: Dec 25, 2022
 *      Author: dolev
 */

#include "DB_MAIN.h"
#include "lwip/apps/tftp_server.h"
#include "lwip/udp.h"
#include "lwip/timeouts.h"
#include "lwip/debug.h"
#include "lwip.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

struct tftp_context *ctx;
struct udp_pcb *pcb;
static int eventsLogCtr =0;
char text[EVENT_MSG_LEN];
BaseType_t xHigherPriorityTaskWoken = pdTRUE;

SemaphoreHandle_t xPrintSemaphore;

QueueHandle_t xCriPoliceQueue;
QueueHandle_t xCriFireQueue;
QueueHandle_t xCriAmbulanceQueue;
QueueHandle_t xCriMaintenQueue;

QueueHandle_t xNotWarPoliceQueue;
QueueHandle_t xNotWarFireQueue;
QueueHandle_t xNotWarAmbulanceQueue;
QueueHandle_t xNotWarMaintenQueue;

/*
 * handles the program
 * its a city dispatcher reads events from buffer using TFTP,
 * send it to queue by department and priority and then
 * try to take from the queue and print the task
 */
void ex_main()
{
	printf("***** START PROGRAM *****\r\n");

/*-----------------------------------------------------------*/
	// creating queue for events.
	xCriPoliceQueue 	= xQueueCreate(CRI_QUEUE_SIZE, sizeof(dispMessage_t));
	xCriFireQueue 		= xQueueCreate(CRI_QUEUE_SIZE, sizeof(dispMessage_t));
	xCriAmbulanceQueue 	= xQueueCreate(CRI_QUEUE_SIZE, sizeof(dispMessage_t));
	xCriMaintenQueue 	= xQueueCreate(CRI_QUEUE_SIZE, sizeof(dispMessage_t));

	xNotWarPoliceQueue 		= xQueueCreate(NOTWAR_QUEUE_SIZE, sizeof(dispMessage_t));
	xNotWarFireQueue 		= xQueueCreate(NOTWAR_QUEUE_SIZE, sizeof(dispMessage_t));
	xNotWarAmbulanceQueue 	= xQueueCreate(NOTWAR_QUEUE_SIZE, sizeof(dispMessage_t));
	xNotWarMaintenQueue 	= xQueueCreate(NOTWAR_QUEUE_SIZE, sizeof(dispMessage_t));

/*-----------------------------------------------------------*/
	// checking all queues were created successfully.
	if (xCriPoliceQueue 	  		!= NULL
		&& xCriFireQueue 			!= NULL
		&& xCriAmbulanceQueue 		!= NULL
		&& xCriMaintenQueue 		!= NULL
		&& xNotWarPoliceQueue 		!= NULL
		&& xNotWarFireQueue 		!= NULL
		&& xNotWarAmbulanceQueue 	!= NULL
		&& xNotWarMaintenQueue 		!= NULL)
	{
/*-----------------------------------------------------------*/
		//creating tasks
		if (xTaskCreate(vLwipTask,			"LWIP init",LWIP_STACK_SIZE,NULL,TFTP_PRIORITY,NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vDispatcher, 		"Dispatcher",configMINIMAL_STACK_SIZE ,NULL, DISP_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vPoliceManage, 		"Police 1.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vPoliceManage, 		"Police 2.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vPoliceManage, 		"Police 3.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vFireManage, 		"Fire 1.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}


		if (xTaskCreate(vFireManage, 		"Fire 2.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vFireManage, 		"Fire 3.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vAmbulanceManage, 	"Ambulance 1.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vAmbulanceManage,	"Ambulance 2.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vAmbulanceManage, 	"Ambulance 3.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vAmbulanceManage, 	"Ambulance 4.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vMaintenanceManage, "Maintenance 1.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vMaintenanceManage, "Maintenance 2.", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

		if (xTaskCreate(vLightLedTask, "Red Led", configMINIMAL_STACK_SIZE,NULL, EVENT_PRIORITY, NULL) != pdPASS)
		{
			printf("Error occurred when creating task, exiting...\r\n");
			exit(1);
		}

/*-----------------------------------------------------------*/
		//creating binary semaphores for print
		xPrintSemaphore = xSemaphoreCreateBinary();

		if (xPrintSemaphore == NULL)
		{
			printf("Couldn't create semaphore.. exiting\r\n");
			exit(1);
		}
/*-----------------------------------------------------------*/
		// GIVE SEMAPHORES
		xSemaphoreGive(xPrintSemaphore);

		// SCHEDULER STARTS
		vTaskStartScheduler();
	}
	else
	{
		printf("Couldn't create queue, exiting program...\r\n");
		exit(1);
	}
	while(1);
}
/*-----------------------------------------------------------*/
void vDispatcher(void *pvParameters){
	BaseType_t xSendStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(TIME_TO_GET_EVENT);
	for(;;)
	{
		// check event department
		switch(events[eventsLogCtr % MAX_EVENT_AMOUNT].handlingDepartment)
		{
			case POLICE:
			{
				// check for event severity
				switch(events[eventsLogCtr % MAX_EVENT_AMOUNT].eventSeverity)
				{
					case Critical :
					{
						// will send to police critical queue if there is a space there
						xSendStatus = xQueueSendToBack(xCriPoliceQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Warning :
					{
						// will send to police common queue for warning and notice events if there is a space there.
						// warning events go to front
						xSendStatus = xQueueSendToFront(xNotWarPoliceQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Notice :
					{
						// will send to police common queue for warning and notice events if there is a space there.
						// notice events go to back
						xSendStatus = xQueueSendToBack(xNotWarPoliceQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					//in case we got other severity than the three mention above
					default:
						break;
				}
				break;
			}
			case FIRE:
			{
				// check for event severity
				switch(events[eventsLogCtr % MAX_EVENT_AMOUNT].eventSeverity)
				{
					case Critical :
					{
						// will send to fire critical queue if there is a space there
						xSendStatus = xQueueSendToBack(xCriFireQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Warning :
					{
						// will send to fire common queue for warning and notice events if there is a space there.
						// warning events go to front
						xSendStatus = xQueueSendToFront(xNotWarFireQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Notice :
					{
						// will send to fire common queue for warning and notice events if there is a space there.
						// notice events go to back
						xSendStatus = xQueueSendToBack(xNotWarFireQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					//in case we got other severity than the three mention above
					default:
						break;
				}
				break;
			}

			case AMBULANCE:
			{
				// check for event severity
				switch(events[eventsLogCtr % MAX_EVENT_AMOUNT].eventSeverity)
				{
					case Critical :
					{
						// will send to ambulance critical queue if there is a space there
						xSendStatus = xQueueSendToBack(xCriAmbulanceQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Warning :
					{
						// will send to ambulance common queue for warning and notice events if there is a space there.
						// warning events go to front
						xSendStatus = xQueueSendToFront(xNotWarAmbulanceQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Notice :
					{
						// will send to ambulance common queue for warning and notice events if there is a space there.
						// notice events go to back
						xSendStatus = xQueueSendToBack(xNotWarAmbulanceQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					//in case we got other severity than the three mention above
					default:
						break;
				}
				break;
			}
			case MAINTENANCE:
			{
				// check for event severity
				switch(events[eventsLogCtr % MAX_EVENT_AMOUNT].eventSeverity)
				{
					case Critical :
					{
						// will send to maintenance critical queue if there is a space there
						xSendStatus = xQueueSendToBack(xCriMaintenQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					case Warning :
					{
						// will send to maintenance common queue for warning and notice events if there is a space there.
						// warning events go to front
						xSendStatus = xQueueSendToFront(xNotWarMaintenQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not sevPrintCityLognd to the queue.\r\n" );
						}
						break;
					}
					case Notice :
					{
						// will send to maintenance common queue for warning and notice events if there is a space there.
						// notice events go to back
						xSendStatus = xQueueSendToBack(xNotWarMaintenQueue, &events[eventsLogCtr % MAX_EVENT_AMOUNT], ZERO_TICKS_TO_WAIT);
						if( xSendStatus != pdPASS )
						{
							printf( "Could not send to the queue.\r\n" );
						}
						break;
					}
					//in case we got other severity than the three mention above
					default:
						break;
				}
				break;
			//in case we got event for other department
			default:
				break;
			}
		}
		eventsLogCtr++;
		vTaskDelay(xTicksToWait);
	}
}

/*
 * in this task we are checking for police queues
 * if there are messages in critical queue, the will be pulled first.
 * we can take from each queue or print out the msg only once
 * until last task printed then a semaphore will release
 */
void vPoliceManage (void *pvParameters){
		const TickType_t xTicksToDelay = pdMS_TO_TICKS(MS_TO_WAIT);
		BaseType_t xRecStatus;
		dispMessage_t ptemp;
		for(;;)
		{
			//check if queue is not empty
			if(prvIsQueueEmpty(xCriPoliceQueue)!= pdTRUE)
			{
				xRecStatus = xQueueReceive(xCriPoliceQueue, &ptemp, ZERO_TICKS_TO_WAIT);
				if (xRecStatus != pdTRUE)
				{
					vTaskDelay(xTicksToDelay);
					continue;
				}
				// waiting for semaphore to print
				xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
				sprintf(text,"POLICE critical event happened in address %s\r\n",(char *)ptemp.eventAddress);
				HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
				HAL_Delay(BUSY_WAIT);
			}
			//check if queue is not empty
			else if(prvIsQueueEmpty(xNotWarPoliceQueue)!= pdTRUE)
			{
				xRecStatus = xQueueReceive(xNotWarPoliceQueue, &ptemp, ZERO_TICKS_TO_WAIT);
				if (xRecStatus != pdTRUE)
				{
					vTaskDelay(xTicksToDelay);
					continue;
				}
				// waiting for semaphore to print
				xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
				sprintf(text,"POLICE warning event happened in address %s\r\n",(char *)ptemp.eventAddress);
				HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
				HAL_Delay(BUSY_WAIT);
			}
		}
}

/*
 * in this task we are checking for fire queues
 * if there are messages in critical queue, the will be pulled first.
 * we can take from each queue or print out the msg only once
 * until last task printed then a semaphore will release
 */
void vFireManage (void *pvParameters){
	const TickType_t xTicksToDelay = pdMS_TO_TICKS(MS_TO_WAIT);
	BaseType_t xRecStatus;
	dispMessage_t ftemp;
	for(;;)
	{
		//check if queue is not empty
		if(prvIsQueueEmpty(xCriFireQueue)!= pdTRUE)
		{
			xRecStatus = xQueueReceive(xCriFireQueue, &ftemp, ZERO_TICKS_TO_WAIT);
			if (xRecStatus != pdTRUE)
					{
						vTaskDelay(xTicksToDelay);
						continue;
					}
			// waiting for semaphore to print
			xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
			sprintf(text,"FIRE critical event happened in address %s\r\n",(char *)ftemp.eventAddress);
			HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
			HAL_Delay(BUSY_WAIT);
		}
		//check if queue is not empty
		else if(prvIsQueueEmpty(xNotWarFireQueue)!= pdTRUE)
		{
			xRecStatus = xQueueReceive(xNotWarFireQueue, &ftemp, ZERO_TICKS_TO_WAIT);
			if (xRecStatus != pdTRUE)
					{
						vTaskDelay(xTicksToDelay);
						continue;
					}
			// waiting for semaphore to print
			xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
			sprintf(text,"FIRE warning event happened in address %s\r\n",(char *)ftemp.eventAddress);
			HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
			HAL_Delay(BUSY_WAIT);
		}
	}
}

/*
 * in this task we are checking for ambulance queues
 * if there are messages in critical queue, the will be pulled first.
 * we can take from each queue or print out the msg only once
 * until last task printed then a semaphore will release
 */
void vAmbulanceManage (void *pvParameters){
	const TickType_t xTicksToDelay = pdMS_TO_TICKS(MS_TO_WAIT);
	BaseType_t xRecStatus;
	dispMessage_t atemp;
	for(;;)
	{
		//check if queue is not empty
		if(prvIsQueueEmpty(xCriAmbulanceQueue)!= pdTRUE)
		{
			xRecStatus = xQueueReceive(xCriAmbulanceQueue, &atemp, ZERO_TICKS_TO_WAIT);
			if (xRecStatus != pdTRUE)
					{
						vTaskDelay(xTicksToDelay);
						continue;
					}
			// waiting for semaphore to print
			xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
			sprintf(text,"AMBULANCE critical event happened in address %s\r\n",(char *)atemp.eventAddress);
			HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
			HAL_Delay(BUSY_WAIT);
		}
		//check if queue is not empty
		else if(prvIsQueueEmpty(xNotWarAmbulanceQueue)!= pdTRUE)
		{
			xRecStatus = xQueueReceive(xNotWarAmbulanceQueue, &atemp, ZERO_TICKS_TO_WAIT);
			if (xRecStatus != pdTRUE)
					{
						vTaskDelay(xTicksToDelay);
						continue;
					}
			// waiting for semaphore to print
			xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
			sprintf(text,"AMBULANCE warning event happened in address %s\r\n",(char *)atemp.eventAddress);
			HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
			HAL_Delay(BUSY_WAIT);
		}
	}
}

/*
 * in this task we are checking for maintenance queues
 * if there are messages in critical queue, the will be pulled first.
 * we can take from each queue or print out the msg only once
 * until last task printed then a semaphore will release
 */
void vMaintenanceManage (void *pvParameters){
	const TickType_t xTicksToDelay = pdMS_TO_TICKS(MS_TO_WAIT);
	BaseType_t xRecStatus;
	dispMessage_t temp;
	for(;;)
	{
		//check if queue is not empty
		if(prvIsQueueEmpty(xCriMaintenQueue)!= pdTRUE)
		{

			xRecStatus = xQueueReceive(xCriMaintenQueue, &temp, ZERO_TICKS_TO_WAIT);
			if (xRecStatus != pdTRUE)
					{
						vTaskDelay(xTicksToDelay);
						continue;
					}
			// waiting for semaphore to print
			xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
			sprintf(text,"MAINTENANCE critical event happened in address %s\r\n",(char *)temp.eventAddress);
			HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
			HAL_Delay(BUSY_WAIT);
		}
		//check if queue is not empty
		else if(prvIsQueueEmpty(xNotWarMaintenQueue)!= pdTRUE)
		{
			xRecStatus = xQueueReceive(xNotWarMaintenQueue, &temp, ZERO_TICKS_TO_WAIT);
			if (xRecStatus != pdTRUE)
					{
						vTaskDelay(xTicksToDelay);
						continue;
					}
			// waiting for semaphore to print
			xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);
			sprintf(text,"MAINTENANCE warning event happened in address %s\r\n",(char *)temp.eventAddress);
			HAL_UART_Transmit_IT(UART_DEBUG,(uint8_t *)text, EVENT_MSG_LEN);
			HAL_Delay(BUSY_WAIT);
		}

	}
}

/*
 * this task init the lwip and tftp server on EVB
 */
void vLwipTask(void *pvParameters)
{
  MX_LWIP_Init();
  tftp_init(ctx);

  vTaskDelete(NULL);

}

/*
 * task for toggle red led
 * has busy wait of 5 ms and then go
 * delay for 1 sec
 */
void vLightLedTask (void *pvParameters){
		for (;;)
		{
			HAL_GPIO_TogglePin(GPIOB, RED_LED);
			HAL_Delay(LED_BUSY_WAIT);
			HAL_GPIO_TogglePin(GPIOB, RED_LED);
			vTaskDelay(pdMS_TO_TICKS(LED_DELAY));
		}
}

/*
 *  CALLBACK after printing the event
 * here we give back semaphore for taking for relevant queue.
 * relevant queue means the one that just printed out the message
 * this is check by the event in text buffer
 * at the end we setting text to o and give semaphore for printing
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	memset(text,BUF_INIT_VALUE,sizeof(text));
	xQueueGiveFromISR(xPrintSemaphore,&xHigherPriorityTaskWoken);
}


