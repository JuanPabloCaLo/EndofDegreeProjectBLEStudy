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

uint16_t contador=0; //Variable que se utiliza para controlar cuando el dispositivo deja de anunciar información
char databeacon[255]={0}; //Variable que se utiliza para copiar la trama recibida y poder mostrarla por pantalla

//Variables conceptuales, juntas componen la trama, el encabezado fijo + la UUID
//const uint8_t UUID_BEACON[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
//const uint8_t HEADER_BEACON[9] = {0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15};

const uint8_t BEACON_LENGHT=30;//Longitud de la trama 25+5 bytes no pertenecientes a la UIID

//TRAMA_X, TRAMA_Y, TRAMA_Z Se definen las tres siempre, por simpleza en los cambios, son las tramas recibidas y enviadas
const uint8_t HEADER_AND_UUID_BEACONX[25]={0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
const uint8_t HEADER_AND_UUID_BEACONY[25]={0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0x20, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x20, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x20};
const uint8_t HEADER_AND_UUID_BEACONZ[25]={0x02, 0x01, 0x04, 0x1A, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0x30, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x30, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x30};
CYBLE_GAPC_DISC_INFO_T scaninfo;
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
    
    //Puntero del tipo CYBLE_GAPC_ADV_REPORT_T llamado advReport iniciado a NULL
    CYBLE_GAPC_ADV_REPORT_T *advReport = NULL;
        
    //Evento que sucede al iniciar el dispositivo    
    case CYBLE_EVT_STACK_ON:
        //Comenzar a escanear
        CyBle_GapcStartDiscovery(&scaninfo);
        break;
        
    /*Cada vez que se escanea un dispositivo salta este evento, donde se puede operar con los resultados*/     
    case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
        
        //Cuanto más tiempo se obtiene una peor eficiencia, por ello se intenta detectar que no es un ibeacon primero
        
        //Variable que se utiliza para poder acceder a datos obtenidos en el escaneo
        advReport = (CYBLE_GAPC_ADV_REPORT_T*)eventParam;
        
//        SECCIÓN 1: DEBE COMENTARSE PARA VER TODAS LAS TRAMAS QUE LLEGAN
        
        /*Si las tramas escaneadas no son de la longitud correcta se termina el evento*/
        if(advReport->dataLen!=BEACON_LENGHT){           
            
            break;
        }
        
        /*Se comprueba si la trama es de nuestra red y si no lo es, se termina el evento*/
        if (memcmp (&HEADER_AND_UUID_BEACONX[0], &advReport->data[0], 25)!=0){
            
            break;            
        }
        
        
        // FIN SECCIÓN 1
        
        //Se detiene el escaneo de tramas si se detecta una de las buscadas
        CyBle_GapcStopDiscovery();
        //Espera a terminar de escanear 
        while(CyBle_GetState()==CYBLE_STATE_SCANNING);
        //Se configura para enviar un solo paquete (3, uno en cada canal)
        CyBle_GappSetNumOfAdvPkts(&cyBle_discoveryModeInfo,3);
        //Se inicia el anuncio del paquete hacia el siguiente dispositivo
        
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
        //LED1_Write(1);Para visualizar el retardo
        
        //SECCIÓN 2: LAS LÍNEAS SIGUIENTES DEBEN DEJAR DE SER CONSIDERADAS COMO COMENTARIO
        //SI SE QUIERE VER POR PANTALLA EL CONTENIDO DE LAS TRAMAS RECIBIDAS
        
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
        
    //Evento que sucede al iniciarse o detenerse el anuncio de tramas    
    case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP :
        //Aumenta el contador en 1
        contador++;
        //Cuando el contador es 2 se detiene el anuncio de tramas, se reinicia el contador
        //y se comienza a escanear de nuevo
        if(contador==2){
            CyBle_GappStopAdvertisement();
            //LED1_Write(0); Para visualizar el retardo
            while(CyBle_GetState()==CYBLE_STATE_ADVERTISING);
            contador=0;
            CyBle_GapcStartDiscovery(&scaninfo);
        }
        break; 
        
    default:
        break;
    }
}


int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    //Configuración de scaninfo con los parámetros deseados, para poder detectar los dispositivos anunciantes
    scaninfo.discProcedure=CYBLE_GAPC_OBSER_PROCEDURE;//Procedimiento de escaneo, como observador
    scaninfo.scanType=CYBLE_GAPC_PASSIVE_SCANNING;//Tipo de escaneo, pasivo
    scaninfo.scanIntv=1000;//Intervalo de escaneo
    scaninfo.scanWindow=500;//Ventana de escaneo
    scaninfo.ownAddrType=CYBLE_GAP_ADDR_TYPE_PUBLIC;//Tipo de dirección propia, pública
    scaninfo.scanFilterPolicy=CYBLE_GAPC_ADV_ACCEPT_ALL_PKT;//Politica de filtrado, aceptar todo
    scaninfo.scanTo=0;
    scaninfo.filterDuplicates=CYBLE_GAPC_FILTER_DUP_DISABLE;//Deshabilitar el filtrado de duplicados
    
    CyBle_Start(AppCallBack);//Se inicia el dispositivo a través de la función de callback, que gestiona los eventos 
    UART_Start();//Se inicia la UART para poder mostrar contenido en pantalla
    UART_PutString("Programa Iniciado Re-transmisor1\r\n");//Se escribe este mensaje en la UART, al iniciar el programa
    
    for(;;)
    {
        CyBle_ProcessEvents();// Se procesan los eventos
            
    }
/*FINAL*/
    

}
/* [] END OF FILE */
