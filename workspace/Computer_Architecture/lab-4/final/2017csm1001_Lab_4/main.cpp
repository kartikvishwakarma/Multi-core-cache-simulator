#include<bits/stdc++.h>
using namespace std;


map <int,int> exe_map;
map <int,int> temp_exe_map;
vector <vector <int> > vector_dependency;
vector <vector <int> > dependency_waitinglist;
vector <vector <int> > task_event_information;
vector <vector <int> > vector_processor_cores;
vector <vector <int> > temp_vector_processor_cores;
vector <vector <int> > Utilizaation_vector_processor;
map <int,int> map_for_no_of_cores_inside_processor;
map <int,int> map_for_cpi;
map <int,int> map_for_clock_pd;
map <int, int> :: iterator itr;
priority_queue<int,vector<int>,greater<int> >priorityQ;
vector<int> readystate,waitingstate,runningstate;
int no_of_processor;
int freeprocessor,freecore;
int start_time=0;
int final_answer=0;


void take_input_from_user()
{
	int i,k,cores,cpi,clock;
	cout << "\n # OF PROCESSOR :    ";
	cin >> no_of_processor;

	
	for(i=0; i<no_of_processor; i++)
	{
		cout << "\n# OF CORE FOR  " << i << " PROCESSOR:-  :";
		cin >> cores;
		map_for_no_of_cores_inside_processor.insert(pair<int,int>(i,cores));
		


		vector<int> temp; // vector for assigning cores to a processor
		temp.push_back(i); 
		for(k=0; k<cores; k++) // vector for assigning cores to a processor
		{	
			temp.push_back(0);
		}
		vector_processor_cores.push_back(temp);
		temp_vector_processor_cores.push_back(temp);
		Utilizaation_vector_processor.push_back(temp);

	}
	cout << "\n***************************************************************\n";
	

	for (itr = map_for_no_of_cores_inside_processor.begin(); itr != map_for_no_of_cores_inside_processor.end(); ++itr)
   
        	cout  << itr->first <<  "\t \t" << itr->second << endl ; 
        	     
}



void find_non_dependency_task()
{
	cout << "\n\n.................SYNCRONIZING INDEPENDENT TASK AND PUT TO READY QUEUE........................\n";
	int i;
	int TOTAL_NODE = exe_map.size();
	int node[TOTAL_NODE]={0};
	for(i=0;i<vector_dependency.size();i++)
	{
		node[vector_dependency[i][1]]=1;
	}	
	for(i=0; i< TOTAL_NODE; i++)
	{
		if(!node[i])
		{
			cout << i  << endl;
			readystate.push_back(i);
		}
	}
	cout << "\n..................TASK ARE NOW IN READY QUEUE....................................................\n";
}


void speedup()
{
	int sum =0;
	for(itr=temp_exe_map.begin(); itr != temp_exe_map.end(); ++itr)
	{
		sum += itr->second;
	}
	cout << "\n SPEEDUP : " << ((float)sum/final_answer);	
}




bool find_free_processor_core()
{
	for (itr = map_for_no_of_cores_inside_processor.begin(); itr != map_for_no_of_cores_inside_processor.end(); ++itr)
    {
    	if((itr->second) > 0)
    	{
    	
			cout << "\n..................A TASK JUST FINISH.................\n";
			cout<<"\n....................free up core.......................\n";
			cout << "PROCESSOR NO. : " << itr->first << "  and CORE NO.. " << itr->second;
			freeprocessor = itr->first;
			freecore = itr->second;
			itr->second = (itr->second)-1;
			return 1;
    	
    	}
    	cout<<"\n.....................................Done.........................................\n";
 
    }
    return 0;
}
void erase_successor_task()
{
	int i,j,flag;
	cout << "\n.........................REMOVING EXECUTED TASK FROM LIST....................\n";
	int n = dependency_waitinglist.size();






	for(i=0; i < dependency_waitinglist.size(); i++)
	{
		
		flag = 0;
		for(j=1; j < dependency_waitinglist[i].size(); j++)
		{
			
			if(dependency_waitinglist[i][j] == readystate[0])
			{
				dependency_waitinglist[i].erase(dependency_waitinglist[i].begin()+j);
				flag=1;
				j--;
			}
		}

		cout<<"\n.......................ALLOCATING CORE TO TASK/TASKS............................\n";
		if(flag && dependency_waitinglist[i].size() == 1)
		{
			readystate.push_back(dependency_waitinglist[i][0]);
			exe_map[dependency_waitinglist[i][0]] += exe_map[readystate[0]];
			cout << "\n TASK_ID " << dependency_waitinglist[i][0] << " WAITING---->READY";
			dependency_waitinglist.erase(dependency_waitinglist.begin()+i);
			i--;
		}
	}
	
}


void core_utilization()
{
	cout <<"\n-------------------------------------------------------------------------------------------\n";
	cout<<"\n|                                        SUMMARY                                            |\n";
	cout<<"\n---------------------------------------------------------------------------------------------\n";
	int i,j;
	cout << "\n--------------------------------------\n";
	cout << "\n CORE UTILIZATION: ";
	for(i=0; i<Utilizaation_vector_processor.size(); i++)
	{
		cout << "\nPROCESSOR NO. " << i << endl;
		for(j=1; j<Utilizaation_vector_processor[i].size(); j++)
		{
			int temp = Utilizaation_vector_processor[i][j];
			cout << "CORE NO. " << j <<" :  " <<  (((float) temp/ final_answer)*100) << "%" << endl;
			
		}
		cout << endl;
	}
}

