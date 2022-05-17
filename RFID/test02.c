/*
	제작기간 : 2022.05.06 ~
	최종 수정 : 2022.05.11 00:35
	프로그램 설명 : RFID 데이터를 분석하여 평균, 표준편차, 평균 인식 인터벌, 타겟태그 위치 등을 제공
				  RFID_Data.txt, tagID_position.txt 사용
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>    // pow(), sqrt(), fabs() 사용
#include <windows.h>

typedef struct taginfo {
	char id[27];
	float rssi;
	double identifiedTime;
	struct taginfo* next;
}TAGINFO;

typedef struct tagdata {
	char id[27];
	float rssi_avg;
	double interval_avg, st_diviation;
	struct tagdata* next;
}TAGDATA;

char referenceIDs[60][27] = {
		"0x35E0170044CF0D590000F544","0x35E0170044CF0D590000F543","0x35E0170044CF0D590000F542","0x35E0170044CF0D590000F551","0x35E0170044CF0D590000F550","0x35E0170044CF0D590000F54F",
		"0x35E0170044CF0D590000F61D","0x35E0170044CF0D590000F61C","0x35E0170044CF0D590000F61B","0x35E0170044CF0D590000F61A","0x35E0170044CF0D590000F629","0x35E0170044CF0D590000F628",
		"0x35E0170044CF0D590000F54E","0x35E0170044CF0D590000F55D","0x35E0170044CF0D590000F55C","0x35E0170044CF0D590000F55B","0x35E0170044CF0D590000F55A","0x35E0170044CF0D590000F569",
		"0x35E0170044CF0D590000F627","0x35E0170044CF0D590000F626","0x35E0170044CF0D590000F635","0x35E0170044CF0D590000F634","0x35E0170044CF0D590000F633","0x35E0170044CF0D590000F632",
		"0x35E0170044CF0D590000F568","0x35E0170044CF0D590000F567","0x35E0170044CF0D590000F566","0x35E0170044CF0D590000F575","0x35E0170044CF0D590000F574","0x35E0170044CF0D590000F573",
		"0x35E0170044CF0D590000F641","0x35E0170044CF0D590000F640","0x35E0170044CF0D590000F63F","0x35E0170044CF0D590000F63E","0x35E0170044CF0D590000F64D","0x35E0170044CF0D590000F64C",
		"0x35E0170044CF0D590000F572","0x35E0170044CF0D590000F581","0x35E0170044CF0D590000F580","0x35E0170044CF0D590000F57F","0x35E0170044CF0D590000F57E","0x35E0170044CF0D590000F58D",
		"0x35E0170044CF0D590000F64B","0x35E0170044CF0D590000F64A","0x35E0170044CF0D590000F659","0x35E0170044CF0D590000F658","0x35E0170044CF0D590000F657","0x35E0170044CF0D590000F656",
		"0x35E0170044CF0D590000F58C","0x35E0170044CF0D590000F58B","0x35E0170044CF0D590000F58A","0x35E0170044CF0D590000F599","0x35E0170044CF0D590000F598","0x35E0170044CF0D590000F597",
		"0x35E0170044CF0D590000F603","0x35E0170044CF0D590000F5F8","0x35E0170044CF0D590000F5F7","0x35E0170044CF0D590000F5F6","0x35E0170044CF0D590000F605","0x35E0170044CF0D590000F604"
};

int referencePoints[60][2] = {
		{ 7, 1 },{ 22, 1 },{ 37, 1 },{ 52, 1 },{ 67, 1 },{ 81, 1 },
		{ 101, 1 },{ 116, 1 },{ 131, 1 },{ 146, 1 },{ 161, 1 },{ 176, 1 },
		{ 7, 41 },{ 22, 41 },{ 37, 41 },{ 52, 41 },{ 67, 41 },{ 81, 41 },
		{ 101, 41 },{ 116, 41 },{ 131, 41 },{ 146, 41 },{ 161, 41 },{ 176, 41 },
		{ 7, 80 },{ 22, 80 },{ 37, 80 },{ 52, 80 },{ 67, 80 },{ 81, 80 },
		{ 101, 80 },{ 116, 80 },{ 131, 80 },{ 146, 80 },{ 161, 80 },{ 176, 80 },
		{ 7, 115 },{ 22, 115 },{ 37, 115 },{ 52, 115 },{ 67, 115 },{ 81, 115 },
		{ 101, 115 },{ 116, 115 },{ 131, 115 },{ 146, 115 },{ 161, 115 },{ 176, 115 },
		{ 7, 144 },{ 22, 144 },{ 37, 144 },{ 52, 144 },{ 67, 144 },{ 81, 144 },
		{ 101, 144 },{ 116, 144 },{ 131, 144 },{ 146, 144 },{ 161, 144 },{ 176, 144 }
};

TAGINFO* fileopen(TAGINFO* list);
TAGINFO* insertList(TAGINFO* list, char* data);
TAGDATA* insertData(TAGDATA* list2, char* tag_id, double rssi_avg, double st_deviation, double interval_avg);
TAGDATA* dataAnalysis(TAGINFO* list);
double changeTime(char* data);
void targetPosition(TAGDATA *list2, char* tag_id, int n);
void print1(TAGDATA *list2);
void print2(TAGDATA* list2, char* target_id);
void menu();

int main() {
	TAGINFO* list = NULL;
	TAGDATA* list2 = NULL;
	int select = 0, n = 0;
	char tag_id[27] = { NULL };
	int count = 0;    // 나중에 지워야함
	int test = 0;	// 나중에 지워야함
	list = fileopen(list);
	list2 = dataAnalysis(list);
	/*while (list2 != NULL) {
		if (test < 6) {
			printf("%.3lf   ", list2->rssi_avg);
			list2 = list2->next;
			test++;
		}
		else {
			printf("\n");
			test = 0;
		}
	}*/
	while (1) {
		//system("CLS");
		menu();
		printf("메뉴를 선택해 주세요 : ");
		scanf("%d", &select);
		getchar();

		if (select == 1) {
			print1(list2);
			system("pause");
		}
		else if (select == 2) {
			printf("타겟태그 \"0x35E0170044CF0D590000F5A5\" 에 해당하는 정보를 출력합니다.\n");
			print2(list2, "0x35E0170044CF0D590000F5A5");
			system("pause");
		}
		else if (select == 3) {
			printf("유사한거 n개 입력 : ");    // 문장 수정해야함
			scanf("%d", &n);
			targetPosition(list2, "0x35E0170044CF0D590000F5A5", n);
			system("pause");
		}
		else if (select == 4) {
			printf("\n프로그램을 종료합니다.\n");
			break;
		}
		else {
			printf("\n잘못된 번호를 입력하셨습니다. 다시 입력하세요\n");
			continue;
		}
	}

}

