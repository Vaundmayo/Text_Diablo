// Linux 호환을 위해 수정된 코드
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Windows 환경 감지
#ifdef _WIN32
    #include <windows.h> // Sleep 등
    #include <conio.h> // getch(), kbhit() 등 입력 함수
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// =============
//  함수 선언
// =============
void Q1_1(); void Q1_2(); void Q1_3(); void Q1_4(); void Q1_5(); void Q1_6();
void Q2_1(); void Q2_2(); void Q2_3(); void Q2_4(); void Q2_5(); void Q2_6();
void Q3_1(); void Q3_2(); void Q3_3(); void Q3_4(); void Q3_5(); void Q3_6();
void Q4_1(); void Q4_2(); void Q4_3();
void h_m(); void set();
void Opening(); void Load(); void Insert_magic(); void Insert_weapon(); void Insert_defence();
void Play_1(); void Item_store(); void Defence_Store(); void Save_option(); void Condition();
void Weapon_Store(); void Battle(); void cheatcenter();
void M_A(); void Mg();
int my_random(int n);
void clearbuff(); // 입력 버퍼 비우기(잘못된 입력 처리)


// ==================
//  os별 함수 구현
// ==================

// Windows 전용 함수 대체
#ifdef _WIN32
    // Windows용 화면 클리어
    void clrscr() {
        system("cls");
    }
    
    // ms 단위 딜레이
    void delay(int ms) {
        Sleep(ms);
    }
     
    // Windows는 conio.h의 kbhit(), getch() 사용
    
