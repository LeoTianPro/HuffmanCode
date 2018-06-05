#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define hfmTree   "hfmTree.txt"
#define ToBeTran  "ToBeTran.txt"
#define CodeFile  "CodeFile.txt"
#define TextFile  "TextFile.txt"
#define CodePrin  "CodePrin.txt"
#define TreePrint "TreePrint.txt"
#define String    "String.txt"

struct Tree {
    char ch;
    int power;
    Tree *lchild;
    Tree *rchild;
};

char menu();

Tree *Initialization();

Tree *min_node(int n, Tree *h[]);

Tree *Encoding(Tree *root);

void Decoding(Tree *root);

int Input_data(); //�����ַ�����С
void Inputstring(char [], int); //���������ַ�
int Inputdata(); //�����ַ�Ȩֵ
void save_node(Tree *parents, FILE *fp);

Tree *readnode(FILE *fp);

void Coding_on(char code[][666], char c[], char num[], int *ip, int *jp, Tree *parents);

void Entobetran(char code[][666], char c[]);

void Print();

void Printf_Huffman(Tree *root);

void Aoru_print(int *ip, char num[], Tree *parents, FILE *fp, int point);

FILE *file_open();

char menu() {
    char fun;
    printf("\n\t\t\t\t\t********��������/������********\t\t\t\t\n\n");
    printf("\t\t\t\t\tI����ʼ��(Initialization)\n");
    printf("\t\t\t\t\tE������(Encoding)\n");
    printf("\t\t\t\t\tD������(Decoding)\n");
    printf("\t\t\t\t\tP��ӡ�����ļ�(Print)\n");
    printf("\t\t\t\t\tT��ӡ��������(Tree printing)\n");
    printf("\t\t\t\t\tQ���˳�\n");
    printf("\n����������ѡ�\n");
    fflush(stdin);
    scanf("%c", &fun);
    fflush(stdin);
    return fun;
}

int main() {
    char your_choose;
    Tree *root = NULL;
    your_choose = menu();
    while (your_choose != 'Q') {
        switch (your_choose) {
            case 'I':
                root = Initialization();
                break;
            case 'E':
                root = Encoding(root);
                break;
            case 'D':
                Decoding(root);
                break;
            case 'P':
                Print();
                break;
            case 'T':
                Printf_Huffman(root);
                break;
            default:
                if (your_choose >= 'a' && your_choose <= 'z')
                    printf("�����������������루��д��ĸ��!\n");
                else
                    printf("������������������!\n");
        }
        system("pause");
        system("cls");
        your_choose = menu();
    }
    printf("\n********�����˳�ϵͳ********\n");
    exit(0);
    return 0;
}

