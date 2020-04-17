#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>

using namespace std;


int number_of_adgacent[100]={0};//this is the number of adgacent nodes for the node [x]
int number_of_adgacent2[100]={0};
int neighborsN[100][100]={0};//neighborsN[node number][neighbor number]
int neighborsM[100][100]={0};//neighborsM[node number][neighbor number]

int costs_from_source[100]={0};//minnimum number of steps from source to node [x]
int costs_from_destination[100]={0};//minnimum number of steps from destination to node [x]
const int INFINITE=99999;
int N,M;
void init_costs(){
    for(int i=0;i<100;i++)
        costs_from_source[i]=INFINITE;
    for(int i=0;i<100;i++)
        costs_from_destination[i]=INFINITE;
}
void print_costs(){
    cout <<" print costs_from_source " <<endl;

    for (int j=0;j<=N ;j++ ){
        cout <<setw(7)<< j << " ";
    }
    cout << endl;

    for (int j=0;j<=N ;j++ ){
        cout <<setw(7)<< costs_from_source[j] << " ";
    }
    cout << endl;

       cout <<" print costs_from_destination " <<endl;

    for (int j=0;j<=N ;j++ ){
        cout <<setw(7)<< j << " ";
    }
    cout << endl;

    for (int j=0;j<=N ;j++ ){
        cout <<setw(7)<< costs_from_destination[j] << " ";
    }
    cout << endl;
}
queue<int> q;//source,cost
void BFS(int source,int cost,int*costs){
    cout <<"BFS on source "<<source<<" with cost " << cost <<endl;
    cout <<"\t"<<"number_of_adgacent for "<<source<<" are "<<number_of_adgacent[source]<<endl;

    for(int i=0;i<number_of_adgacent[source];i++)
        {
            cout << "costs[neighborsN[source][i]] = " << costs[neighborsN[source][i]] << "cost = " << cost <<  endl ;
             if(costs[neighborsN[source][i]]>cost){//not set
                    cout << "\t"<<source <<"-"<<neighborsN[source][i]<<":"<<endl;
            //cost from source to the neghbot number i of source
                costs[neighborsN[source][i]]=cost;
                //costs[neighborsN[source][i]]=cost;
                q.push(neighborsN[source][i]);
            }else{
                cout << "\t discard already visited"<<source <<"-"<<neighborsN[source][i]<<"."<<endl;
            }
        }
    while(!q.empty()){
        int new_source=q.front();
        q.pop();
        BFS(new_source,costs[new_source]+1,costs);
    }
}

struct domino{
    int x;
    int y;
    void print_domino(){
        cout <<x<<"-"<<y;
    }
    int path_cost(){//this is the minnimum cost for a path passing through this domino
        return min(
           costs_from_source[x],
           costs_from_source[y]
           )+
       min(
           costs_from_destination[x],
           costs_from_destination[y]
           );
    }
};
int is_connecting(domino d){
    if(costs_from_source[d.x]!=INFINITE && costs_from_source[d.y]==INFINITE
       &&
       costs_from_destination[d.y]!=INFINITE && costs_from_destination[d.x]==INFINITE
       ){//x in source and y in destination
           return costs_from_source[d.x]+costs_from_destination[d.y]+1;//total cost
       }
    if(costs_from_destination[d.x]!=INFINITE && costs_from_destination[d.y]==INFINITE
       &&
       costs_from_source[d.y]!=INFINITE && costs_from_source[d.x]==INFINITE
       ){//x in destination and y in source
            return costs_from_destination[d.x]+costs_from_source[d.y]+1;//total cost
       }
    if(costs_from_destination[d.x]!=INFINITE && costs_from_destination[d.y]!=INFINITE
       &&
       costs_from_source[d.y]!=INFINITE && costs_from_source[d.x]!=INFINITE
       ){//x in destination and y in source
           int cost_des =  costs_from_destination[d.x] ;
           int cost_sor =  costs_from_source[d.y] ;
            return min(cost_des , cost_sor );//total cost
       }
    /*if(costs_from_source[d.x]!=INFINITE && costs_from_source[d.y]!=INFINITE
       &&
       costs_from_destination[d.y]!=INFINITE && costs_from_destination[d.x]!=INFINITE
       ){//x in source and y in destination
           return costs_from_source[d.x]+1;//total cost
       }*/
    return 0;
}

void print_path(int cost){
    for(int i=0;i<N;i++){
        if(costs_from_source[i]+costs_from_destination[i]==cost)
            cout<<" node "<<i << " is in the path sequence "<<costs_from_source[i]<<endl;
    }
}
int main()
{
    int x,y;
    init_costs();
    domino market_dominos[100];
    cin >> N;
    for (int i=0;i<N ;i++ )
        {
            cin>>x>>y;
            if(x==y)
                cout <<"Error x==y"<<endl;
            neighborsN[x][number_of_adgacent[x]++]=y;
            neighborsN[y][number_of_adgacent[y]++]=x;
        }
    cin >> M;
    for (int i=0;i<M ;i++ )
        {
            cin>>x>>y;
            if(x==y)
                cout <<"Error x==y"<<endl;
            market_dominos[i].x=x;
            market_dominos[i].y=y;
        }
    cin>>x>>y;
    costs_from_source[x]=0;
    costs_from_destination[y]=0;
    BFS(x,1,costs_from_source);
    BFS(y,1,costs_from_destination);

    print_costs();

    int minnimmum_cost = INFINITE;
    if(costs_from_source[y]==INFINITE)//x and y not connected
    {
        cout << "Currently there is no way between "<<x <<" and " <<y <<endl;
        //we need to connect
        for (int i=0;i<M ;i++ )
        {
            int cost;
            if(cost=is_connecting(market_dominos[i]))//this domino can connect
            {
                cout <<"Great domino ";
                market_dominos[i].print_domino();
                cout<<" can make life better "
                <<" with cost  = " << cost << endl;
                minnimmum_cost=min(minnimmum_cost,cost);
            }else{
                cout << "Useless domino ";
                market_dominos[i].print_domino();
                cout <<"Still disconnected"<<endl;
            }
        }
        if(minnimmum_cost==INFINITE)
        {
            cout <<"-1 impossible"<<endl;
            return 0;
        }
    }else{//already connected, but searching for better solution
        minnimmum_cost=costs_from_source[y];
        cout << "you are here " << endl ;
        for (int i=0;i<M ;i++ )
        {
            int cost = is_connecting(market_dominos[i]);
            cout << "this is cost  = " << cost << endl;
            cout << "minimum cost = " << minnimmum_cost << endl;
            if(cost<minnimmum_cost){
                minnimmum_cost=min(minnimmum_cost,cost);
                   cout << "Domino "<<market_dominos[i].x<<"-"<<market_dominos[i].y<<
                   " Can be useful, may make cost = "<<market_dominos[i].path_cost()<<endl;
               }else{
                   cout << "Useless domino "<<market_dominos[i].x<<"-"<<market_dominos[i].y<<
                   " may make cost = "<<market_dominos[i].path_cost()<<endl;
               }
            /**
            if
                X nearest to source
                and
                Y nearest to destination
            then it is useful
            OR if
                X nearest to destination
                and
                Y nearest to source
            */

        }
    }

    cout << "Final answer , minnimum path length = "<<minnimmum_cost<<endl;


    if(minnimmum_cost!=-1)
        print_path(minnimmum_cost);//note, we can print path if it was connected before market
        //otherwise we need to do another BFS
    return 0;
}
