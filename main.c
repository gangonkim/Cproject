#define _CRT_SECURE_NO_WARNINGS
#include "Env.h"
#include "OfferService.h"
#include <conio.h>  // conio.h를 사용하여 키 입력 및 커서 조작을 위한 라이브러리

// ANSI 색상 코드
#define RESET_COLOR "\033[0m"
#define SELECTED_BG_COLOR "\033[47;30m"  // 하얀색 배경, 검은색 글자
#define DEFAULT_BG_COLOR "\033[0m"       // 기본 배경 (어떤 색도 지정하지 않음)

void generate_report();   // 보고서 출력 함수 프로토타입
void execute_trade(int choice);  // 매수/매도 선택을 처리하는 함수 프로토타입

void display_menu(int choice) {
    cls;
    printf("==== 주식 거래 프로그램 ====\n");
	printf("로그인한 %s: %s\n", authUserType == '0' ? "사용자" : "관리자", authUser);
	printf("============================\n");

    // 메뉴 항목에 선택된 항목에 대해 배경색을 변경하여 출력
    printf("%s1. 매수%s\n", choice == 1 ? SELECTED_BG_COLOR : DEFAULT_BG_COLOR, RESET_COLOR);
    printf("%s2. 매도%s\n", choice == 2 ? SELECTED_BG_COLOR : DEFAULT_BG_COLOR, RESET_COLOR);
    printf("%s3. 체결/미체결%s\n", choice == 3 ? SELECTED_BG_COLOR : DEFAULT_BG_COLOR, RESET_COLOR);
    printf("%s4. 잔고%s\n", choice == 4 ? SELECTED_BG_COLOR : DEFAULT_BG_COLOR, RESET_COLOR);
    printf("%s5. 보고서%s\n", choice == 5 ? SELECTED_BG_COLOR : DEFAULT_BG_COLOR, RESET_COLOR);

    printf("===========================\n");
    printf("상하 화살표키로 선택 후 Enter\n");
}

void handle_menu_selection() {
    int choice = 1;  // 기본 선택 1번 (매수)
    char key;

    while (1) {
        display_menu(choice);  // 메뉴를 출력하며 현재 선택을 표시

        key = _getch();  // 사용자가 입력한 키 받아오기

        if (key == -32) {  // 방향키 입력 시
            key = _getch();
            if (key == 72) {  // 위 방향키 (상)
                if (choice > 1) {
                    choice--;
                }
            }
            else if (key == 80) {  // 아래 방향키 (하)
                if (choice < 5) {
                    choice++;
                }
            }
        }
        else if (key == 13) {  // Enter 키 입력 시
            execute_trade(choice);  // 선택한 메뉴에 맞는 함수 실행
        }

        // 메뉴 선택이 변경되면 새로 고침
        if (key == 13) {
            break;
        }
    }
}

void execute_trade(int choice) {
    switch (choice) {
    case 1:
        showstockprice();  // 매수 함수 호출
        add_offer();
        break;
    case 2:
        showstockprice();  // 매도 함수 호출 (show_stock_price()는 매수/매도 모두 포함될 수 있습니다)
        add_offer();
        break;
    case 3:
        printf("체결/미체결 목록 출력\n");
        // 체결/미체결 관련 출력 로직
        break;
    case 4:
        showholdings();  // 잔고 출력 함수 호출
        break;
    case 5:
        generate_report();  // 보고서 생성 함수 호출
        break;
    default:
        break;
    }
}


void generate_report() {
    printf("보고서 출력\n");
    // 보고서 출력 

    //유저 role에 따라 삼항연산으로 넘기기
    exportJson(authUserType == '0' ? 0 : 1);

}
void exportJson(int n) {
    //0 : 일반사용자
    //1 : 관리자

    //파일 위치 로컬경로
    FILE* file = fopen("C:/coolfin/projc/public/user_data.json", "w");
    if (file == NULL) {
        printf("파일 로드 실패");
        return;
    }


    fprintf(file, "{\n");
    fprintf(file, "    \"userId\": \"%s\",\n", authId);
    fprintf(file, "    \"name\": \"%s\"\n", authUser);
    fprintf(file, "}\n");


    fclose(file);

    printf("JSON 파일 생성이 완료되었습니다\n");
    printf("계속하시려면 아무 키나 입력해 주세요...");
    pause;
    cls;
}

int main() {
    printf("C 프로젝트 불러오는 중...\n");
    for (int i = 0; i <= 100; i++) {
        printProgressBar(i);  // 진행 바 출력
        Sleep(5); // 1초 대기
    }

    printf("\n로딩 성공!\n");
    Sleep(1000);

    cls;

    login();

    handle_menu_selection();  // 메뉴 선택 실행
    return 0;
}
