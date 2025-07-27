unsigned long roadtwoStartTime;
enum RoadTwoStage {
    STAGE_STOP,
    STAGE_MOVE_FORWARD,
    STAGE_DONE
};
RoadTwoStage roadTwoStage = STAGE_STOP;  // 初始状态

void road_two() {
    switch(roadTwoStage) {
        case STAGE_STOP:
            // 第一次进入时初始化
            if (Get_Stats != 1) {
                Get_Stats = 1;  // 禁用111检测
                Move(Stop, 255, 255);  // 停车
                return_color_card();  // 开始颜色识别
                roadtwoStartTime = millis();  // 记录开始时间
              
            }
            
            // 检查是否停车满1秒
            if (millis() - roadtwoStartTime >= 500) {
                roadTwoStage = STAGE_MOVE_FORWARD;  // 切换到前进阶段
                roadtwoStartTime = millis();  // 重置计时器
                
            }
            break;

        case STAGE_MOVE_FORWARD:
              Automatic_Tracking_analogRead();
            
            // 检查是否前进满300ms
            if (millis() - roadtwoStartTime >= 880) {
                roadTwoStage = STAGE_DONE;
                
            }
            break;

        case STAGE_DONE:
            currentState = STATE_COLOR_CHECK;  // 最终状态切换
            taskState   = TASK_CHECK;
            roadTwoStage = STAGE_STOP;  // 重置状态机
          
            break;
    }
}
