#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MAX_LENGTH 128
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#pragma warning(disable : 6011)
#pragma warning(disable : 6001)
#pragma warning(disable : 28182)

// 연결 리스트 노드
typedef struct ListNode {
	int coef; // 계수
	int expon; // 지수
	struct ListNode* link;
}LLNode;


// 연결 리스트 헤더
typedef struct ListHeader {
	int length; // 연결리스트 길이
	LLNode* head; // 시점
	LLNode* tail; // 종점
}LLHeader;

// 초기화 함수
void init(LLHeader* plist)
{
	plist->length = 0;
	plist->head = plist->tail = NULL;
}

void insert(LLHeader* plist, int coef, int expon)
{
	LLNode* temp = (LLNode*)malloc(sizeof(LLNode)); // 받은 변수 임시 저장 노드 동적 활당
	if (temp == NULL) { // 만약 에러로 동적 할당이 안되었다면 => 에러를 발생
		errno = "Memory Allocation Error";
		perror("Error Message"); 
	}
	temp->coef = coef; // 임시 저장 노드의 coefficient 저장
	temp->expon = expon; // 임시 저장 노드의 exponential 저장
	temp->link = NULL; // 마지막에 추가하는 노드이므로 link를 NULL로 설정
	if (plist->tail == NULL) { // plist가 아무 노드도 없을 때
		plist->head = plist->tail = temp; // 첫 번째 노드이므로 시점과 종점을 temp로 지정한다.
	}
	else { // plist에 노드들이 있을 때
		plist->tail->link = temp; // 노드 추가하기 전 마지막 노드(앞의 노드)의 link가 temp를 가리키도록 설정
		plist->tail = temp; // 종점 가리키는 노드를 temp로 설정(종점을 temp)로 저장
	}
	plist->length++; // 노드가 하나 추가되었으므로 길이 1추가

}

void add(LLHeader* plist1, LLHeader* plist2, LLHeader* plist3) { //polynominal 덧셈 연산
	LLNode* a = plist1->head;
	LLNode* b = plist2->head;
	int sum;
	while (a && b) { // a와 b 모두 끝까지 참조 안했을 동안(Linked List 안 노드들을 둘 중 하나라도 모두 소진하지 않을 동안 반복)
		if (a->expon == b->expon) { // a의 지수와 b의 지수가 같다면
			sum = a->coef + b->coef; // a와 b의 coef를 더한 것을 임시 parameter에 저장
			if (sum != 0) insert(plist3, sum, a->expon); // a와 b의 expon이 같을 경우 임시 저장했던 coef를 덧셈 연산한 Linked List의 노드로 추가한다.
			a = a->link; b = b->link; // a와 b 모두 사용(참조)했으므로 다음 노드를 참조할 수 있도록 변경한다.
		}
		else if (a->expon > b->expon) { // a의 지수가 b의 지수보다 크다면
			insert(plist3, a->coef, a->expon); // a의 정보만 덧셈 연산 Linked List로 넘긴다.
			a = a->link; // a만 참조했으므로 a만 다음 노드를 참조할 수 있도록 변경한다.
		}
		else { // a의 지수가 b의 지수보다 작으면(a->expon < b->expon)
			insert(plist3, b->coef, b->expon); /// b의 정보만 덧셈 연산 Linked List로 넘긴다.
			b = b->link; // b만 참조했으므로 b만 다음 노드를 참조할 수 있도록 변경한다.
		}
	}
	// a와 b중 하나라도 먼저 끝났을 때
	// 남은 다항식 복사
	for (; a != NULL; a = a->link) {
		insert(plist3, a->coef, a->expon); // a가 남았을 때(b는 다 소진)
	}
	for (; b != NULL; b = b->link) {
		insert(plist3, b->coef, b->expon); // b가 남았을 때(a는 다 소진)
	}
}

void merge(LLHeader* mlist) // expon이 같은 것의 coef를 더하는 함수(Sorting 후 실행)
{
	LLNode* m = mlist->head;
	for (; m != NULL; ) {
		if (m->link != NULL) {
			if (m->expon == m->link->expon) {
				m->coef += m->link->coef;
				m->link = m->link->link;
				mlist->length--;
			}
		}
		else {
			break;
		}
		if (m->expon != m->link->expon) m = m->link;
	}
}