Tree *Initialization() {
    int i;
    int n = 0;
    Tree *h[666];
    char ch[666];
    int f[666];
    Tree *p, *q, *r, *root;
    char your_choose;
    do {
        system("cls");
        printf("�������ʼ����ʽ:\n\n");
        printf("1:���ļ�\"������Ϣ.txt\"�ж�ȡ��������\n");
        printf("2:�ֶ����뽨����������\n");
        printf("0:�������˵�\n");
        scanf("%c", &your_choose);
        fflush(stdin);
        switch (your_choose) {
            case '1': {
                FILE *fp;
                fp = file_open();
                if (fp == NULL)
                    return NULL;
                fscanf(fp, "%d", &n);
                for (i = 0; i < n; i++) {
                    fscanf(fp, "%c", &ch[i]);
                }
                for (i = 0; i < n; i++) {
                    fscanf(fp, "%d", &f[i]);
                }
                printf("\n�ļ��Ѷ�ȡ��ϣ�\n");
                fclose(fp);
            }
                break;
            case '2': {

                printf("\n\n�������ַ�����С:\n");
                n = Input_data();
                printf("\n�����������ַ�:\n");
                Inputstring(ch, n);
                for (i = 0; i < n; i++) {
                    printf("������\"%c\"��Ȩֵ:\n", ch[i]);
                    f[i] = Inputdata();
                }

            }
                break;
            case '0':
                return NULL;
            default:
                printf("�����������������룡\n");
        }
        system("pause");
    } while (your_choose != '1' & your_choose != '2');
//��Ҷ�ӽڵ�
    for (i = 0; i < n; i++) {
        h[i] = (Tree *) malloc(sizeof(Tree));
        if (h[i] == NULL) {
            printf("�ڴ�������\n");
            system("pause");
            exit(0);
        }
        h[i]->ch = ch[i];
        h[i]->power = f[i];
        h[i]->lchild = NULL;
        h[i]->rchild = NULL;
    }
//����
    do {
        p = min_node(n, h);
        n--;
        q = min_node(n, h);
        n--;
        r = (Tree *) malloc(sizeof(Tree));
        if (r == NULL) {
            printf("�ڴ�������!\n");
            return NULL;
        }
        r->lchild = p;
        r->rchild = q;
        r->power = p->power + q->power;
        r->ch = '\0';  //���µõ��Ľڵ���ַ���Ϊ���ַ�
        h[n] = r;
        n++;
    } while (n > 1);
    root = h[0];
//д���ļ�hfmTree��
    FILE *fp;
    fp = fopen(hfmTree, "wt");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        return root;
    }
    save_node(root, fp);
    fclose(fp);
    printf("\n\n���������ѱ������ļ�\"hfmTree.txt\"��\n");
    return root;
}

Tree *min_node(int n, Tree *h[]) { //����h[]����СȨֵ�ڵ�
    int i;
    Tree *p = h[0];
    int m = 0;

    for (i = 1; i < n; i++) {
        if (p->power > h[i]->power) {
            p = h[i];
            m = i;
        }
    }
    h[m] = h[n - 1];
    return p;
}

Tree *Encoding(Tree *root) {
//���������������ڣ����hfmTree.txt�ж�ȡ
    if (root == NULL) {
        printf("\n�ڴ��в����ڹ������������ڴ��ļ�\"hfmTree.txt\"�ж�ȡ��������...\n");
        FILE *fp;
        fp = fopen(hfmTree, "rt");
        if (fp == NULL) {
            printf("\n�ļ������ڻ��ʧ��!\n");
            return root;
        }
        root = readnode(fp);
        fclose(fp);
        if (root == NULL) {
            printf("�ļ�\"hfmTree.txt\"�е���Ϊ����!");
            return root;
        } else
            printf("\n���������Ѵ��ļ�\"hfmTree.txt\"�ж�ȡ���!\n");
    }
//���й���������
    char c[666];  //ÿ���ַ�
    char code[666][666];  //ÿ���ַ��ı���
    char num[666];
    int i = 0;
    int j = 0;
    int *ip = &i, *jp = &j;
    Coding_on(code, c, num, ip, jp, root);
    printf("\n�������������£�\n\n");
    for (i = 0; i < j; i++) {
        printf("%c:%s\n", c[i], code[i]);
    }
    system("pause");
//���б���
    printf("\n���ڶ��ļ�\"ToBeTran.txt\"�ļ��е����Ľ��б���...\n");
    Entobetran(code, c);
    return root;
}

