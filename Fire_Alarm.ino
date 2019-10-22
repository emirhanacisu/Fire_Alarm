#include <math.h>                             //sıcaklığı çevirmek için logoritma kullanıldığından gerekli olan kütüphane dahil edildi.
#define ag_ismi  "dilara"                   //bağlanılacak ağın ssid si tanımlanıldı.
#define ag_sifresi "dilara11"             //bağlanılacak ağın pw si tanımlanıldı.
#define SERVER "api.pushetta.com"     //kullandığımız sitenin api si tanımlanıldı.

String API("47dd445fcd4bace72690149fc53e6ec631d936bf");    //Pushetta da kullandığımız kanalın Api Key i tanımlanıldı.
String CHANNEL("yangin224");     //kullandığımız kanalın ismi tanımlanıldı.
int buzzerPin=8;                                //buzzerin takılı olduğu 8. pin kolaylık olması için  isimlendirildi.

void setup() 
{
 Serial.begin(115200);                    
 pinMode(buzzerPin,OUTPUT);          //buzzerPin yani 8 pin çıkış olarak ayarlanıldı.
 Serial.println("AT");                            //Esp ye her şeyin yolunda olup olmadığını soran                                        komut
 delay(5000);                                          
 
 if(Serial.find("OK"))                           //esp modülü ile bağlantıyı kurabilmişse modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
{
Serial.println("AT+CWMODE=1");   //esp modülümüzün WiFi modunu STA şekline getirildi. Bu mod ile modülümüz başka ağlara bağlanabilecek.
delay(2000);
String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
Serial.println(baglantiKomutu);
//CWJAP Join Access Point  komutu yolluyor ve ağa dahil olunuluyor.
delay(5000);
 }
}
double Termistor(int analogOkuma) //Termistor adında bir fonksiyon oluşturuldu ve sıcaklığı derece cinsine cevirmek için kullanıldı.
{
 double sicaklik;
 sicaklik = log(((10240000 / analogOkuma) - 10000));
 sicaklik = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik * sicaklik)) * sicaklik);
 sicaklik = sicaklik - 273.15;
 return sicaklik;
}


void loop()
{
 int deger = analogRead(A0);         //A0 pinine bağlı olan NTC den gelen değeri değere atadı.
  double sicaklik = Termistor(deger);  //Termistor fonksiyonundan donen değeri sıcaklık değişkenine atıyor.
  Serial.println(sicaklik);     //Sıcaklık ekrana basıldı.
  delay(500);
  if(sicaklik>30)                     //Eğer sıcaklık 30 dan büyükse ;
  {
    digitalWrite(buzzerPin,HIGH);       //BuzzerPin yani 8. pini aktif edildi.
    tone(buzzerPin,1000);
    veriPushetta(CHANNEL,"YANGiN VAR");      // VeriPushetta fonksiyonu calıştırıldı.
   
  }
  else    
  {
    digitalWrite(buzzerPin,LOW);
    noTone(buzzerPin);
   }
}

                                                        //Pushetta uygulamasına veri yollama
void veriPushetta(String CHANNEL,String text) {      
Serial.println(String("AT+CIPSTART=\"TCP\",\"") + SERVER + "\",80"); //server'a bağlanma
  delay(3000);
 String   deneme =("POST /api/pushes/");
    deneme+= (CHANNEL);
    deneme+= ("/ HTTP/1.1");
    deneme+= ("\r\n");
    deneme+= ("Host: ");
    deneme+= (SERVER);
    deneme+= ("\r\n");
    deneme+= ("Authorization: Token ");
    deneme+= (API);
    deneme+= ("\r\n");
    deneme+= ("Content-Type: application/json");
    deneme+= ("\r\n");
    deneme+= ("Content-Length: ");
    deneme+= (text.length() + 46);
    deneme+= ("\r\n\r\n");
    deneme+= ("{ \"body\" : \"");
    deneme+= (text);
    deneme+= ("\", \"message_type\" : \"text/plain\" }");
    deneme+= ("\r\n\r\n");
    
    Serial.print("AT+CIPSEND=");
 delay(100);
 Serial.println(deneme.length());
            // Sorun oluşmadıysa veriyi gönder.
 if(Serial.find(">")){
 Serial.println(deneme);
 Serial.println("AT+CIPCLOSE=0");
 delay(100);

 }
 else{
 Serial.println("AT+CIPCLOSE=0"); //bağlantının kapatılması için kullanılan komut
}
}

