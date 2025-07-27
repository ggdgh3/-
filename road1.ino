
unsigned long roadOneStartTime;
void road_one() {
    switch(roadOneStage) {
        case STAGE_PRE:
            Get_Stats=1;//禁用000
            Move(Forward, high_Speed1, high_Speed2);
//            Forward_Left_Speed=high_Speed1 ;
//            Forward_Right_Speed=high_Speed2 ;
            
            roadOneStartTime = millis();
            roadOneStage = STAGE_ON_SLOPE;
           
            break; 
        case STAGE_ON_SLOPE:
            unsigned long currentTime = millis();
            unsigned long elapsed = currentTime - roadOneStartTime;
            if(elapsed >= 2150) 
            {
//                Move(Forward, Forward_Left_Speed, Forward_Right_Speed);
//                Forward_Left_Speed = 140; 
//                Forward_Right_Speed = 135; 
                   
                  currentState = STATE_guandao;
            }
            break;  
            
  }
}