void calculate_final_result()
{
	int i,j;
	final_answer=0;
	for(i=0; i<vector_processor_cores.size(); i++)
	{
		for(j=1; j<vector_processor_cores[i].size(); j++)
		{
			if(vector_processor_cores[i][j] > final_answer)
			{
				final_answer = vector_processor_cores[i][j];
			}	
		}
		
	}
	cout << "\n  TIME TO FINISH ALL TASKS  " << final_answer ;
}
void readystate_to_runningstate(int comp_time,int temp_exe)
{
	int i;
	for(i=0; i<vector_processor_cores.size(); i++)
	{
		if(vector_processor_cores[i][0] == freeprocessor)
			break;

	}

	cout << "\n TASK NO " << readystate[0] << " READY----->RUNNING \n";
	cout << "\n PROCESSOR NO " << freeprocessor << " CORE NO " << freecore;
	vector_processor_cores[i][freecore] = comp_time;

	temp_vector_processor_cores[i][freecore] = temp_exe;
	Utilizaation_vector_processor[i][freecore] += temp_exe_map[readystate[0]];
	
	erase_successor_task();
	

}

void free_core_of_processor()
{
	int min = INT_MAX;
	int i,j,current,processorindex,coreindex;
	for(i=0; i<temp_vector_processor_cores.size(); i++)
	{
		for(j=1; j<temp_vector_processor_cores[i].size(); j++)
		{
			current = temp_vector_processor_cores[i][j];
			if(min > current)
			{
				min = current;
				processorindex = i;
				coreindex = j;
			}
		}
	}
	map_for_no_of_cores_inside_processor[processorindex] += 1;
	temp_vector_processor_cores[processorindex][coreindex] = 0;

}
void schedule()
{
	
	int i,comp_time;
	bool available;
	while(readystate.size() > 0)
	{
		
		int index = 0,sum=0;
		comp_time=INT_MAX;
		for(i=0; i<readystate.size(); i++)
		{
			sum = exe_map[readystate[i]];
			if(comp_time > sum)
			{
				comp_time = sum;
				index = i;
			}
		}

		
		
		int a = readystate[0];
		readystate[0] = readystate[index];
		readystate[index] = a;

		
		for(i=0; i<readystate.size(); i++)
		{
			cout << readystate[i] << "  ";
		}
		available = find_free_processor_core();
		if(available)
		{
			readystate_to_runningstate(comp_time,exe_map[readystate[0]]);
			readystate.erase(readystate.begin());
		}
		else //busy
		{
			cout << "\n...............................ALL CORE ARE ALLOCATTED................................ ";
			free_core_of_processor();
		}

	}

}


void calculate_dependencylist()
{
	int i,j;

	for (itr = exe_map.begin(); itr != exe_map.end(); ++itr)
    {
    	vector<int>temp;
    	temp.push_back(itr->first);
    	for(i=0;i<vector_dependency.size();i++)
		{
			if(itr->first == vector_dependency[i][1])
				
				temp.push_back(vector_dependency[i][0]);
			
		}
		if(temp.size() != 1)
		{
			//cout << "\n hello";
			dependency_waitinglist.push_back(temp);	
		}
		temp.clear();
		
	}
	      	

	cout << "\n.................# TASK IN WAITING QUEUE..............................\n";
	

	for(i=0;i<dependency_waitinglist.size();i++)
	{
		//cout << dependency_waitinglist[i][0] << "\t";
		for(j=0;j<dependency_waitinglist[i].size();j++)
		{
			cout<< dependency_waitinglist[i][j]<<"  ";
		}
		cout<<endl;
	}
	
}

void readfromfile()
{
	int i,j;
	int taskid,taskexe;
	int source,dest,cost;
	ifstream task_execution,task_dependency;
	task_execution.open("task.txt");
	task_dependency.open("dependency.txt");
	while(!task_execution.eof())
	{
		task_execution >> taskid >> taskexe;
		exe_map.insert(pair<int,int>(taskid,taskexe)); 
		temp_exe_map.insert(pair<int,int>(taskid,taskexe));
	}
	while(!task_dependency.eof())
	{
		task_dependency >> source >> dest >> cost;
		vector <int> temp;
		temp.push_back(source);
		temp.push_back(dest);
		temp.push_back(cost);
		vector_dependency.push_back(temp);
		
	}
vector_dependency.pop_back();
	
	cout << "\n ********************TASK DEPENDENCY*****************************\n";
	cout << "\n SOURCE   "<< " DESTINATION " << " COST \n";
	for(i=0;i<vector_dependency.size();i++)
	{
		for(j=0;j<vector_dependency[i].size();j++)
		{
			cout<<vector_dependency[i][j]<<"\t\t";
		}
		cout<<endl;
	}
	cout << "\n*****************************************************************\n";
	cout << "\n\n********************COMPUTATION COST OF TASK******************* \n";
	for (itr = exe_map.begin(); itr != exe_map.end(); ++itr)
    {
        	cout  <<"\t\t\t"<< itr->first <<  '\t' << itr->second << '\n';
    }
    
    cout << "\n################################################################\n";

}

int main()
{
	
	readfromfile();
	
	take_input_from_user();

	cout << "*******************RUNNING SCHEDULAR*****************************\n";
	find_non_dependency_task();
	calculate_dependencylist();
	schedule();
	calculate_final_result();
	core_utilization();
	speedup();
	cout << endl;
}
