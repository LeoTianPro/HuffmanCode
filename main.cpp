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

int Input_data(); //输入字符集大小
void Inputstring(char [], int); //依次输入字符
int Inputdata(); //输入字符权值
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
    printf("\n\t\t\t\t\t********哈夫曼编/译码器********\t\t\t\t\n\n");
    printf("\t\t\t\t\tI：初始化(Initialization)\n");
    printf("\t\t\t\t\tE：编码(Encoding)\n");
    printf("\t\t\t\t\tD：译码(Decoding)\n");
    printf("\t\t\t\t\tP：印代码文件(Print)\n");
    printf("\t\t\t\t\tT：印哈夫曼树(Tree printing)\n");
    printf("\t\t\t\t\tQ：退出\n");
    printf("\n请输入您的选项：\n");
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
                    printf("输入有误，请重新输入（大写字母）!\n");
                else
                    printf("输入有误，请重新输入!\n");
        }
        system("pause");
        system("cls");
        your_choose = menu();
    }
    printf("\n********正在退出系统********\n");
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
        printf("请输入初始化方式:\n\n");
        printf("1:从文件\"建树信息.txt\"中读取所需数据\n");
        printf("2:手动输入建树所需数据\n");
        printf("0:返回主菜单\n");
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
                printf("\n文件已读取完毕！\n");
                fclose(fp);
            }
                break;
            case '2': {

                printf("\n\n请输入字符集大小:\n");
                n = Input_data();
                printf("\n请依次输入字符:\n");
                Inputstring(ch, n);
                for (i = 0; i < n; i++) {
                    printf("请输入\"%c\"的权值:\n", ch[i]);
                    f[i] = Inputdata();
                }

            }
                break;
            case '0':
                return NULL;
            default:
                printf("输入有误，请重新输入！\n");
        }
        system("pause");
    } while (your_choose != '1' & your_choose != '2');
//建叶子节点
    for (i = 0; i < n; i++) {
        h[i] = (Tree *) malloc(sizeof(Tree));
        if (h[i] == NULL) {
            printf("内存分配出错！\n");
            system("pause");
            exit(0);
        }
        h[i]->ch = ch[i];
        h[i]->power = f[i];
        h[i]->lchild = NULL;
        h[i]->rchild = NULL;
    }
//建树
    do {
        p = min_node(n, h);
        n--;
        q = min_node(n, h);
        n--;
        r = (Tree *) malloc(sizeof(Tree));
        if (r == NULL) {
            printf("内存分配出错!\n");
            return NULL;
        }
        r->lchild = p;
        r->rchild = q;
        r->power = p->power + q->power;
        r->ch = '\0';  //把新得到的节点的字符设为空字符
        h[n] = r;
        n++;
    } while (n > 1);
    root = h[0];
//写入文件hfmTree中
    FILE *fp;
    fp = fopen(hfmTree, "wt");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        return root;
    }
    save_node(root, fp);
    fclose(fp);
    printf("\n\n哈夫曼树已保存在文件\"hfmTree.txt\"中\n");
    return root;
}

Tree *min_node(int n, Tree *h[]) { //查找h[]中最小权值节点
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
//哈夫曼树若不存在，则从hfmTree.txt中读取
    if (root == NULL) {
        printf("\n内存中不存在哈夫曼树，正在从文件\"hfmTree.txt\"中读取哈夫曼树...\n");
        FILE *fp;
        fp = fopen(hfmTree, "rt");
        if (fp == NULL) {
            printf("\n文件不存在或打开失败!\n");
            return root;
        }
        root = readnode(fp);
        fclose(fp);
        if (root == NULL) {
            printf("文件\"hfmTree.txt\"中的树为空树!");
            return root;
        } else
            printf("\n哈夫曼树已从文件\"hfmTree.txt\"中读取完毕!\n");
    }
//进行哈夫曼编码
    char c[666];  //每个字符
    char code[666][666];  //每个字符的编码
    char num[666];
    int i = 0;
    int j = 0;
    int *ip = &i, *jp = &j;
    Coding_on(code, c, num, ip, jp, root);
    printf("\n哈夫曼编码如下：\n\n");
    for (i = 0; i < j; i++) {
        printf("%c:%s\n", c[i], code[i]);
    }
    system("pause");
//进行编码
    printf("\n正在对文件\"ToBeTran.txt\"文件中的正文进行编码...\n");
    Entobetran(code, c);
    return root;
}