void sorting(LLHeader *list) // list가 expon이 낮아지도록 Sorting
{
	LLNode* p = list->head;
	LLNode* temp1 = (LLNode*)malloc(sizeof(LLNode));
	LLNode* temp2 = (LLNode*)malloc(sizeof(LLNode));
	int iter = list->length;
	int i, j;
	for (i = iter - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (j == 0) { // 첫 번째 항이라면
				if (p->expon < p->link->expon) {
					temp1->link = p->link->link;
					p->link->link = p;
					p->link = temp1->link;
				}	
			}
			else { // 중간항이라면
				if (p->link->expon < p->link->link->expon) {
					temp1->link = p->link->link->link;
					temp2->link = p->link->link;
					p->link->link->link = p->link;
					p->link->link = temp1->link;
					p->link = temp2->link;
				}
				p = p->link;
			}
		}
		p = list->head;
	}

}


void multiple(LLHeader* plist1, LLHeader* plist2, LLHeader* plist4) // polynominal 곱셈
{
	LLNode* a = plist1->head; // list1을 받는다
	LLNode* b = plist2->head; // list2를 받는다

	int mcoef, mexpon;

	for (; a != NULL; a = a->link) {
		for (; b != NULL; b = b->link) {
			mcoef = a->coef * b->coef;
			mexpon = a->expon + b->expon;
			insert(plist4, mcoef, mexpon);
		}
		b = plist2->head;
	}

}



// 제대로 연산 되었는지 확인 코드
void print(LLHeader* plist)
{
	LLNode* p = plist->head;
	for (; p; p = p->link) {
		printf("%d %d\n", p->coef, p->expon);
	}
}



