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

// ���� ����Ʈ ���
typedef struct ListNode {
	int coef; // ���
	int expon; // ����
	struct ListNode* link;
}LLNode;


// ���� ����Ʈ ���
typedef struct ListHeader {
	int length; // ���Ḯ��Ʈ ����
	LLNode* head; // ����
	LLNode* tail; // ����
}LLHeader;

// �ʱ�ȭ �Լ�
void init(LLHeader* plist)
{
	plist->length = 0;
	plist->head = plist->tail = NULL;
}

void insert(LLHeader* plist, int coef, int expon)
{
	LLNode* temp = (LLNode*)malloc(sizeof(LLNode)); // ���� ���� �ӽ� ���� ��� ���� Ȱ��
	if (temp == NULL) { // ���� ������ ���� �Ҵ��� �ȵǾ��ٸ� => ������ �߻�
		errno = "Memory Allocation Error";
		perror("Error Message"); 
	}
	temp->coef = coef; // �ӽ� ���� ����� coefficient ����
	temp->expon = expon; // �ӽ� ���� ����� exponential ����
	temp->link = NULL; // �������� �߰��ϴ� ����̹Ƿ� link�� NULL�� ����
	if (plist->tail == NULL) { // plist�� �ƹ� ��嵵 ���� ��
		plist->head = plist->tail = temp; // ù ��° ����̹Ƿ� ������ ������ temp�� �����Ѵ�.
	}
	else { // plist�� ������ ���� ��
		plist->tail->link = temp; // ��� �߰��ϱ� �� ������ ���(���� ���)�� link�� temp�� ����Ű���� ����
		plist->tail = temp; // ���� ����Ű�� ��带 temp�� ����(������ temp)�� ����
	}
	plist->length++; // ��尡 �ϳ� �߰��Ǿ����Ƿ� ���� 1�߰�

}

void add(LLHeader* plist1, LLHeader* plist2, LLHeader* plist3) { //polynominal ���� ����
	LLNode* a = plist1->head;
	LLNode* b = plist2->head;
	int sum;
	while (a && b) { // a�� b ��� ������ ���� ������ ����(Linked List �� ������ �� �� �ϳ��� ��� �������� ���� ���� �ݺ�)
		if (a->expon == b->expon) { // a�� ������ b�� ������ ���ٸ�
			sum = a->coef + b->coef; // a�� b�� coef�� ���� ���� �ӽ� parameter�� ����
			if (sum != 0) insert(plist3, sum, a->expon); // a�� b�� expon�� ���� ��� �ӽ� �����ߴ� coef�� ���� ������ Linked List�� ���� �߰��Ѵ�.
			a = a->link; b = b->link; // a�� b ��� ���(����)�����Ƿ� ���� ��带 ������ �� �ֵ��� �����Ѵ�.
		}
		else if (a->expon > b->expon) { // a�� ������ b�� �������� ũ�ٸ�
			insert(plist3, a->coef, a->expon); // a�� ������ ���� ���� Linked List�� �ѱ��.
			a = a->link; // a�� ���������Ƿ� a�� ���� ��带 ������ �� �ֵ��� �����Ѵ�.
		}
		else { // a�� ������ b�� �������� ������(a->expon < b->expon)
			insert(plist3, b->coef, b->expon); /// b�� ������ ���� ���� Linked List�� �ѱ��.
			b = b->link; // b�� ���������Ƿ� b�� ���� ��带 ������ �� �ֵ��� �����Ѵ�.
		}
	}
	// a�� b�� �ϳ��� ���� ������ ��
	// ���� ���׽� ����
	for (; a != NULL; a = a->link) {
		insert(plist3, a->coef, a->expon); // a�� ������ ��(b�� �� ����)
	}
	for (; b != NULL; b = b->link) {
		insert(plist3, b->coef, b->expon); // b�� ������ ��(a�� �� ����)
	}
}

