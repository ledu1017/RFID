/*
	���۱Ⱓ : 2022.05.06 ~ 2022.05.15
	���� ���� : 2022.05.15 20:42
	���α׷� ���� : RFID �����͸� �м��Ͽ� ���, ǥ������, ��� �ν� ���͹�, Ÿ���±� ��ġ ���� ����
				  RFID_Data.txt, tagID_position.txt ���
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>    // pow(), sqrt(), fabs() ���
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
	float distance;
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

TAGINFO* fileopen(TAGINFO* list);    // txt���� ����
TAGINFO* insertList(TAGINFO* list, char* data);    // txt������ ������ ����ü�� ����
double changeTime(char* data);    // txt������ �ð��� ����ϱ� ���� ���·� ��ȯ
TAGDATA* insertData(TAGDATA* list2, char* tag_id, double rssi_avg, double st_deviation, double interval_avg);    // txt������ �����͸� �ʿ��� ���·� ����
TAGDATA* dataAnalysis(TAGINFO* list);    // txt������ �����͸� �ʿ��� ���·� ���
void targetPosition(TAGDATA* list2, char* tag_id, int n);    // Ÿ���±��� ���� ��ġ
void print1(TAGDATA* list2);    // 1 �Է½� �޴�
void print2(TAGDATA* list2, char* target_id);    // 2 �Է½� �޴�
void menu();    // ���θ޴�

int main() {
	TAGINFO* list = NULL;
	TAGDATA* list2 = NULL;
	int select = 0, n = 0;
	char tag_id[27] = { NULL };

	list = fileopen(list);
	list2 = dataAnalysis(list);
	while (1) {
		system("CLS");
		menu();
		printf("�޴��� ������ �ּ��� : ");
		scanf("%d", &select);
		getchar();

		if (select == 1) {
			print1(list2);
			system("pause");
		}
		else if (select == 2) {
			system("CLS");
			printf("\nŸ���±� \"0x35E0170044CF0D590000F5A5\" �� �ش��ϴ� ������ ����մϴ�.\n");
			print2(list2, "0x35E0170044CF0D590000F5A5");
			system("pause");
		}
		else if (select == 3) {
			printf("\nN���� ����� �����±׸� ���� Ÿ�� �±��� ��ġ�� �����մϴ�.(1 <= n <=60)\n");
			printf("N�� �Է� ���ּ��� : ");
			scanf("%d", &n);
			if (n > 60 || n <= 0) {
				printf("N�� ���� �߸��Ǿ����ϴ�.\n");
				system("pause");
				continue;
			}
			system("CLS");
			targetPosition(list2, "0x35E0170044CF0D590000F5A5", n);
			system("pause");
		}
		else if (select == 4) {
			printf("\n���α׷��� �����մϴ�.\n");
			break;
		}
		else {
			printf("\n�߸��� ��ȣ�� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ���\n");
			continue;
		}
	}

}

TAGINFO* fileopen(TAGINFO* list) {
	FILE* RFID_Data;
	char data[200];

	RFID_Data = fopen("RFID_Data.txt", "r");    // �б����·� ����
	if (RFID_Data == NULL) {
		printf("File open error\n");
		return NULL;
	}

	while (!feof(RFID_Data)) {    // ������ ���������� ���پ� �о� ����ü ����
		fgets(data, sizeof(data), RFID_Data);
		list = insertList(list, data);
	}

	return list;
}

TAGINFO* insertList(TAGINFO* list, char* data) {
	TAGINFO* newnode = NULL;
	char* result;

	if ((newnode = (TAGINFO*)malloc(sizeof(TAGINFO))) == NULL) {    // �޸� �Ҵ�
		printf("Memory allocation error\n");
		return NULL;
	}

	result = strtok(data, " ,=T");
	while (result != NULL) {    // �ʿ��� �� �����Ͽ� ����
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
	while (result != NULL) {    // �ð��� �ش��ϴ� �����Ϳ��� Ư������ ����
		strcat(time, result);
		result = strtok(NULL, ".:\n");
	}

	return atof(time) / 1000;
}

TAGDATA* insertData(TAGDATA* list2, char* tag_id, double rssi_avg, double st_deviation, double interval_avg) {
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

TAGDATA* dataAnalysis(TAGINFO* list) {
	TAGINFO* head = NULL;
	TAGDATA* list2 = NULL;
	double rssi_avg = 0, st_deviation = 0, interval_avg = 0, tail = 0;    // ���, ǥ������, ��� �ν� ���͹�
	int count = 0;
	char* tag_id;

	for (int i = 0; i < 61; i++) {
		head = list;
		if (i == 60) tag_id = "0x35E0170044CF0D590000F5A5";
		else tag_id = referenceIDs[i];
		rssi_avg = 0, st_deviation = 0, interval_avg = 0, tail = 0, count = 0;

		while (head != NULL) {    // RSSI ���
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
		while (head != NULL) {    // RSSI ǥ������
			if (strcmp(head->id, tag_id) == 0) {
				st_deviation += pow(rssi_avg - head->rssi, 2);    // (��� - �ش��ϴ� ��)^2 <- �л� 
			}
			head = head->next;
		}
		st_deviation = sqrt(st_deviation / count);    // �л��� ������ <- ǥ������

		head = list;
		while (head != NULL) {    // ��� �ν� ���͹�
			if (strcmp(head->id, tag_id) == 0) {
				if (tail != 0) interval_avg += tail - head->identifiedTime;    // �ð��� ū ������ ������ ���� interval_avg�� ���ϱ�
				tail = head->identifiedTime;
			}
			head = head->next;
		}
		interval_avg /= count;    // �� �տ��� count �� ���� ��� ���ϱ�

		list2 = insertData(list2, tag_id, rssi_avg, st_deviation, interval_avg);
	}

	return list2;
}

void targetPosition(TAGDATA* list2, char* tag_id, int n) {    // ��ġ ������ ���ϱ�
	TAGDATA* target = NULL, * head = NULL;
	float distance_sort[60] = { 0 };    // (Ÿ�� �±� ��� RSSI - ���� �±� ��� RSSI)���� ���� �迭
	float temp;
	int count = 0, check = 0;
	int x_coordinate = 0;    // Ÿ�� �±��� ���� X��ǥ
	int y_coordinate = 0;    // Ÿ�� �±��� ���� Y��ǥ
	target = head = list2;

	while (target->next != NULL) target = target->next;    // ���Ḯ��Ʈ �������� ��ġ�� Ÿ���� ������ ã��

	while (head->next != NULL) {
		if (head->rssi_avg == 0) {    // rssi_avg�� 0�ΰ��� �ش� �±װ� ���ٴ� ��
			head->distance = 10000;    // �ش��ϴ� �±װ� ���� ������ ���� 10000���� ����
			distance_sort[count] = 10000;
			count++;
			head = head->next;
			continue;
		}
		head->distance = fabs(target->rssi_avg - head->rssi_avg);    // �Ÿ��� Ÿ���±� ��� RSSI - �����±� ��� RSSI�� ���밪
		distance_sort[count] = fabs(target->rssi_avg - head->rssi_avg);
		count++;
		head = head->next;
	}

	for (int i = 0; i < 60; i++) {    // �Ÿ��� ����� ������ ����
		for (int j = 0; j < 59; j++) {
			if (distance_sort[j] > distance_sort[j + 1]) {
				temp = distance_sort[j];
				distance_sort[j] = distance_sort[j + 1];
				distance_sort[j + 1] = temp;
			}
		}
	}

	count = 1;
	head = list2;
	while (head->next != NULL) {
		check = 0;
		for (int i = 0; i < n; i++) {
			if (head->distance == distance_sort[i]) {
				
				x_coordinate += referencePoints[count - 1][0];
				y_coordinate += referencePoints[count - 1][1];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);    // �ؽ�Ʈ�� ���������� ��ȯ
				printf("R%d\t", count);
				if (count % 6 == 0) printf("\n\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);    // �ؽ�Ʈ�� ������� ��ȯ
				head = head->next;
				count++;
				check = 1;
				break;
			}
		}
		if (check == 0) {
				printf("R%d\t", count);
				if (count % 6 == 0) printf("\n\n");
				count++;
				head = head->next;
		}
	}
	head = list2;
	count = 1;
	while (distance_sort[0] != head->distance) {    // Ÿ�� �±׿� ���� ����� �����±� ã��
		count++;
		head = head->next;
	}
	printf("������ ǥ�õ� ��ġ�� Ÿ�� �±׿� ���� ����� %d���� ��ġ�Դϴ�.\n\n", n);
	printf("Ÿ�� �±׿� ���� ����� �����±� : \"%s\" = R%d\n\n", head->id, count);
	printf("\"%s\"�� Ÿ���±��� ��� RSSI���� ���� %.3f�Դϴ�.\n\n", head->id, distance_sort[0]);
	printf("Ÿ�� �±��� ���� ��ġ ��ǥ: (%d, %d)\n\n", x_coordinate / n, y_coordinate / n);
}


void print1(TAGDATA* list2) {
	TAGDATA* head = list2;
	char tag_id[27] = { NULL };
	int count = 0;
	int check = 0;
	printf("ã����� �±� ���̵� �Է����ּ��� : ");
	scanf("%s", tag_id);
	system("CLS");
	printf("\n�����±� \"%s\" �� �ش��ϴ� ������ ����մϴ�.\n", tag_id);
	while (head != NULL) {
		if (strcmp(head->id, tag_id) == 0) {
			if (head->rssi_avg == 0) {
				printf("\n�ش� �±״� ���������� �����Ͱ� �������� �ʽ��ϴ�.\n\n");
				check = 1;
				break;
			}
			printf("\n\n��� : %.3lf\n\nǥ������ : %.3lf\n\n��� �ν� ���͹� : %.3lf\n\n", head->rssi_avg, head->st_diviation, head->interval_avg);
			return;
		}
		head = head->next;
	}
	if (check == 0) printf("\n�ش� �±״� �������� �ʽ��ϴ�.\n\n");
}

void print2(TAGDATA* list2, char* target_id) {
	while (list2 != NULL) {
		if (strcmp(list2->id, target_id) == 0) {
			printf("\n��� : %.3lf\n\nǥ������ : %.3lf\n\n��� �ν� ���͹� : %.3lf\n\n", list2->rssi_avg, list2->st_diviation, list2->interval_avg);
			return;
		}
		list2 = list2->next;
	}
}

void menu() {
	printf("\n============================================\n\n");
	printf("1. �����±� ������ �м�\n\n");
	printf("2. Ÿ���±� ������ �м�\n\n");
	printf("3. Ÿ���±� ��ġ ���� ��\n\n");
	printf("4. ����\n\n");
	printf("============================================\n");
}