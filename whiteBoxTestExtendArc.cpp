#include<iostream>
#include<stdlib.h>
#include<malloc.h>
#include<string>
#include<cstring>
#define MAX 100 //允许输入的最大结点数
using namespace std;

//char arcArr[MAX][MAX]={'\0'};
int arcTail[MAX]={0};
int arcHead[MAX]={0};
char Judge[MAX]={'\0'};
int vexArr[MAX]={0};
int basic_path[MAX];//存储基本路径，并实时更新 
int CC = 0; //圈复杂度：（1）判定结点数+1；（2）e(边)-n(结点)+2 
char is_EXT = 'N';
string order_out = "";

typedef struct  arcNode{
        int tail,head;  //弧的尾结点和头结点的位置
        struct arcNode *hlink, *tlink;      //弧头或弧尾相同的弧链表
        char judge;   //T/F/N   
        bool is_go;  
}arcNode;

typedef struct vexNode{
        int data;
        bool visited;   
        arcNode *fin, *fout; //分别指向该顶点的第一条入弧和出弧        
}vexNode;

typedef struct{
        vexNode list[MAX]; //表头向量顶点列表
        int vnum, anum; //有向图当前的顶点和弧数        
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
     int i = length(arcTail);
     string _tail = str.substr(0,str.find('-'));
     string _head = str.substr(str.find('>')+1,str.find(',')-str.find('>')-1);
     char _judge = str[str.size()-1];
     if(!is_inArr(vexArr, atoi(_tail.c_str())))
          vexArr[pos++] = atoi(_tail.c_str());
     if(!is_inArr(vexArr, atoi(_head.c_str())))
          vexArr[pos++] = atoi(_head.c_str());
     arcTail[i] = atoi(_tail.c_str());
     arcHead[i] = atoi(_head.c_str());  
     Judge[i] = _judge;         
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
     //初始化顶点列表 
     int _tail, _head;
     arcNode *arcTemp;
     G->vnum = G->anum =0;
     for(int i=0; i<length(vexArr); i++){
             (*G).list[i].data = vexArr[i];
             G->list[i].fin = NULL;
             G->list[i].fout = NULL;  
             G->vnum++;  
             G->list[i].visited = false;
     }     
     //初始化弧并构建十字链表
     for(int i=0; i<length(arcTail); i++){
             if(Judge[i] == 'T')
                     CC ++;
             _tail = LocateVex(G,arcTail[i]);
             _head = LocateVex(G,arcHead[i]);
             arcTemp = (arcNode*)malloc(sizeof(arcNode));
             arcTemp->tail = _tail;
             arcTemp->head = _head;
             arcTemp->tlink = (*G).list[_tail].fout;
             (*G).list[_tail].fout = arcTemp;
             arcTemp->hlink = (*G).list[_head].fin;
             (*G).list[_head].fin = arcTemp;
             arcTemp->judge = Judge[i]; 
             arcTemp->is_go = false;
             G->anum++;                       
     } 
}


void display(Graph *G,int arr[]){
     char ch[10];
     int temp =0;
     char is_no = 'N';
    // for(int i=0; i<length(arr)-1; i++){
     //        if((!G->list[arr[i]].fout->is_go) && (G->list[arr[i]].fout->tail == G->list[arr[i+1]].data))        
    //                is_no = 'Y';
    // }
     for(int i=0; i<length(arr)-1; i++){
             string Arc = "";
             temp = arr[i];
             snprintf(ch,sizeof(ch),"%d",temp);
             Arc += ch;
             Arc += ",";
             temp = arr[i+1];
             snprintf(ch,sizeof(ch),"%d",temp);
             Arc += ch;
             if(order_out.find(Arc) != order_out.npos)
                      // is_no = 'Y';  
                      continue;
             else{
                 is_no = 'Y';
                 break;
             } 
             i += 1;     
     }
     if(is_no == 'Y'){
     for(int i=0; i<length(arr)-1; i++){
             temp = arr[i];
             //itoa(arr[i],ch,10);
             snprintf(ch,sizeof(ch),"%d",temp);
             order_out += ch;
             order_out += ",";
     }
     memset(ch,0,10);
     //itoa(arr[length(arr)-1],ch,10);
     snprintf(ch,sizeof(ch),"%d",arr[length(arr)-1]);
     order_out += ch;
     //以","分开每个路径结点；以"|"分开没条基本路径 
     order_out += "|"; 
     }
}