TAGINFO* fileopen(TAGINFO* list) {
	FILE* RFID_Data;
	char data[200];

	RFID_Data = fopen("RFID_Data.txt", "r");
	if (RFID_Data == NULL) {
		printf("File open error\n");
		return NULL;
	}

	while (!feof(RFID_Data)) {    // 파일이 끝날때까지 한줄씩 읽어 구조체 삽입
		fgets(data, sizeof(data), RFID_Data);
		list = insertList(list, data);
	}

	return list;
}

TAGINFO* insertList(TAGINFO* list, char* data) {
	TAGINFO* newnode = NULL;
	char* result;

	if ((newnode = (TAGINFO*)malloc(sizeof(TAGINFO))) == NULL) {    // 메모리 할당
		printf("Memory allocation error\n");
		return NULL;
	}

	result = strtok(data, " ,=T");
	while (result != NULL) {    // 필요한 값 구분하여 삽입
		if (strcmp(result, "tag_id") == 0) {
			result = strtok(NULL, " ,=T");
			strcpy(newnode->id, result);
		}
		else if (strcmp(result, "rssi") == 0) {
			result = strtok(NULL, " ,=T");
			newnode->rssi = atof(result);
		}
		else if (strcmp(result, "time") == 0) {
			result = strtok(NULL, " ,=T");
			result = strtok(NULL, " ,=T\n");
			newnode->identifiedTime = changeTime(result);
		}
		else {
			result = strtok(NULL, " ,=T");
		}
	}

	newnode->next = list;
	list = newnode;

	return list;
}

double changeTime(char* data) {
	char time[10] = { NULL };
	char* result;

	result = strtok(data, ".:");
	while (result != NULL) {    // 시간에 해당하는 데이터에서 특수문자 제거
		strcat(time, result);
		result = strtok(NULL, ".:\n");
	}

	return atof(time) / 1000;
}

