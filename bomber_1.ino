#include <Wire.h>
#include <MPU9250_WE.h>
MPU9250_WE myMPU9250 = MPU9250_WE(0x68);

void setup()
{   
    Serial.begin(115200);
    Wire.begin(); 
    
    //led pin
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    //инициализация гироскопа
    if(!myMPU9250.init()){
      Serial.println("MPU9250 does not respond");
    }
    else{
      Serial.println("MPU9250 is connected");
    }
    
    //Во время ресета плату желательно держать неподвижно и горизонтально
    //чтобы гироскоп откалибровался
    Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
    delay(300);
    myMPU9250.autoOffsets();
    Serial.println("Done!");
    
    //настройки гироскопа (ничего необычного)
    myMPU9250.enableGyrDLPF();
    myMPU9250.setGyrDLPF(MPU9250_DLPF_6);
    myMPU9250.setSampleRateDivider(5);
    myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
    
    myMPU9250.enableGyrAxes(MPU9250_ENABLE_00Z); //нас интересует только ось Z
    
    delay(200);
}

int count = 0; //счетчик

void loop()
{
  xyzFloat gyr = myMPU9250.getGyrValues();
  Serial.print(count);
  Serial.print(" ");
  Serial.println(gyr.z);

  //если угловая скорость по оси Z больше 30 градусов в секунду - то прибавляем счетчик   
  if( fabs(gyr.z) > 30 && count < 255){
    count++;
  } else {
    count = 0; //иначе сбрасываем счетчик
  }
  
  //если счетчик не был сброшен в посление 5 сек - жест засчитан - WEAPONS ENGAGE!
  if(count >50){ 
    
    digitalWrite(LED_BUILTIN, HIGH);//Включаем диод
    delay(5000);//ждем 5 сек
    digitalWrite(LED_BUILTIN, LOW);//выключаем диод
    count = 0;//сбрасываем счетчик
    
  }else{
    digitalWrite(LED_BUILTIN, LOW);//не помню зачем это здесь
  }
  
  delay(100); //шаг цикла 0.1 сек
}