int main()
{
	
	// 연결리스트 생성
	LLHeader llist1, llist2, llist3, llist4;

	// 연결리스트 초기화
	init(&llist1);
	init(&llist2);
	init(&llist3);
	init(&llist4);
	
	// 파일 읽어오기
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) { // 파일이 없으면 NULL 반환 => 에러 처리 용이
		printf("No File Dectected. Closing the program.");
		return 0;
	}
	else {
		// 텍스트 줄 계산
		// 임시 메모리
		char buffer[MAX_LENGTH];
		// NULL 판단(다항식 분리 인식에 활용)
		int check_nullity = 0;
		// 줄 수
		//int line_count = 0;
		// 저장자 지정
		
		// 파일에서 텍스트 한 줄씩 읽어오기
		while (fgets(buffer, MAX_LENGTH, input) != NULL) { // 다 읽어내면 NULL 반환
			int b_coef = 0, b_expon = 0;
			//line_count++; // 줄 수 증가
			//printf("[%d]번째 줄 : %s", line_count, buffer); // Data 확인
			//printf("Nullity : %d\n", check_nullity); // 공백 인식 확인

			
			if (buffer[0] == ' ') check_nullity++; // 공백 인식 시 공백수 1 증가

			if (buffer[0] == '0') continue; // input.txt에서 coef에 실수로 0을 입력할 시 에러처리 
			
			// 다항식 저장
			if (check_nullity == 1 && buffer[0] != ' ') { // 첫 번째 다항식 생성
				//printf("Check\n"); // 확인 연산자


				int i, j; // index 추출 (i = coef끝 / j = 데이터의 끝(줄에서))

				for (i = 0; buffer[i] != ' '; ++i); // i-1이 coef의 끝 인덱스
				for (j = 0; buffer[j] != '\0'; ++j); // j-3이 expon의 끝 인덱스
				/*printf("i값 : %d, j값 : %d\n", i, j);
				printf("coef : %c, expon : %c\n", buffer[i - 1], buffer[j - 3]);*/
				char tcoef[100];
				char texpon[100];
				int k, f;
				for (k = 0; k < i; k++) { // 계수 문자를 문자열로 합침
					tcoef[k] = buffer[k];
					if (k == i-1) tcoef[k + 1] = '\0';
				}
				b_coef = atoi(tcoef); // 문자열을 정수로 변환
				//printf("coef : %d\n", b_coef); // 확인
				for (f = i + 1; f < j - 2; f++) { // 지수 문자를 문자열로 합침
					texpon[f - (i + 1)] = buffer[f];
					if (f == j - 3) texpon[f - i] = '\0';
				}
				b_expon = atoi(texpon); // 문자열을 정수로 변환
				//printf("expon : %d\n", b_expon); // 확인
				// llist1에 추가
				insert(&llist1, b_coef, b_expon);
			}
			if (check_nullity == 2 && buffer[0] != ' ') { // 두 번째 다항식 생성
				int i, j; // index 추출 (i = coef끝 / j = 데이터의 끝(줄에서))

				for (i = 0; buffer[i] != ' '; ++i); // i-1이 coef의 끝 인덱스
				for (j = 0; buffer[j] != '\0'; ++j); // j-3이 expon의 끝 인덱스
				/*printf("i값 : %d, j값 : %d\n", i, j);
				printf("coef : %c, expon : %c\n", buffer[i - 1], buffer[j - 3]);*/
				char tcoef[100];
				char texpon[100];
				int k, f;
				for (k = 0; k < i; k++) { // 계수 문자를 문자열로 합침
					tcoef[k] = buffer[k];
					if (k == i - 1) tcoef[k + 1] = '\0';
				}
				b_coef = atoi(tcoef); // 문자열을 정수로 변환
				//printf("coef : %d\n", b_coef); // 확인
				for (f = i + 1; f < j - 2; f++) { // 지수 문자를 문자열로 합침
					texpon[f - (i + 1)] = buffer[f];
					if (f == j - 3) texpon[f - i] = '\0';
				}
				b_expon = atoi(texpon); // 문자열을 정수로 변환
				//printf("expon : %d\n", b_expon); // 확인
				// llist2에 추가
				insert(&llist2, b_coef, b_expon);
			}

		}
	}
	fclose(input);

	
	// 연산 전 Sorting 실시
	sorting(&llist1);
	sorting(&llist2);
	// 연산 전 merge 실시
	merge(&llist1);
	merge(&llist2);
	
	// 데이터 확인
	//print(&llist1); // 확인
	/*printf("length : %d\n", llist1.length);
	printf("\n");*/
	//print(&llist2); // 확인
	/*printf("length : %d\n", llist2.length);
	printf("\n");*/

	// 덧셈 연산한 polynominal Linked List (L3 = L1 + L2)
	add(&llist1, &llist2, &llist3); // 덧셈 연산
	sorting(&llist3); // 결과 Sorting
	merge(&llist3); // 결과 merge
	//print(&llist3); // 확인용
	//printf("length : %d\n", llist3.length);
	//printf("\n");

	// 곱셈 연산한 polynominal Linked List (L4 = L1 * L2)
	multiple(&llist1, &llist2, &llist4); // 곱셈 연산
	sorting(&llist4); // 결과 Sorting
	merge(&llist4); // 결과 merge
	//print(&llist4); // 확인용
	//printf("length : %d\n", llist4.length);
	//printf("\n");

	// 파일 내보내기
	FILE* output = fopen("output.txt", "w");
	// 덧셈 연산
	fprintf(output, "Addition\n");
	LLNode* o_add = llist3.head; // 안의 노드들 접근 위해 Node 선언
	while (o_add != NULL) {
		fprintf(output, "%d %d\n", o_add->coef, o_add->expon);
		o_add = o_add->link;
	}
	// 곱셈 연산
	fprintf(output, "Multiplication\n");
	LLNode* o_mul = llist4.head; // 안의 노드들 접근 위해 Node 선언
	while (o_mul != NULL) {
		fprintf(output, "%d %d", o_mul->coef, o_mul->expon);
		if (o_mul->link != NULL) fprintf(output, "\n");
		o_mul = o_mul->link;
	}

	/*printf("처리완료했습니다.");*/
	return 0;
}