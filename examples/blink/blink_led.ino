#include <Arduino.h>
#include <SyraUOS.h>



void task1()
{
  static int x = 0;
  _TaskWait(500);

  Serial.print("------ T1 : ");
  Serial.println(x++);
}
 
void task2()
{
  _TaskWait(1000);

  int led_status = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, HIGH);  
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("################ Start ###############");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);

  if(uOS.Setup(2))
  {
    Serial.println("Tasks Created");
    uOS.TaskCreate(task1);
    uOS.TaskCreate(task2);
  }  
}

void loop() {
  uOS.Run();  
}
