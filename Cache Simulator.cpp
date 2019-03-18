#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <math.h>
#include <random>
#include <time.h>
#include <windows.h>

#define SHOW_BLOCK 0
#define SHOW_HIT_MiSS 0
#define NEW_TRACE 0
#define hline 44


using namespace std;
/*Global Variables*/
int TAG = 7;
int INDEX = 7;
int OFFSET = 4;
int Main_Memory=256;
int Cache_Memory=128;
int Block_size=1;
string Mapping="Direct";

void print_horizontal_line(int x,char type){
	for(int i=0; i<x; i++){
		cout<<type;
	}
	cout<<endl;
}
/*Conversion Functions*/
int stoi(string exp)
{
     int num=0;
    int k=10;
    int temp;
    int len=exp.length();
    /*if(exp[0]>'0')
    {
     num=exp[0]-'0';
    }*/
    for(int i=0;i<len;i++)
    {
     temp=exp[i]-'0';
     num=num*k+ temp;
    }
    return num;
}

string itos( int num)
{
    string exp="";
    int rem;
    char temp;
    while(num>0)
    {
        rem=num%10;
        temp=(rem+'0');
        exp=temp+exp;
        num/=10;
    }
    return exp;
}

int btoi(string bin)
{
    int dec=0;
    int n=bin.length();
    for(int i=0;i<n;i++)
    {
        if(bin[i]=='1')
        {
        dec+= pow(2,n-1-i);
        }
    }
    return dec;
}
string dec_to_bin(unsigned int dec)
{
    int rem;
    string bin="";
    char temp;
    while(dec>0)
    {
        rem=dec%2;
        temp=(rem+'0');
        bin=temp+bin;
        dec/=2;

    }
    return bin;
}
int hex_to_int(string hex){
    int dec=0;
if(hex.substr(0,2)!="0x")
{
    return -1;
}
for(unsigned int i=2;i<hex.length();i++)
{
    dec*=16;
    if(hex[i]>='0' && hex[i]<='9'){
        dec+= (hex[i]-'0');
    }
    if(tolower(hex[i])>='a' && tolower(hex[i])<='z'){
        dec+= (hex[i]-'a')+10;
    }
}
return dec;


}

class cache_block{
public:
    string tag;
    int lru;
    bool dirty,valid;

