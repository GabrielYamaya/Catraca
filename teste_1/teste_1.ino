#include <SoftwareSerial.h>
#include <RDM6300.h>

#define usuarios 2 //Número de usuários cadastrados subtraído um.

boolean check();

//Inicializa a serial nos pinos 12 (RX) e 13 (TX) 
SoftwareSerial RFID(12, 13);

//Configura o led na porta digital D2
int Led = 4;

uint8_t Payload[6]; // used for read comparisons

uint8_t cartao_valido[usuarios][6]; // usado como um usuário válido

RDM6300 RDM6300(Payload);

boolean confere = true;

void setup()
{
  //Define o pino do led como saida
  pinMode(Led, OUTPUT);
  
  //Inicializa a serial para o leitor RDM6300
  RFID.begin(9600);
  
  //Inicializa a serial para comunicacao com o PC
  Serial.begin(9600);
  
  //Informacoes iniciais
  Serial.println("Leitor RFID RDM6300");

  //usuários válidos
  cartao_valido[0][0] = 0x3;
  cartao_valido[0][1] = 0x25;
  cartao_valido[0][2] = 0x4A;
  cartao_valido[0][3] = 0x6B;
  cartao_valido[0][4] = 0xD9;

  cartao_valido[1][0] = 0x43;
  cartao_valido[1][1] = 0x0;
  cartao_valido[1][2] = 0x4B;
  cartao_valido[1][3] = 0x6B;
  cartao_valido[1][4] = 0xD8;
  
  cartao_valido[2][0] = 0x2;
  cartao_valido[2][1] = 0x6;
  cartao_valido[2][2] = 0x6C;
  cartao_valido[2][3] = 0x4D;
  cartao_valido[2][4] = 0x11;
}

void loop()
{
  //Apaga o led
  digitalWrite(Led, HIGH);
  
  //Aguarda a aproximacao da tag RFID
  while (RFID.available() > 0)
  {
    
    uint8_t c = RFID.read();
    if (RDM6300.decode(c))
    {
      if(check()){
        Serial.print("ID TAG: ");
        //Mostra os dados no serial monitor
        for (int i = 0; i < 5; i++) {
          Serial.print(Payload[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        //Aciona o led
        digitalWrite(Led, LOW);
        Serial.println("Acesso permitido. ");
        Serial.println("");
      } else {
        Serial.print("Acesso negado. ");
        Serial.println("");
      }
    }
  }

  //Aguarda 2 segundos e reinicia o processo
  delay(2000);
}

boolean check(){
  for(int k = 0; k <= usuarios; k++){
    for(int j = 0; j < 5; j++){
      confere = true;
      if(Payload[j] != cartao_valido[k][j]){
        confere = false;
      }
    }
    if(confere == true){
    return true;
    break;
    }
  }
  return false;
}