int my_memset(int arr[],int _data){
    // CC++;
     int pos = 0;
     int _length = length(arr);
     for(int i=0; i<_length; i++)
           if(_data == arr[i]){
                     pos = i;
                     break;
           }
     //不能使用i<length(arr)清空arr的时候length(arr)就会改变 
     for(int i=pos+1; i<_length; i++)
             arr[i] = 0;  
     //display(arr); 
     return pos;  
}

//有向图的深度非递归遍历 
void DFSTraverse(Graph G, int pos){
     arcNode *p;
    // int _pos = 0; //记录不同路径的分离点 
     arcNode *Queue[MAX*MAX];
     int _len_=0;
     int queue = -1;
     int pointer = -1;
     p = G.list[pos].fout;
     basic_path[++pointer] = G.list[pos].data;
     if(!p){
           display(&G,basic_path);
           return;
     }
     while(p){ 
         //  p->is_go = true;
           if(!is_inArr(basic_path,G.list[p->tail].data))
                  basic_path[++pointer] = G.list[p->tail].data;
           if((p->judge == 'T') || (p->judge == 'F')){ 
                  if((p->judge == 'T') && (!G.list[p->tail].visited)){
                         Queue[++queue] = p;
                         p = p->tlink;
                  }
                  else if(p->judge == 'F')
                       Queue[++queue] = p->tlink; 
                  G.list[p->tail].visited = true; 
           }
           //统一记录下值 
           if(is_inArr(basic_path,G.list[p->head].data)){
                  basic_path[++pointer] = G.list[p->head].data;  
                  if(queue >= 0){ 
                                display(&G,basic_path);         
                                p = Queue[queue];
                                queue --; 
                                pointer = my_memset(basic_path,G.list[p->tail].data);     
                  }
                  //不加else会导致死循环，因为到最后一层的时候数组为空。直接把头结点加入到循环队列中 
                  else{  
                         display(&G,basic_path); 
                         break;
                  } 
                  continue;                                            
           }
           basic_path[++pointer] = G.list[p->head].data;
           pos = LocateVex(&G,G.list[p->head].data);
           p = G.list[pos].fout;
           if(!p){
                if(queue >= 0){     
                      display(&G,basic_path); 
                      p = Queue[queue];
                      queue --;  
                      pointer = my_memset(basic_path,G.list[p->tail].data);
                }
                else{
                     display(&G,basic_path);
                     break;     
                }
           }
           
     }
}

void showchar(string str){
     for(int i=0; i<str.size(); i++)
             cout << str[i];
     cout << endl;     
}

//按序输出基本路径 
/*
void orderDisplay(string str){
    // cout << str << endl;
     cout << "CC=" << CC+1 << endl;
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
     //cout << small << endl;
     showchar(small);
     int _pos = str.find(small);
     str.replace(_pos,small.size()+1,"");
     _str = small = str;
     pos = str.find('|');
     is_end = 'N';
     }       
}
*/

int returnNum(string str){
    int sum =0;
    while(str.size()){
          int pos = str.find(',');
          if(pos != str.npos){
               str = str.substr(pos+1);
               sum++; 
          }
          else 
               break;          
    }
    return sum+1;
        
}

bool is_contain(string big,string small){
    for(int i=0; i<small.size()-2; i++){
            string Arc = "";
            Arc += small[i];
            Arc += small[i+1];
            Arc += small[i+2];
            if(big.find(Arc) != big.npos)
                             continue;
            else
                return false; 
            i+=2;       
    }
    return true;
}

void remove_(string str){
     int pos = order_out.find(str);
     order_out.replace(pos,str.size()+1,"");     
}

