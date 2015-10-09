                  #include <EEPROM.h>
                  
                  int pinC1=12;  //····X···   ... viendo el "keypad" por frente (no por el reverso)
                  int pinC2=11;  //·····X··
                  int pinC3=10;  //······X·
                  int pinC4=9;   //·······X
                  int pinR1=8;   //X·······
                  int pinR2=7;   //·X······
                  int pinR3=6;   //··X·····
                  int pinR4=5;   //···X····
                  
                  int pinLED = 4;
                  int buzzer = 3;
                  
                  char teclas[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
                  
                  int claveMaster[] = {0,0,0,0};
                  int claveGuest[] = {1,1,1,1};
                  int claveIn[4];
                  int claveSerial[5];
                  int claveIndx = 0;
                  int bContador = 0;
                  int counter = 0;
                  int aContador=0;
                  int master, guest, user;
                  int SNumber;
                  int index=0;



            int teclaPresionada() {
                int r1,r2,r3,r4,numTecla;
                static int oldNumTecla=0;
                int contadorColumna;
                numTecla=0;  //-1==>Tecla repetida, 0==>No hay tecla presionada
                for (contadorColumna=1;contadorColumna<5;contadorColumna++) {
                  switch(contadorColumna) {
                     case 1:
                            digitalWrite(pinC1,LOW);
                            digitalWrite(pinC2,HIGH);
                            digitalWrite(pinC3,HIGH);
                            digitalWrite(pinC4,HIGH);
                            break;
                     case 2:
                            digitalWrite(pinC1,HIGH);
                            digitalWrite(pinC2,LOW);
                            digitalWrite(pinC3,HIGH);
                            digitalWrite(pinC4,HIGH);
                            break;
                     case 3:
                            digitalWrite(pinC1,HIGH);
                            digitalWrite(pinC2,HIGH);
                            digitalWrite(pinC3,LOW);
                            digitalWrite(pinC4,HIGH);
                            break;
                     case 4:
                            digitalWrite(pinC1,HIGH);
                            digitalWrite(pinC2,HIGH);
                            digitalWrite(pinC3,HIGH);
                            digitalWrite(pinC4,LOW);
                            break;
                  }
                  r1=digitalRead(pinR1);
                  r2=digitalRead(pinR2);
                  r3=digitalRead(pinR3);
                  r4=digitalRead(pinR4);
                  if(r1==LOW) {
                     numTecla=contadorColumna;
                     if (numTecla!=oldNumTecla) {
                        oldNumTecla=numTecla;
                        break;
                     } else
                        numTecla=-1;
                  } else if (r2==LOW) {
                     numTecla=4+contadorColumna;
                     if (numTecla!=oldNumTecla) {
                        oldNumTecla=numTecla;
                        break;
                     } else
                        numTecla=-1;
                  } else if (r3==LOW) {
                     numTecla=8+contadorColumna;
                     if (numTecla!=oldNumTecla) {
                        oldNumTecla=numTecla;
                        break;
                     } else
                        numTecla=-1;
                  } else if (r4==LOW) {
                     numTecla=12+contadorColumna;
                     if (numTecla!=oldNumTecla) {
                        oldNumTecla=numTecla;
                        break;
                     } else
                        numTecla=-1;
                  }
                }
                if ((contadorColumna==5)&&(numTecla==0))  //No se encontro tecla presionada
                   oldNumTecla=0;
                if (numTecla==-1)
                   numTecla=0;
                return(numTecla);
            }

            void setup() {
               pinMode(pinC1,OUTPUT);
               pinMode(pinC2,OUTPUT);
               pinMode(pinC3,OUTPUT);
               pinMode(pinC4,OUTPUT);
               pinMode(pinR1,INPUT_PULLUP);
               pinMode(pinR2,INPUT_PULLUP);
               pinMode(pinR3,INPUT_PULLUP);
               pinMode(pinR4,INPUT_PULLUP);
               pinMode(pinLED, OUTPUT);
               pinMode(buzzer, OUTPUT);
               Serial.begin(9600);
                             
                        if(EEPROM.read(8) ==1){                                            //Lee los valores disponibles en la EEPROM                                          
                          for(int j=0; j< 4; j++){
                            claveMaster[j]=EEPROM.read(j);
                            claveGuest[j]=EEPROM.read(j+4);
                          }
                       }
                    }
    
            void loop() {
               int teclaIndx;
               teclaIndx=teclaPresionada();
               char avail= ' ';
              
                  if((Serial.available() > 4)&&(Serial.available() < 7)){                  //Checa si hay algun byte disponible para lectura en serial y que sea menor a 7
                   
                  for(int i =0; i<5; i ++){
                    avail=Serial.read();                                                   //Lee los valores que hay en serial
                    claveSerial[i]=avail - '0';                                            //Guarda los valores en un arreglo integer
                   }
            
                  if(claveSerial[0] == 1){                                                 //Si el primer # en el arrgelo es 1 se cambia la clave master
                    claveMaster[0] = claveSerial[1];
                     claveMaster[1] = claveSerial[2];
                      claveMaster[2] = claveSerial[3];
                       claveMaster[3] = claveSerial[4];
                        Serial.println("Clave Master Cambiada :\n");
                        Serial.print("\t");Serial.println(claveMaster[0]);
                        Serial.print("\t");Serial.println(claveMaster[1]);
                        Serial.print("\t");Serial.println(claveMaster[2]);
                        Serial.print("\t");Serial.println(claveMaster[3]);
                    
                  }
                  else if(claveSerial[0] == 2){                                            //Si el primer # en el arrgelo es 2 se cambia la clave guest
                    claveGuest[0] = claveSerial[1];
                     claveGuest[1] = claveSerial[2];
                      claveGuest[2] = claveSerial[3];
                       claveGuest[3] = claveSerial[4];   
                        Serial.println("Clave Guest Cambiada :\n");
                        Serial.print("\t");Serial.println(claveGuest[0]);
                        Serial.print("\t");Serial.println(claveGuest[1]);
                        Serial.print("\t");Serial.println(claveGuest[2]);
                        Serial.print("\t");Serial.println(claveGuest[3]);       
                  }
                  else {
                    Serial.println("No se cambio ninguna clave");
                  }
                        int value = 1;                                                    //Guarda los nuevos valores en la EEPROM
                                        EEPROM.write(8,value);
                                        EEPROM.write(0,claveMaster[0]);
                                        EEPROM.write(1,claveMaster[1]);
                                        EEPROM.write(2,claveMaster[2]);
                                        EEPROM.write(3,claveMaster[3]);
                                        EEPROM.write(4,claveGuest[0]);
                                        EEPROM.write(5,claveGuest[1]);
                                        EEPROM.write(6,claveGuest[2]);
                                        EEPROM.write(7,claveGuest[3]);
               }
               else if(teclaIndx!=0) {                                                  //Si se presiona una tecla
                  Serial.print("Presionada ");
                  Serial.println(teclas[teclaIndx-1]);
                  
                  switch(teclas[teclaIndx-1]){
            
                    //Case para la tecla 0
                    case '0':
                      if(counter<4){                                                   //Mientras que haya espacio disponible guarda las teclas presionadas
                        claveIn[counter]=0;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;           
                          }
                            else if((counter==3)&&(aContador==1)){
                                aContador=2;
                                }
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }
                     break;
            
                     //Case para la tecla 1
                      case '1':
                        if(counter<4){
                          claveIn[counter]=1;
                               if((counter==3)&&(bContador==1)){
                                  bContador=2;
                            } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                               else if((counter==3)&&(bContador==2)){
                                  bContador=3;
                                }
                                   counter++;
                              }
                       else if(counter > 3){
                          counter=0;
                          }
                      break;
                     
                     //Case para la tecla 2
                     case '2':
                      if(counter<4){
                        claveIn[counter]=2;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }
                    break;
            
                     //Case para la tecla 3
                    case '3':
                      if(counter<4){
                        claveIn[counter]=3;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }        
                    break;
                  
                    //Case para la tecla 4
                    case '4':
                      if(counter<4){
                        claveIn[counter]=4;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }
                     break;
            
                     //Case para la tecla 5
                      case '5':
                        if(counter<4){
                          claveIn[counter]=5;
                               if((counter==3)&&(bContador==1)){
                                  bContador=2;
                            } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                               else if((counter==3)&&(bContador==2)){
                                  bContador=3;
                                }
                                   counter++;
                              }
                       else if(counter > 3){
                          counter=0;
                          }
                      break;
                     
                     //Case para la tecla 6
                     case '6':
                      if(counter<4){
                        claveIn[counter]=6;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }
                    break;
            
                     //Case para la tecla 7
                    case '7':
                      if(counter<4){
                        claveIn[counter]=7;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }        
                    break;
                     
                     //Case para la tecla 8
                     case '8':
                      if(counter<4){
                        claveIn[counter]=8;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }
                    break;
            
                     //Case para la tecla 9
                    case '9':
                      if(counter<4){
                        claveIn[counter]=9;
                             if((counter==3)&&(bContador==1)){
                                bContador=2;
                          } else if((counter==3)&&(aContador==1)){
                                aContador=2;}
                             else if((counter==3)&&(bContador==2)){
                                bContador=3;
                              }
                                 counter++;
                            }
                     else if(counter > 3){
                        counter=0;
                        }        
                    break;
                    case '*':
                      claveIndx = 0;
                      bContador=0;
                      break;
                    
                    case '#':                                                             //Establecer la EEPROM en "0"
                    EEPROM.write(8,0);
                    EEPROM.write(0,0);
                    EEPROM.write(1,0);
                    EEPROM.write(2,0);
                    EEPROM.write(3,0);
                    EEPROM.write(4,0);
                    EEPROM.write(5,0);
                    EEPROM.write(6,0);
                    EEPROM.write(7,0);
                    break;
                    
                    case 'A':
            
                            counter=0, master=0, guest=0;                                       //Inicializa los valores en 0
                                                                                                //Primer toque de la tecla a  
                         if(aContador==0){ 
                                                                                                //Preparar la tecla  A por si la vuelven a presionar
                          aContador = 1;
                                  }
                                  else if((counter < 4)&&(aContador==2)){
             
                                      for(int i=0; i < 4; i++){
                                        if(claveIn[i] != claveMaster [i]){
                                          master ++;
                                          
                                        }
                                       if(claveIn[i] != claveGuest [i]){
                                          guest ++; 
                                         
                                          } 
            
                                          if((i==3)&&(master==0)&&(guest!=0)){                  //Si los valores del la verificacion con master fueron 0 y los de guest mayor que 0. Se da la bienvenida al master
                                        Serial.print("WELCOME MASTER\n");
                                        digitalWrite(pinLED, HIGH);
                                        delay(1000);
                                        digitalWrite(pinLED, LOW); 
                                        counter = 0;
                                            
                                          }
                                          else if((i==3)&&(guest==0)&&(master!=0)){            //Si los valores del la verificacion con guest fueron 0 y los de master mayor que 0. Se da la bienvenida al guest
                                        Serial.print("WELCOME GUEST\n");
                                        digitalWrite(pinLED, HIGH);
                                        delay(1000);
                                        digitalWrite(pinLED, LOW); 
                                        counter = 0;
                                            
                                          }
                                          else if((guest!=0)&&(master!=0)&&(i==3)){           //Si nunguna clave coincide se reinicia todo y no se acepta la entrada
                                          Serial.print("Wrong code\n");
                                          counter = 0;
                                            }               
            
                                      }
                                                          
                                      aContador=0;
                                      master=0;
                                      guest=0;
                                    }
                                    else{
                                      counter = 0;
                                      aContador=0;
                                      }
                      break;
                    
                    case 'B':
            
                           counter = 0; 
                                                                                                            //Primer toque de la tecla B  
                         if(bContador==0){ 
                                                                                                           //Preparar la tecla B por si la vuelven a presionar
                          bContador = 1;
                                  }
                                                                                                          //Si vuelven a presionar la tecla B verifica que la contraseña sea correcta
                          else if(bContador==2){
            
                                                                                                          //Checa que la clave sea igual a la introducida, si la clave es menor a 3 
                                    if(counter < 4){
                                                                                                           //Recorre la clave introducida para verificar si es igual a la clave
                                        for(int i=0; i < 4; i++){
                                        if(claveIn[i] != claveMaster [i]){
                                          master ++;
                                          
                                        }
                                       if(claveIn[i] != claveGuest [i]){
                                          guest ++; 
                                          
                                          } 
            
                                          if((i==3)&&(master==0)&&(guest!=0)){                             //Se da la bienvenida al master y se prepara para introducir otro codigo
                                        Serial.print("WELCOME MASTER\n");
                                        digitalWrite(pinLED, HIGH);
                                        delay(1000);
                                        digitalWrite(pinLED, LOW);
                                         //Prepara para recibir la nueva contraseña
                                     Serial.print("\nIngresa codigo nuevo\n");  
                                        counter = 0;
                                        user= 1;
                                            
                                          }
                                          else if((i==3)&&(guest==0)&&(master!=0)){                     //Se da la bienvenida al guest y se prepara para introducir otro codigo
                                        Serial.print("WELCOME GUEST\n");
                                        digitalWrite(pinLED, HIGH);
                                        delay(1000);
                                        digitalWrite(pinLED, LOW); 
                                         //Prepara para recibir la nueva contraseña
                                     Serial.print("\nIngresa codigo nuevo\n"); 
                                        counter = 0;
                                        user = 2;
                                            
                                          }
                                          else if((guest!=0)&&(master!=0)&&(i==3)){                     //No se deja entrar
                                          Serial.print("Wrong code\n");
                                          counter = 0;
                                          bContador=0;
                                            }               
            
                                      }
                                    }
                                    master=0, guest=0;                          
                                       
                                    
                                                      
                              }
                                 else if(bContador==3){     
                                  int value=0;
                                                                                                           //Guarda la clave nueva para el master
                                      if(user== 1){ 
                                        claveMaster[0]=claveIn[0];
                                        claveMaster[1]=claveIn[1]; 
                                        claveMaster[2]=claveIn[2];
                                        claveMaster[3]=claveIn[3];
                                        
                                        value=1;
                                      }
                                      else if(user == 2){
                                        claveGuest[0]=claveIn[0];                                         //Guarda la clave para el guest
                                        claveGuest[1]=claveIn[1]; 
                                        claveGuest[2]=claveIn[2];  
                                        claveGuest[3]=claveIn[3];
                                        
                                        value=1;            
                                      }
            
                                        EEPROM.write(8,value);                                          //Guarda los valores en la EEPROM
                                        EEPROM.write(0,claveMaster[0]);
                                        EEPROM.write(1,claveMaster[1]);
                                        EEPROM.write(2,claveMaster[2]);
                                        EEPROM.write(3,claveMaster[3]);
                                        EEPROM.write(4,claveGuest[0]);
                                        EEPROM.write(5,claveGuest[1]);
                                        EEPROM.write(6,claveGuest[2]);
                                        EEPROM.write(7,claveGuest[3]);
                                   
                                    for(int i=0; i < 4; i++){
                                      claveIn[i] = 0;
                                        }
                                       
                                                          
                                    //Imprime la clave nueva
                                    digitalWrite(pinLED, HIGH);
                                    delay(1000);
                                    digitalWrite(pinLED, LOW);
                                    delay(500);   
                                    digitalWrite(pinLED, HIGH);
                                    delay(1000);
                                    digitalWrite(pinLED, LOW);   
                                    bContador=0;
                        }
                      break;
                    case 'C':                                                                       //Imprime los valores de la EEPROM
                    Serial.println(EEPROM.read(0));
                    Serial.println(EEPROM.read(1));
                    Serial.println(EEPROM.read(2));
                    Serial.println(EEPROM.read(3));
                    Serial.println(EEPROM.read(4));
                    Serial.println(EEPROM.read(5));
                    Serial.println(EEPROM.read(6));
                    Serial.println(EEPROM.read(7));
                    Serial.println(EEPROM.read(8));
                    break;
                    case 'D':
                    Serial.println(claveMaster[0]);
                    Serial.println(claveMaster[1]);
                    Serial.println(claveMaster[2]);
                    Serial.println(claveMaster[3]);
                    Serial.println(claveGuest[0]);
                    Serial.println(claveGuest[1]);
                    Serial.println(claveGuest[2]);
                    Serial.println(claveGuest[3]);
                   
                    break;
                    default:
                      claveIn[claveIndx] = teclas[teclaIndx-1];
                      if(++claveIndx>3){
                          claveIndx = 3;
                      }
               
                  }
                    }
               
               delay(10);
            }
