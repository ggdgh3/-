unsigned long guandaoStartTime;
bool isGuandaoFirstCall = true;
bool isSpeedReduced = false;  // 新增：标记是否已降速

void guandao() {
    // 第一次进入时初始化计时器
    if (isGuandaoFirstCall) {
        guandaoStartTime = millis();
        Gray_Aoto_Mode = 1;  // 启用管道模式
        isGuandaoFirstCall = false;
        isSpeedReduced = false;  // 重置降速标记
    }

    unsigned long currentDuration = millis() - guandaoStartTime;

    // 500ms后降速（仅执行一次）
    if (!isSpeedReduced && currentDuration >= 500) {
        Forward_Left_Speed = 140;
        Forward_Right_Speed = 145;
        Move(Forward, Forward_Left_Speed, Forward_Right_Speed);
        isSpeedReduced = true;  // 标记已降速
    }

    // 持续执行管道跟踪（速度由Move函数控制）
    Automatic_Tracking_analogRead();

    // 检查是否超过6秒
    if (currentDuration >= 2500) {
//      Forward_Left_Speed = 120;
//        Forward_Right_Speed = 130;
        Gray_Aoto_Mode = 0;  // 退出管道模式
        Get_Stats = 2;
        currentState = STATE_TRACKING;
        isGuandaoFirstCall = true;  // 重置标志位
    }
}