    cache_block(){
    tag="invalid";
    dirty=false;
    valid=false;
    lru=-1;
    }

};
class Cache{
public:
    int memory_reads,memory_writes;
    int cache_hits,cache_misses;
    int write_policy;
    int cache_size,block_size;
    int num_of_blocks;
    cache_block* blocks;
    Cache(){}
    Cache(int cache_size,int block_size,int write_policy)
    {
        this->cache_size=cache_size;
        this->block_size=block_size;
        this->write_policy=write_policy;
        this->num_of_blocks=ceil((float)cache_size/block_size);
        blocks=new cache_block[cache_size/block_size];
        memory_reads=0;
        memory_writes=0;
        cache_hits=0;
        cache_misses=0;

    }
    void efficiency(){
     float n=(float)cache_hits/(cache_misses+cache_hits);
     cout<<"\nEfficiency of the Cache is :"<<setprecision(2)<<n<<endl;

    }
   void Read_from_cache(string address){};
    void Write_from_cache(string address){};
    void status(){
    cout<<"\nCache Hits:"<<cache_hits<<endl;
    cout<<"Cache Misses:"<<cache_misses<<endl;
    cout<<"Memory Reads:"<<memory_reads<<endl;
    cout<<"Memory Writes:"<<memory_writes<<endl;
    efficiency();

    }

};
class Direct_Mapping:public Cache{

public:
   Direct_Mapping(int cache_size,int block_size,int write_policy)
    {
        this->cache_size=cache_size;
        this->block_size=block_size;
        this->write_policy=write_policy;
        this->num_of_blocks=ceil((float)cache_size/block_size);
        blocks=new cache_block[num_of_blocks];
        memory_reads=0;
        memory_writes=0;
        cache_hits=0;
        cache_misses=0;
    }
 void Read_from_cache(string address);
 void Write_from_cache(string address);


};
void Direct_Mapping::Read_from_cache(string address){

string tag,index,offset;
bool hit=false;
int dec=hex_to_int(address);
string bin_address=dec_to_bin(dec);
/*Adding padding to the string*/
while(bin_address.length()<(TAG+INDEX+OFFSET)){
        bin_address="0"+bin_address;
}

tag=bin_address.substr(0,TAG);
index=bin_address.substr(TAG,INDEX);
offset=bin_address.substr(TAG+INDEX,OFFSET);

cache_block* block=&blocks[btoi(index)%num_of_blocks];
if(SHOW_BLOCK){
      cout<<"\nBinary:"<<bin_address<<endl;
    print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-23)<<"TAG"<<"|"<<setw(10)<<"INDEX"<<"|"<<setw(10)<<"OFFSET"<<"|"<<endl;
     print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-23)<<tag<<"|"<<setw(10)<<index<<"|"<<setw(10)<<offset<<"|"<<endl;
    cout<<"|"<<setw(hline-23)<<btoi(tag)<<"|"<<setw(10)<<btoi(index)<<"|"<<setw(10)<<btoi(offset)<<"|"<<endl;
     print_horizontal_line(hline,'-');
     cout<<setw(20)<<setfill('*')<<"Before"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
     print_horizontal_line(hline,'-');
     cout<<setfill(' ');
     cout<<"|TAG:"<<setw(11)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;

}
if(block->valid==true && block->tag==tag)
{
    cache_hits++;
    hit=true;

}
else{

    cache_misses++;
    memory_reads++;
    if(write_policy==1 && block->dirty==true){
        memory_writes++;
        block->dirty=false;
    }
    block->valid=true;
   block->tag =tag;

}
if(SHOW_BLOCK){
print_horizontal_line(hline,'-');
cout<<setw(20)<<setfill('*')<<"After"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
print_horizontal_line(hline,'-');
 cout<<setfill(' ');
cout<<"|TAG:"<<setw(11)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;
print_horizontal_line(hline,'-');
}
if(SHOW_HIT_MiSS){
if(hit==true){ cout<<"CACHE HIT!"<<endl;}
else { cout<<"MISS"<<endl;}
}
}

void Direct_Mapping::Write_from_cache(string address){

string tag,index,offset;
bool hit=false;
int dec=hex_to_int(address);
string bin=dec_to_bin(dec);
string bin_address=bin;
while(bin_address.length()<(TAG+INDEX+OFFSET)){
        bin_address="0"+bin_address;
}

tag=bin_address.substr(0,TAG);
index=bin_address.substr(TAG,INDEX);
offset=bin_address.substr(TAG+INDEX,OFFSET);
cache_block* block=&blocks[btoi(index)%num_of_blocks];
if(SHOW_BLOCK){
      cout<<"\nBinary:"<<bin_address<<endl;
    print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-24)<<"TAG"<<"|"<<setw(10)<<"INDEX"<<"|"<<setw(10)<<"OFFSET"<<"|"<<endl;
     print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-24)<<tag<<"|"<<setw(10)<<index<<"|"<<setw(10)<<offset<<"|"<<endl;
    cout<<"|"<<setw(hline-24)<<btoi(tag)<<"|"<<setw(10)<<btoi(index)<<"|"<<setw(10)<<btoi(offset)<<"|"<<endl;
     print_horizontal_line(hline,'-');
     cout<<setw(20)<<setfill('*')<<"Before"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
     cout<<setfill(' ');
     print_horizontal_line(hline,'-');
     cout<<"|TAG:"<<setw(hline-28)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;

}

 if(block->valid == true && block->tag==tag)
    {
        if(write_policy == 0)
        {
            memory_writes++;
        }
        block->dirty = 1;
        cache_hits++;
         hit=true;
    }
    else
    {
        cache_misses++;
        memory_reads++;

        if(write_policy == 0)
        {
            memory_writes++;
        }

        if(write_policy == 1 && block->dirty == true)
        {
            memory_writes++;
        }

        block->dirty = 1;

        block->valid = 1;



        block->tag = tag;
    }
