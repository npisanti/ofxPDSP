
const int pin_A = 5;
const int pin_B = 6;
char selected_channel = 1;

void setup() {  
    Serial.begin(250000); 

    pinMode(pin_A, OUTPUT);
    analogWrite(pin_A, 0);
    pinMode(pin_B, OUTPUT);
    analogWrite(pin_B, 0);
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
// please don't use delay, they will give you bad timing if you use more than one channel
void action ( char channel, char value ){
    switch( channel ){
        case 1:
            analogWrite(pin_A, value*2);  
        break;

        case 2:
            analogWrite(pin_B, value*2);  
        break;
        
    }    
}

