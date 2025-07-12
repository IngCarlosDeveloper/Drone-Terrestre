#include <Ps3Controller.h>
#include <ESP32Servo.h>

int player = 1;
int battery = 0;


//Inicializacion de los Servos
Servo ServoX;
Servo ServoY;
int pin_servo_x = 14;
int pin_servo_y = 12;

int pos_servo_x = 90;
int aux_x = 0;
int pos_servo_y = 90;
int aux_y = 0;

int lim_sup = 3;
int lim_inf = -3;


//Salidas puente H-Motorreductores

int SalidaA = 26;
int SalidaB = 25;
int SalidaC = 33;
int SalidaD = 27;

//MOSFET para el disparador
int Mosfet = 32;

//Conexion ESP32-CAM

int cam = 13;

void controles(){
    //----------------------------------------------------CONFIGURACION DEL STICK DERECHO -- MOTORES ----------------------------------------
    if(abs(Ps3.data.analog.stick.rx) > 2 || abs(Ps3.data.analog.stick.ry) > 2){ //Se mueve el stick de la derecha

      if(Ps3.data.analog.stick.ry < -2){ //Joystick derecho se mueve ARRIBA
        digitalWrite(SalidaA, HIGH);
        digitalWrite(SalidaC, HIGH);
        Serial.println("Moviendome Arriba");
        //delay(2000);
      }

      if(Ps3.data.analog.stick.ry > 2){ //Joystick derecho se mueve ABAJO
        digitalWrite(SalidaB, HIGH);
        digitalWrite(SalidaD, HIGH);
        Serial.println("Moviendome Abajo");
        //delay(2000);
      }

      if(Ps3.data.analog.stick.rx > 2){ //Joystick derecho se mueve DERECHA
        digitalWrite(SalidaA, HIGH);
        digitalWrite(SalidaD, HIGH);
        Serial.println("Moviendome Derecha");
        //delay(2000);
      }

      if(Ps3.data.analog.stick.rx < -2){ //Joystick derecho se mueve IZQUIERDA
        digitalWrite(SalidaB, HIGH);
        digitalWrite(SalidaC, HIGH);
        Serial.println("Moviendome Izquierda");
        //delay(2000);
      }

      }

    if(abs(Ps3.data.analog.stick.rx) <= 2 && abs(Ps3.data.analog.stick.ry) <= 2){ //Joystick derecho regresa a centro
      digitalWrite(SalidaA, LOW);
      digitalWrite(SalidaB, LOW);
      digitalWrite(SalidaC, LOW);
      digitalWrite(SalidaD, LOW);
    }
    //----------------------------------------------------CONFIGURACION DEL STICK IZQUIERDO -- SERVOS ----------------------------------------

    if(abs(Ps3.data.analog.stick.lx) > 2 || abs(Ps3.data.analog.stick.ly) > 2){ //Se mueve el stick de la izquierda

      if(abs(Ps3.data.analog.stick.lx) > 2 ){
        aux_x = Ps3.data.analog.stick.lx;
        aux_x = map(aux_x, -128, 128, lim_inf, lim_sup);
        pos_servo_x = constrain(pos_servo_x + aux_x, 0, 180);
        ServoX.write(pos_servo_x);
        delay(15);

        if(pos_servo_x > 90){
          Serial.println("Moviendo Servo X a la derecha");
        }
        
        if(pos_servo_x < 90){
          Serial.println("Moviendo Servo X a la izquierda");
        }
      }

      if(abs(Ps3.data.analog.stick.ly) > 2 ){
        aux_y = Ps3.data.analog.stick.ly;
        aux_y = map(aux_y, -128, 128, lim_inf, lim_sup);
        pos_servo_y = constrain(pos_servo_y + aux_y, 0, 180);
        ServoY.write(pos_servo_y);
        delay(15);

        if(pos_servo_y > 90){
          Serial.println("Moviendo Servo Y hacia arriba");
        }
        
        if(pos_servo_y < 90){
          Serial.println("Moviendo Servo Y abajo");
        }

      }

    }

  }

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  Ps3.attach(controles);
  Ps3.begin("2c:33:7a:d7:47:25");

  //--------------MOTORREDUCTORES-----------
  pinMode(SalidaA, OUTPUT);
  pinMode(SalidaB, OUTPUT);
  pinMode(SalidaC, OUTPUT);
  pinMode(SalidaD, OUTPUT);

  //-----------------SERVOS---------------
  ServoX.attach(pin_servo_x);
  ServoY.attach(pin_servo_y);

  //-----------------MOSFET---------------

  pinMode(Mosfet, OUTPUT);
  
  ServoX.write(90);
  ServoY.write(90);

}

void loop() {
  // put your main code here, to run repeatedly:
  Ps3.setPlayer(player);

  if(Ps3.data.button.cross){
    ServoX.write(90);
    ServoY.write(90);
    pos_servo_x = 90;
    pos_servo_y = 90;
    delay(15);

    Serial.println("Centrado Exitosamente");

  }
  if(Ps3.data.button.r1){
    digitalWrite(Mosfet, HIGH);
    delay(250);
    digitalWrite(Mosfet, LOW);
    Serial.println("Shoot");
  }
  delay(100);
}
