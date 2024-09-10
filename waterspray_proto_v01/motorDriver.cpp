#ifndef _MOTOR_SOURCE_
#define _MOTOR_SOURCE_

/*
This file is control both motor for rotate turntable and photo sensor for count teeth of turntable.

단위 각도는 teeth 변수를 전달 받아서 계산하도록 만든다. 단위 각도는 왕복 회전 알고리즘에서 사용된다.
왕복해야 하는 각도의 범위가 주어지면, 해당 범위 안에서 속도를 조금씩 올리면서 움직이다가
중간 지점 혹은 도착 예정 지점에 도착하기 전에 속도가 0이 되도록 조정해서 멈출 수 있도록 한다.

회전 알고리즘: 속도를 올리는 부분은 간단하게 구현하면 됨.
속도를 줄일 때가 문제가 됨. 부드럽게 도착하도록 하는 방법은? 여기서 PID를 사용해야하나?

*/



#endif