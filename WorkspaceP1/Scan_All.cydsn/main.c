/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *a+
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t contador=0; //Contador para controlar las tramas recibidas
char databeacon[255]={0}; //Variable que se utiliza para copiar la trama recibida y poder mostrarla por pantalla

//const uint8_t UUID_BEACON[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
//const uint8_t HEADER_BEACON[9] = {0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15};
const uint8_t BEACON_LENGHT=30;//25+5 por información de Apple inc
const uint8_t HEADER_AND_UUID_BEACONX[25]={0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
const uint8_t HEADER_AND_UUID_BEACONY[25]={0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0x20, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x20, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x20};
const uint8_t HEADER_AND_UUID_BEACONZ[25]={0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0x30, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x30, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x30};
/**
 * @brief                   BLE module application callback.
 * @param[in] event         Type of BLE event as defined in the BLE Stack.
 * @param[in] eventParam    Parameters associated with the specific BLE event.
 */
void AppCallBack(uint32 event, void *eventParam)
{
    (void) eventParam;

    switch (event)
    {
    //int contador=0;  
    //char str[10]={0};
    //char str2[2]={0};
    CYBLE_GAPC_ADV_REPORT_T *advReport = NULL;
        //Puntero del tipo CYBLE_GAPC_ADV_REPORT_T llamado advReport iniciado a NULL
        
    case CYBLE_EVT_STACK_ON:
        CyBle_GapcStartScan(CYBLE_SCANNING_FAST);
        break;
        
    case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
        //Cuanto más tiempo se pierda peor, por ello intentamos detectar que no es un ibeacon primero
        
        
        /*Cada vez que se escanea un dispositivo salta este evento, donde se puede operar con los resultados*/
        advReport = (CYBLE_GAPC_ADV_REPORT_T*)eventParam;
        
//        SECCIÓN 1: DEBE COMENTARSE PARA VER TODAS LAS TRAMAS QUE LLEGAN
        
         /*Comprobamos si la trama NO es de nuestra red comparando la longitud de trama recibida*/
        if(advReport->dataLen!=BEACON_LENGHT){
            break;
        }
        
        /*Comprobamos si la trama NO es de nuestra red comparando la trama recibida*/
        if ((memcmp (&HEADER_AND_UUID_BEACONX[0], &advReport->data[0], 25)!=0)&&(memcmp (&HEADER_AND_UUID_BEACONY[0], &advReport->data[0], 25)!=0)&&(memcmp (&HEADER_AND_UUID_BEACONZ[0], &advReport->data[0], 25)!=0)){
            break;            
        }
        
        
        // FIN SECCIÓN 1
        
        
        contador++;
        
        //SECCIÓN 2: LAS LÍNEAS SIGUIENTES DEBEN DEJAR DE SER CONSIDERADAS COMO COMENTARIO
        //SI SE QUIERE VER POR PANTALLA EL CONTENIDO DE LAS TRAMAS
        
//        
//        memcmp(databeacon, advReport->data, advReport->dataLen);
//        for (uint32 i=0; i<advReport->dataLen; i++){
//            
//            snprintf(databeacon, 255, "%02x", advReport->data[i]);
//            UART_PutString(databeacon);
//            UART_PutString(",");
//            memset(databeacon, 0, 255);
//            //snprintf(&databeacon[i], 255,"%x", advReport->data[i] );
//            
//            //copiamos en data beacon lo que llega en el advReport
//            
//        }
//        UART_PutString("\r\n,");
//        CyDelay(500);
        
        
        // FIN SECCIÓN 2
           
        
        break;
        
    case CYBLE_EVT_GAPC_SCAN_START_STOP :
        break; 
        
    default:
        break;
    }
}


int main(void)
{
    char PrintBuffer[255]={0}; //Variable que utilizamos para que la UART pueda mostrar el valor del contador
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    CyBle_Start(AppCallBack);
    UART_Start();
    UART_PutString("Programa Iniciado\r\n");
    
    for(;;)
    {
        CyBle_ProcessEvents();// Se procesan los eventos
        // Si se pulsa el botón de usuario se imprime por pantalla el valor del contador y termina el programa
        if(BUTTON_Read()==0){
            
            snprintf(PrintBuffer, 255,"%u", contador);
            UART_PutString(PrintBuffer);
            
            return(0);
        
        }    
    }
/*FINAL*/
    

}
/* [] END OF FILE */
