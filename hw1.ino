// 计时变量
unsigned long mismatchStartTime; 
unsigned long trackingStartTime;
unsigned long daoStartTime;
unsigned long servoActionStartTime; // 新增：舵机动作开始时间
int currentDaoStep = 0;            // 新增：当前执行的DAO步骤

#define MISMATCH_DURATION 698
#define POST_MATCH_TRACKING_DURATION 600
#define DAO_DURATION 15000
#define SERVO_ACTION_DURATION1 1300 // 每个舵机动作持续时间
#define SERVO_ACTION_DURATION2  1000
void hw1() {
    switch (taskState) {
        case TASK_IDLE:
            Automatic_Tracking_analogRead();
            break;

        case TASK_CHECK:
            if (digitalRead(hw) == 0) {
                Move(Stop, 255, 255);
                return_color_ballon();
                taskState = TASK_MATCH;
                Serial.println("红外触发，开始颜色匹配");
            } else {
                Automatic_Tracking_analogRead();
            }
            break;

        case TASK_MATCH:
            if (color_detection_ballon == color_detection_card) {
                taskState = TASK_POST_MATCH_TRACKING;
                trackingStartTime = millis();
                Serial.println("颜色匹配，开始循迹900ms");
            } else {
                taskState = TASK_MISMATCH;
                mismatchStartTime = millis();
                Serial.println("颜色不匹配，循迹650ms");
            }
            break;

        case TASK_POST_MATCH_TRACKING:
            Automatic_Tracking_analogRead();
            if (millis() - trackingStartTime >= POST_MATCH_TRACKING_DURATION) {
                taskState = TASK_EXECUTING;
                daoStartTime = millis();
                currentDaoStep = 0; // 重置DAO步骤计数器
                servoActionStartTime = millis(); // 记录第一步开始时间
                Serial.println("匹配后循迹完成，开始执行dao动作");
            }
            break;

        case TASK_MISMATCH:
            Automatic_Tracking_analogRead();
            if (millis() - mismatchStartTime >= MISMATCH_DURATION) {
                taskState = TASK_CHECK;
                Serial.println("650ms循迹结束，重新检测红外");
            }
            break;

        case TASK_EXECUTING:
            {
                bool daoCompleted = executeDaoStepByStep(); // 非阻塞执行DAO
                if (daoCompleted || (millis() - daoStartTime >= DAO_DURATION)) {
                    taskState = TASK_DONE;
                    Serial.println("dao动作执行完成");
                }
            }
            break;

        case TASK_DONE:
            Get_Stats = 3;
            currentState = STATE_TRACKING;
            break;
    }
}

// 非阻塞式DAO执行函数
bool executeDaoStepByStep() {
    unsigned long currentTime = millis();
    
    switch (currentDaoStep) {
        case 0: // 第一步: 停止移动并设置舵机2
            Move(Stop, 255, 255);
            servo2.write(500);
            if (currentTime - servoActionStartTime >= SERVO_ACTION_DURATION1) {
                currentDaoStep++;
                servoActionStartTime = currentTime;
            }
            return false;
            
        case 1: // 第二步: 设置舵机3
            servo3.write(1365);
            if (currentTime - servoActionStartTime >= SERVO_ACTION_DURATION1) {
                currentDaoStep++;
                servoActionStartTime = currentTime;
            }
            return false;
            
        case 2: // 第三步: 设置舵机2
            servo2.write(1150);
            if (currentTime - servoActionStartTime >= SERVO_ACTION_DURATION1) {
                currentDaoStep++;
                servoActionStartTime = currentTime;
            }
            return false;
            
        case 3: // 第四步: 设置舵机1
            servo1.write(400);
            if (currentTime - servoActionStartTime >= SERVO_ACTION_DURATION1) {
                currentDaoStep++;
                servoActionStartTime = currentTime;
            }
            return false;
            
        case 4: // 第五步: 再次设置舵机2
            servo2.write(500);
            if (currentTime - servoActionStartTime >= SERVO_ACTION_DURATION2) {
                currentDaoStep++;
                servoActionStartTime = currentTime;
            }
            return false;
            
        case 5: // 第六步: 再次设置舵机3
            servo3.write(1880);
            if (currentTime - servoActionStartTime >= SERVO_ACTION_DURATION2) {
                return true; // 所有步骤完成
            }
            return false;
            
        default:
            return true;
    }
}
