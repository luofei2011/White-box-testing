#include<iostream>
#include<malloc.h>
#include<string>
#include<cstring>
#define MAX 512 //����������������
using namespace std;

char arcArr[MAX][MAX]={'\0'};
int vexArr[MAX]={0};
int basic_path[MAX];//�洢����·������ʵʱ���� 
int CC = 0; //Ȧ���Ӷȣ���1���ж������+1����2��e(��)-n(���)+2 
int header=0; //ͷ���λ�� 
char is_EXT = 'N';
string order_out = "";

typedef struct  arcNode{
        int tail,head;  //����β����ͷ����λ��
        struct arcNode *hlink, *tlink;      //��ͷ��β��ͬ�Ļ�����
        char judge;   //T/F/N        
}arcNode;

typedef struct vexNode{
        int data;
        arcNode *fin, *fout; //�ֱ�ָ��ö���ĵ�һ���뻡�ͳ���        
}vexNode;

typedef struct{
        vexNode list[MAX]; //��ͷ���������б�
        int vnum, anum; //����ͼ��ǰ�Ķ���ͻ���        
}Graph;

bool is_end(string str){
    // transform(str.begin(),str.end(), str.begin(), ::toupper);
     if(str == "END" || str == "end" || str == "EXT" || str == "ext")
            return true;
     return false;
}

int length(int num[]){
    int sum=0;
    for(int i=0; i<MAX; i++){
          if(num[i] != 0)
                sum++; 
          else
              break;
    }
    return sum;   
}

bool is_inArr(int ch[],int in){
     for(int i=0; i<length(ch); i++)
             if(in == ch[i])
                   return true;
     return false;
}

void insert(string str){
     int pos = length(vexArr);
     string _tail = str.substr(0,str.find('-'));
     string _head = str.substr(str.find('>')+1,str.find(',')-str.find('>')-1);
     char _judge = str[str.size()-1];
     if(!is_inArr(vexArr, atoi(_tail.c_str())))
          vexArr[pos++] = atoi(_tail.c_str());
     if(!is_inArr(vexArr, atoi(_head.c_str())))
          vexArr[pos++] = atoi(_head.c_str());
     int i = atoi(_tail.c_str())-1;
     int j = atoi(_head.c_str())-1;  
     arcArr[i][j] = _judge;          
}

int LocateVex(Graph *G, int vertex){
    int j=0,k;
    for(k=0; k<length(vexArr); k++)
             if(G->list[k].data == vertex){
                   j = k;
                   break;                   
             }    
    return j;
}

void CreateGraph(Graph *G){
     //��ʼ�������б� 
     int _tail, _head;
     arcNode *arcTemp;
     G->vnum = G->anum =0;
     for(int i=0; i<length(vexArr); i++){
             (*G).list[i].data = vexArr[i];
             G->list[i].fin = NULL;
             G->list[i].fout = NULL;  
             G->vnum++;  
     }     
     //��ʼ����������ʮ������
     int total_len = sizeof(arcArr)/sizeof(arcArr[0][0]);
     int line_len = sizeof(arcArr)/sizeof(arcArr[0]);
     for(int i=0; i<line_len; i++)
             for(int j=0; j<total_len/line_len; j++){
                     if(arcArr[i][j] == 'T')
                           CC ++;
                     if((arcArr[i][j] == 'T') || (arcArr[i][j] == 'N') || (arcArr[i][j] == 'F')){
                           _tail = LocateVex(G,i+1);
                           _head = LocateVex(G,j+1);
                           arcTemp = (arcNode*)malloc(sizeof(arcNode));
                           arcTemp->tail = _tail;
                           arcTemp->head = _head;
                           arcTemp->tlink = (*G).list[_tail].fout;
                           (*G).list[_tail].fout = arcTemp;
                           arcTemp->hlink = (*G).list[_head].fin;
                           (*G).list[_head].fin = arcTemp;
                           arcTemp->judge = arcArr[i][j]; 
                           G->anum++;                 
                     }        
             } 
}

