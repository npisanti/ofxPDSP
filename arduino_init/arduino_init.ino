
char selected_channel = 1;

void setup() {  
    Serial.begin(250000); 
    // set up your variables
    // ...
}


void loop() {
    while (Serial.available()) {
        // get the new byte:
        signed char new_byte = (signed char)Serial.read();
        if (new_byte < 0) {
            selected_channel = - new_byte;  
        }else{
            action ( selected_channel, new_byte);
        }
    }
}

// set up your code here
// please don't use delay(), it will give you bad timing if you use more than one channel
void action ( char channel, char value ){
    switch( channel ){
        case 1:
             // action for channel 1
        break;
        
        case 2:
              // action for channel 2
        break;

        // etc ....
    }    
}

