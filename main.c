#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgbus/messagebus.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <chprintf.h>
#include <motors.h>
#include <audio/microphone.h>
#include <selector.h>
#include <sensors/proximity.h>
#include <audio/play_melody.h>
#include <audio/audio_thread.h>
#include <arm_math.h>
#include <deplacement.h>

// Define pour ne pas avoir de magic number dans le code
#define DEUX_CENTS  200

// On initialise ici le bus afin de pouvoir utiliser les capteurs de proximite "proximity"
messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

// Fonction qui nous a permis d'afficher sur REALterm  les chprintf du robot
static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

    messagebus_init(&bus, &bus_lock, &bus_condvar);


    //Start la communication en serie
    serial_start();
    //Start la communication USB
    usb_start();
    //Initialsation des moteurs pas a pas du robot
    motors_init();
    //Start le capteur longue distance
	VL53L0X_start();
	//Start des capteurs de proximite
	proximity_start();
	//Start la camera du robot
	dcmi_start();
    po8030_start();
    //Mise du white balance a zero pour mieux distinguer les couleurs RGB
    po8030_set_awb(0);
    // Start de l'analyse d'image
	process_image_start();
	// Calibration du capteur de courte proximite
	calibrate_ir();
	//Start les melodies utilisees par le robot
	dac_start();
	playMelodyStart();


    /* Boucle infinie */
    while (1) {

    	//Selecteur == 0 : mode static + jeu de LEDs
    	if(get_selector()==0)
    	{
            init_vitesse_mot();
    		lumiere_demarrage();
    	}
        
        /*
        *  Selecteur == 1 :
        *       Tests pour l'instant - accelerometre
        *    
        */
        if(get_selector()==1)
        {
            lumiere_eteinte();
            

            //boucle infinie pour la fin de la tache
            while(1)
            {
                 lumiere_clignote();
                 chThdSleepMilliseconds(1000);
            }             
        }
    	
        /*
        * Selecteur == 2:
    	* Le robot au son  "purge" se met a analyser les objets dans son arene
    	* et sortira uniquement les rouges.
    	* Selecteur == 3:
        * le robot sort tous les objets.
        */
    	if((get_selector()==2)||(get_selector()==3))
    	{
           
            //boucle infinie pour la fin de la tache
            while(1)
            {
                 lumiere_clignote();
                 chThdSleepMilliseconds(1000);
            }            
    	}

        /*
        *  Selecteur == 4 :
        *  
        * 
        *   
        */
        if(get_selector()==4)
        {
            
        }
        
        /*
        *  Selecteur == 5 :
        *  
        *  
        *  
        */

        if((get_selector()==5))
        {
           
        }


    	else
    	{
    		// Mode static sans jeu de lumiere 
    		lumiere_eteinte();
			init_vitesse_mot();
    	}
    }
 }

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
