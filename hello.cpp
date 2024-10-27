// #include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <bits/stdc++.h>

    // Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;
map<string,int>mp;    
// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

public:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


class network{
    
    public:
	vector <Graph_Node> Pres_Graph;


	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        // vector<Graph_Node>::iterator listIt;
        // int count=0;
        // for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        // {
        //     if(listIt->get_name().compare(val_name)==0)
        //         return count;
        //     count++;
        // }
        return mp[val_name];
    }
// get the node at nth index
    Graph_Node get_nth_node(int n)
    {
    //    vector<Graph_Node>::iterator listIt;
    //     int count=0;
    //     for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
    //     {
    //         if(count==n)
    //             return listIt;
    //         count++;
    //     }
    //     return listIt; 
        return Pres_Graph[n];

    }
    //get the iterator of a node with a given name
    vector<Graph_Node>::iterator search_node(string val_name)
    {
        vector<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    vector<Graph_Node>::iterator listIt;
                    vector<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

int find_index(Graph_Node &node,string &s){
    vector<string>str=node.values;
    for(int i=0;i<str.size();i++){
        if(str[i]==s){
            return i; 
        }
    }
    return 0;
}

int find_cpt_index(Graph_Node &node, vector<map<string,int>> &dp, network& Alarm, vector<vector<string>>& dataVector, int ind){
    vector<int>parent_index(node.Parents.size()+1);
    for(int i=1;i<=node.Parents.size();i++){
        parent_index[i]=Alarm.get_index(node.Parents[i-1]);
    }
    parent_index[0]=Alarm.get_index(node.Node_Name);

    vector<int>nums;
    string node_name=node.Node_Name;
    for(int j=0;j<parent_index.size();j++){
        nums.push_back(dp[parent_index[j]][dataVector[ind][parent_index[j]]]);
    }
    vector<int>suff(nums.size());
    suff[nums.size()-1]=1;
    int x=nums.size();
    for(int j=x-2;j>=0;j--){
        auto node1=Alarm.get_nth_node(parent_index[j+1]);
        suff[j]=suff[j+1]*(node1.nvalues);
    }
    int ans=0;
    for(int j=0;j<nums.size();j++){
        ans+=(nums[j]*suff[j]);
    }
    return ans;
}
int find_cpt_index_forx(Graph_Node& node, vector<map<string,int>> &dp, network& Alarm, vector<vector<string>>& dataVector, int ind,string value){
    vector<int>parent_index(node.Parents.size()+1);
    for(int i=1;i<=node.Parents.size();i++){
        parent_index[i]=Alarm.get_index(node.Parents[i-1]);
    }
    parent_index[0]=Alarm.get_index(node.Node_Name);
    vector<int>nums;
    string node_name=node.Node_Name;
    nums.push_back(dp[Alarm.get_index(node.Node_Name)][value]);
    for(int j=1;j<parent_index.size();j++){
        nums.push_back(dp[parent_index[j]][dataVector[ind][parent_index[j]]]);
    }
    vector<int>suff(nums.size());
    suff[nums.size()-1]=1;
    int x=nums.size();
    for(int j=x-2;j>=0;j--){
        auto node1=Alarm.get_nth_node(parent_index[j+1]);
        suff[j]=suff[j+1]*(node1.nvalues);
    }
    int ans=0;
    for(int j=0;j<nums.size();j++){
        ans+=(nums[j]*suff[j]);
    }
    return ans;

}

int main(int argc,char** argv)
{    
    string nalla(argv[2]); 
	network Alarm;
	Alarm=read_network();
    auto startTime = chrono::steady_clock::now();

    const int timeLimitSeconds = 110; 
    auto currentTime = chrono::steady_clock::now();
    auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

	cout<<"Perfect! Hurrah! \n";

    ifstream dataFile(nalla); // Replace "data.txt" with your data file's name


    vector<vector<string>> dataVector;
    string line;

    while (getline(dataFile, line)) {
        istringstream lineStream(line);
        vector<string> row;

        string word;
        
        
        while (lineStream >> word) {
            int x=word.size();
            //word=word.substr(1,x-2);
            row.push_back(word);
        }
        
        dataVector.push_back(row);
    }
    vector<vector<string>> dataVector_copy;
    vector<float>weight;
    vector<int>indices;
    int n=Alarm.Pres_Graph.size();
    for(auto& it:dataVector){
        int ind=-1;
        for(int i=0;i<it.size();i++){
            if(it[i]=="\"?\""){
                ind=i;
                break;
            }
        }
        
        
        if(ind==-1){
            dataVector_copy.push_back(it);
            //weight.push_back(1.0);
            indices.push_back(ind);
            continue;
        }
        indices.push_back(ind);
        auto node=Alarm.get_nth_node(ind);
        for(int j=0;j<node.nvalues;j++){
            it[ind]=node.values[j];
            dataVector_copy.push_back(it);
            //float x=(float)1.0/(float)node.nvalues;
            //weight.push_back(x);
            //indices.push_back(ind);
        }
    }
    //cout<<"ana"<<endl;
    vector<vector<vector<int>>>counter(n);
    for(int j=0;j<n;j++){
        vector<vector<int>>temp(Alarm.Pres_Graph[j].CPT.size());
        counter[j]=temp;
    }




    for(auto &it:Alarm.Pres_Graph){
        float fff=1.0/(float)it.nvalues;
        for(int i=0;i<it.CPT.size();i++){
            it.CPT[i]=fff;
        }
    }
    
    vector<map<string,int>>dp(n);
    for(int j=0;j<n;j++){
        auto node=Alarm.get_nth_node(j);
        mp[node.Node_Name]=j;
        for(int k=0;k<node.values.size();k++){
            dp[j][node.values[k]]=k;
        }
    }

    int utk=0;
    for(auto& it:dataVector_copy){
        for(int j=0;j<n;j++){
            auto node2=Alarm.get_nth_node(j);
            int ind=find_cpt_index_forx(node2,dp,Alarm,dataVector_copy,utk,dataVector_copy[utk][j]);
            counter[j][ind].push_back(utk);
        }
        utk++;
    }

    int count1=0;
    int manu=0;
    
    // for(auto& it:dataVector){
    //     int ind=-1;
    //     for(int i=0;i<it.size();i++){
    //         if(it[i]=="\"?\""){
    //             ind=i;
    //             break;
    //         }
    //     }
    //     indices.push_back(ind);
    // }
    cout<<dataVector_copy.size()<<endl;
    cout<<dataVector.size()<<endl;
    dataVector_copy.clear();
    weight.clear();
    while(true){
        auto currentTime = chrono::steady_clock::now();
        auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
        
        if (elapsedSeconds >= timeLimitSeconds) {
            cout << "Time limit reached. Exiting the program." << "\n";
            cout<<manu<<"\n";
            break;
        }
        //cout<<"ranjan"<<"\n";
        
        count1=0;
        for(auto& it1:dataVector){
            // cout<<"hello"<<"\n";
            int ind=indices[count1];
            // for(int i=0;i<it1.size();i++){
            //     if(it1[i]=="\"?\""){
            //         ind=i;
            //         break;
            //     }
            // }
            if(ind==-1){
                weight.push_back(1.0);
                dataVector_copy.push_back(it1);
                //cout<<"hello"<<endl;
                count1++;
                continue;
            }
            auto node=Alarm.get_nth_node(ind);
            //cout<<"ut"<<"\n";
            vector<float>probab_values(node.nvalues);
            float sum=FLT_MIN;
            //int index=-1;
            for(int i=0;i<node.nvalues;i++){
                //cout<<"guppu"<<"\n";
                //cout<<find_cpt_index_forx(node,dp,Alarm,dataVector,count1,node.values[i])<<" ";
                float pro=node.CPT[find_cpt_index_forx(node,dp,Alarm,dataVector,count1,node.values[i])];
                //cout<<"lodu";
                for(int j=0;j<node.Children.size();j++){
                    auto node2=Alarm.get_nth_node(node.Children[j]);
                    pro*=node2.CPT[find_cpt_index(node2,dp,Alarm,dataVector,count1)];
                }
                probab_values[i]=(pro);
                sum+=pro;
            }
            for(int j=0;j<probab_values.size();j++){
                vector<string>temp=it1;
                temp[ind]=node.values[j];
                dataVector_copy.push_back(temp);
                weight.push_back(probab_values[j]/sum);
            }
            count1++;
        }
        
        vector<vector<float>>count_prob(Alarm.Pres_Graph.size());
        int eshu=0;
        for(auto &it:Alarm.Pres_Graph){
            for(int k=0;k<it.CPT.size();k++){
                count_prob[eshu].push_back(0.00);
            }
            eshu++;
        }
        
        //cout<<"vidu"<<endl;
        int plop=dataVector_copy.size();
        int hardu=0;
        //cout<<weight.size()<<endl;
        for(int i=0;i<n;i++){
            int x=Alarm.get_nth_node(i).CPT.size();
            //cout<<"papa"<<endl;
            for(int j=0;j<x;j++){
                for(int k=0;k<counter[i][j].size();k++){
                    if(counter[i][j][k]<weight.size())
                    count_prob[i][j]+=weight[counter[i][j][k]];
                }
            }
        }
        //cout<<"raghu"<<endl;
        hardu=0;
        //cout<<y<<"\n";
        
        int whiskey;
        for(auto &it:Alarm.Pres_Graph){
            int kasis=it.CPT.size()/it.nvalues;
            for(int k=0;k<it.CPT.size();k++){
                whiskey=k%kasis;
                float pika=FLT_MIN;
                for(int j=whiskey;j<it.CPT.size();j+=kasis){
                    pika+=count_prob[hardu][j];
                }
                it.CPT[k]=count_prob[hardu][k]/pika;
                // cout<<it.CPT[k]<<"\n";
            }
            hardu++;
        }
        
        // cout<<"hello"<<"\n";
        manu++;
        //cout<<dataVector_copy.size()<<"ana"<<endl;
        dataVector_copy.clear();
        weight.clear();
        // cout<<"he"<<"\n";
    }
    //cout<<"print"<<"\n";
    ifstream fFile("alarm.bif"); 
    int nol=0;
    while(getline(fFile,line)){
        nol++;
    }


    ifstream inFile("alarm.bif"); 
    ofstream outputFile("solved_alarm.bif"); // Replace "output.txt" with the desired file name

    if (!outputFile.is_open()) {
        cerr << "Failed to open the output file." << "\n";
        return 1;
    }
    int lol=0;
    int kushu=0;
    while (getline(inFile, line)) {
        istringstream lineStream(line);
        string word;
        
        lol++;
        if (lineStream >> word && word=="table"){
            outputFile<<"\ttable"<<" ";
            Graph_Node nd=Alarm.get_nth_node(kushu);
            for(int i=0;i<nd.CPT.size();i++){
                // if(nd.CPT[i]<0.0001) nd.CPT[i]=0.0001;
                outputFile<<fixed<<setprecision(4)<<nd.CPT[i]<<" ";
            }
            // outputFile<<fixed<<setprecision(8)<<nd.CPT[nd.CPT.size()-1];
            // for(auto u:nd.CPT) outputFile<<fixed<<setprecision(6)<<u<<" ";
            outputFile<<";"<<"\n";
            kushu++;
        }else{
            outputFile<<line;
            
            if(lol!=nol)outputFile<<"\n";
        }
        
        
    }
    // for(auto u:dataVector_copy){
    //     for(auto aloo:u){
    //         outputFile<<aloo<<" ";
    //     }
    //     outputFile<<"\n";

    // }
    
    // Iterate through the vector and write each element to the file
    // for (auto element : Alarm.Pres_Graph) {
    //     for(auto x : element.CPT){
    //         outputFile << x << " "; // You can adjust the formatting as needed
    //     }
    //     outputFile<<"\n";
    // }



    // for (auto element : weight) {
    //     //for(auto x : element){
    //         outputFile << element ; // You can adjust the formatting as needed
    //     //}
    //     outputFile<<"\n";
    // }

    // Close the file when you're done
    outputFile.close();
    inFile.close();
    currentTime = chrono::steady_clock::now();
    elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
    cout<<elapsedSeconds<<"\n";
        
    cout << "Data has been written to the file." << "\n";

	
}
