#include<iostream>
#include <queue>
#include<vector>
#include<fstream>
using namespace std;

class vertix
{   
    public:
    char name;
    int represent=0;
    int x=0;
    int y=0;
};
class span_relation
{
    public:
    bool which=0;//determine steiner[0] or steiner[1]
    bool done=0;
    int vertex1=0;//index1 of V_list
    int vertex2=0;//index2 of V_list
    vector<int> steiner[2];//{{x1 y1},{x2 y2}}
};
vector<vertix> V_list;//vertices{A B C D E ....}
vector<vector<int>>E_list;//{{represent1,represent2,Manhaton length},{...},{...},....}
vector<vector<int>>span_edges;
int main() 
{
    char temp[15];
    int vertices_num=0; //vertices 數量
    int edges_num=0;//edges 數量
    int edge_length=0;
    ifstream infile;
    infile.open("input.txt",ios::in);
    if(!infile){
    cerr<<"cannot open input file,break out";
    exit(1);
    }
    else{
        infile>>temp;
        infile>>vertices_num;
        //V_list.reserve(vertices_num);
        V_list.reserve(10);
        E_list.reserve(100);
        //E_list.reserve(vertices_num*vertices_num);
        span_edges.reserve(10);
        //span_edges.reserve(vertices_num);

        V_list.clear();
        E_list.clear();
        span_edges.clear();
        for (int i = 0; i < vertices_num; i++){
            infile>>V_list[i].name;
            V_list[i].represent=i;
            infile>>V_list[i].x;
            infile>>V_list[i].y;
        }
        for (int i = 0; i < vertices_num-1; i++){
            for (int j = i+1; j <vertices_num ; j++){
                E_list.push_back(vector<int>());//pushback subvector
                E_list[edges_num].push_back(i);
                E_list[edges_num].push_back(j);
                if(V_list[i].x>=V_list[j].x)
                    edge_length=V_list[i].x-V_list[j].x;
                else
                    edge_length=V_list[j].x-V_list[i].x;
                if (V_list[i].y>=V_list[j].y)
                    edge_length+=(V_list[i].y-V_list[j].y);
                else
                    edge_length+=(V_list[j].y-V_list[i].y);
                
                E_list[edges_num].push_back(edge_length);
                edge_length=0;//initialized
                //cout<<"("<<E_list[edges_num][0]<<","<<E_list[edges_num][1]
                //<<","<<E_list[edges_num][2]<<")"<<endl;//testing
                edges_num++;
            }  
        }
        //cout<<V_list[0].represent<<V_list[1].represent<<V_list[2].represent;//testing

        vector<vector<pair<int, int>>> g(vertices_num);
                for (const auto& e : E_list) {//adjacency list
            g[e[0]].emplace_back(e[1], e[2]);//u v 權重
            g[e[1]].emplace_back(e[0], e[2]);//v u 權重
        }
        
        priority_queue<pair<int, int>> q; // (-w, v)
        vector<int> seen(vertices_num);//已加入spanning tree的vertices
        q.emplace(0, 0); // (-w, v)
        
        int cost = 0;
        int index=0;//span_edge index
        span_edges[0].push_back(0);//first push vertice(0) into it
        for (int i = 0; i < vertices_num; ++i) {
            while (true) {
            const int w = -q.top().first;
            const int v = q.top().second;
            q.pop();
            if (seen[v]++) continue;
            cout<<" new vertices"<<v<<endl;//testing
            if(v!=0){
            span_edges[index].push_back(v);
            span_edges[++index].push_back(v); }

            cost += w;
            for (const auto& p : g[v]) {
                if (seen[p.first]) continue;
                q.emplace(-p.second, p.first);//dynamic adding more possible edges
            }
            break;
            }
        }
        cout << cost << endl;//testing
        cout<<"("<<span_edges[0][0]<<","<<span_edges[0][1]<<")"<<endl;//testing
        cout<<"("<<span_edges[1][0]<<","<<span_edges[1][1]<<")"<<endl;
        cout<<"("<<span_edges[2][0]<<","<<span_edges[2][1]<<")"<<endl;
        cout<<"("<<span_edges[3][0]<<","<<span_edges[3][1]<<")"<<endl;
        cout<<"("<<span_edges[4][0]<<","<<span_edges[4][1]<<")"<<endl;
        cout<<"("<<span_edges[5][0]<<","<<span_edges[5][1]<<")"<<endl;
        cout<<"("<<span_edges[6][0]<<","<<span_edges[6][1]<<")"<<endl;
        cout<<"("<<span_edges[7][0]<<","<<span_edges[7][1]<<")"<<endl;
        cout<<"("<<span_edges[8][0]<<","<<span_edges[8][1]<<")"<<endl;
        
        vector<span_relation> relation_list(9);
        relation_list.clear();
        for (int i = 0; i < 9; i++){
            //span_edges[i][0] span_edges[i][1] 分別是兩端vertix在V_list中的index
            //steiner point1
            relation_list[i].steiner[0].push_back(V_list[span_edges[i][0]].x);
            relation_list[i].steiner[0].push_back(V_list[span_edges[i][1]].y);
            //steiner point2
            relation_list[i].steiner[1].push_back(V_list[span_edges[i][0]].y);
            relation_list[i].steiner[1].push_back(V_list[span_edges[i][1]].x);
            //define terminal vertices
            relation_list[i].vertex1=span_edges[i][0];
            relation_list[i].vertex2=span_edges[i][1];
        }
        //走訪relation_list 直到所有done指標都變1
        //testing 
        for (int i = 0; i < 9; i++){
            cout<<"done of relation_list["<<i<<"] is"<<relation_list[i].done<<endl;
        }

            //priority1 必取的steiner(x--x同)or(y--y同)
            for (int i = 0; i < 9; i++){
                //x type
                if (V_list[relation_list[i].vertex1].x
                ==V_list[relation_list[i].vertex2].x){
                    if(relation_list[i].steiner[0][0]==V_list[relation_list[i].vertex1].x)
                    {relation_list[i].which=0;}

                    if(relation_list[i].steiner[1][0]==V_list[relation_list[i].vertex1].x)
                    {relation_list[i].which=1;}

                    relation_list[i].done=1;
                   }
                //y type
                if(V_list[relation_list[i].vertex2].y
                ==V_list[relation_list[i].vertex2].y){
                    if(relation_list[i].steiner[0][1]==V_list[relation_list[i].vertex1].y)
                    {relation_list[i].which=0;}

                    if(relation_list[i].steiner[1][1]==V_list[relation_list[i].vertex1].y)
                    {relation_list[i].which=1;}
                
                    relation_list[i].done=1;
                }               
            }
            //testing
            for (int i = 0; i < 9; i++){
            cout<<"done of relation_list["<<i<<"] is"<<relation_list[i].done<<endl;
        }

            //priority2 重複的steiner point要取
            for (int i = 0; i < 8; i++){
                if(relation_list[i].done==1) continue;//若done是1直接不測
                for (int j = i+1; j < 9; j++){
                    if(relation_list[j].done==1) continue;//若done是1直接不測
                    if(relation_list[i].steiner[0][0]==relation_list[j].steiner[0][0]
                    &&relation_list[i].steiner[0][1]==relation_list[j].steiner[0][1]){
                        relation_list[i].done=1;
                        relation_list[j].done=1;
                        relation_list[i].which=0;//steiner0 比 steiner0
                        relation_list[j].which=0;
                    }
                    if(relation_list[i].steiner[0][0]==relation_list[j].steiner[1][0]
                    &&relation_list[i].steiner[0][1]==relation_list[j].steiner[1][1]){
                        relation_list[i].done=1;
                        relation_list[j].done=1;
                        relation_list[i].which=0;//steiner0 比 steiner1
                        relation_list[j].which=1;
                    }
                    if(relation_list[i].steiner[1][0]==relation_list[j].steiner[0][0]
                    &&relation_list[i].steiner[1][1]==relation_list[j].steiner[0][1]){
                        relation_list[i].done=1;
                        relation_list[j].done=1;
                        relation_list[i].which=1;//steiner1 比 steiner0
                        relation_list[j].which=0;
                    }
                    if(relation_list[i].steiner[1][0]==relation_list[j].steiner[1][0]
                    &&relation_list[i].steiner[1][1]==relation_list[j].steiner[1][1]){
                        relation_list[i].done=1;
                        relation_list[j].done=1;
                        relation_list[i].which=1;//steiner1 比 steiner1
                        relation_list[j].which=1;
                    }
                }
            }

        //testing
            for (int i = 0; i < 9; i++){
            cout<<"done of relation_list["<<i<<"] is"<<relation_list[i].done<<endl;
        }
        
        vector<int> undone(10);
        undone.clear();
        for (int i = 0; i < 9; i++){//store every undone indexes
            if(relation_list[i].done==0) undone.push_back(i);
        }

        cout<<"still success"<<endl;
        cout<<"undone size"<<undone.size();
        //undone vector stores every indexes still needed
        
        

        infile.close();
    }
    
    return 0;
    system("PAUSE");
}