void Decoding(Tree *root) {
    if (root == NULL) {
        printf("\n�������������ڣ�������I(��E)������(���ȡ)��������!\n");
        return;
    }
    char your_choose;
    char code[1024];
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    do {
        system("cls");//����
//�˵�
        printf("���������뷽ʽ:\n\n");
        printf("1:���ļ�\"CodeFile.txt\"�еĴ����������\n");
        printf("2:����һ�������Ʊ����������\n");
        printf("0:�������˵�\n");
        scanf("%c", &your_choose);
        fflush(stdin);
        switch (your_choose) {
            case '1': {
                fp1 = file_open();
                if (fp1 == NULL)
                    return;
            }
                break;
            case '2': {
                fp1 = fopen(String, "wt");
                if (fp1 == NULL) {
                    printf("�ļ�\"String.txt\"��ʧ��\n");
                    return;
                }
//��������
                printf("\n�������ַ���:\n");
                scanf("%s", code);
                fprintf(fp1, "%s", code);
                fclose(fp1);
                fp1 = fopen(String, "rt");
                if (fp1 == NULL) {
                    printf("�ļ�\"String.txt\"�����ڻ��ʧ��\n");
                    return;
                }

            }
                break;
            case '0':
                return;
            default:
                printf("������������������\n");
        }
        system("pause");
    } while (your_choose != '1' & your_choose != '2');
    fp2 = fopen(TextFile, "wt");
    if (fp2 == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        fclose(fp1);
        return;
    }
    char ch;
    Tree *p = root;
    while ((ch = (char) fgetc(fp1)) != EOF) {
        if (ch == '0') {
            p = p->lchild;
        } else if (ch == '1') {
            p = p->rchild;
        } else {
            printf("\"CodeFile.txt\"�ļ��д��ڷǷ��ַ�\n");
            fclose(fp1);
            fclose(fp2);
            return;
        }
        if (p->lchild == p->rchild) { //p->lchild==p->rchild����p->lchild��p->rchild��ΪNULL
            fputc(p->ch, fp2);
            p = root;
        }
    }
    if (p != root) {
        printf("\"CodeFile.txt\"�ļ������𻵻����\n");
        fclose(fp1);
        fclose(fp2);
        return;
    }
    printf("\n�ļ�\"CodeFile.txt\"���������ѱ������ļ�\"TextFile.txt\"��\n");
    fclose(fp1);
    fclose(fp2);
    return;
}


int Input_data() {
    char a[200];
    intput_nextint:
    int x = 0;
    scanf("%s", a);
    fflush(stdin);
    for (int i = 0; strlen(a) > i; i++) {
        if (a[i] > '9' || a[i] < '0') {
            printf("�����������������룡\n");
            goto intput_nextint;
        } else {
            x *= 10;
            x += (a[i] - '0');
        }
    }
    if (x > 666) {
        printf("�ַ�������,������һ���������ֵ��������666���ڣ�\n");
        goto intput_nextint;
    }
    if (x < 2) {
        printf("�ַ���̫С��������һ���������ֵ��\n");
        goto intput_nextint;
    }
    return x;
}

void Inputstring(char ch[], int n) {
    int i, j;
    input_nextchar:
    gets(ch);
    if (strlen(ch) != n) {
        printf("�����ַ������ַ�����С��ƥ�䣬����������!\n");
        goto input_nextchar;
    }
    for (i = 0; i <= n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (ch[i] == ch[j]) {
                printf("������ַ�\"%c\"�ظ����֣�����������\n", ch[i]);
                goto input_nextchar;
            }
        }
    }
}

int Inputdata() {
    char a[200];
    intput_nextdata:
    int y = 0;
    scanf("%s", a);
    getchar();
    for (int i = 0; strlen(a) > i; i++) {
        if (a[i] > '9' || a[i] < '0') {
            printf("������������������!\n");
            goto intput_nextdata;
        } else {
            y *= 10;
            y += (a[i] - '0');
        }
    }
    if (y > 666) {
        printf("�����Ȩֵ��������������һ�������Ȩֵ������666���ڣ�!\n");
        goto intput_nextdata;
    }
    if (y == 0) {
        printf("�����Ȩֵ�����壬������һ�������Ȩֵ!\n");
        goto intput_nextdata;
    }
    return y;
}

void save_node(Tree *parents, FILE *fp) { //��(�� ��)����ʽ�洢�ڵ���hfmTree.txt��
    if (parents->ch == '\0')
        fputc('(', fp);
    if (parents->lchild == NULL && parents->rchild == NULL)
        fputc(parents->ch, fp);
    else {
        if (parents->lchild != NULL)
            save_node(parents->lchild, fp);
        if (parents->rchild != NULL)
            save_node(parents->rchild, fp);
    }
    if (parents->ch == '\0')
        fputc(')', fp);
}