void printCC(Graph G){
     //cout << "CC=" << CC+1 << endl; 
     cout << "CC=" << ((G.anum)-(G.vnum)+2) << endl;    
}

void display(int arr[]){
     char ch[10];
     for(int i=0; i<length(arr)-1; i++){
             itoa(arr[i],ch,10);
             order_out += ch;
             order_out += ",";
     }
     memset(ch,0,10);
     itoa(arr[length(arr)-1],ch,10);
     order_out += ch;
     //��","�ֿ�ÿ��·����㣻��"|"�ֿ�û������·�� 
     order_out += "|";   
}

int my_memset(int arr[],int _data){
     int pos = 0;
     int _length = length(arr);
     for(int i=0; i<_length; i++)
           if(_data == arr[i]){
                     pos = i;
                     break;
           }
     //����ʹ��i<length(arr)���arr��ʱ��length(arr)�ͻ�ı� 
     for(int i=pos+1; i<_length; i++)
             arr[i] = 0;  
     //display(arr); 
     return pos;  
}

//����ͼ����ȷǵݹ���� 
void DFSTraverse(Graph G, int pos){
     arcNode *p;
     int _pos = 0; //��¼��ͬ·���ķ���� 
     arcNode *Queue[MAX*MAX];
     int queue = -1;
     int pointer = -1;
     p = G.list[pos].fout;
     while(p){ 
           if(!is_inArr(basic_path,G.list[p->tail].data))
                  basic_path[++pointer] = G.list[p->tail].data;
           if((p->judge == 'T') || (p->judge == 'F')){ 
                  if(p->judge == 'T'){
                         Queue[++queue] = p;
                         p = p->tlink;
                  }
                  else if(p->judge == 'F')
                       Queue[++queue] = p->tlink;;     
           }
           //ͳһ��¼��ֵ 
           basic_path[++pointer] = G.list[p->head].data;
           pos = LocateVex(&G,G.list[p->head].data);
           p = G.list[pos].fout;
           if(p){
                 if(is_inArr(basic_path,G.list[p->head].data)){//bug
                        basic_path[++pointer] = G.list[p->head].data;
                        if(queue >= 0){ 
                                display(basic_path);         
                                p = Queue[queue];
                                queue --; 
                                pointer = my_memset(basic_path,G.list[p->tail].data);
                                pos = LocateVex(&G,G.list[p->head].data);
                                p = G.list[pos].fout; 
                                if(!p){
                                       if(is_inArr(basic_path,G.list[pos].data))
                                              basic_path[++pointer] = G.list[pos].data;
                                       display(basic_path);
                                }               
                        }
                        //����else�ᵼ����ѭ������Ϊ�����һ���ʱ������Ϊ�ա�ֱ�Ӱ�ͷ�����뵽ѭ�������� 
                        else{  
                             display(basic_path); 
                             break;
                        }                            
                 }
                 continue;
           }
           else{   
                if(queue >= 0){     
                      display(basic_path); 
                      p = Queue[queue];
                      queue --;  
                      pointer = my_memset(basic_path,G.list[p->tail].data);
                      //��ԭ����p->head�ĳ� G.list[p->head].data,�����߸����Ͳ���һ������
                      /*
                      �տ�ʼֻ���ǽ����һλ����ʱ��p->head��ֵ���������data�Ƕ�Ӧ�洢��
                      ���ǵ���ɶ�λ�����߽��˳��������ʱ��������򱻴��� 
                      */ 
                      pos = LocateVex(&G,G.list[p->head].data);
                      p = G.list[pos].fout;  
                      if(!p){
                            if(is_inArr(basic_path,G.list[pos].data))
                                 basic_path[++pointer] = G.list[pos].data;
                            display(basic_path);
                      }
                }
                else{
                     display(basic_path);
                     break;     
                }
           }
           
     }
}

