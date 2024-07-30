#include <Servo.h> // incluir libreria de servos

// Establecemos el maximo grado de los servos
Servo horizontal; // horizontal servo
int servoh = 180;   

int servohLimitHigh = 180;
int servohLimitLow = 60;


Servo vertical;   // Vetical servo
int servov = 45;    
int servovLimitHigh = 80;
int servovLimitLow = 15;


// pines de las LDR
int ldrlt = 0; //LDR top izquierda
int ldrrt = 1; //LDR top derecha 
int ldrld = 2; //LDR down izquierda
int ldrrd = 3; //ldr down rigt derecha

void setup()
{
  Serial.begin(9600);
// conecciones de los servos
// pinesa
  horizontal.attach(9); 
  vertical.attach(10);
  horizontal.write(180);
  vertical.write(45);
  delay(3000);
}

void loop() 
{
  // Leemos las LDR
  int lt = analogRead(ldrlt); 
  int rt = analogRead(ldrrt); 
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd); 
  
  int dtime = 10;
  int tol = 50;
  // Sacamos promedio para la ubicacion del panel
  int avt = (lt + rt) / 2; // promedio arriba
  int avd = (ld + rd) / 2; // promedio abajo
  int avl = (lt + ld) / 2; // promedio izquierda
  int avr = (rt + rd) / 2; // promedio derecha

  int dvert = avt - avd; // controlamos la diferencia de arriba y abajo
  int dhoriz = avl - avr; // controlamos la diferencia de izquierda y derecha 
  
  // Enviamos datos
  Serial.print(avt);
  Serial.print(" ");
  Serial.print(avd);
  Serial.print(" ");
  Serial.print(avl);
  Serial.print(" ");
  Serial.print(avr);
  Serial.print("   ");
  Serial.print(dtime);
  Serial.print("   ");
  Serial.print(tol);
  Serial.println(" ");
  
    
  if (-1*tol > dvert || dvert > tol) // Controlamos si esta en el rango caso contrario cambiamos el angulo vertical
  {
  if (avt > avd)
  {
    servov = ++servov;
     if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
  }
  else if (avt < avd)
  {
    servov= --servov;
    if (servov < servovLimitLow)
  {
    servov = servovLimitLow;
  }
  }
  vertical.write(servov);
  }
  
  if (-1*tol > dhoriz || dhoriz > tol) // Controlamos si la diferencia esta en el rango indicado sino cambiamos el angulo horizontal
  {
  if (avl > avr)
  {
    servoh = --servoh;
    if (servoh < servohLimitLow)
    {
    servoh = servohLimitLow;
    }
  }
  else if (avl < avr)
  {
    servoh = ++servoh;
     if (servoh > servohLimitHigh)
     {
     servoh = servohLimitHigh;
     }
  }
  else if (avl = avr)
  {
    // nothing
  }
  horizontal.write(servoh);
  }
   delay(dtime);

}