void Decoding(Tree *root) {
    if (root == NULL) {
        printf("\n哈夫曼树不存在，请输入I(或E)以输入(或读取)哈夫曼树!\n");
        return;
    }
    char your_choose;
    char code[1024];
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    do {
        system("cls");//清屏
//菜单
        printf("请输入译码方式:\n\n");
        printf("1:对文件\"CodeFile.txt\"中的代码进行译码\n");
        printf("2:输入一串二进制编码进行译码\n");
        printf("0:返回主菜单\n");
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
                    printf("文件\"String.txt\"打开失败\n");
                    return;
                }
//读入数据
                printf("\n请输入字符串:\n");
                scanf("%s", code);
                fprintf(fp1, "%s", code);
                fclose(fp1);
                fp1 = fopen(String, "rt");
                if (fp1 == NULL) {
                    printf("文件\"String.txt\"不存在或打开失败\n");
                    return;
                }

            }
                break;
            case '0':
                return;
            default:
                printf("输入有误，请重新输入\n");
        }
        system("pause");
    } while (your_choose != '1' & your_choose != '2');
    fp2 = fopen(TextFile, "wt");
    if (fp2 == NULL) {
        printf("文件打开失败！\n");
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
            printf("\"CodeFile.txt\"文件中存在非法字符\n");
            fclose(fp1);
            fclose(fp2);
            return;
        }
        if (p->lchild == p->rchild) { //p->lchild==p->rchild，即p->lchild和p->rchild均为NULL
            fputc(p->ch, fp2);
            p = root;
        }
    }
    if (p != root) {
        printf("\"CodeFile.txt\"文件存在损坏或错误！\n");
        fclose(fp1);
        fclose(fp2);
        return;
    }
    printf("\n文件\"CodeFile.txt\"的译码结果已保存于文件\"TextFile.txt\"中\n");
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
            printf("输入有误，请重新输入！\n");
            goto intput_nextint;
        } else {
            x *= 10;
            x += (a[i] - '0');
        }
    }
    if (x > 666) {
        printf("字符集过大,请输入一个合理的数值！（比如666以内）\n");
        goto intput_nextint;
    }
    if (x < 2) {
        printf("字符集太小，请输入一个合理的数值！\n");
        goto intput_nextint;
    }
    return x;
}

void Inputstring(char ch[], int n) {
    int i, j;
    input_nextchar:
    gets(ch);
    if (strlen(ch) != n) {
        printf("输入字符数和字符集大小不匹配，请重新输入!\n");
        goto input_nextchar;
    }
    for (i = 0; i <= n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (ch[i] == ch[j]) {
                printf("输入的字符\"%c\"重复出现，请重新输入\n", ch[i]);
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
            printf("输入有误，请重新输入!\n");
            goto intput_nextdata;
        } else {
            y *= 10;
            y += (a[i] - '0');
        }
    }
    if (y > 666) {
        printf("输入的权值过大，请重新输入一个合理的权值（比如666以内）!\n");
        goto intput_nextdata;
    }
    if (y == 0) {
        printf("输入的权值无意义，请输入一个合理的权值!\n");
        goto intput_nextdata;
    }
    return y;
}

