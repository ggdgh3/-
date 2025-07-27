#include <Wire.h>         //调用IIC库函数
#include "MH_TCS34725.h"  //调用颜色识别传感器库函数
#include <ServoTimer2.h>
//爪子400和894
// 创建三个舵机对象
ServoTimer2 servo1;
ServoTimer2 servo2;
ServoTimer2 servo3;
MH_TCS34725 tcs = MH_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//引脚设定
int Car_DC_Motor_Pin[4] = {9, 10, 5, 6}; //直流电机引脚
int Gray_SensorPin[3] = {A3, A2, A0}; //寻迹、检测路口传感器
int motor_num = sizeof(Car_DC_Motor_Pin) / sizeof(Car_DC_Motor_Pin[0]);  //定义电机数量
int Car_Head_Gray_SensorPin_Num = 3;    //灰度传感器的数量

//小车初始速度设定
 volatile int Forward_Left_Speed = 131;  //小车前进时左轮速度
volatile int Forward_Right_Speed = 140;  //小车前进时右轮速度

#define high_Speed1   250 //小车man右轮速度
#define high_Speed2   255 //小车man左轮速度

#define Back_Left_Speed   90     //小车后退时左轮速度
#define Back_Right_Speed  85    //小车后退时右轮速度

int Left_Left_Speed = 140;   //小车左转时左轮速度
int Left_Right_Speed = 150;    //小车左转时右轮速度

int Right_Left_Speed = 150;    //小车右转时左轮速度
int Right_Right_Speed = 160;   //小车右转时右轮速度

//状态设定五种
enum {
  Forward,           //前进
  Back,              //后退
  Left,              //左转
  Right,             //右转
  Stop,              //停止
};  //跟踪自动

//红外各种模式状态
enum TaskState {
  TASK_IDLE,
  TASK_STOP,      // 停车检测色卡
  TASK_MOVING,    // 短暂前进（新增状态）
  TASK_CHECK,     // 检测红外+色卡
  TASK_MATCH,       // 色卡匹配
  TASK_POST_MATCH_TRACKING,
  TASK_EXECUTING,
  TASK_MISMATCH,
  TASK_DONE       // 任务完成
};

TaskState taskState = TASK_IDLE;  // 默认空闲
unsigned long taskStartTime;

//路口一状态枚举
enum { 
  STAGE_PRE, 
  STAGE_ON_SLOPE
};

volatile int roadOneStage = STAGE_PRE; // 防止编译器优化或意外修改

//三种控制状态设置
enum SystemState {
  STATE_pre,
  STATE_TRACKING,     // 正常循迹模式
  STATE_ROAD_ONE,     // 路口1状态机
  STATE_guandao,      //管道
  STATE_ROAD_two,     //路口2状态机
  STATE_COLOR_CHECK,   // 颜色检测模式
  STATE_ROAD_three    //
};

volatile SystemState currentState =  STATE_pre;

// 路口计数器（volatile防止优化）
volatile int Gray_Three = 0;  //记录三个灰度传感器同时触发的次数（即记录小车经过特殊路口的次数）

#define TrackingSensorNum 3     //小车寻迹时使用的灰度传感器数量

//循迹模式和状态初始化
int Gray_Aoto_Mode = 0; //常规灰度
int Get_Stats = 0;      //111检测

int hw = A1;

//标志位设定
int road1_bigin = 0;

//颜色识别的代码
int color_detection_card = 0; //记录颜色传感器识别到色卡的数值（红色为1，蓝色为2，绿色为3）
int color_detection_ballon = 0; //记录颜色传感器识别到要到弹珠的色卡的数值（红色为1，蓝色为2，绿色为3）

////时间设定
//unsigned long previousMillis = 0;
//unsigned long elapsedTimea = 0;        // 记录已经经过的时间
//void setange()
//{
//  servo1.write(100);//爪子张开
//  delay(500);
//  servo2.write(1179);//向下
//  delay(1000);
//  servo1.write(922);//爪子闭合
//  delay(500);
//  servo2.write(1210);//向上
//  delay(500);
//  currentState=STATE_TRACKING;
//}
// 时间设定
unsigned long previousMillis1 = 0;
unsigned long elapsedTimea1 = 0;        // 记录已经经过的时间
int setangeState = 0;  // 状态机状态变量

void setange() {
  unsigned long currentMillis1 = millis();
  
  switch(setangeState) {
    case 0: // 初始状态 - 爪子张开
      servo1.write(100);
      servo3.write(1885);
      previousMillis1 = currentMillis1;
      setangeState = 1;
      break;
      
    case 1: // 等待500ms
      if (currentMillis1 - previousMillis1 >= 500) {
        servo2.write(1388); // 向下
        previousMillis1 = currentMillis1;
        setangeState = 2;
      }
      break;
      
    case 2: // 等待1000ms
      if (currentMillis1 - previousMillis1 >= 1500) {
        servo1.write(922); // 爪子闭合
        delay(500);
         currentState = STATE_TRACKING;
        setangeState = 0; // 重置状态机
        previousMillis1 = currentMillis1;
      }
      break;
        
  }
}
void setup() {
  Serial.begin(9600);   //打开串口并启用9600波特率  （和计算机进行通信的速率）
  Motor_Sensor_Init();  //电机及灰度传感器引脚初始化
  pinMode(hw, INPUT);   //初始化红外传感器 
   Color_Init();
   servo1.attach(3);
  servo2.attach(8);
  servo3.attach(11);
  
}

void loop() {     
   switch(currentState) 
   {  
    case STATE_pre: setange();  break;
    case STATE_TRACKING: Automatic_Tracking_analogRead(); break;
    case STATE_ROAD_ONE: road_one(); break;
    case STATE_guandao: guandao(); break;  // 
    case STATE_ROAD_two: road_two(); break;  //
    case STATE_COLOR_CHECK :hw1();  break; // 颜色检测模式: road_two(); break;  // 
    case STATE_ROAD_three :road3(); break;
   }
}