TAGDATA* dataAnalysis(TAGINFO* list) {
	TAGINFO* head = NULL;
	TAGDATA* list2 = NULL;
	double rssi_avg = 0, st_deviation = 0, interval_avg = 0, tail = 0;    // 평균, 표준편차, 평균 인식 인터벌
	int count = 0;
	char* tag_id;
	int test = 0;

	for (int i = 0; i < 61 ; i++) {
		head = list;
		if (i == 60) tag_id = "0x35E0170044CF0D590000F5A5";
		else tag_id = referenceIDs[i];
		rssi_avg = 0, st_deviation = 0, interval_avg = 0, tail = 0, count = 0;

		while (head != NULL) {    // RSSI 평균
			if (strcmp(head->id, tag_id) == 0) {
				rssi_avg += head->rssi;
				count++;
			}
			head = head->next;
		}

		if (count == 0) {
			list2 = insertData(list2, tag_id, 0, 0, 0);
			continue;
		}
		rssi_avg = rssi_avg / count;

		head = list;
		while (head != NULL) {    // RSSI 표준편차
			if (strcmp(head->id, tag_id) == 0) {
				st_deviation += pow(rssi_avg - head->rssi, 2);    // (평균 - 해당하는 값)^2 <- 분산 
			}
			head = head->next;
		}
		st_deviation = sqrt(st_deviation / count);    // 분산의 제곱근 <- 표준편차

		head = list;
		while (head != NULL) {    // 평균 인식 인터벌
			if (strcmp(head->id, tag_id) == 0) {
				if (tail != 0) interval_avg += tail - head->identifiedTime;    // 시간값 큰 값에서 작은값 빼고 interval_avg에 합하기
				tail = head->identifiedTime;
			}
			head = head->next;
		}
		interval_avg /= count;    // 총 합에서 count 를 나눠 평균 구하기
		
		list2 = insertData(list2, tag_id, rssi_avg, st_deviation, interval_avg);
	}

	return list2;
}

TAGDATA* insertData(TAGDATA* list2, char *tag_id, double rssi_avg, double st_deviation, double interval_avg) {
	TAGDATA* head = list2, * newnode = NULL;
	int count = 0;
	if ((newnode = (TAGDATA*)malloc(sizeof(TAGDATA))) == NULL) {
		printf("newnode memory allocation error");
		return NULL;
	}
	strcpy(newnode->id, tag_id);
	newnode->rssi_avg = rssi_avg;
	newnode->st_diviation = st_deviation;
	newnode->interval_avg = interval_avg;
	newnode->next = NULL;

	if (list2 == NULL) list2 = newnode;
	else {
		while (head->next != NULL) {
			head = head->next;
		}
		head->next = newnode;
	}

	return list2;
}

void targetPosition(TAGDATA *list2, char* tag_id, int n) {    // 위치 예측값 구하기
	TAGDATA* target = NULL, * head = NULL;
	float* min = malloc(sizeof(float) * n);    // 사용자가 입력한 n개만큼 배열 생성
	int* position = malloc(sizeof(int) * n);
	int count = 0;
	target = head = list2;

	while (target->next != NULL) target = target->next;    // 연결리스트 마지막에 위치한 타겟의 데이터 찾기

	for (int i = 0; i < n; i++) {    // 동적할당된 min에 초기값 100000으로 설정
		*(min + i) = 100000;
	}

	if (n == 1) {
		while (head->next != NULL) {    // 마지막은 타겟이니까 타겟이 되기 전까지
			if (min[0] > (fabs(target->rssi_avg - head->rssi_avg))) {
				min[0] = fabs(target->rssi_avg - head->rssi_avg);
				position[0] = count;
			}
			count++;
			head = head->next;
		}
		printf("타겟 태그와 가장 가까운 참조 태그의 위치 : (%d, %d)\n", referencePoints[position[0]][0], referencePoints[position[0]][1]);
	}

	if (n == 2) {
		while (head->next != NULL) {

		}
	}

	free(min);
}

void print1(TAGDATA* list2) {
	TAGDATA* head = list2;
	char tag_id[27] = { NULL };
	int count = 0;
	printf("찾고싶은 태그 아이디를 입력해주세요 : ");
	scanf("%s", tag_id);

	while (head != NULL) {
		if (strcmp(head->id, tag_id) == 0) {
			printf("평균 : %.3lf\n표준편차 : %.3lf\n평균 인식 인터벌 : %.3lf\n", head->rssi_avg, head->st_diviation, head->interval_avg);
			return;
		}
		head = head->next;
	}
	printf("해당 아이디에 대한 데이터가 없습니다.\n");
}

void print2(TAGDATA* list2, char* target_id) {
	while (list2 != NULL) {
		if (strcmp(list2->id, target_id) == 0) {
			printf("평균 : %.3lf\n표준편차 : %.3lf\n평균 인식 인터벌 : %.3lf\n", list2->rssi_avg, list2->st_diviation, list2->interval_avg);
			return;
		}
		list2 = list2->next;
	}
}

void menu() {
	printf("\n============================================\n\n");
	printf("1. 참조태그 데이터 분석\n\n");
	printf("2. 타겟태그 데이터 분석\n\n");
	printf("3. 타겟태그 위치 예측 값\n\n");
	printf("4. 종료\n\n");
	printf("============================================\n");
}