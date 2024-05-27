/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

char datorx;//Variable necesaria para introducir datos en la UART

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
    case CYBLE_EVT_STACK_ON:
        //Comenzar advertising
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
        break;

    default:
        break;
    }
}

/* Interrupcion necesaria para poder introducir datos en la UART y que el dispositivo los lea */
CY_ISR(InterruptRX) 
{
    //Leer dato introducido en la UART
    datorx=UART_GetChar();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    CyBle_Start(AppCallBack);// Iniciamos el dispositivo a través de la función de callback
    UART_Start();//Iniciamos la UART para poder visualizar e introducir datos
    
    isrRX_StartEx(InterruptRX);//Se activa la interrupción

    for(;;)
    {
        //Encender led, escribir, a través de la UART "Transmitiendo Datos1"
        LED1_Write(1);
        UART_PutString("Transmitiendo Datos1\r\n");
        CyBle_ProcessEvents();
        CyDelay(100);

            
            //Si el dato introducido es 0, se deja de anunciar tramas y se apaga el led
            if (datorx== '0')
            {
            LED1_Write(0);
            CyBle_GappStopAdvertisement();
            UART_PutString("Transmision detenida \r \n");            
             }
    }
}

/* [] END OF FILE */