if(SHOW_BLOCK){
print_horizontal_line(hline,'-');
cout<<setw(20)<<setfill('*')<<"After"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
cout<<setfill(' ');
print_horizontal_line(hline,'-');
cout<<"|TAG:"<<setw(hline-28)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;
print_horizontal_line(hline,'-');

}
if(SHOW_HIT_MiSS){
if(hit==true){ cout<<"CACHE HIT!"<<endl;}
else { cout<<"MISS"<<endl;}
}

}
class Fully_Associative:public Cache{
public:
    int last_read;
    Fully_Associative(int cache_size,int block_size,int write_policy)
    {
        last_read=-1;
        this->cache_size=cache_size;
        this->block_size=block_size;
        this->write_policy=write_policy;
        this->num_of_blocks=ceil((float)cache_size/block_size);
        blocks=new cache_block[num_of_blocks];
        memory_reads=0;
        memory_writes=0;
        cache_hits=0;
        cache_misses=0;
    }
 void Read_from_cache(string address);
 void Write_from_cache(string address);




};
void Fully_Associative::Read_from_cache(string address)
{
   string tag,offset;
bool hit=false;
cache_block* block;
int dec=hex_to_int(address);
string bin_address=dec_to_bin(dec);
/*Adding padding to the string*/
while(bin_address.length()<(TAG+OFFSET)){
        bin_address="0"+bin_address;
}

tag=bin_address.substr(0,TAG);
offset=bin_address.substr(TAG,OFFSET);
for(int i=0;i<=last_read;i++)
{
    block=&blocks[i];
if(block->valid==true && block->tag==tag)
{
    cache_hits++;
    hit=true;
    block->lru++;
    break;

}
}

if(hit==false){

        if(last_read<num_of_blocks-1){
          last_read=(last_read+1);
          block=&blocks[last_read];

        }
  else{
    block=&blocks[0];
      for(int i=0;i<num_of_blocks;i++)
     {
        if(block->lru > blocks[i].lru)
        {
         block=&block[i];
        }
     }
      }
}
if(SHOW_BLOCK){
      cout<<"\nBinary:"<<bin_address<<endl;
    print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-13)<<"TAG"<<"|"<<setw(10)<<"OFFSET"<<"|"<<endl;
     print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-13)<<tag<<"|"<<setw(10)<<offset<<"|"<<endl;
    cout<<"|"<<setw(hline-13)<<btoi(tag)<<"|"<<setw(10)<<btoi(offset)<<"|"<<endl;
     print_horizontal_line(hline,'-');
     cout<<setw(20)<<setfill('*')<<"Before"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
     print_horizontal_line(hline,'-');
     cout<<setfill(' ');
     cout<<"|TAG:"<<setw(hline-28)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;

}
    if(hit==false){
     cache_misses++;
     memory_reads++;
      block->lru=0;
     if(write_policy==1 && block->dirty==true){
        memory_writes++;
        block->dirty=false;
       }
     block->valid=true;
    block->tag =tag;


}

if(SHOW_BLOCK){
print_horizontal_line(hline,'-');
cout<<setw(20)<<setfill('*')<<"After"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
print_horizontal_line(hline,'-');
 cout<<setfill(' ');
cout<<"|TAG:"<<setw(hline-28)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;
print_horizontal_line(hline,'-');
}
if(SHOW_HIT_MiSS){
if(hit==true){ cout<<"CACHE HIT!"<<endl;}
else { cout<<"MISS"<<endl;}
}
}

