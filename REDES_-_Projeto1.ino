#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <NewPing.h>



//defines dos sonares
#define ECHO_PIN 7
#define TRIGGER_PIN 8

#define ECHO_PIN2 6
#define TRIGGER_PIN2 5

//distancia maxima que o sonar detecta em centimetros
#define MAX_DISTANCE 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2,ECHO_PIN2,MAX_DISTANCE);


// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
// buffers for receiving and sending data
String mensagem; 


byte mac[] = {
 0x00, 0xA0, 0xBB, 0xCC, 0xDE, 0x02
};
//Ethernet shield IP
//IPAddress ip(192, 168, 1, 41);

//server IP
IPAddress server(192, 168, 1, 42);



//variaveis globais necessarias 
int flag=0,passou_sensor_2=0,passou_sensor_1=0,pessoas=0,pessoas_max=0,packetSize;
unsigned int distancia_sonar1, distancia_sonar2;
int porta=8881;

void setup() {
   
  // start the Ethernet connection:
  Ethernet.begin(mac);
  //client port
  Udp.begin(8181);
  Serial.begin(9600);

  /*if(Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  //ip = Ethernet.localIP();*/
  
  
}

void loop() {

  distancia_sonar1 = sonar.ping_cm();
  distancia_sonar2 = sonar2.ping_cm();

  if(flag==0)
   {
      if(distancia_sonar1!=0)
      { 
        flag = 1;
        ativou_sonar1();
      }
      if(distancia_sonar2!=0)
      {
        flag=1;
        ativou_sonar2();
      }
   }
   delay(100);

}







void ativou_sonar1()
{
  Serial.println("ativou sensor 1 ");
  
    distancia_sonar1 = 0;
    distancia_sonar2 = 0;
    delay(600);
    do
    {
     distancia_sonar1 = sonar.ping_cm();
      if(distancia_sonar1>0 && distancia_sonar1<=4)
        distancia_sonar1=0;
      distancia_sonar2 = sonar2.ping_cm();
      if(distancia_sonar2>0 && distancia_sonar2<=4)
        distancia_sonar2=0;
    }while(distancia_sonar1 == 0 && distancia_sonar2 == 0); 

    
    if(distancia_sonar2 != 0)
    {
      passou_sensor_2=1;
    }
    if(distancia_sonar1 != 0 && passou_sensor_2 == 0)
    {
      passou_sensor_1=1;
    }
    
    if(passou_sensor_2)
    { 
       pessoas+=1; 
       //enviar mensagem avisando que a sala comecou a ser usada se pessoas =1
       if(pessoas == 1)
       {
          mensagem=String("1");
       
          Udp.beginPacket(server,porta);
          Udp.print(mensagem);
          Udp.endPacket();
          delay(10);
       }

        if(pessoas>pessoas_max)
          pessoas_max=pessoas;
       
       Serial.print("pessoas na sala=");
       Serial.println(pessoas);
       flag=0;
       passou_sensor_2=0;
       distancia_sonar1=0;
       distancia_sonar2=0;
       delay(1000);
    }
    if(passou_sensor_1)
    {
      Serial.println("entrou no nao fazer nada do sensor 1");
      passou_sensor_1=0;
      flag=0;
      delay(1000);
    
    }
    
}

void ativou_sonar2()
{
  Serial.println("ativou sensor 2 ");
 
    distancia_sonar1 = 0;
    distancia_sonar2 = 0;
        delay(600);
    do
    {
      distancia_sonar1 = sonar.ping_cm();
      if(distancia_sonar1>0 && distancia_sonar1<=4)
        distancia_sonar1=0;
      distancia_sonar2 = sonar2.ping_cm();
      if(distancia_sonar2>0 && distancia_sonar2<=4)
        distancia_sonar2=0;
      
    }while(distancia_sonar1 == 0 && distancia_sonar2 == 0); 

    
    
    if(distancia_sonar1 != 0)
    {
      passou_sensor_1=1;
    }

     if(distancia_sonar2 != 0 && passou_sensor_1 == 0)
    {
      passou_sensor_2=1;
    }
    
    if(passou_sensor_1)
    { 
       //Serial.println("entrou na diminuicao de pessoa");
       if(pessoas != 0)
         pessoas-=1; 
        
        Serial.print("pessoas na sala=");
        Serial.println(pessoas);
        
        if(pessoas == 0 && pessoas_max  != 0)
       {
          mensagem = String("2");
          Udp.beginPacket(server,porta);
          Udp.print(mensagem);
          Udp.endPacket();
          delay(10);

          mensagem = String(pessoas_max);
          Serial.print("Numero total de pessoas que estiveram na sala = ");
          Serial.println(pessoas_max);
          Udp.beginPacket(server,porta);
          Serial.println("\n\n");
          Udp.print(mensagem);
          Udp.endPacket();
          delay(10);          
          pessoas_max=0;
       }
       
       flag=0;
       passou_sensor_1=0;
       distancia_sonar1=0;
       distancia_sonar2=0;
       
       delay(1000);
    }
    if(passou_sensor_2)
    {
       Serial.println("entrou no nao fazer nada do sensor 2 ");
      passou_sensor_2=0;
      flag=0;
      delay(1000);
    }

}