Tree *readnode(FILE *fp) { //��hfmTree.txt�ж�ȡ��(�� ��)��ʽ�洢�Ľڵ�
    char c;
    Tree *parents;
    parents = (Tree *) malloc(sizeof(Tree));
    if (parents == NULL) {
        printf("�ڴ�������!\n");
        return NULL;
    }
    c = (char) fgetc(fp);
    if (c == '(') {
        parents->lchild = readnode(fp);
        parents->rchild = readnode(fp);
        parents->ch = '\0';
        c = (char) fgetc(fp);
        if (c != ')') {
            printf("�ļ�\"hfmTree.txt\"�����ݳ���!\n");
            fclose(fp);
            return NULL;
        }
    } else {
        parents->ch = c;
        parents->lchild = NULL;
        parents->rchild = NULL;
    }
    return parents;
}

void Coding_on(char code[][666], char c[], char num[], int *ip, int *jp, Tree *parents) { //���б���
    if (parents->ch == '\0') {
        num[*ip] = '0';
        (*ip)++;
        Coding_on(code, c, num, ip, jp, parents->lchild);
        num[*ip] = '1';
        (*ip)++;
        Coding_on(code, c, num, ip, jp, parents->rchild);
        num[*ip] = '\0';
        (*ip)--;
    } else {
        c[*jp] = parents->ch;
        strcpy(code[*jp], num);
        (*jp)++;
        num[*ip] = '\0';
        (*ip)--;
    }
}

void Entobetran(char code[][666], char c[]) { //��ToBeTran.txt�����Ľ��б���,��������CodeFile.txt��
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    char your_choose;
    char codes[1024];
    do {
        system("cls");//����
//�˵�
        printf("��������뷽ʽ:\n\n");
        printf("1:���ļ�\"ToBeTran.txt\"���б���\n");
        printf("2:�����ַ������б���\n");
        printf("0:�������˵�\n");
        scanf("%c", &your_choose);
        fflush(stdin);
        switch (your_choose) {
            case '1': {
                fp1 = file_open();
                if (fp1 == NULL)
                    return;
            }
                break;
            case '2': {
                fp1 = fopen(String, "wt");
                if (fp1 == NULL) {
                    printf("�ļ�\"String.txt\"��ʧ��!\n");
                    return;
                }
//��������
                printf("\n�������ַ���:\n\n");
                scanf("%s", codes);
                fprintf(fp1, "%s", codes);
                fclose(fp1);
                fp1 = fopen(String, "rt");
                if (fp1 == NULL) {
                    printf("�ļ�\"String.txt\"�����ڻ��ʧ��!\n");
                    return;
                }

            }
                break;
            case '0':
                return;
            default:
                printf("�����������������룡\n");
        }
        system("pause");//��ͣ
    } while (your_choose != '1' & your_choose != '2');
    fp2 = fopen(CodeFile, "wt");
    if (fp2 == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        fclose(fp1);
        return;
    }
    int n = strlen(c);
    int i;
    char ch;
    while ((ch = (char) fgetc(fp1)) != EOF) {
        for (i = 0; i < n; i++) {
            if (ch == c[i]) {
                fputs(code[i], fp2);
                break;
            }
        }
        if (i == n) {
            printf("�����޷�ƥ����ַ���\n");
            fclose(fp1);
            fclose(fp2);
            return;
        }
    }
    printf("\n\n�ѽ����ַ���ʽ�ı��뱣�����ļ�\"CodeFile.txt\"��\n");
    fclose(fp1);
    fclose(fp2);
    return;
}

