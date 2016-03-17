
const int treshold = 0;

// pins for the LEDs:
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

const int pinPoteRojo = A0;
const int pinPoteVerde = A1;
const int pinPoteAzul = A2;

int ultimoRojo = 0;
int ultimoVerde = 0;
int ultimoAzul = 0;

int ultimoValorPoteRojo = -1;
int ultimoValorPoteVerde = -1;
int ultimoValorPoteAzul = -1;

String numeroRecibido = "";    // string to hold input
char inByte;
char ultimaLetra = 'H';
long ms_ultimo_informe = 0;
bool modo_manual = true;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("arrancamos");
}

void loop() {
  bool hay_que_informar = false;
  // if there's any serial available, read it:  
  while (Serial.available() > 0) {
    inByte = Serial.read();
    if(isDigit(inByte)){
        numeroRecibido += inByte;
    }else{
        if (ultimaLetra == 'R'||ultimaLetra == 'r'){
          ultimoRojo = numeroRecibido.toInt();
          analogWrite(redPin, ultimoRojo);   
          hay_que_informar = true;       
        }
        if (ultimaLetra == 'G'||ultimaLetra == 'g'){
          ultimoVerde = numeroRecibido.toInt();
          analogWrite(greenPin, ultimoVerde);  
          hay_que_informar = true;  
        }
        if (ultimaLetra == 'B'||ultimaLetra == 'b'){
          ultimoAzul = numeroRecibido.toInt();
          analogWrite(bluePin, ultimoAzul);
          hay_que_informar = true;  
        }
        
        if (ultimaLetra == 'A'||ultimaLetra == 'a'){
          modo_manual = true;
          Serial.println("modo manual: activado");
        }
        
        if (ultimaLetra == 'D'||ultimaLetra == 'd'){
          modo_manual = false;
          Serial.println("modo manual: desactivado");
        }
        
        ultimaLetra = inByte;
        numeroRecibido = "";
    }
  } 
  int valorPoteRojo = leerValorPote(pinPoteRojo);
  if((abs(ultimoValorPoteRojo-valorPoteRojo) > treshold) && modo_manual){
      ultimoValorPoteRojo = valorPoteRojo;
      ultimoRojo = valorPoteRojo;
      analogWrite(redPin, valorPoteRojo);   
      hay_que_informar = true;       
  }
  
  int valorPoteVerde = leerValorPote(pinPoteVerde);
  if((abs(ultimoValorPoteVerde-valorPoteVerde) > treshold) && modo_manual){
      ultimoValorPoteVerde = valorPoteVerde;
      ultimoVerde = valorPoteVerde;
      analogWrite(greenPin, valorPoteVerde);   
      hay_que_informar = true;       
  }
  
  int valorPoteAzul = leerValorPote(pinPoteAzul);
  if((abs(ultimoValorPoteAzul-valorPoteAzul) > treshold) && modo_manual){
      ultimoValorPoteAzul = valorPoteAzul;
      ultimoAzul = valorPoteAzul;
      analogWrite(bluePin, valorPoteAzul);   
      hay_que_informar = true;       
  }
  
  if(hay_que_informar && millis()-ms_ultimo_informe>0){
     Serial.println("R" + String(ultimoRojo) + "G" + String(ultimoVerde) + "B" + String(ultimoAzul));
     hay_que_informar = false;
     ms_ultimo_informe = millis();
  }
  
}

int leerValorPote(int pin){
  //int valorPote = map(analogRead(pin), 0, 1023, -50, 300) ;
  //if(valorPote<0)valorPote=0;
  //if(valorPote>255)valorPote=255;
  float valorPote = 0;
  for(int i=0; i<10; i++){
    valorPote += analogRead(pin);
    delay(1);
  }
  valorPote = round(valorPote/10);
  valorPote = map(valorPote, 0, 1023, -200, 2700);
  valorPote = floor(valorPote/10);
  valorPote = constrain(valorPote,0, 255) ;
  
  return valorPote;
}