void Fully_Associative::Write_from_cache(string address)
{
 string tag,index,offset;
bool hit=false;
cache_block* block;
int dec=hex_to_int(address);
string bin=dec_to_bin(dec);
string bin_address=bin;
while(bin_address.length()<(TAG+OFFSET)){
        bin_address="0"+bin_address;
}
tag=bin_address.substr(0,TAG);
offset=bin_address.substr(TAG,OFFSET);
for(int i=0;i<=last_read;i++)
{
     block=&blocks[i];
     if(block->valid == true && block->tag==tag)
    {
        if(write_policy == 0)
        {
            memory_writes++;
        }
        block->dirty = 1;
        cache_hits++;
         hit=true;
         block->lru++;
    }
}


if(hit==false)
    {
        if(last_read<num_of_blocks-1){
            last_read++;
             block=&blocks[last_read];
        }
        else{
            block=&blocks[0];
           for(int i=0;i<num_of_blocks;i++)
          {
           if(block->lru > blocks[i].lru)
          {
            block=&block[i];
          }
          }
        }
    }
    if(SHOW_BLOCK){
      cout<<"\nBinary:"<<bin_address<<endl;
    print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-13)<<"TAG"<<"|"<<setw(10)<<"OFFSET"<<"|"<<endl;
     print_horizontal_line(hline,'-');
    cout<<"|"<<setw(hline-13)<<tag<<"|"<<setw(10)<<offset<<"|"<<endl;
    cout<<"|"<<setw(hline-13)<<btoi(tag)<<"|"<<setw(10)<<btoi(offset)<<"|"<<endl;
     print_horizontal_line(hline,'-');
     cout<<setw(20)<<setfill('*')<<"Before"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
     cout<<setfill(' ');
     print_horizontal_line(hline,'-');
     cout<<"|TAG:"<<setw(hline-28)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;

}
if(hit==false){
        cache_misses++;
        memory_reads++;
       block->lru=0;
        if(write_policy == 0)
        {
            memory_writes++;
        }

        if(write_policy == 1 && block->dirty == true)
        {
            memory_writes++;
        }

        block->dirty = 1;

        block->valid = 1;
        block->tag = tag;
    }
if(SHOW_BLOCK){
print_horizontal_line(hline,'-');
cout<<setw(20)<<setfill('*')<<"After"<<setw(hline-21)<<setfill('*')<<""<<"|"<<endl;
cout<<setfill(' ');
print_horizontal_line(hline,'-');
cout<<"|TAG:"<<setw(hline-28)<<block->tag<<"|Valid:"<<setw(4)<<block->valid<<"|Dirty:"<<setw(4)<<block->dirty<<"|"<<endl;
print_horizontal_line(hline,'-');

}
if(SHOW_HIT_MiSS){
if(hit==true){ cout<<"CACHE HIT!"<<endl;}
else { cout<<"MISS"<<endl;}
}

}