int return_header(Graph *G,int num){
    int pos = LocateVex(G,num);
    return pos;    
}

//�����������·�� 
void orderDisplay(string str){
     string _str = str;
     string small = str;
     string temp = "";
     char is_end = 'N';
     int pos = str.find('|');
     while(str.size()){
     for(int i=0; i<_str.size(); i++){
           temp = _str.substr(0,pos);
           if(small.size() > temp.size())
                 small = temp;
           else if(small.size() == temp.size())
                 if(small.compare(temp) > 0)
                       small = temp; 
           if(is_end == 'N'){
                _str = _str.substr(pos+1);  
                pos = _str.find('|');
                if(pos != _str.npos)
                     continue;
                else{
                     pos = _str.size();
                     is_end = 'Y';     
                } 
           } 
           else
               break;   
     }
     cout << small << endl;
     int _pos = str.find(small);
     str.replace(_pos,small.size()+1,"");
     _str = small = str;
     pos = str.find('|');
     is_end = 'N';
     }       
}

void changeVexArr(int num){
     for(int i=0; i<length(vexArr); i++)
             if(vexArr[i] == num){
                   vexArr[i] = num*10 + 1;
                   vexArr[length(vexArr)] = num*10 + 2;
                   break;             
             }             
}

void CreatePath(string str){
     string _vertex = str.substr(0,str.find(','));
     string _condition = str.substr(str.find(',')+1);
     int _num = atoi(_vertex.c_str());
     int _T = 0,_F = 0,_in[MAX]={0};
     int l =0;
     changeVexArr(_num);  
     int total_len = sizeof(arcArr)/sizeof(arcArr[0][0]);
     int line_len = sizeof(arcArr)/sizeof(arcArr[0]); 
     
     for(int i=0; i<line_len; i++)
                   if(arcArr[i][_num-1] != '\0')
                          _in[l++] = i;                    
     for(int j=0; j<total_len/line_len; j++){
                   if(arcArr[_num-1][j] == 'F')
                        _F = j;
                   else if(arcArr[_num-1][j] == 'T') 
                        _T = j;                            
     }
     for(int i=0; i<l; i++){
                   arcArr[_in[i]][_num*10] = arcArr[_in[i]][_num-1];
                   arcArr[_in[i]][_num-1] = '\0';         
     }
     if(_condition == "AND"){         
           arcArr[_num*10][_F] = 'F';
           arcArr[_num*10][_num*10+1] = 'T';
           arcArr[_num*10+1][_T] = 'T';
           arcArr[_num*10+1][_F] = 'F';
     }
     else if(_condition == "OR"){
           arcArr[_num*10][_T] = 'T';
           arcArr[_num*10][_num*10+1] = 'F';
           arcArr[_num*10+1][_F] = 'F';
           arcArr[_num*10+1][_T] = 'T';
    
     }
     arcArr[_num-1][_F] = '\0';
     arcArr[_num-1][_T] = '\0';
}

int main(){
    Graph G;
    int i=0;
    string temp ="";
    string _head = "";
    int _header = 0;
    getline(cin,temp);
    for(int k=0; k<temp.size(); k++)
            if((temp[k] >= '0') && (temp[k] <= '9'))
                  _head += temp[k];
    vexArr[i++] = atoi(_head.c_str());
    _header = vexArr[0];
    while(1){
             getline(cin,temp);
             if(temp == "EXT" || temp == "ext")
                   is_EXT = 'Y';
             if(is_end(temp)) 
                   break;
             insert(temp);        
    }
    if(is_EXT == 'Y')
             while(1){
                      getline(cin,temp);
                      if(is_end(temp))
                            break;
                      CreatePath(temp);         
             }
    CreateGraph(&G);
    header = return_header(&G,_header);
    printCC(G);
    DFSTraverse(G,header);
    orderDisplay(order_out);
    //system("pause");
    return 0;   
}
