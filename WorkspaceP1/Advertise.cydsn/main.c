/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *a+
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *Juan Pablo Cano López
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t contador=0; //Contador utilizado para controlar cuando el dispositivo detiene el envío de tramas
char PrintBuffer[255]={0};//String utilizado para mostrar información por la UART
extern CYBLE_GAPP_DISC_MODE_INFO_T cyBle_discoveryModeInfo; //Variable definida externamente utilizada como dato 
                                                            //en la orden CyBle_GappSetNumOfAdvPkts

/**
 *FUNCIÓN DE CALLBACK
 *Esta función establece las acciones que debe realizar el dispositivo en respuesta a diferentes eventos 
 * @brief                   BLE module application callback.
 * @param[in] event         Type of BLE event as defined in the BLE Stack.
 * @param[in] eventParam    Parameters associated with the specific BLE event.
 */
void AppCallBack(uint32 event, void *eventParam)
{
    (void) eventParam;

    switch (event)
    {
  
    //Evento que sucede al iniciar el dispositivo
    case CYBLE_EVT_STACK_ON:
        
        //Se fija una cantidad total de 600 paquetes (200 paquetes en cada uno de los canales) 
        CyBle_GappSetNumOfAdvPkts(&cyBle_discoveryModeInfo,600);
        //Espera de 5s antes de iniciar a transmitir
        CyDelay(5000);
        //Encender led para indicar que se comienza a transmitir tramas
        LED1_Write(1);
        //Comenzar a enviar tramas de anuncio
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
        
        break;
        
    //Evento que sucede cuando se inicia el advertisement o se detiene
    //Cuando el anunciador termina de anunciar los paquetes entra en este estado
    //con valor de contador = 1, se le suma 1 a este valor, detiene el advertising y apaga el led de control    
    case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
        
    contador++;
        
    //Sección que sirve para visualizar el contador, tendrá 2, como valor al finalizar el programa
        
    //snprintf(PrintBuffer, 255,"%u", contador);
    //UART_PutString(PrintBuffer);
    //UART_PutString("\r\n");
        
    //Fin sección de visualización del contador
    
    //Cuando el contador toma el valor 2 el dispositivo cesa de enviar tramas de anuncio y apaga el led de control        
    if(contador==2){
        //Detener anuncios
        CyBle_GappStopAdvertisement();
        //Apagar led de control
        LED1_Write(0);
    }
    
    break;    
   
    default:
        break;
    }
}

int main(void)
{

    CyGlobalIntEnable; /* Enable global interrupts. */
    CyBle_Start(AppCallBack);// Iniciamos el dispositivo a través de la función de callback
    UART_Start();//Iniciamos la UART para poder visualizar datos

    for(;;)
    {    
        //El dispositivo procesa las órdenes enviadas
        CyBle_ProcessEvents();
        //Espera de un tiempo pequeño debe ser la mitad del advertising interval o menos
        CyDelay(50);
        
       }
}
/*FINAL*/    

/* [] END OF FILE */