void merge(LLHeader* mlist) // expon�� ���� ���� coef�� ���ϴ� �Լ�(Sorting �� ����)
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


void sorting(LLHeader *list) // list�� expon�� ���������� Sorting
{
	LLNode* p = list->head;
	LLNode* temp1 = (LLNode*)malloc(sizeof(LLNode));
	LLNode* temp2 = (LLNode*)malloc(sizeof(LLNode));
	int iter = list->length;
	int i, j;
	for (i = iter - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			if (j == 0) { // ù ��° ���̶��
				if (p->expon < p->link->expon) {
					temp1->link = p->link->link;
					p->link->link = p;
					p->link = temp1->link;
				}	
			}
			else { // �߰����̶��
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


void multiple(LLHeader* plist1, LLHeader* plist2, LLHeader* plist4) // polynominal ����
{
	LLNode* a = plist1->head; // list1�� �޴´�
	LLNode* b = plist2->head; // list2�� �޴´�

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



// ����� ���� �Ǿ����� Ȯ�� �ڵ�
void print(LLHeader* plist)
{
	LLNode* p = plist->head;
	for (; p; p = p->link) {
		printf("%d %d\n", p->coef, p->expon);
	}
}



int main()
{
	
	// ���Ḯ��Ʈ ����
	LLHeader llist1, llist2, llist3, llist4;

	// ���Ḯ��Ʈ �ʱ�ȭ
	init(&llist1);
	init(&llist2);
	init(&llist3);
	init(&llist4);
	
	// ���� �о����
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) { // ������ ������ NULL ��ȯ => ���� ó�� ����
		printf("No File Dectected. Closing the program.");
		return 0;
	}
	else {
		// �ؽ�Ʈ �� ���
		// �ӽ� �޸�
		char buffer[MAX_LENGTH];
		// NULL �Ǵ�(���׽� �и� �νĿ� Ȱ��)
		int check_nullity = 0;
		// �� ��
		//int line_count = 0;
		// ������ ����
		
		// ���Ͽ��� �ؽ�Ʈ �� �پ� �о����
		while (fgets(buffer, MAX_LENGTH, input) != NULL) { // �� �о�� NULL ��ȯ
			int b_coef = 0, b_expon = 0;
			//line_count++; // �� �� ����
			//printf("[%d]��° �� : %s", line_count, buffer); // Data Ȯ��
			//printf("Nullity : %d\n", check_nullity); // ���� �ν� Ȯ��

			
			if (buffer[0] == ' ') check_nullity++; // ���� �ν� �� ����� 1 ����

			if (buffer[0] == '0') continue; // input.txt���� coef�� �Ǽ��� 0�� �Է��� �� ����ó�� 
			
			// ���׽� ����
			if (check_nullity == 1 && buffer[0] != ' ') { // ù ��° ���׽� ����
				//printf("Check\n"); // Ȯ�� ������


				int i, j; // index ���� (i = coef�� / j = �������� ��(�ٿ���))

				for (i = 0; buffer[i] != ' '; ++i); // i-1�� coef�� �� �ε���
				for (j = 0; buffer[j] != '\0'; ++j); // j-3�� expon�� �� �ε���
				/*printf("i�� : %d, j�� : %d\n", i, j);
				printf("coef : %c, expon : %c\n", buffer[i - 1], buffer[j - 3]);*/
				char tcoef[100];
				char texpon[100];
				int k, f;
				for (k = 0; k < i; k++) { // ��� ���ڸ� ���ڿ��� ��ħ
					tcoef[k] = buffer[k];
					if (k == i-1) tcoef[k + 1] = '\0';
				}
				b_coef = atoi(tcoef); // ���ڿ��� ������ ��ȯ
				//printf("coef : %d\n", b_coef); // Ȯ��
				for (f = i + 1; f < j - 2; f++) { // ���� ���ڸ� ���ڿ��� ��ħ
					texpon[f - (i + 1)] = buffer[f];
					if (f == j - 3) texpon[f - i] = '\0';
				}
				b_expon = atoi(texpon); // ���ڿ��� ������ ��ȯ
				//printf("expon : %d\n", b_expon); // Ȯ��
				// llist1�� �߰�
				insert(&llist1, b_coef, b_expon);
			}
			if (check_nullity == 2 && buffer[0] != ' ') { // �� ��° ���׽� ����
				int i, j; // index ���� (i = coef�� / j = �������� ��(�ٿ���))

				for (i = 0; buffer[i] != ' '; ++i); // i-1�� coef�� �� �ε���
				for (j = 0; buffer[j] != '\0'; ++j); // j-3�� expon�� �� �ε���
				/*printf("i�� : %d, j�� : %d\n", i, j);
				printf("coef : %c, expon : %c\n", buffer[i - 1], buffer[j - 3]);*/
				char tcoef[100];
				char texpon[100];
				int k, f;
				for (k = 0; k < i; k++) { // ��� ���ڸ� ���ڿ��� ��ħ
					tcoef[k] = buffer[k];
					if (k == i - 1) tcoef[k + 1] = '\0';
				}
				b_coef = atoi(tcoef); // ���ڿ��� ������ ��ȯ
				//printf("coef : %d\n", b_coef); // Ȯ��
				for (f = i + 1; f < j - 2; f++) { // ���� ���ڸ� ���ڿ��� ��ħ
					texpon[f - (i + 1)] = buffer[f];
					if (f == j - 3) texpon[f - i] = '\0';
				}
				b_expon = atoi(texpon); // ���ڿ��� ������ ��ȯ
				//printf("expon : %d\n", b_expon); // Ȯ��
				// llist2�� �߰�
				insert(&llist2, b_coef, b_expon);
			}

		}
	}
	fclose(input);

	
	// ���� �� Sorting �ǽ�
	sorting(&llist1);
	sorting(&llist2);
	// ���� �� merge �ǽ�
	merge(&llist1);
	merge(&llist2);
	
	// ������ Ȯ��
	//print(&llist1); // Ȯ��
	/*printf("length : %d\n", llist1.length);
	printf("\n");*/
	//print(&llist2); // Ȯ��
	/*printf("length : %d\n", llist2.length);
	printf("\n");*/

	// ���� ������ polynominal Linked List (L3 = L1 + L2)
	add(&llist1, &llist2, &llist3); // ���� ����
	sorting(&llist3); // ��� Sorting
	merge(&llist3); // ��� merge
	//print(&llist3); // Ȯ�ο�
	//printf("length : %d\n", llist3.length);
	//printf("\n");

	// ���� ������ polynominal Linked List (L4 = L1 * L2)
	multiple(&llist1, &llist2, &llist4); // ���� ����
	sorting(&llist4); // ��� Sorting
	merge(&llist4); // ��� merge
	//print(&llist4); // Ȯ�ο�
	//printf("length : %d\n", llist4.length);
	//printf("\n");

	// ���� ��������
	FILE* output = fopen("output.txt", "w");
	// ���� ����
	fprintf(output, "Addition\n");
	LLNode* o_add = llist3.head; // ���� ���� ���� ���� Node ����
	while (o_add != NULL) {
		fprintf(output, "%d %d\n", o_add->coef, o_add->expon);
		o_add = o_add->link;
	}
	// ���� ����
	fprintf(output, "Multiplication\n");
	LLNode* o_mul = llist4.head; // ���� ���� ���� ���� Node ����
	while (o_mul != NULL) {
		fprintf(output, "%d %d", o_mul->coef, o_mul->expon);
		if (o_mul->link != NULL) fprintf(output, "\n");
		o_mul = o_mul->link;
	}

	/*printf("ó���Ϸ��߽��ϴ�.");*/
	return 0;
}