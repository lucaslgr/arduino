#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  /** xTaskCreate - Parâmetros
   * 1º : Endereço da funcao a ser executada na task
   * 2º : Label da task para ser identificada em processos de depuração por exemplko
   * 3º : Número de bytes de memória para a tarefa
   * 4º : Parametro opcional para ser carregado quando a tarefa for inicializada
   * 5º : Prioridade, quanto maior, maior é a prioridade
   * 6º : Handle, ou seja, um manipulador que permite acessar essa tarefa posteriormente para manipular a sua prioridade por exemplo
   */
  xTaskCreate(
      TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(
      TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(
      Taskprint, "task3", 128, NULL, 1, NULL);
  vTaskStartScheduler();
}
void loop()
{
}
void TaskBlink1(void *pvParameters)
{
  pinMode(8, OUTPUT);
  while (1)
  {
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(8, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
void TaskBlink2(void *pvParameters)
{
  pinMode(7, OUTPUT);
  while (1)
  {
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
void Taskprint(void *pvParameters)
{
  int counter = 0;
  while (1)
  {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}