void simulate(int type){
    string str[3][4];
    int select=0;
    /*Animation no 0*/
    str[0][1]="\r- \\ | /***Simulating***- \\ | /";
    str[0][2]="\r\\ | / -***Simulating***\\ | / -";
    str[0][3]="\r| / - \\***Simulating***| / - \\";
    str[0][0]="\r/ - \\ |***Simulating***/ - \\ |";
    /*Animation no 1*/
    str[1][0]="\r+ x + x***Simulation***+ x + x";
    str[1][1]="\rx + x +***Simulation***x + x +";
    str[1][2]="\r+ x + x***Simulation***+ x + x";
    str[1][3]="\rx + x +***Simulation***x + x +";
    /*Animation no 2*/
    str[2][0]="\r|->***Simulation***                  <-|";
    str[2][1]="\r|--->***Simulation***                <-|";
    str[2][2]="\r|->                  ***Simulation***<-|";
    str[2][3]="\r|->                ***Simulation***<---|";
        cout<<"                                          ";
for(int i=0;i<50;i++){
    select=(select+1)%4;
    cout<<str[type][select];
    Sleep(20);
}
cout<<"\r                                          ";

}
void Generate_trace(int mma,string name,int n){
 srand(time(NULL));
ofstream out(name);
char op;
char type[]={'R','W'};
//out.open(name);
long long int addr;
if(out.is_open()){
for(int i=0;i<n;i++)
{
    op=type[rand()%2];
    addr=rand()%mma;
out<<showbase<<hex<<addr<<' '<<op<<endl;
}
out.close();
}
else{
    cout<<"\nCould not open file!";
}


}
void Display_settings(){
     cout<<"Main Memory:"<<Main_Memory<<" KB\n";
    cout<<"Cache Memory:"<<Cache_Memory<<" KB\n";
    cout<<"Block Size:"<<Block_size<<" KB\n";
    cout<<"Cache Mapping: "<<Mapping<<endl;
    cout<<"TAG: "<<TAG<<" bits"<<endl;
    if(Mapping=="Direct")
    {
    cout<<"BLOCK: "<<INDEX<<" bits"<<endl;
    }
    cout<<"OFFSET: "<<OFFSET<<" bits"<<endl;

}
int main()
{
    int write_policy;
    string str;
    char choice='n';
    ifstream in;
    int mapping=1;
    Cache* cache;
    cout <<"**********CACHE SIMULATION**********"<< endl;
    cout<<"Default Settings:"<<endl;
    Display_settings();
    cout<<"Do you wish to change the Default Settings?(y\\n)"<<endl;
    cin>>choice;
    if(choice=='y'){
            bool valid=false;
    while(!valid){
    cout<<"Enter Main Memory Size(in KB)\n";
    cin>>Main_Memory;
    cout<<"Enter Cache Size(in KB)\n";
    cin>>Cache_Memory;

    int bits=ceil(log2(Main_Memory*1024));
    int temp=bits;
    cout<<"\nEnter Block Size(in KB)\n";
    cin>>Block_size;
     int blk=ceil(log2((float)Cache_Memory/Block_size));
     cout<<"Select Mapping\n1.Direct Mapping\n2.Fully Associative Mapping\n";

     cin>>mapping;
     switch(mapping)
     {
         case 1:{
          cout<<"Your Main Memory address has :"<<bits<<" bits\n"<<"blks:"<<blk<<endl;
         cout<<"Enter number of bits for TAG,BLOCK,Offset: ";
         cin>>TAG;
         cin>>INDEX;
         cin>>OFFSET;
         if(((TAG+INDEX+OFFSET)!=bits)){
         cout<<"Invalid number of bits"<<endl;
          }
          else valid=true;
          cout<<"\nSelect Write Policy\n1.Write Through\n2.Write Back\n";
           cin>>write_policy;
          if(!(write_policy==1||write_policy==2))
          {
         cout<<"Invalid choice!"<<endl;
          }


          break;

         }
        case 2:{
         cout<<"Your Main Memory address has :"<<bits<<" bits\n"<<"blks:"<<blk<<endl;
         cout<<"Enter number of bits for TAG,Offset: ";
         cin>>TAG;
         cin>>OFFSET;
         Mapping="Fully_Associative";
         if((TAG+OFFSET)!=bits){
         cout<<"Invalid number of bits"<<endl;
          }
          else valid=true;
          cout<<"\nSelect Write Policy\n1.Write Through\n2.Write Back\n";
         cin>>write_policy;
          if(!(write_policy==1||write_policy==2))
         {
             cout<<"Invalid choice!"<<endl;
          }
         break;


        }
     }
    }
    cout<<"Changed Settings:"<<endl;
    Display_settings();
    }
     Direct_Mapping cache1(Cache_Memory,Block_size,write_policy-1);
      Fully_Associative cache2(Cache_Memory,Block_size,write_policy-1);


/*Create new trace file*/
 if(NEW_TRACE)
 {
     srand(time(NULL));
     Generate_trace(Main_Memory*1024,"trace.txt",rand()%500);
 }


in.open("trace.txt");
string hex;
char type;
int line=0;
if(in.is_open()){
        in>>hex;
        in>>type;
    while(in.eof()==0)
    {

        if(SHOW_HIT_MiSS){
        cout<<"\nLine:"<<line++<<"  hex="<<hex<<" type="<<type<<endl;
            }
        if(type=='R')
        {
            if(mapping==2)
            {
                cache2.Read_from_cache(hex);
            }
            else{
                cache1.Read_from_cache(hex);
            }

        }
        else if(type=='W'){
                if(mapping==2)
                {
                    cache2.Write_from_cache(hex);
                }
                else{
                    cache1.Write_from_cache(hex);
                }

        }
        in>>hex;
        in>>type;

    }
    in.close();

}
else{
    cout<<"\nCouldn't open file!";
}

    //cin>>str;
   // cout<<endl<<hex_to_int(str);
   if(!SHOW_BLOCK)
   {
       simulate(2);
   }
  if(mapping==2)
     {
            cache2.status();
      }
    else{
            cache1.status();
         }
    cout<<endl;

    return 0;
}
