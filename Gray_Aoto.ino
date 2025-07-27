//检测和循迹  灰度传感器A0,A2,A3 用来小车巡线时，返回传感器数值；

int Detection_tracking_analogRead() //灰度传感器A0，A2,A3 用来小车巡线时，返回传感器数值；
{
  int num = 0;
  int analogRead_data[3] = {0, 0, 0};
  for (int i = 0; i < Car_Head_Gray_SensorPin_Num; i++)
  {
    analogRead_data[i] = analogRead(Gray_SensorPin[i]); //A3,A2,A0
    switch(Gray_Aoto_Mode) {
      case 0:{
        //常规灰度
        if( analogRead_data[0] <= 430){analogRead_data[0] = 0;} else{analogRead_data[0] = 1;}//17 右
        if( analogRead_data[1] <= 420){analogRead_data[1] = 0;} else{analogRead_data[1] = 1;}//16 中
        if( analogRead_data[2] <= 410){analogRead_data[2] = 0;} else{analogRead_data[2] = 1;} //14 左
        break;
      }
      case 1:{
          
         //管道灰度
        if( analogRead_data[0] <= 300){analogRead_data[0] = 0;} else{analogRead_data[0] = 1;}//17 右
        if( analogRead_data[1] <= 355){analogRead_data[1] = 0;} else{analogRead_data[1] = 1;}//16 中
        if( analogRead_data[2] <= 325){analogRead_data[2] = 0;} else{analogRead_data[2] = 1;} //14 左

        break;
      }

      default: break;

      
    }
    num |= ( (analogRead_data[i]) << i);
  } 
 // Serial.println(num);
  return num;
}