#else
    // Linux/macOS용 구현
    void clrscr() {
        printf("\033[2J\033[1;1H");
        fflush(stdout);
    }
    
    // ms 단위 딜레이. usleep은 마이크로초 단위 -> ms*1000
    void delay(int ms) {
        usleep(ms * 1000);
    }
    // kbhit 대체 (non-blocking 키 입력 확인)
    int kbhit(void) {
        struct termios oldt, newt;
        int ch;
        int oldf;
        int hit = 0;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();
        
        if (ch != EOF) {
            ungetc(ch, stdin);
            hit = 1;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        return hit;
    }
    // getch 대체
    int getch(void) {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif

// 공통 함수들 (OS 무관)

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}
// random 대체
int my_random(int n) {
    if (n <= 0) return 0;
    return rand() % n;
}
// randomize 대체
void randomize() {
    srand(time(NULL));
}

// 입력 버퍼 비우기(엔터 등 남아있을때 처리)
void clearbuff() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// =================
//  구조체 정의
// =================

// 몬스터 정보
struct monster_struct {
  char name[100];
  int lv; //레벨
  int hp; //체력
  int mp; //마나
  int attack; //공격력
  int exp; //경험치
  int gold; //골드
  int defence; //방어력
  int nhp; //현재 체력
  int nmp; //현재 마나
} monster; //몬스터 구조체

// 마법 정보
struct magic_struct {
  char name[100]; //마법 이름
  int power; //마법 공격력
  int ump; //마나 소모량
  int lv; //마법 레벨
} magic[8]; //마법 구조체 배열

// 방어구 정보
struct defence_struct {
  char name[100]; //방어구 이름
  int defence; //방어력
  int hp; //체력 보너스
  int mp; //마나 보너스
  int cost; //가격
} defence[100]; //방어구 구조체 배열

// 무기 정보
struct weapon_struct { 
  char name[100]; //무기 이름
  int power; //공격력
  int hp_bonus; //체력 보너스
  int mp_bonus; //마나 보너스
  int cost; //가격
} weapon[100]; //무기 구조체 배열

// 플레이어 정보
struct my_struct {
  char name[100]; //이름
	int item[8]; //아이템 갯수
	int cs; //직업
	int lv; //레벨
	int hp; //체력
	int nhp; //현재 체력
	int mp; //마나
	int nmp; //현재 마나
	int attack; //공격력
	int exp; //경험치
	int gold; //골드
	int defence; //방어력
	int wh; //현재 웨이포인트
	int code; //저장 코드
} user; //유저 구조체

// =================
//  전역변수
// =================
int cheat=0,l_m,count1,count2;
int escape = 0; // 도망가기 전역변수 선언

// ==================
//  main
// ==================
int main() { 
  int a;
  #ifdef _WIN32
        // Windows 콘솔을 UTF-8 모드로 설정 (한글 깨짐 방지)
        SetConsoleOutputCP(65001);  // UTF-8 출력
        SetConsoleCP(65001);        // UTF-8 입력
  #endif
	randomize(); // 랜덤 시드 설정
	cheat=0;
	while(1)
  {
  	clrscr();
  	printf("\n\n\n\n\n\n");
  	printf("\n            ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
  	printf("\n            ┃                                            ┃");
  	printf("\n            ┃              Text Diablo III               ┃");
  	printf("\n            ┃            Title: 하수구의 군주            ┃");
  	printf("\n            ┃                             Demo V0.99     ┃");
  	printf("\n            ┃               1. New Game                  ┃");
  	printf("\n            ┃               2. Load Game                 ┃");
  	printf("\n            ┃               3. End                       ┃");
  	printf("\n            ┃ Press Input number:                        ┃");
  	printf("\n            ┃                          Copy Left 2025    ┃");
  	printf("\n            ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
  	gotoxy(35,16);scanf("%d",&a); // 메뉴 번호 입력

    // 1~3 사이 값이면 메뉴선택 완료, 아니면 다시 입력
  	if(a>0 && a<=3) break;
    else {
        clearbuff(); // 잘못된 입력으로 남은 버퍼 제거
        continue;
    }
	}
  if(a==1) Opening(); // 새 게임
  if(a==2) Load(); // 불러오기
  if(a==3) {
    clrscr();
    exit(0); // 프로그램 종료
  }

  Insert_magic(); 
  Insert_weapon();
  Insert_defence();
  Play_1();
  return 0;
}

// ===================
//  무기 데이터
// ===================
void Insert_weapon() {
  int i;
  FILE *fp13=fopen("weapon.qwe","r"); // 텍스트 모드로 읽기
  if(fp13 == NULL) {
    printf("Error: Cannot open weapon.qwe\n");
    return;
}
  // 첫 줄 : 무기 개수
  fscanf(fp13,"%d",&count1);
  // 최대 100개 무기 정보 읽기
  for(i=0;i<100;i++)
	fscanf(fp13,"%s %d %d %d %d",weapon[i].name,&weapon[i].power,&weapon[i].hp_bonus,&weapon[i].mp_bonus,&weapon[i].cost);
  fclose(fp13);
  return;
}

// =================
//  방어구 데이터
// =================
void Insert_defence() {
  int i;
  FILE *fp56=fopen("defence.qwe","rt");
  if(fp56 == NULL) {
    printf("Error: Cannot open defence.qwe\n");
    return;
}
  // 첫 줄 : 방어구 개수
  fscanf(fp56,"%d",&count2);
  // count2 만큼 읽기
  for(i=0;i<count2;i++)
	fscanf(fp56,"%s %d %d %d %d",defence[i].name,&defence[i].defence,&defence[i].hp,&defence[i].mp,&defence[i].cost);
  fclose(fp56);
  return;
}

// ===============
//  캠프 메뉴
// ===============
void Play_1() { 
  int i,q;
  while(1){ // 재귀 삭제 (무한 호출 방지)
    clrscr();

    // 캠프 방문시 hp, mp 최대치로 회복
    user.nhp=user.hp;
    user.nmp=user.mp;

    // 포션 개수 20개 초과시 자동으로 소모
    for(i=0;i<8;i++)
	  if(user.item[i]>20) {
      user.item[i]=20;printf("%d번 포션의 수량이 20개 이상이 되어서 자동으로 소모됩니다",i+1);getch();clrscr(); 
    }

    // 방어력 20이상이 되면 하락
    if(user.defence>20) {
      user.defence=20;printf("디펜스가 20이상이 되면 자동으로 디펜스가 하락됩니다.");getch();clrscr();
    }
    printf("\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━ Camp ━━━━━━━━━━━━━━━━━━━━━━\n\n");
    printf("                    1. Condition                  \n");
    printf("                    2. Item Store                 \n");
    printf("                    3. Weapon Store               \n");
    printf("                    4. Armor Store                \n");
    printf("                    5. Battle Quest               \n");
    printf("                    6. Save                       \n");
    printf("                    7. Exit                       \n");
    printf("   Please Insert Number:                          \n");
    gotoxy(26,11);scanf("%d",&q);

    // 메뉴에 따라 함수 호출
    if( q==2) Item_store();
    if( q==4) Defence_Store();
    if( q==6) Save_option();
    if( q==7) {clrscr(); exit(0);}
    if( q==1) Condition();
    if( q==3) Weapon_Store();
    if( q==5) Battle();
    if( q==1008) cheatcenter();

    // 다른 입력은 무시하고 다시 메뉴
    else {
      q = 0; // q 초기화
      clearbuff();
      continue;
    }
  }
  return;
}

// ================
//  치트 센터
// ================
void cheatcenter()
{
  int ca;
  clrscr();
  while(1)
  {
    printf("\n1.Money + 100000");
    printf("\n2.Att + 1");
    printf("\n3.Hp + 10");
    printf("\n4.Mp + 10");
    printf("\n5.Waypoint +1");
    printf("\nInput Number:");
    gotoxy(14,7);scanf("%d",&ca);
    // 1~5 이외엔 다시 하기
    if(ca<1 || ca>5) continue;
    // 능력치 증가
    switch(ca)
    {
      case 1: user.gold+=100000;break;
      case 2: user.attack+=1;break;
      case 3: user.hp+=10;break;
      case 4: user.mp+=10;break;
      case 5: user.wh+=1;break;
    }
  printf("\nComplete!");
  clearbuff();
  getch();
  break;
  }
return;
}

void Battle() {
  escape = 0; // 도망 상태 초기화
  int l,time;
  FILE *fp24 = NULL;
  while(1)
  {
    char data;
    clrscr();
    printf("\n");
    printf("  ┏━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┓\n");
    printf("  ┃  A C T 1 ┃  A C T 2 ┃  A C T 3 ┃  A C T 4 ┃\n");
    printf("  ┣━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━┫\n");
    printf("  ┃1. Quest 1┃7. Quest 1┃13.Quest 1┃19.Quest 1┃\n");
    printf("  ┃2. Quest 2┃8. Quest 2┃14.Quest 2┃20.Quest 2┃\n");
    printf("  ┃3. Quest 3┃9. Quest 3┃15.Quest 3┃21.Quest 3┃\n");
    printf("  ┃4. Quest 4┃10 Quest 4┃16.Quest 4┃0. Exit   ┃\n");
    printf("  ┃5. Quest 5┃11 Quest 5┃17.Quest 5┃          ┃\n");
    printf("  ┃6. Quest 6┃12 Quest 6┃18.Quest 6┃          ┃\n");
    printf("  ┗━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┛\n");

    l = 22; // 잘못 입력시 이전 l값이 유지됨, 초기화

    printf("Select Quest Number(1~%2d):",user.wh);
    gotoxy(27,12);scanf("%d",&l); // 현재 wh 범위까지 선택 가능
    if(l==0) break;
    if(l > 0 && l <= user.wh) {} // 입력이 유효한 범위일 경우 아무 작업 X
      else { // 범위 밖일 경우 continue
      printf("\n You can't go there....");
      clearbuff();
      getch();
      continue;
    }
    if(l==1)   fp24=fopen("Quest1_1.dat","rt");
    if(l==2)   fp24=fopen("Quest1_2.dat","rt");
    if(l==3)   fp24=fopen("Quest1_3.dat","rt");
    if(l==4)   fp24=fopen("Quest1_4.dat","rt");
    if(l==5)   fp24=fopen("Quest1_5.dat","rt");
    if(l==6)   fp24=fopen("Quest1_6.dat","rt");
    if(l==7)   fp24=fopen("Quest2_1.dat","rt");
    if(l==8)   fp24=fopen("Quest2_2.dat","rt");
    if(l==9)   fp24=fopen("Quest2_3.dat","rt");
    if(l==10)  fp24=fopen("Quest2_4.dat","rt");
    if(l==11)  fp24=fopen("Quest2_5.dat","rt");
    if(l==12)  fp24=fopen("Quest2_6.dat","rt");
    if(l==13)  fp24=fopen("Quest3_1.dat","rt");
    if(l==14)  fp24=fopen("Quest3_2.dat","rt");
    if(l==15)  fp24=fopen("Quest3_3.dat","rt");
    if(l==16)  fp24=fopen("Quest3_4.dat","rt");
    if(l==17)  fp24=fopen("Quest3_5.dat","rt");
    if(l==18)  fp24=fopen("Quest3_6.dat","rt");
    if(l==19)  fp24=fopen("Quest4_1.dat","rt");
    if(l==20)  fp24=fopen("Quest4_2.dat","rt");
    if(l==21)  fp24=fopen("Quest4_3.dat","rt");
    if(l>0 && l<=user.wh)
    {
      clrscr();
      printf("\n ");
      if(fp24 == NULL) { // fp24가 fopen에 실패하거나 NULL인 경우
        printf("파일이 없거나 경로가 잘못되었습니다. <Enter>\n");
        clearbuff();
        getch();
        continue;
      }
      time=30; // 퀘스트 출력 딜레이 기본값

      // 텍스트를 한글자씩 읽어서 출력
      while(fscanf(fp24,"%c",&data) != EOF) 
      {
       if(kbhit()) time=0; // 빠르게 넘기기
       printf("%c",data);
       delay(time);
      }
      if(time==0) getch();
      getch();
      fclose(fp24); // 파일 닫기 누락 -> 추가
    }
    if(l==1) Q1_1();
    if(l==2) Q1_2();
    if(l==3) Q1_3();
    if(l==4) Q1_4();
    if(l==5) Q1_5();
    if(l==6) Q1_6();
    if(l==7) Q2_1();
    if(l==8) Q2_2();
    if(l==9) Q2_3();
    if(l==10) Q2_4();
    if(l==11) Q2_5();
    if(l==12) Q2_6();
    if(l==13) Q3_1();
    if(l==14) Q3_2();
    if(l==15) Q3_3();
    if(l==16) Q3_4();
    if(l==17) Q3_5();
    if(l==18) Q3_6();
    if(l==19) Q4_1();
    if(l==20) Q4_2();
    if(l==21) Q4_3();
    }
  return;
  }

// =============
//  포션 메뉴
// =============
void Potion() {
  int l;
  printf("\n    M    E    N    U          YOURS ");
  printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  printf("\n  1.Light Healing Potion     [ %3d ]",user.item[0]);
  printf("\n  2.Light Mana Potion        [ %3d ]",user.item[1]);
  printf("\n  3.Super Healing Potion     [ %3d ]",user.item[2]);
  printf("\n  4.Super Mana Potion        [ %3d ]",user.item[3]);
  printf("\n  5.Great Healing Potion     [ %3d ]",user.item[4]);
  printf("\n  6.Great Mana Potion        [ %3d ]",user.item[5]);
  printf("\n  7.Full Healing Potion      [ %3d ]",user.item[6]);
  printf("\n  8.Full Mana Potion         [ %3d ]",user.item[7]);
  printf("\n  9.OUT  		         ");
  printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  printf("\nWhat you eat? :  ");
  scanf("%d",&l);
  if(l==9) set(); // 9번 누르면 전투 메뉴로 복귀
  if(user.item[l-1] != 0)	{
	  switch(l)
	  {
		  case 1: user.nhp+=25; if(user.nhp>user.hp) user.nhp=user.hp;user.item[0]--; break;
  		case 2: user.nmp+=25; if(user.nmp>user.mp) user.nmp=user.mp;user.item[1]--; break;
  		case 3: user.nhp+=50; if(user.nhp>user.hp) user.nhp=user.hp;user.item[2]--; break;
  		case 4: user.nmp+=50; if(user.nmp>user.mp) user.nmp=user.mp;user.item[3]--; break;
  		case 5: user.nhp+=100; if(user.nhp>user.hp) user.nhp=user.hp;user.item[4]--; break;
  		case 6: user.nmp+=100; if(user.nmp>user.mp) user.nmp=user.mp;user.item[5]--; break;
  		case 7: user.nhp=user.hp;user.item[6]--;break;
  		case 8: user.nmp=user.mp;user.item[7]--;break;
	  }
  h_m(); // 몬스터 반격
  } else {
    printf("You don't have that potion!!!"); // 포션이 없으면 알림
    getch();
    set(); // 전투 메뉴로 복귀
  }
 return;
 }

// ===========
//  무기 상점
// ===========
void Weapon_Store() {
  int i,l;
  while(1) {
    clrscr();
    printf("\n");
    printf("             M    E    N    U         POWER  BONUS-HP  BONUS-MP  G O L D\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    for(i=0;i<count1;i++)
  	printf("%2d.%30s     [%4d]  [%4d]  [%4d]   [%4d]\n",i+1,weapon[i].name,weapon[i].power,weapon[i].hp_bonus,weapon[i].mp_bonus,weapon[i].cost);
    printf("%2d.Out\n",count1+1);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("\nWhat do you Need? :    GOLD: %5d  Power: %3d",user.gold,user.attack); // 현재 공격력 출력
    // 선택 입력
    gotoxy(20,6+count1);scanf("%d",&l);
    if(l == count1+1 ) break; // Out 선택
    // 유효 범위 확인
    if(l > 0 && l < count1+1 ) {
    } else {
      clearbuff();
      continue;
    }
    // 돈이 부족한 경우
    if(weapon[l-1].cost > user.gold && l>0 && l<count1+1) {
      printf("\n Need More Money");clearbuff();getch();continue;
    } else {
    user.gold-=weapon[l-1].cost;
    user.hp+=weapon[l-1].hp_bonus;
    user.mp+=weapon[l-1].mp_bonus;
    user.attack+=weapon[l-1].power;
      clrscr();
      printf("\nH    P: %3d + %3d -> %3d",user.hp-weapon[l-1].hp_bonus,weapon[l-1].hp_bonus,user.hp);
      printf("\nM    P: %3d + %3d -> %3d",user.mp-weapon[l-1].mp_bonus,weapon[l-1].mp_bonus,user.mp);
      printf("\nATTACK: %3d + %3d -> %3d",user.attack-weapon[l-1].power,weapon[l-1].power,user.attack);
      clearbuff();
      getch();
    }
  }
 return;
}

// ==============
//  방어구 상점
// ==============
void Defence_Store() {
  int i,l;
  while(1) {
    clrscr();
    printf("\n");
    printf("             M    E    N    U        DEFENCE BONUS-HP BONUS-MP G O L D\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    for(i=0;i<count2;i++)
  	printf("%2d.%30s     [%4d]  [%4d]  [%4d]   [%4d]\n",i+1,defence[i].name,defence[i].defence,defence[i].hp,defence[i].mp,defence[i].cost);
    printf("%2d.Out\n",count2+1);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("\n*디펜스는 20 까지로 제한됩니다.");
    if(user.defence>=20) { // 디펜스 20 초과 시 조정
      printf(" *디펜스가 20이 넘었으므로 20으로 조정됩니다.* \"%d -> 20\"",user.defence);
      user.defence=20;
    }
    printf("\nWhat do you Need? :    GOLD: %5d  Defence: %2d",user.gold,user.defence); // 현재 디펜스 출력
    gotoxy(20,7+count2);scanf("%d",&l);
    if(l == count2+1 ) break; // Out 선택시 탈출
    // 유효 범위 검사
    if(l > 0 && l < count2+1 ) {
    } else {
      clearbuff();
      continue;
    }
    // 돈 부족
    if(defence[l-1].cost > user.gold && l>0 && l<count1+1) {
      printf("\n Need More Money");clearbuff();getch();continue;
    } else {
      user.gold-=defence[l-1].cost;
      user.hp+=defence[l-1].hp;
      user.mp+=defence[l-1].mp;
      user.defence+=defence[l-1].defence;
      clrscr();
      printf("\nH    P : %3d + %3d -> %3d",user.hp-defence[l-1].hp,defence[l-1].hp,user.hp);
      printf("\nM    P : %3d + %3d -> %3d",user.mp-defence[l-1].mp,defence[l-1].mp,user.mp);
      printf("\nDEFENCE: %3d + %3d -> %3d",user.defence-defence[l-1].defence,defence[l-1].defence,user.defence);
      clearbuff();
      getch();
    }
  }
  return;
}
  
// ================
//  아이템 상점
// ================
void Item_store()  {
  int cost[8]={50,50,100,100,500,500,1000,1000};
  int l;
  while(1)  {
    clrscr();
    printf("\n    M    E    N    U       GOLD      YOURS ");
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("\n  1.Light Healing Potion   [ 50 ]    [ %3d ]",user.item[0]);
    printf("\n  2.Light Mana Potion      [ 50 ]    [ %3d ]",user.item[1]);
    printf("\n  3.Super Healing Potion   [ 100]    [ %3d ]",user.item[2]);
    printf("\n  4.Super Mana Potion      [ 100]    [ %3d ]",user.item[3]);
    printf("\n  5.Great Healing Potion   [ 500]    [ %3d ]",user.item[4]);
    printf("\n  6.Great Mana Potion      [ 500]    [ %3d ]",user.item[5]);
    printf("\n  7.Full Healing Potion    [1000]    [ %3d ]",user.item[6]);
    printf("\n  8.Full Mana Potion       [1000]    [ %3d ]",user.item[7]);
    printf("\n  9.OUT  		         ");
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("\nWhat do you Need? :    GOLD: %5d  ",user.gold);
    gotoxy(20,14);scanf("%d",&l); // 메뉴 번호 입력
    if(l == 9 ) break;
    if(l > 0 && l < 10 ) { // 잘못 입력시 다시 입력
    } else {
      clearbuff();
      continue;
    }
    if(user.item[l-1] == 20) { // 포션 최대 개수 제한
      printf("\n %d번 포션을 더 가질 수 없습니다!", l );
      clearbuff();
      getch();
      continue;
    }
    else if(cost[l-1] > user.gold && l>0 && l<9) {
      clrscr();printf("\n Need More Money");getch();continue;
    } else {
      user.gold-=cost[l-1];
      user.item[l-1]++;
    }
  }
return;
}

// ===================================================
//  save_option : 현재 user를 savedata.sav 파일에 저장
// ===================================================
void Save_option() {
  FILE *fp3;
  clrscr();
  // 세이브 무결성 코드 계산
  user.code=(user.lv+user.cs+user.nhp+user.nmp+user.gold+user.exp)/user.lv+user.lv;
  // 저장 파일 쓰기 모드
  fp3=fopen("savedata.sav","w+");
  // 순서대로 저장
  fprintf(fp3,"%s",user.name);
  fprintf(fp3,"\n%d",user.lv);
  fprintf(fp3,"\n%d",user.cs);
  fprintf(fp3,"\n%d",user.hp);
  fprintf(fp3,"\n%d",user.nhp);
  fprintf(fp3,"\n%d",user.mp); 
  fprintf(fp3,"\n%d",user.nmp);
  fprintf(fp3,"\n%d",user.exp);
  fprintf(fp3,"\n%d",user.attack);
  fprintf(fp3,"\n%d",user.gold);
  fprintf(fp3,"\n%d",user.defence);
  fprintf(fp3,"\n%d",user.wh);
  fprintf(fp3,"\n%d",user.code);
  fprintf(fp3,"\n%d",user.item[0]);
  fprintf(fp3,"\n%d",user.item[1]);
  fprintf(fp3,"\n%d",user.item[2]);
  fprintf(fp3,"\n%d",user.item[3]);
  fprintf(fp3,"\n%d",user.item[4]);
  fprintf(fp3,"\n%d",user.item[5]);
  fprintf(fp3,"\n%d",user.item[6]);
  fprintf(fp3,"\n%d",user.item[7]);
  fclose(fp3);  
  printf("\n 세이브가 완료되었습니다<Enter>");
  getch();
  return;
}

// ==============================
//  condition : 현재 캐릭터 정보
// ==============================
void Condition() {
  int i;
  clrscr();
  printf("\n\n");
  printf("          N   a  m   e: %s\n",user.name);
  printf("          L e  v  e  l: %d\n",user.lv);
  printf("          H          P: %d/%d\n",user.nhp,user.hp);
  printf("          M          P: %d/%d\n",user.nmp,user.mp);
  printf("          Attack Point: %d ~ %d\n",user.attack-2,user.attack+1); // 공격력 범위(-2 ~ +1)
  printf("          DefencePoint: %d\n",user.defence);
  printf("          Need     Exp: %d \n",user.exp);
  printf("          G   O  L   D: %d \n",user.gold);
  printf("     Can Private Magic: \n ");
  printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━");
  for(i=0;i<8;i++)  {
  	if(magic[i].lv <= user.lv)  {
      printf("\n NAME: %16s ┃  Damage: %3d ┃ Mp: %4d ┃  Level: %3d",magic[i].name,magic[i].power,magic[i].ump,magic[i].lv);
    }
  }
  printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━");
  printf("\n\n\n  < E N T E R >");
  getch();
  return;
}

// ==============================================
//  insert_magic : 직업 따라 맞는 데이터 파일 열기
// ==============================================
void Insert_magic()  {
	FILE *fp2;
	int i;
  // 직업에 따른 다른 파일 선택
	if(user.cs==1) fp2=fopen("Amamagic.dat","rt");
	if(user.cs==2) fp2=fopen("Socmagic.dat","rt");
	if(user.cs==3) fp2=fopen("Necmagic.dat","rt");
	for(i=0;i<8;i++)
  // 마법 정보 읽어오기
	fscanf(fp2,"%s %d %d %d",magic[i].name,&magic[i].power,&magic[i].ump,&magic[i].lv);
  fclose(fp2);
  return;
}

// =======================================
//  load : 데이터 읽어와 user에 채움
// =======================================
void Load() {
  char name[30];
  int code,lv,cs,nmp,defence,gold,mp,nhp,hp,exp,attack,wh;
  FILE *fp1;
  clrscr();
  fp1=fopen("savedata.sav","rt");
  // 저장 시 순서대로 읽어오기
  fscanf(fp1,"%s",name);
  fscanf(fp1,"%d",&lv);
  fscanf(fp1,"%d",&cs);
  fscanf(fp1,"%d",&hp); 
  fscanf(fp1,"%d",&nhp);
  fscanf(fp1,"%d",&mp);
  fscanf(fp1,"%d",&nmp);
  fscanf(fp1,"%d",&exp);
  fscanf(fp1,"%d",&attack);
  fscanf(fp1,"%d",&gold);
  fscanf(fp1,"%d",&defence);
  fscanf(fp1,"%d",&wh);
  fscanf(fp1,"%d",&code);
  fscanf(fp1,"%d",&user.item[0]);
  fscanf(fp1,"%d",&user.item[1]);
  fscanf(fp1,"%d",&user.item[2]);
  fscanf(fp1,"%d",&user.item[3]);
  fscanf(fp1,"%d",&user.item[4]);
  fscanf(fp1,"%d",&user.item[5]);
  fscanf(fp1,"%d",&user.item[6]);
  fscanf(fp1,"%d",&user.item[7]);

  // 읽어온 값 user에 복사
  strcpy(user.name,name);
  user.gold=gold;
  user.lv=lv;
  user.cs=cs;
  user.hp=hp;
  user.nhp=nhp;
  user.mp=mp;
  user.nmp=nmp;
  user.exp=exp;
  user.attack=attack;
  user.gold=gold;
  user.defence=defence;
  user.wh=wh;
  fclose(fp1);
  // 세이브 검사
  if(code != (user.lv+user.cs+user.nhp+user.nmp+user.gold+user.exp)/user.lv+user.lv) {
    printf("이런 치사한넘 .. 이런 게임을 에디트 하려 하다니... !!");
    getch();
    exit(0);
  }
  return;
}

// ========================================
//  opening : 오프닝, 직업 선택, 스탯 설정
// ========================================
void Opening() {
  FILE *fp0=fopen("Opening.qwe","rt");
  int ch,i;
  char data;
  randomize();
  clrscr();
  while(fscanf(fp0,"%c",&data) != EOF)  {
  	printf("%c",data);
  	delay(30);
  }
  getch();
  printf("Press Enter to Continue...");
  getch();
  while(1){
    clrscr();
	  printf("\n\n\n\n\n");
	  printf("\n           ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	  printf("\n           ┃                                          ┃");
	  printf("\n           ┃                                          ┃");
	  printf("\n           ┃            Choose Your Character         ┃");
	  printf("\n           ┃                                          ┃");
	  printf("\n           ┃                 1. Amazon                ┃");
	  printf("\n           ┃                 2. Sorceress             ┃");
	  printf("\n           ┃                 3. Necromancer           ┃");
	  printf("\n           ┃   Press Input number:                    ┃");
	  printf("\n           ┃                                          ┃");
	  printf("\n           ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	  user.gold=0;
    gotoxy(37,15);scanf("%d",&ch);

    // 1. Amazon
    if(ch==1)	{
  	  strcpy(user.name,"Amazon");
  	  user.cs=1;
  	  user.lv=1;
  	  user.nhp=user.hp=my_random(28)+25;
  	  user.nmp=user.mp=my_random(15)+5;
  	  user.attack=my_random(8)+10;
  	  user.exp=my_random(8*user.lv*user.lv)+100;
  	  user.defence=0;
  	  user.wh=1;
      break;
	  }
    // 2. Sourceress
	  if(ch==2)	{
  	  strcpy(user.name,"Sorceress");
  	  user.cs=2;
  	  user.lv=1;
  	  user.nhp=user.hp=my_random(20)+10;
  	  user.nmp=user.mp=my_random(20)+30;
  	  user.attack=my_random(8)+5;
  	  user.exp=my_random(8*user.lv*user.lv)+100;
  	  user.defence=0;
  	  user.wh=1;
      break;
	  }
    // 3. Neromancer
	  if(ch==3)	{
  	  strcpy(user.name,"Necromancer");
  	  user.cs=3;
  	  user.lv=1;
  	  user.nhp=user.hp=my_random(20)+15;
  	  user.nmp=user.mp=my_random(20)+25;
  	  user.attack=my_random(8)+7;
  	  user.exp=my_random(8*user.lv*user.lv)+100;
  	  user.defence=0;
  	  user.wh=1;
      break;
	  }
    else {
      // 1~3 이외는 다시 선택
      clearbuff();
      continue;
    }
  }
  // 시작 스탯
  user.gold=500;
	for(i=0;i<8;i++)
	  user.item[i]=0;
  fclose(fp0);
  return;
  }
	
// ==================
//  set : 전투 루프
// ==================
void set() {

  int input;
  randomize(); // 매 턴 마다 랜덤
  while(1)  {
    clrscr();
    // 몬스터 죽었는지 확인
    if(monster.nhp<=0) {
	    printf("\n 캬오~~凸 우케케케케케 꾸르르르르 끄억");

	    printf("\n 당신은 %d 만큼의 골드와 %d 만큼의 경험치를 얻습니다",monster.gold,monster.exp);
    	getch();
    	user.exp-=monster.exp;
    	user.gold+=monster.gold;
    	l_m--; // 남은 몬스터 수 감소

      // 레벨업 : exp <= 0
      if(user.exp<=0) {
    		clrscr();
    		printf("\n Level Up!");

        // 직업별 레벨업 보너스
  		  switch(user.cs) 
        {
    		  case 1: user.attack+=my_random(1)+1;user.nmp=user.mp+=my_random(4);user.nhp=user.hp+=my_random(5);user.exp=my_random(80*user.lv)+100;break;
    		  case 2: user.attack+=my_random(1);user.nmp=user.mp+=my_random(9);user.nhp=user.hp+=my_random(3);user.exp=my_random(80*user.lv)+100; break;
    		  case 3: user.attack+=my_random(2);user.nmp=user.mp+=my_random(7);user.nhp=user.hp+=my_random(8);user.exp=my_random(80*user.lv)+100;break;		 
        }
  	    user.lv++; // 레벨 증가
      	getch();
    	}

    	break; // 전투 종료
    }

    // 플레이어가 죽었는지 확인
    if(user.nhp<=0) {
	    printf("\n 으억! 이렇게 당하다니 이세계의 미래는.....");
    	getch();
    	clrscr();
    	printf("\n Game Over");
    	getch();
    	exit(1);
  	}

    // 전투 UI
    gotoxy(1,2);printf("                               ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(1,3);printf("   1.Melee Attack              ┃   Name: %12s       %10s    ┃",user.name,monster.name);
    gotoxy(1,4);printf("   2.Magic                     ┃   Lev :   %3d              %3d           ┃",user.lv,monster.lv);
    gotoxy(1,5);printf("   3.Eat potion                ┃   H  P: %4d/%4d        %4d/%4d       ┃",user.nhp,user.hp,monster.nhp,monster.hp);
    gotoxy(1,6);printf("   4.Run Away                  ┃   M  P: %4d/%4d        %4d/%4d       ┃",user.nmp,user.mp,monster.nmp,monster.mp);
    gotoxy(1,7);printf("                               ┃   Att : %4d~%4d        %4d~%4d       ┃",user.attack-2,user.attack+2,monster.attack-2,monster.attack+2);
    gotoxy(1,8);printf("                               ┃   Def :  %4d             %4d           ┃",user.defence,monster.defence);
    gotoxy(1,9);printf("                               ┃   EXP :  %5d           %5d           ┃",user.exp,monster.exp);
    gotoxy(1,10);printf("                               ┃   Gold:  %6d          %5d           ┃",user.gold,monster.gold);
    gotoxy(1,11);printf("                               ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ");
    gotoxy(30,12);printf("Leave Monster: %d",l_m);
    gotoxy(1,12);printf("Battle Order(1~4): ");scanf("%d",&input);
    // 입력 검사
    if(input > 0 && input < 5) {}
    else {
      clearbuff();
      continue;
    }
    // 입력에 따른 행동
    switch(input) 
    { 
      case 1: M_A(); break; // 일반 공격
      case 2: Mg(); break; // 마법 공격
      case 3: Potion(); break; // 포션 먹기
      case 4: printf("\n도망가기 성공! 캠프로 복귀합니다.");
              fflush(stdout);

              clearbuff(); // 버퍼에 남은 '\n' 제거
              getch();
              escape = 1; // 도망 성공 표시
              return; // 전투 함수 종료
    }
  }
  return;
}

// ===========================================
//  퀘스트 Q1_1 ~ Q4_3 : 패턴 비슷, 수치만 다름
//  -> 구체적인 주석 : Q2_4
// ===========================================
void  Q1_1()  {
	int i;
	if(user.lv>=16) {
    printf("\n당신은 레벨이 높아 더이상 출입 불가능합니다.<Enter>");getch();return;
  }
	clrscr();
	monster.lv=1;
	l_m=30;
	for(i=0;i<10;i++) {
		strcpy(monster.name,"Skel_Hasu");
    monster.hp=my_random(7)+10;
    monster.nhp=monster.hp;
		monster.attack=my_random(4)+3;
    monster.mp=my_random(7)+1;
		monster.nmp=monster.mp;
		monster.exp=my_random(4)+1;
		monster.gold=my_random(6)+1;
		monster.defence=0;
		set();
    if(escape) return;
	}

	for(i=0;i<10;i++) {
		strcpy(monster.name,"Skel_Mid");
    monster.hp=my_random(14)+7;
    monster.nhp=monster.hp;
		monster.mp=my_random(10)+5;
    monster.nmp=monster.mp;
		monster.attack=my_random(5)+2;
		monster.gold=my_random(5)+3;
		monster.defence=2;
		monster.exp=my_random(5)+1;
		set();
    if(escape) return;
	}

	for(i=0;i<9;i++) {
		strcpy(monster.name,"Skel_Gosu");
    monster.hp=my_random(20)+9;
    monster.nhp=monster.hp;
		monster.mp=my_random(12)+7;
    monster.nmp=monster.mp;
		monster.attack=my_random(1)+3;
		monster.gold=my_random(3)+1;
		monster.defence=3;
		monster.exp=my_random(7)+2;
		set();
    if(escape) return;
  }

	clrscr();
	printf("\n 뽀너스: Light Healing Potion +1");
	user.item[0]+=1;
	getch();
	monster.lv=3;
	strcpy(monster.name,"Skel_Boss");
  monster.hp=my_random(5)+60;
  monster.nhp=monster.hp;
	monster.mp=my_random(20)+10;
  monster.nmp=monster.mp;
	monster.attack=my_random(4)+5;
	monster.gold=my_random(7)+1;
	monster.defence=1;
	monster.exp=my_random(35)+7;
	set();
  if(escape) return;

 if(user.wh==1) user.wh++;
 return;
}

void Q1_2() {
  l_m=2;
  monster.lv=3;
  monster.hp=my_random(20)+50;
  monster.nhp=monster.hp;
	monster.mp=my_random(20)+30;
  monster.nmp=monster.mp;
  strcpy(monster.name,"Iron_Fish");
  monster.attack=my_random(5)+5;
  monster.gold=my_random(7)+50;
  monster.defence=2;
  monster.exp=my_random(7)+22;
  set();
  if(escape) return;

  monster.lv=8;
  monster.hp=my_random(40)+100;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+50;
  monster.nmp=monster.mp;
  strcpy(monster.name,"Gold_Fish");
  monster.attack=my_random(6)+5;
  monster.gold=my_random(7)+45;
  monster.defence=6;
  monster.exp=my_random(8)+30;
  set();
  if(escape) return;

  if(user.wh==2) user.wh++;
  return;
}

void Q1_3() {

  l_m=1;
  monster.lv=10;
  strcpy(monster.name,"Saladin");
  monster.attack=my_random(7)+4;
  monster.hp=my_random(70)+200;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+60;
  monster.nmp=monster.mp;
  monster.gold=100;
  monster.defence=3;
  monster.exp=my_random(37)+45;
  set();
  if(escape) return;

  if(user.wh==3) user.wh++;
  return;
}

void Q1_4() {
  int i;
  monster.lv=12;
  l_m=2;
  for(i=0;i<2;i++) {
    strcpy(monster.name,"케인똘마니");
    monster.attack=my_random(6)+10;
    monster.hp=my_random(50)+150;
    monster.nhp=monster.hp;
	  monster.mp=my_random(20)+40;
    monster.nmp=monster.mp;
    monster.gold=50;
    monster.defence=0;
    monster.exp=my_random(6)+14;
    set();
    if(escape) return;
  }
  strcpy(monster.name,"데까드케인");
  monster.attack=my_random(6)+4;
  monster.hp=my_random(60)+200;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+100;
  monster.nmp=monster.mp;
  monster.gold=60;
  monster.defence=5;
  monster.exp=my_random(70)+37;
  set();
  if(escape) return;

  if(user.wh==4) user.wh++;
  return;
}

void Q1_5() {
  int i;
  l_m=10;
  monster.lv=11;
  for(i=0;i<3;i++) {
    strcpy(monster.name,"각목사나이");
    monster.attack=my_random(6)+2;
    monster.hp=my_random(60)+100;
    monster.nhp=monster.hp;
	  monster.mp=my_random(30)+40;
    monster.nmp=monster.mp;
    monster.gold=10;
    monster.defence=1;
    monster.exp=my_random(7)+10;
    set();
    if(escape) return;
  }
  for(i=0;i<4;i++)  {
    strcpy(monster.name,"사시미군단");
    monster.attack=my_random(6)+15;
    monster.hp=my_random(20)+20;
    monster.nhp=monster.hp;
	  monster.mp=my_random(20)+30;
    monster.nmp=monster.mp;
    monster.gold=30;
    monster.defence=2;
    monster.exp=my_random(7)+10;
    set();
    if(escape) return;
  }

  for(i=0;i<3;i++)  {
    strcpy(monster.name,"장관급부하");
    monster.attack=my_random(6)+11;
    monster.hp=my_random(30)+70;
    monster.nhp=monster.hp;
	  monster.mp=my_random(30)+40;
    monster.nmp=monster.mp;
    monster.gold=10;
    monster.defence=4;
    monster.exp=my_random(7)+20;
    set();
    if(escape) return;
  }
  if(user.wh==5) user.wh++;
  return;
}

void Q1_6() {
  if(user.wh>6) {
    printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");getch();
    return;
  }
  clrscr();
  printf("\nAnddaerial: 니가 내 아그들 건드렸냐?");
  delay(1200);
  printf("\n%s: 그래 어쩔래? 니가 보태준거 있냐?",user.name);
  delay(1200);
  printf("\nAnddaerial: 없다. 죽고 싶냐 말투가 그게 머냐?");
  delay(1200);
  printf("\n%s: 후훗.. 간이 배밖으로 나왔나 보군 없애주마!!",user.name);
  delay(1200);
  printf("\nAnddaerial: 레벨도 %d 밖에 안되는 주제에 내 레벨은 %d 다!!",user.lv,monster.lv=user.lv+10);
  delay(1200);
  printf("\n%s: 헉... 그래도 승부다!! -.-;; \n <Enter> ",user.name);
  delay(1200);
  getch();

  l_m=1;
  strcpy(monster.name,"안때리얼");
  monster.attack=my_random(6)+14;
  monster.hp=my_random(300)+500;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+120;
  monster.nmp=monster.mp;
  monster.gold=10;
  monster.defence=3;
  monster.exp=my_random(400)+200;
  set();
  if(escape) return;
  if(user.wh==6) user.wh++;
  return;
}

void Q2_1() {
  monster.lv=12;
  l_m=5;
  strcpy(monster.name,"Turtle_1");
  monster.attack=my_random(20)+6;
  monster.hp=my_random(40)+60;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+30;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+50;
  monster.defence=8;
  monster.exp=my_random(30)+6;
  set();
  if(escape) return;
  strcpy(monster.name,"Turtle_2");
  monster.attack=my_random(20)+6;
  monster.hp=my_random(40)+70;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+40;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+50;
  monster.defence=6;
  monster.exp=my_random(30)+6;
  set();
  if(escape) return;
  strcpy(monster.name,"Turtle_3");
  monster.attack=my_random(20)+6;
  monster.hp=my_random(40)+75;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+40;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+50;
  monster.defence=7;
  monster.exp=my_random(30)+6;
  set();
  if(escape) return;
  strcpy(monster.name,"Turtle_4");
  monster.attack=my_random(20)+6;
  monster.hp=my_random(40)+80;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+50;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+50;
  monster.defence=4;
  monster.exp=my_random(30)+6;
  set();
  if(escape) return;
  strcpy(monster.name,"Turtle_5");
  monster.attack=my_random(30)+6;
  monster.hp=my_random(40)+85;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+50;
  monster.nmp=monster.mp;
  monster.gold=my_random(30)+10;
  monster.defence=5;
  monster.exp=my_random(50)+20;
  set();
  if(escape) return;
  strcpy(monster.name,"동상 단군");
  monster.attack=my_random(10)+16;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+80;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+150;
  monster.defence=9;
  monster.exp=my_random(40)+40;
  set();
  if(escape) return;

  if(user.wh==7) user.wh++;
  return;
}

void Q2_2() {
  l_m=4;
  strcpy(monster.name,"GoldHamster");
  monster.attack=my_random(20)+8;
  monster.hp=my_random(40)+90;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+60;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=7;
  monster.exp=my_random(40)+8;
  set();
  if(escape) return;
  strcpy(monster.name,"Jangarian");
  monster.attack=my_random(20)+10;
  monster.hp=my_random(40)+100;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+60;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=4;
  monster.exp=my_random(30)+5;
  set();
  if(escape) return;
  strcpy(monster.name,"기니피그");
  monster.attack=my_random(20)+6;
  monster.hp=my_random(40)+100;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+60;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+50;
  monster.defence=6;
  monster.exp=my_random(30)+10;
  set();
  if(escape) return;
  strcpy(monster.name,"~시궁쥐~");
  monster.attack=my_random(46)+16;
  monster.hp=my_random(80)+200;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+100;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=3;
  monster.exp=my_random(40)+40;
  set();
  if(escape) return;

  if(user.wh==8) user.wh++;
  return;
}

void Q2_3() {
  l_m=4;
  strcpy(monster.name,"Boradolei");
  monster.attack=my_random(36)+16;
  monster.hp=my_random(40)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+70;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=8;
  monster.exp=my_random(40)+30;
  set();
  if(escape) return;
  strcpy(monster.name,"Ddubi");
  monster.attack=my_random(36)+16;
  monster.hp=my_random(40)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+70;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=4;
  monster.exp=my_random(40)+30;
  set();
  if(escape) return;
  strcpy(monster.name,"NaNa");
  monster.attack=my_random(36)+16;
  monster.hp=my_random(40)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+70;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=4;
  monster.exp=my_random(40)+30;
  set();
  if(escape) return;
  strcpy(monster.name,"Bo");
  monster.attack=my_random(36)+16;
  monster.hp=my_random(40)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(30)+70;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+10;
  monster.defence=3;
  monster.exp=my_random(40)+30; 
  set();
  if(escape) return;
  strcpy(monster.name,"BabyOfSun");
  monster.attack=my_random(10)+8;
  monster.hp=my_random(50)+200;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+100;
  monster.nmp=monster.mp;
  monster.gold=my_random(500)+100;
  monster.defence=6;
  monster.exp=my_random(40)+130;
  set();
  if(escape) return;

  if(user.wh==9) user.wh++;
  return;
}

void Q2_4() {
  l_m=4; // 몬스터 수
  monster.lv=15; // 몬스터 레벨

  strcpy(monster.name,"보노보노"); // 몬스터 이름
  monster.attack=my_random(40)+15; // 몬스터 공격력
  monster.hp=my_random(50)+180; // 몬스터 hp
  monster.nhp=monster.hp; // 현재 hp를 최대 hp로 초기화
	monster.mp=my_random(40)+110; // 몬스터 mp
  monster.nmp=monster.mp; // 현재 mp를 최대 mp로 초기화
  monster.gold=my_random(50)+50; // 처치 시 gold
  monster.defence=6; // 방어력
  monster.exp=my_random(100)+160; // 처치시 exp
  set(); // 전투 시작
  if(escape) return; // 도망 시 퀘스트 함수 종료

  strcpy(monster.name,"가리비");
  monster.attack=my_random(45)+12;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+110;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+20;
  monster.defence=5;
  monster.exp=my_random(100)+160;
  set();
  if(escape) return;

  strcpy(monster.name,"홍합");
  monster.attack=my_random(45)+12;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+110;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+20;
  monster.defence=5;
  monster.exp=my_random(100)+160;
  set();
  if(escape) return;

  strcpy(monster.name,"키조개");
  monster.attack=my_random(45)+12;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+110;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+20;
  monster.defence=5;
  monster.exp=my_random(100)+160;
  set();
  if(escape) return;

  if(user.wh==10) user.wh++;
  return;
}
void Q2_5() {
  l_m=6;
  monster.lv=17;

  strcpy(monster.name,"1학년 담임");
  monster.attack=my_random(10)+13;
  monster.hp=my_random(50)+170;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+130;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+30;
  monster.defence=5;
  monster.exp=my_random(150)+240;
  set();
  if(escape) return;

  strcpy(monster.name,"2학년 담임");
  monster.attack=my_random(15)+14;
  monster.hp=my_random(50)+170;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+130;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+30;
  monster.defence=5;
  monster.exp=my_random(150)+240;
  set();
  if(escape) return;

  strcpy(monster.name,"3학년 담임");
  monster.attack=my_random(20)+15;
  monster.hp=my_random(50)+170;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+130;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+30;
  monster.defence=5;
  monster.exp=my_random(150)+240;
  set();
  if(escape) return;

  strcpy(monster.name,"4학년 담임");
  monster.attack=my_random(30)+16;
  monster.hp=my_random(50)+180;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+130;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+40;
  monster.defence=5;
  monster.exp=my_random(150)+240;
  set();
  if(escape) return;

  strcpy(monster.name,"5학년 담임");
  monster.attack=my_random(35)+17;
  monster.hp=my_random(50)+180;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+130;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+40;
  monster.defence=5;
  monster.exp=my_random(150)+240;
  set();
  if(escape) return;

  strcpy(monster.name,"6학년 담임");
  monster.attack=my_random(40)+18;
  monster.hp=my_random(50)+180;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+130;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+40;
  monster.defence=5;
  monster.exp=my_random(150)+240;
  set();
  if(escape) return;

  if(user.wh==11) user.wh++;
  return;
}
void Q2_6() {
  if(user.wh>12) {
    printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");getch();
    return;
  }

  clrscr();
  printf("\n듀리엘: 얼어라! 부셔져라!");
  delay(1200);
  printf("\n%s: 창원에 눈...?",user.name);
  delay(1200);
  printf("\n듀리엘: 사계절 다 눈이 내리도록 해주지!");
  delay(1200);
  printf("\n%s: 야호! \n <Enter> ",user.name);
  delay(1200);
  getch();

  l_m=1;
  monster.lv=20;
  strcpy(monster.name,"듀리엘");
  monster.attack=my_random(40)+20;
  monster.hp=my_random(400)+700;
  monster.nhp=monster.hp;
	monster.mp=my_random(60)+220;
  monster.nmp=monster.mp;
  monster.gold=3500;
  monster.defence=10;
  monster.exp=my_random(600)+1400;
  set();
  if(escape) return;

  if(user.wh==12) user.wh++;
  return;
}
void Q3_1() {
  int i;
  l_m=5;
  monster.lv=21;

  strcpy(monster.name,"DJ증오");
  monster.attack=my_random(35)+15;
  monster.hp=my_random(55)+180;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+160;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=9;
  monster.exp=my_random(200)+300;
  set();
  if(escape) return;

  for(i=0;i<2;i++){
    strcpy(monster.name,"방송요정");
    monster.attack=my_random(25)+15;
    monster.hp=my_random(50)+170;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+100;
    monster.nmp=monster.mp;
    monster.gold=my_random(60)+60;
    monster.defence=6;
    monster.exp=my_random(200)+300;
    set();
    if(escape) return;
  }

  for(i=0;i<2;i++){
    strcpy(monster.name,"증오의 주파수");
    monster.attack=my_random(25)+15;
    monster.hp=my_random(50)+170;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+100;
    monster.nmp=monster.mp;
    monster.gold=my_random(60)+60;
    monster.defence=6;
    monster.exp=my_random(200)+300;
    set();
    if(escape) return;
  }

  if(user.wh==13) user.wh++;
  return;
}
void Q3_2() {
  int i;
  l_m=6;
  monster.lv=22;

  strcpy(monster.name,"마녀 징징이");
  monster.attack=my_random(40)+20;
  monster.hp=my_random(55)+200;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+120;
  monster.nmp=monster.mp;
  monster.gold=my_random(50)+60;
  monster.defence=6;
  monster.exp=my_random(200)+350;
  set();
  if(escape) return;

  for(i=0;i<5;i++){
    strcpy(monster.name,"라면 슬라임");
    monster.attack=my_random(30)+15;
    monster.hp=my_random(80)+80;
    monster.nhp=monster.hp;
	  monster.mp=my_random(50)+120;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=4;
    monster.exp=my_random(200)+350;
    set();
    if(escape) return;
  }

  if(user.wh==14) user.wh++;
  return;
}
void Q3_3() {
  int i;
  l_m=10;
  monster.lv=23;

  for(i=0;i<2;i++){
    strcpy(monster.name,"잉어킹");
    monster.attack=my_random(40)+20;
    monster.hp=my_random(60)+140;
    monster.nhp=monster.hp;
	  monster.mp=my_random(50)+140;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=8;
    monster.exp=my_random(200)+400;
    set();
    if(escape) return;
  }

  for(i=0;i<2;i++){
    strcpy(monster.name,"지옥어");
    monster.attack=my_random(40)+20;
    monster.hp=my_random(60)+140;
    monster.nhp=monster.hp;
	  monster.mp=my_random(50)+140;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=6;
    monster.exp=my_random(200)+400;
    set();
    if(escape) return;
  }

  for(i=0;i<2;i++){
    strcpy(monster.name,"붕어독");
    monster.attack=my_random(40)+20;
    monster.hp=my_random(60)+140;
    monster.nhp=monster.hp;
	  monster.mp=my_random(50)+140;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=6;
    monster.exp=my_random(200)+400;
    set();
    if(escape) return;
  }

  for(i=0;i<2;i++){
    strcpy(monster.name,"고등등");
    monster.attack=my_random(40)+20;
    monster.hp=my_random(60)+140;
    monster.nhp=monster.hp;
	  monster.mp=my_random(50)+140;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=6;
    monster.exp=my_random(200)+400;
    set();
    if(escape) return;
  }

  for(i=0;i<2;i++){
    strcpy(monster.name,"치치독");
    monster.attack=my_random(40)+20;
    monster.hp=my_random(60)+140;
    monster.nhp=monster.hp;
	  monster.mp=my_random(50)+140;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=6;
    monster.exp=my_random(200)+400;
    set();
    if(escape) return;
  }

  if(user.wh==15) user.wh++;
  return;
}
void Q3_4() {
  l_m=8;
  monster.lv=22;

  strcpy(monster.name,"뽀로로");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"크롱");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"루피");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"에디");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"포비");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"해리");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"패티");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  strcpy(monster.name,"로디");
  monster.attack=my_random(50)+25;
  monster.hp=my_random(50)+150;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+200;
  monster.nmp=monster.mp;
  monster.gold=my_random(60)+60;
  monster.defence=7;
  monster.exp=my_random(200)+500;
  set();
  if(escape) return;

  if(user.wh==16) user.wh++;
  return;
}
void Q3_5() {
  int i;
  l_m=10;
  monster.lv=24;
  for(i=0;i<3;i++) {
    strcpy(monster.name,"타락 캐셔");
    monster.attack=my_random(40)+30;
    monster.hp=my_random(70)+180;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+210;
    monster.nmp=monster.mp;
    monster.gold=my_random(200)+70;
    monster.defence=8;
    monster.exp=my_random(200)+600;
    set();
    if(escape) return;
  }
  for(i=0;i<5;i++)  {
    strcpy(monster.name,"광분 소비자");
    monster.attack=my_random(20)+25;
    monster.hp=my_random(50)+180;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+210;
    monster.nmp=monster.mp;
    monster.gold=my_random(50)+60;
    monster.defence=6;
    monster.exp=my_random(200)+600;
    set();
    if(escape) return;
  }

  for(i=0;i<2;i++)  {
    strcpy(monster.name,"계산 로봇");
    monster.attack=my_random(50)+28;
    monster.hp=my_random(100)+100;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+210;
    monster.nmp=monster.mp;
    monster.gold=my_random(100)+100;
    monster.defence=9;
    monster.exp=my_random(200)+600;
    set();
    if(escape) return;
  }
  if(user.wh==17) user.wh++;
  return;
}
void Q3_6() {
  if(user.wh>18) {
    printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");getch();
    return;
  }

  clrscr();
  printf("\n메피스토: 지금부터 서로 죽여라!!! 99%% 세일!!!");
  delay(1200);
  printf("\n%s: 매장이 난장판이 됐군..",user.name);
  delay(1200);
  printf("\n메피스토: 너는 왜 멀쩡한거지? 이거 낼 돈도 없는게냐?");
  delay(1200);
  printf("\n%s: 너랑 대화할 여유따윈 없다. \n <Enter> ",user.name);
  delay(1200);
  getch();

  l_m=1;
  strcpy(monster.name,"메피스토");
  monster.attack=my_random(60)+30;
  monster.hp=my_random(450)+750;
  monster.nhp=monster.hp;
	monster.mp=my_random(50)+350;
  monster.nmp=monster.mp;
  monster.gold=6000;
  monster.defence=11;
  monster.exp=my_random(400)+2500;
  set();
  if(escape) return;

  if(user.wh==18) user.wh++;
  return;
}
void Q4_1() {
  int i;
  l_m=7;
  monster.lv=28;
  for(i=0;i<2;i++) {
    strcpy(monster.name,"하수 좀비");
    monster.attack=my_random(30)+10;
    monster.hp=my_random(50)+200;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+250;
    monster.nmp=monster.mp;
    monster.gold=my_random(300)+500;
    monster.defence=9;
    monster.exp=my_random(300)+800;
    set();
    if(escape) return;
  }
  for(i=0;i<2;i++)  {
    strcpy(monster.name,"오염 슬라임");
    monster.attack=my_random(20)+15;
    monster.hp=my_random(50)+200;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+250;
    monster.nmp=monster.mp;
    monster.gold=my_random(300)+500;
    monster.defence=9;
    monster.exp=my_random(300)+800;
    set();
    if(escape) return;
  }

  for(i=0;i<3;i++)  {
    strcpy(monster.name,"하수 감시자");
    monster.attack=my_random(20)+20;
    monster.hp=my_random(50)+180;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+250;
    monster.nmp=monster.mp;
    monster.gold=my_random(400)+500;
    monster.defence=10;
    monster.exp=my_random(350)+800;
    set();
    if(escape) return;
  }
  if(user.wh==19) user.wh++;
  return;
}
void Q4_2() {
  int i;
  l_m=9;
  monster.lv=34;
  for(i=0;i<2;i++) {
    strcpy(monster.name,"중수 좀비");
    monster.attack=my_random(30)+15;
    monster.hp=my_random(50)+220;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+300;
    monster.nmp=monster.mp;
    monster.gold=my_random(400)+700;
    monster.defence=10;
    monster.exp=my_random(300)+900;
    set();
    if(escape) return;
  }
  for(i=0;i<3;i++)  {
    strcpy(monster.name,"오염 슬라임");
    monster.attack=my_random(20)+15;
    monster.hp=my_random(50)+220;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+300;
    monster.nmp=monster.mp;
    monster.gold=my_random(400)+700;
    monster.defence=10;
    monster.exp=my_random(300)+900;
    set();
    if(escape) return;
  }

  for(i=0;i<4;i++)  {
    strcpy(monster.name,"중수 감시자");
    monster.attack=my_random(40)+20;
    monster.hp=my_random(100)+180;
    monster.nhp=monster.hp;
	  monster.mp=my_random(40)+300;
    monster.nmp=monster.mp;
    monster.gold=my_random(400)+700;
    monster.defence=11;
    monster.exp=my_random(350)+900;
    set();
    if(escape) return;
  }
  if(user.wh==20) user.wh++;
  return;
}
void Q4_3() {
  if(user.wh>21) {
    printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");getch();
    return;
  }

  clrscr();
  printf("\n디아블로: 기어코 여기까지 왔구나.....");
  delay(1200);
  printf("\n%s: 빨리 끝내고 너구리 게임이나 해야지~ 덤벼라!",user.name);
  delay(1200);
  printf("\n디아블로: 나와 싸우려면 내 부하부터 쓰려뜨려야 할거다.. 본때를 보여줘라!!");
  delay(1200);
  printf("\n%s: 귀찮게 됐군... \n <Enter> ",user.name);
  delay(1200);
  getch();

  l_m=3;
  int i;
  monster.lv=40;
  for(i=0;i<2;i++) {
    strcpy(monster.name,"고수 감시자");
    monster.attack=my_random(40)+15;
    monster.hp=my_random(50)+250;
    monster.nhp=monster.hp;
	  monster.mp=my_random(150)+320;
    monster.nmp=monster.mp;
    monster.gold=my_random(500)+1400;
    monster.defence=13;
    monster.exp=my_random(200)+1200;
    set();
    if(escape) return;
  }
  strcpy(monster.name,"디아블로");
  monster.attack=my_random(60)+30;
  monster.hp=my_random(500)+880;
  monster.nhp=monster.hp;
	monster.mp=my_random(40)+320;
  monster.nmp=monster.mp;
  monster.gold=my_random(2000)+1400;
  monster.defence=14;
  monster.exp=my_random(1000)+2000;
  set();
  if(escape) return;

  clrscr();
  printf("\n디아블로를 쓰러뜨렸다!");
  delay(1200);
  printf("\n이대로 세상이 평화로워 질 수 있을까..?(게임 클리어) \n <Enter> ");
  delay(1200);
  getch();
  if(user.wh==21) user.wh++;
  return;
}

// ======================
//  M_A : 플레이어 공격
// ======================
void M_A()  { 
  int cr,ud;
  randomize();
  // 실제 데미지 = 랜덤 + 공격력 - 몬스터 방어력
  ud=my_random(4)-2+user.attack-monster.defence;
  if(ud<0) ud=0;

  cr=my_random(40); // 크리티컬
  if(cr==3) {
  gotoxy(10,14);printf("풀 파워~! 크리티컬 어택 %d만큼의 데미지!",ud*3);
  monster.nhp=monster.nhp-(ud*3);
  clearbuff();
  } else {
	  if(ud==0) {
      gotoxy(10,14);printf(" ~~ ㅋㅋ.."); // 0 데미지
    } else {
	    gotoxy(10,14);printf("당신은 %s에게 물리적인 힘으로 %d만큼의 충격을 가합니다....",monster.name,ud);
      monster.nhp-=ud;
	  }
  }
  // 몬스터가 아직 살아있다면 몬스터 공격
  if(monster.nhp>0) h_m();
  else {
    getch();
  }
  return;
}

// ====================
//  h_m : 몬스터 공격 
// ====================
void h_m() {
  int md,s;
  randomize();
  s=my_random(6); // 0~5 : 공격 문구 선택
  md=my_random(4)-2+monster.attack-user.defence;
  if(md <= 0) md=1; // 최소 1 데미지

  printf("\n");

  // 5번은 데미지 2배
  switch(s)
  {
	  case 0: printf("당신은 %s에게 명치를 가격 당하면서 %d 만큼의 충격을 받습니다...",monster.name,md);  break;
	  case 1: printf("%s가 당신의 목에 올라타 순식간에 목을 뒤로 꺽으면서 %d의 충격을 가합니다..",monster.name,md); break;
	  case 2: printf("%s가 잔상을 남기며 사라지며 순식간에 앞으로 다가와 두 눈을 손가락으로 강력하게 찌르면서 %d의 데미지를 줍니다..",monster.name,md);break;
	  case 3: printf("%s의 분신 3개가 나타나 각각 머리 가슴 배 를 구타하고 사라지며 %d만큼의 데미지를 줍니다..",monster.name,md);break;
	  case 4: printf("무시무시한 %s의 손등이 번개같이 당신의 목 뒤를 후려 갈기면서 %d만큼의 데미지를 줍니다..",monster.name,md);break;
	  case 5: printf("%s의 잔인한 죽음의 손길이 당신의 죽음을 재촉합니다..순식간에 %d만큼의 엄청난 충격이 몸으로 흡수됩니다...",monster.name,md*2);md=md*2;break;
  }
  user.nhp-=md; // 플레이어 hp 감소
  clearbuff();
  getch();
  return;
  }

// ==============================
//  Mg 함수 : 플레이어 마법 공격
// ==============================
void Mg()  {
  int bonus,w,in,i,s=0;
  while(1){
    gotoxy(1,14);printf("     Can Private Magic: \n ");
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━");
    for(i=0;i<8;i++)  {
        if(magic[i].lv <= user.lv)  {
          printf("\n%2d.NAME: %12s   ┃ Damage: %3d  ┃ Mp: %3d  ┃  Level: %3d",s+1,magic[i].name,magic[i].power,magic[i].ump,magic[i].lv);s++;
      }
    }
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━");
    printf("\n0.Cancel ,Magic Order(1~%d):",s);
    scanf("%d", &in);

    // 0이면 전투 메뉴로 복귀
    if (in == 0) {
      return;
    }
    // 1~s 범위 밖이면 잘못된 입력
    if (in > 0 && in <= s) {}
    else {
      printf("\n잘못된 입력입니다. <Enter>");
      clearbuff();
      getch();
      return;
    }
    // MP 부족 체크
    if (magic[in-1].ump > user.nmp) {
      printf("\nMP가 부족합니다. <Enter>");
      clearbuff();
      getch();
      return;
    }
    // 직업에 따라 마법 추가 데미시
    w=my_random(3); // 대사 랜덤
    if(user.cs==1) bonus=my_random(magic[in-1].power+user.lv*4);
    if(user.cs==2) bonus=my_random(magic[in-1].power+user.lv*8);
    if(user.cs==3) bonus=my_random(magic[in-1].power+user.lv*6);

    // 대사
    switch(w)
    {
      case 0:printf("\n 피에 굶주린 자들이여 성스러운 %s 를 받아라~~~",magic[in-1].name); break;
      case 1:printf("\n 나의 주먹을 맛 보아라~~~~%s!!!!",magic[in-1].name);break;
      case 2:printf("\n %s!!! 하핫 아프지? ",magic[in-1].name);break;
    }
    printf("\n 당신은 %s 에게 %d 만큼의 데미지를 가합니다",monster.name,magic[in-1].power+bonus);
    monster.nhp-=(magic[in-1].power+bonus);
    user.nmp-=magic[in-1].ump; // 마나 소모
    
    if(monster.nhp>0) h_m(); // 몬스터 살아있을시 반격
    else {
      getch();
    }
    return; // 턴 종료
  }
}