void out(){
     string str_1 = order_out;
     string str_2 = order_out;
     int pos_1 = str_1.find('|');
     string temp = str_1;
     while(str_1.size()){
              temp = str_1.substr(0,pos_1);
              int _pos = str_1.find(temp);
              str_2.replace(_pos,temp.size()+1,"");  
              string cmp = str_2;
              int pos_2 = str_2.find('|');
              while(str_2.size()){
                     cmp = str_2.substr(0,pos_2); 
                     if(cmp.size() < temp.size()){
                             if(is_contain(temp,cmp))
                                     remove_(cmp);      
                     }
                     else if (cmp.size() > temp.size()){
                             if(is_contain(cmp,temp))
                                     remove_(temp);
                     }
                     str_2 = str_2.substr(pos_2+1);
                     pos_2 = str_2.find('|');                 
              }         
              str_1 = str_1.substr(pos_1+1);
              pos_1 = str_1.find('|');        
     }     
}

void orderDisplay(string str){
    // out();
     //cout << str << endl;
     cout << "CC=" << CC+1 << endl;
     string _str = str;
     string small = str;
     int small_num = 100;
     string temp = "";
     char is_end = 'N';
     int pos = str.find('|');
     while(str.size()){
     for(int i=0; i<_str.size(); i++){
           temp = _str.substr(0,pos);
           if(returnNum(temp) < small_num){
              //   if(!is_contain(temp)){
                        small = temp;
                        small_num = returnNum(temp);
                // }
           }
           else if(returnNum(temp) == small_num)
                 if(small.compare(temp) > 0){
                    //   if(!is_contain(temp)){
                              small = temp; 
                              small_num = returnNum(temp);
                     //  }
                 }
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
     //cout << small << endl;
     showchar(small);
    // CC++;
     int _pos = str.find(small);
     str.replace(_pos,small.size()+1,"");
     _str = small = str;
     small_num =100;
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
     int l = length(arcTail);
     int l_l =0;
     changeVexArr(_num);  
     
     for(int i=0; i<l; i++)
                   if(arcHead[i] == _num)
                          _in[l_l++] = i;                    
     for(int j=0; j<l; j++){
                   if(Judge[j] == 'F')
                        _F = j;
                   else if(Judge[j] == 'T') 
                        _T = j;                            
     }
     for(int i=0; i<l_l; i++)
                   arcHead[_in[i]] = _num*10+1;
     if(_condition == "AND"){   
           int _into = length(arcTail);
           arcTail[_F] = _num*10+1;   
           arcTail[_into] = _num*10+1;   
           arcHead[_into] = _num*10+2;
           Judge[_into] = 'T';
           _into ++;
           arcTail[_T] = _num*10+2;
           arcTail[_into] = _num*10+2;
           arcHead[_into] = arcHead[_F];
           Judge[_into] = 'F';
     }
     else if(_condition == "OR"){
           int _into = length(arcTail);
           arcTail[_T] = _num*10+1;
           arcTail[_into] = _num*10+1;
           arcHead[_into] = _num*10+2;
           Judge[_into] = 'F';
           _into++;
           arcTail[_F] = _num*10+2;
           arcTail[_into] = _num*10+2;
           arcHead[_into] = arcHead[_T];
           Judge[_into] = 'T';
     }
}

int main(){
    Graph G;
    int i=0;
    string temp ="";
    string _head = "";
    int _header = 0;
    getline(cin,temp);
    if(temp == "END")
            return 0;
    for(int k=0; k<temp.size(); k++)
            if((temp[k] >= '0') && (temp[k] <= '9'))
                  _head += temp[k];
    vexArr[i++] = atoi(_head.c_str());
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
             /*
    for(int i=0;i<length(vexArr);i++)
            cout << vexArr[i] << endl;
    for(int i=0; i<length(arcTail);i++)
    {
            cout << arcTail[i] << "->";
            cout << arcHead[i] << ",";
            cout << Judge[i] << endl;        
    }
    */
    CreateGraph(&G);
    DFSTraverse(G,LocateVex(&G,vexArr[0]));
    orderDisplay(order_out);
    system("pause");
    return 0;   
}
