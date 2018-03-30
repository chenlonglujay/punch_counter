//blue tooth(HC05) module test
//slave:Arduino pro mini
//data format as below
//left arm :lxxxx.      ex:l0586.
//right arm:rxxx.      ex:r1004.
#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
//please install freeRTOS from Manage Libraries(install FreeRTOS by RichardBarry) ,refer as below website
//https://www.hackster.io/feilipu/using-freertos-multi-tasking-in-arduino-ebc3cc
SemaphoreHandle_t xSerialSemaphore;

#include <punchBT_slave.h>
punchBT_slave punch_L;

#define AT_Mode 0       //first,you need use AT_Mode to pair BT,please refer to BT_pair document
#define slave_Mode 1   //second, use slave_Mode to recrive data from master or transmit data to master


void setup() {
  Serial.begin(9600);
  
#if AT_Mode
        punch_L.punchBT_slave_initial_set(AT_mode, left);         
#else slave_Mode  
        punch_L.punchBT_slave_initial_set(Slave_mode, left);       //if this module need set right arm module,please type argument 2 is right
#endif

  if ( xSerialSemaphore == NULL )  {
      // Check to confirm that the Serial Semaphore has not already been created.
      xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
      if ( ( xSerialSemaphore ) != NULL )
          xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
  
  // Now set up two Tasks to run independently.
  xTaskCreate(
    TaskSerialRead
    ,  (const portCHAR *)"SerialRead"  // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 1 being the highest, and 4 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskSerialSend
    ,  (const portCHAR *) "SerialSend"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );
  
}

void loop() {
}


void TaskSerialRead( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
     // A Task shall never return or exit.
     bool receiveSeq = false; 
    for (;;) {   
          if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) {    
          // We were able to obtain or "Take" the semaphore and can now access the shared resource.
          // We want to have the Serial Port for us alone, as it takes some time to print,
          // so we don't want it getting stolen during the middle of a conversion.
          // print out the value you read:
           receiveSeq = punch_L.Slave_mode_receive_reset();       
          xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
      }
      vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
    }
}

void TaskSerialSend( void *pvParameters __attribute__((unused)) ) { // This is a Task.
    for (;;)  {
#if AT_Mode  
        punch_L.AT_mode_function();
#else if slave_Mode
       transmitDataTest(&punch_L); 
 #endif
      vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void transmitDataTest(punchBT_slave *input) {
//test :
//slave transmits transmitData(0-9999)
//if transmitData%10 =0,pause
       //int data = input->get_transmitData() ;
       input->transmitData_add();         
       input->set_punch_pause(punch);    
       
       if(input->get_transmitData() == 9999) { 
             input->set_transmitData(0);  
       }
        input->Slave_mode_transmit();  
       if(input->get_transmitData() % 10 == 0) {       
             input->set_punch_pause(pause);  
             input->Slave_mode_transmit();           
       }
        delay(1000);
}