void save_node(Tree *parents, FILE *fp) { //以(左 右)的形式存储节点于hfmTree.txt中
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

Tree *readnode(FILE *fp) { //从hfmTree.txt中读取以(左 右)形式存储的节点
    char c;
    Tree *parents;
    parents = (Tree *) malloc(sizeof(Tree));
    if (parents == NULL) {
        printf("内存分配出错!\n");
        return NULL;
    }
    c = (char) fgetc(fp);
    if (c == '(') {
        parents->lchild = readnode(fp);
        parents->rchild = readnode(fp);
        parents->ch = '\0';
        c = (char) fgetc(fp);
        if (c != ')') {
            printf("文件\"hfmTree.txt\"的内容出错!\n");
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

void Coding_on(char code[][666], char c[], char num[], int *ip, int *jp, Tree *parents) { //进行编码
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

void Entobetran(char code[][666], char c[]) { //对ToBeTran.txt的正文进行编码,并保存于CodeFile.txt中
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    char your_choose;
    char codes[1024];
    do {
        system("cls");//清屏
//菜单
        printf("请输入编码方式:\n\n");
        printf("1:对文件\"ToBeTran.txt\"进行编码\n");
        printf("2:输入字符串进行编码\n");
        printf("0:返回主菜单\n");
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
                    printf("文件\"String.txt\"打开失败!\n");
                    return;
                }
//读入数据
                printf("\n请输入字符串:\n\n");
                scanf("%s", codes);
                fprintf(fp1, "%s", codes);
                fclose(fp1);
                fp1 = fopen(String, "rt");
                if (fp1 == NULL) {
                    printf("文件\"String.txt\"不存在或打开失败!\n");
                    return;
                }

            }
                break;
            case '0':
                return;
            default:
                printf("输入有误，请重新输入！\n");
        }
        system("pause");//暂停
    } while (your_choose != '1' & your_choose != '2');
    fp2 = fopen(CodeFile, "wt");
    if (fp2 == NULL) {
        printf("文件打开失败！\n");
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
            printf("存在无法匹配的字符！\n");
            fclose(fp1);
            fclose(fp2);
            return;
        }
    }
    printf("\n\n已将该字符形式的编码保存于文件\"CodeFile.txt\"中\n");
    fclose(fp1);
    fclose(fp2);
    return;
}

void Print() {
    FILE *fp1, *fp2;
    fp1 = fopen(CodeFile, "rt");
    if (fp1 == NULL) {
        printf("文件\"CodeFile.txt\"不存在或打开失败\n");
        return;
    }
    fp2 = fopen(CodePrin, "wt");
    if (fp2 == NULL) {
        printf("文件打开失败!\n");
        fclose(fp1);
        return;
    }
    char ch;
    int i = 0;
    printf("\n文件\"CodeFile.txt\"中的内容如下：\n\n");
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
            printf("\n文件\"CodeFile.txt\"中存在非法字符!\n");
            fclose(fp1);
            fclose(fp2);
            return;
        }
    }
    printf("\n\n文件\"CodeFile.txt\"中内容已保存于文件\"CodePrin.txt\"中\n");
    fclose(fp1);
    fclose(fp2);
    return;
}

void Printf_Huffman(Tree *root) { //印哈夫曼树
    if (root == NULL) {
        printf("\n内存中不存在哈夫曼树，正在从文件\"hfmTree.txt\"中读取哈夫曼树...\n");
        FILE *fp;
        fp = fopen(hfmTree, "rt");
        if (fp == NULL) {
            printf("文件不存在或打开失败!\n");
            return;
        }
        root = readnode(fp);
        fclose(fp);
        if (root == NULL) {
            printf("文件\"hfmTree.txt\"中的树为空树!");
            return;
        } else
            printf("\n哈夫曼树已从文件\"hfmTree.txt\"中读取完毕!\n");
    }
//递归调用，将哈夫曼树以凹入表示形式保存并输出
    printf("\n该哈夫曼树凹入表形式如下：\n\n");
    int i = 0;
    int *ip = &i;
    FILE *fp;
    fp = fopen(TreePrint, "wt");
    if (fp == NULL) {
        printf("文件打开失败!\n");
        return;
    }
    char num[666];
//lchild时为0,rchild时为1
    Aoru_print(ip, num, root, fp, 0);
    printf("\n该哈夫曼树已保存于文件\"TreePrint.txt\"中\n");
    fclose(fp);
    return;
}

void Aoru_print(int *ip, char num[], Tree *parents, FILE *fp, int point) { //递归调用，将哈夫曼树以凹入表示形式保存并输出
    if (parents->ch == '\0') {
//左
        num[*ip] = ' ';
        (*ip)++;
        num[*ip] = ' ';
        (*ip)++;
        Aoru_print(ip, num, parents->lchild, fp, 0);//lchild时为0,rchild时为1
//根
        num[(*ip) - 1] = '|';
        num[*ip] = '#';
        (*ip)++;
        num[*ip] = '|';
        (*ip)++;
//输出
        printf("%s\n", num);
        fputs(num, fp);
        fputc('\n', fp);
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
//lchild时为0,rchild时为1
        if (point == 0)
            num[(*ip) - 1] = '|';
        if (point == 1)
            num[(*ip) - 1] = ' ';
//右
        num[*ip] = ' ';
        (*ip)++;
        num[*ip] = '|';
        (*ip)++;
        Aoru_print(ip, num, parents->rchild, fp, 1);//lchild时为0,rchild时为1
//恢复
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
//输出
        printf("%s\n", num);
        fputs(num, fp);
        fputc('\n', fp);
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
//恢复
        num[*ip] = '\0';
        (*ip)--;
        num[*ip] = '\0';
        (*ip)--;
    }
}


FILE *file_open() {
    char name[1024];
    printf("\n请输入文件名(绝对路径):\n");
    scanf("%s", name);
    FILE *fp;
    if ((fp = fopen(name, "rt")) == 0) {
        printf("文件不存在或打开失败！\n");
        exit(-1);
    } else
        return fp;
}




