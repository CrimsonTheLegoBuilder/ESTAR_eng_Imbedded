#include <SoftwareSerial.h> //pms7003과 시리얼 통신을 하기위한 라이브러리

SoftwareSerial pms(12, 13);   //아두이노 12번 핀 --> pms7003 TX핀, 아두이노 13번핀 --> pms7003 RX핀

void setup() {
  Serial.begin(9600);  //시리얼 출력을 확인하기위한 시리얼통신 시작
  pms.begin(9600);    //pms7003으로부터 데이터를 받기 위한 시리얼통신 시작
}

void loop() {
  if(pms.available()>=32){    //신호 받은 갯수가 32개 이상일때
  int chksum=0;   //체크섬 변수선언
  int pm1, pm25, pm10;
  unsigned char pms_data[32];   //데이터 배열변수 선언
    for(int j=0; j<32 ; j++){   //데이터 32개를 받는 반복
      pms_data[j]=pms.read();   //데이터를 받음
      switch(j){    //데이터 종류를 보기위한 구문
        case 0: Serial.print("시작1:"); break;
        case 1: Serial.print("시작2:"); break;
        case 2: Serial.print("길이1:"); break;
        case 3: Serial.print("길이2:"); break;
        case 4: Serial.print("데이터1-1:"); break;
        case 5: Serial.print("데이터1-2:"); break;
        case 6: Serial.print("데이터2-1:"); break;
        case 7: Serial.print("데이터2-2:"); break;
        case 8: Serial.print("데이터3-1:"); break;
        case 9: Serial.print("데이터3-2:"); break;
        case 10: Serial.print("데이터4-1:"); break;
        case 11: Serial.print("데이터4-2:"); break;
        case 12: Serial.print("데이터5-1:"); break;
        case 13: Serial.print("데이터5-2:"); break;
        case 14: Serial.print("데이터6-1:"); break;
        case 15: Serial.print("데이터6-2:"); break;
        case 16: Serial.print("데이터7-1:"); break;
        case 17: Serial.println(" "); Serial.print("데이터7-2:"); break;
        case 18: Serial.print("데이터8-1:"); break;
        case 19: Serial.print("데이터8-2:"); break;
        case 20: Serial.print("데이터9-1:"); break;
        case 21: Serial.print("데이터9-2:"); break;
        case 22: Serial.print("데이터10-1:"); break;
        case 23: Serial.print("데이터10-2:"); break;
        case 24: Serial.print("데이터11-1:"); break;
        case 25: Serial.print("데이터11-2:"); break;
        case 26: Serial.print("데이터12-1:"); break;
        case 27: Serial.print("데이터12-2:"); break;
        case 28: Serial.print("데이터13-1:"); break;
        case 29: Serial.print("데이터13-2:"); break;
        case 30: Serial.print("체크1:"); break;
        case 31: Serial.print("체크2:"); break;
        default: break;
      }
      if(j==31){    //데이터의 끝에서 줄바꿈
        Serial.println(pms_data[j]);
      }
      else{   //아닐경우 데이터 출력
        Serial.print(pms_data[j]);
        Serial.print(" ");
      }
      if(j<30)    //데이터를 체크섬 변수에 계속 더함
        chksum+=pms_data[j];
    }

    if(pms_data[30] != (unsigned char)(chksum>>8) || pms_data[31]!= (unsigned char)(chksum) ){
      return 0;
      Serial.println("데이터 체크 오류");    //체크섬 데이터로 확인했을때 오류가 있을 때 
    }
    if(pms_data[0]!=0x42 || pms_data[1]!=0x4d ){
      return 0;
      Serial.println("시작 신호 오류");   //시작신호에서부터 오류가 있을때
    }
    /*
     * 
  Serial.print("1.0ug/m3:");    //이런식으로 출력할 경우 오류가 있음
  Serial.print(pms_data[10]);
  Serial.print(pms_data[11]);
  Serial.print("  2.5ug/m3:");
  Serial.print(pms_data[12]);
  Serial.print(pms_data[13]);
  Serial.print("  10ug/m3:");
  Serial.print(pms_data[14]);
  Serial.println(pms_data[15]);
  Serial.println("");
     */
  pm1=pms_data[10]<<8 | pms_data[11];   //수정된 출력 방법
  pm25=pms_data[12]<<8 | pms_data[13];
  pm10=pms_data[14]<<8 | pms_data[15];
  Serial.print("1.0ug/m^3:");
  Serial.print(pm1);
  Serial.print("  2.5ug/m^3:");
  Serial.print(pm25);
  Serial.print("  10ug/m^3:");
  Serial.println(pm10);
  Serial.println("");
  } 
}
//[출처] 아두이노로 PMS7003 미세먼지센서 원리 및 사용하기|작성자 탱두