void Print() {
    FILE *fp1, *fp2;
    fp1 = fopen(CodeFile, "rt");
    if (fp1 == NULL) {
        printf("�ļ�\"CodeFile.txt\"�����ڻ��ʧ��\n");
        return;
    }
    fp2 = fopen(CodePrin, "wt");
    if (fp2 == NULL) {
        printf("�ļ���ʧ��!\n");
        fclose(fp1);
        return;
    }
    char ch;
    int i = 0;
    printf("\n�ļ�\"CodeFile.txt\"�е��������£�\n\n");
    while ((ch = (char) fgetc(fp1)) != EOF) {
        if (ch == '0' || ch == '1') {
            fputc(ch, fp2);
            printf("%c", ch);
            i++;
            if (i % 50 == 0) {
                fputc('\n', fp2);
                printf("\n");
            }
        } else {
            printf("\n�ļ�\"CodeFile.txt\"�д��ڷǷ��ַ�!\n");
            fclose(fp1);
            fclose(fp2);
            return;
        }
    }
    printf("\n\n�ļ�\"CodeFile.txt\"�������ѱ������ļ�\"CodePrin.txt\"��\n");
    fclose(fp1);
    fclose(fp2);
    return;
}

void Printf_Huffman(Tree *root) { //ӡ��������
    if (root == NULL) {
        printf("\n�ڴ��в����ڹ������������ڴ��ļ�\"hfmTree.txt\"�ж�ȡ��������...\n");
        FILE *fp;
        fp = fopen(hfmTree, "rt");
        if (fp == NULL) {
            printf("�ļ������ڻ��ʧ��!\n");
            return;
        }
        root = readnode(fp);
        fclose(fp);
        if (root == NULL) {
            printf("�ļ�\"hfmTree.txt\"�е���Ϊ����!");
            return;
        } else
            printf("\n���������Ѵ��ļ�\"hfmTree.txt\"�ж�ȡ���!\n");
    }
//�ݹ���ã������������԰����ʾ��ʽ���沢���
    printf("\n�ù��������������ʽ���£�\n\n");
    int i = 0;
    int *ip = &i;
    FILE *fp;
    fp = fopen(TreePrint, "wt");
    if (fp == NULL) {
        printf("�ļ���ʧ��!\n");
        return;
    }
    char num[666];
//lchildʱΪ0,rchildʱΪ1
    Aoru_print(ip, num, root, fp, 0);
    printf("\n�ù��������ѱ������ļ�\"TreePrint.txt\"��\n");
    fclose(fp);
    return;
}

void Aoru_print(int *ip, char num[], Tree *parents, FILE *fp, int point) { //�ݹ���ã������������԰����ʾ��ʽ���沢���
    if (parents->ch == '\0') {
//��
        num[*ip] = ' ';
        (*ip)++;
        num[*ip] = ' ';
        (*ip)++;
        Aoru_print(ip, num, parents->lchild, fp, 0);//lchildʱΪ0,rchildʱΪ1
//��
        num[(*ip) - 1] = '|';
        num[*ip] = '#';
        (*ip)++;
        num[*ip] = '|';
        (*ip)++;
//���
        printf("%s\n", num);
        fputs(num, fp);
        fputc('\n', fp);
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
//lchildʱΪ0,rchildʱΪ1
        if (point == 0)
            num[(*ip) - 1] = '|';
        if (point == 1)
            num[(*ip) - 1] = ' ';
//��
        num[*ip] = ' ';
        (*ip)++;
        num[*ip] = '|';
        (*ip)++;
        Aoru_print(ip, num, parents->rchild, fp, 1);//lchildʱΪ0,rchildʱΪ1
//�ָ�
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
    } else {
        num[(*ip) - 1] = '|';
        num[*ip] = parents->ch;
        (*ip)++;
        num[*ip] = '\0';
        (*ip)++;
//���
        printf("%s\n", num);
        fputs(num, fp);
        fputc('\n', fp);
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
//�ָ�
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
    }
}


FILE *file_open() {
    char name[1024];
    printf("\n�������ļ���(����·��):\n");
    scanf("%s", name);
    FILE *fp;
    if ((fp = fopen(name, "rt")) == 0) {
        printf("�ļ������ڻ��ʧ�ܣ�\n");
        exit(-1);
    } else
        return fp;
}




