This is the report and the programs made in PSoC Creator I used for my end of degree project.
It is a study of BLE (Bluetooth Low Energy) devices using a directional communication model instead of the typical broadcast one.
It is specifically designed to work with CY5677 CYSMART devices because I was physically provided with four of them so I could make real tests.
The programs are explained in spanish inside the report but a summary would be the next:
  Advertise is the one used by the gadget that sends the first message, it builds the message structure and sends it.
  ReTransmisor_1 waits for the message to be catched and resends it modifying the message identification.
  ReTransmisor_2 waits for the message identified as the one from retransmisor one and modifies again the identification.
  Finally the scanner only scans for messages identified as the ones from retransmisor two. 
This is done making changes to parameters of the advertising process and the scanning one to make an study of how these changes affect this model
of communication, that is rarely used with these gadgets.
