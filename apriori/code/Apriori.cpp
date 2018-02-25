#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include <math.h>

using namespace std;
class Apriori
{
  private:
        bool **data;//Actual transactional database 
	multimap <int, string> transdatabase;
	map<string,int> candidate_itemsets;//candidate Item sets
	map<string,int> frequent_item;//frequent item set L[k-1]
	map<string,int> frequent_itemsets;//frequent item set L[k]
	map<string,int> dummy_itemsets;//this dummy set is used to assign id to each items ex- a-0,b-1,c-2
	float minsup,confidence;
	int total_transaction;//total number of transactions
  public:
       
        //split the string using delimiter(,)
        int split(string s,string *buffer,int no_of_strings)
        	{ string s1;
        	  for(int i=0;i<no_of_strings;i++)
        	  	{
        	  		buffer[i]="";
        	  	}
        	  no_of_strings=0;	
        	  for(int i=0;i<s.length();i++)
			{      if (s.at(i)!=','&&s.at(i)!='\n')
					s1+=s[i];
				else
				   { 
				     buffer[no_of_strings++]=s1;
				    
				     s1="";
				   }	
			}
		return	no_of_strings;//return total number of splitted strings
        	    
        	}
        //initialising candidate itemset(c1)	
   	void initialising_database(string s,int transnumber )
		{string s1,s2;
		 int i=0;
		 
		 
			for(int i=0;i<s.length();i++)
			{      if (s.at(i)!=','&&s.at(i)!='\n')
					s1+=s[i];
				else
				   { s2=s1;
				     transdatabase.insert(pair <int, string> (transnumber, s2));
				     candidate_itemsets.insert(pair <string, int> (s2, 0));//initialising the 1st candidate itemset
				     s1="";
				     s2="";
				   }	

				
			}
	  	  	
			
		}
	//initialising the transactional database in ECLAT database, to reduce the comparisons
	/*ex         a  b  c  d  e
	transaction1 0	1  1  0  1   it means b,c,e items are present in transcation 1 ....this data is stored in boolean array **data*/
	void database(int trans)
	{ int i=0;
	  int candidate=0;
	  multimap <int, string> :: iterator itr;		 
          map <string, int> :: iterator cand;
          total_transaction=trans;
		for (cand = candidate_itemsets.begin(); cand != candidate_itemsets.end(); ++cand)
			    {
				
						dummy_itemsets.insert(pair <string, int> (cand->first,i));
						i++;
			    }
		candidate=candidate_itemsets.size();
		data=new bool *[trans];
		for(int i=0;i<trans;i++)
		{
		   data[i]=new bool[candidate];
		} 
		for(int i=0;i<trans;i++)
			{	
				for(int j=0;j<candidate;j++)
					 data[i][j]=false;	
			} 
		 for (itr = transdatabase.begin(); itr != transdatabase.end(); ++itr)
			    {
				data[itr->first][dummy_itemsets[itr->second]]=true;
				     
			    }		
			    		
	  cout<<"database created\n";
	}	
	//calculate support of candidate itemset with the help of transasction database
        void 	calculate_support( map <string, int> &maps )
        	{ map <string, int> :: iterator cand;
        	  
        	  string temp;
        	  int no_of_strings=0;
        	  bool check=true ;
        	  string buffer[500];
        	  //cout<<"in calculate_support"<<endl;
        	 	 
			 for (cand = maps.begin(); cand != maps.end(); ++cand)
			    {   temp="";
			        temp=cand->first;
			        temp+="\n";			        
			        no_of_strings=split(temp,buffer,no_of_strings); //split the string 			
				for (int i=0; i<total_transaction; ++i)
					    {   
					     
						for(int j=0;j<no_of_strings;j++)	
						    {
						       check &=data[i][dummy_itemsets[buffer[j]]];
						    }	
					        if(check==true)
							{      
								++(cand->second);
								
							} 
						check=true;	   
					    }
			    }
        	
        	}
        //calculate candidate itemsets	
        void calculate_candidate()
        	{
        	      calculate_support(candidate_itemsets);
        	
        	}	
        //calculate frequent itemset using threshold value
        void calculate_frequent_item_set()
        	{
        		map <string, int> :: iterator cand;
        		map <string, int> :: iterator freq;
        		frequent_item.clear();
        		for (freq = frequent_itemsets.begin(); freq != frequent_itemsets.end(); ++freq)
			    {
				
					frequent_item.insert(pair <string, int> (freq->first,freq->second));//frequent itemset L[k-1]
					
			    }	
        		frequent_itemsets.clear();
        		cout<<"in calculate_frequent"<<endl;
         		for (cand = candidate_itemsets.begin(); cand != candidate_itemsets.end(); ++cand)//frequent itemset L[k]
			    {
				if((cand->second)>=minsup)
					{
						frequent_itemsets.insert(pair <string, int> (cand->first,cand->second));
					}
			    }	
        	
        	}
        //prune	
        bool prune(string *str,int k)
        	{
        	 string subset[k][k-1];
        	 map <string, int> :: iterator freq;
        	 int n=0,val=0;
        	 string s="";
        	 //cout<<"prune"<<endl;
			for(int i=0;i<k-1;i++)
				{
					for(int j=0;j<k;j++)
						{
					
								subset[j][i]=s[val];
								n++;
								if(n%(k-1)==0)
								{
									val++;
									n=0;
								}
						}
		
				}
			for(int i=0;i<k;i++)
				{
					for(int j=0;j<k-1;j++)
						{
							s+=subset[i][j];
							s+=",";
						}
					s.erase(s.length()-1,1);
					  freq = frequent_itemsets.find(s);
					  if (freq != frequent_itemsets.end())
					   	return false;
					s="";
				}
			return true;	
        	
        	}	
        //join frequent item set	
        void join()
        	{
        		map <string, int> :: iterator fre1,fre2;
        		int check=1,no_of_strings1=0,no_of_strings2=0;
        		string temp1,temp2,buffer1[500],buffer2[500];
        		string subset[500];
        		candidate_itemsets.clear();
        		cout<<"in join"<<endl;
        		 for (fre1 = frequent_itemsets.begin(); fre1!= frequent_itemsets.end(); ++fre1)
			    {   temp1=fre1->first;
			        temp1+="\n";			        
			        no_of_strings1=split(temp1,buffer1,no_of_strings1); 
			        for (fre2 = frequent_itemsets.begin(); fre2 != frequent_itemsets.end(); ++fre2)
					    {   temp2=fre2->first;
						temp2+="\n";			        
						no_of_strings2=split(temp2,buffer2,no_of_strings2);
						for(int i=0;i<no_of_strings2-1;i++)
							{
								if(dummy_itemsets[buffer1[i]]==dummy_itemsets[buffer2[i]])
									check=1;
								else 
								        {
								        check=0;
								        break;
								        }	
							}
						if(check==1&&dummy_itemsets[buffer1[no_of_strings1-1]]<dummy_itemsets[buffer2[no_of_strings2-1]])
						{	for(int i=0;i<no_of_strings1;i++)
								{
									subset[i]=buffer1[i];
								}
							subset[no_of_strings2-1]=buffer2[no_of_strings2-1];	
							if( prune(subset,no_of_strings1+1))
								{
									candidate_itemsets.insert(pair <string, int> ((fre1->first)+","+ buffer2[no_of_strings2-1], 0));	
								}
						}	
						
					
						 
					    }
			    }
        			
        	}
        //display the ECLAT Transactional Database				
         void displaytransaction(int trans)
         	{ int candidate=0;
         	 
		 candidate=candidate_itemsets.size();
         		for(int i=0;i<trans;i++)
			{	
				for(int j=0;j<candidate;j++)
					 cout<<data[i][j]<<" ";
				cout<<endl;	 	
			} 
         	}
         //display the candidate itemset	
         void displaycandidate()
         	{
         		map <string, int> :: iterator itr;
			    cout << "\nThe map candidate_itemset are : \n";
			    cout << "\tITEMS\t\t\tSUPPORT\n";
			    for (itr = candidate_itemsets.begin(); itr != candidate_itemsets.end(); ++itr)
			    {
				cout  <<  '\t' << itr->first
				      <<  "\t" << itr->second << '\n';
			    }
         	} 
         //write content of frequent itemset L[k-1] into file	
         void displayfrequent(ofstream &fo)
         	{
         		map <string, int> :: iterator itr;
			    for (itr = frequent_item.begin(); itr != frequent_item.end(); ++itr)
			    {
				fo<<itr->first<<","<<  "\t" <<",support,"<<itr->second << '\n';
			    }
			    cout<<"frequent itemset succesfully generated"<<endl;
         	}
         //find the total possible subsets for given items excluding trivial subset	
       void powerset(string *set,string *subset, int set_size,int pow_set_size)
			{
			    string s;
			    int counter, j;
			 
				   for(counter = 0; counter < pow_set_size; counter++)
				    {
				      for(j = 0; j < set_size; j++)
					       {
						  
						  if(counter & (1<<j))
						      { s+=set[j];
							s+=",";
						      } 
					       }
				       if(s.length()>0)
				       s.erase(s.length()-1,1);
				       subset[counter]=s;
				       s="";
				       
				    }
				 for(counter = 0; counter < pow_set_size; counter++)
				    {
				      
				       
				    }	  
					           
				     
			} 
	bool check(string *buffer1,string *buffer2,int no_of_strings1,int no_of_strings2)
			{
				for(int i=0;i<no_of_strings1;i++)
					{
						for(int j=0;j<no_of_strings2;j++)
							{
								if(buffer1[i]==buffer2[j])
									return false;
							}
					
					}
				return true;	
			
			}
//generate association rules based on confidence in file association_rules_confidence.csv
//generate correlation betw lhs & rhs of association rules based on Interest factor in file association_rules_interest_factor.csv					 	
       void association_rules(ofstream &fo,ofstream &fo1)
       		{map <string, int> :: iterator itr;
       		 map<string,int> lhs,rhs;
       		 float conf=0,interest=0,a,b,c;
       		 string set[500],buffer1[500],buffer2[500],temp,temp1,temp2,*subset;
       		 int no_of_strings=0,no_of_strings1=0,no_of_strings2=0;
       		 unsigned int pow_set_size;
       		 
			    for (itr = frequent_item.begin(); itr != frequent_item.end(); ++itr)
			    {
				 temp=itr->first;
				 temp+="\n";
				 no_of_strings=split(temp,set,no_of_strings); 
				 if(no_of_strings==1)
				 	{
				 		cout<<"association rules cannot be generated for frequent itemset containing single item in a row\n";
				 		return;
				 	}
				 pow_set_size = pow(2, no_of_strings);
				 pow_set_size=pow_set_size-1;
				 subset=new string[pow_set_size];
				 powerset(set,subset,no_of_strings,pow_set_size);
				 for(int i=0;i<pow_set_size;i++)
				 	{  if(subset[i]=="")
				 	         continue;
				 	   temp1=subset[i];
				 	   temp1+="\n";
				 	   no_of_strings1=split(temp1,buffer1,no_of_strings1);
				 		for(int j=0;j<pow_set_size;j++)
				 			{  if(subset[j]=="")
				 	        		 continue;
				 			   temp2=subset[j]; 
				 			   temp2+="\n";
				 			   no_of_strings2=split(temp2,buffer2,no_of_strings2); 
				 			   if(((no_of_strings2+no_of_strings1)== no_of_strings)&& check(buffer1,buffer2,no_of_strings1,no_of_strings2))
				 			   	{       
				 			   		 lhs.insert(pair <string, int> (subset[i], 0));
				 			   		 lhs.insert(pair <string, int> (subset[i]+","+subset[j], 0));
				 			   		 rhs.insert(pair <string, int> (subset[j], 0));
				 			   		 calculate_support(lhs);
				 			   		 calculate_support(rhs);
				 			   		 a=lhs[subset[i]+","+subset[j]];
				 			   		 b=lhs[subset[i]];
				 			   		 conf=a/b;
				 			   		 if(conf>=confidence)
				 			   		 	fo<<subset[i]<<",("<<lhs[subset[i]]<<") "<<"\t,------->\t,"<<subset[j]<<",("<<rhs[subset[j]]<<") ,"<<",confidence,"<<conf<<endl; 
									 c=rhs[subset[j]];
				 			   		 interest=b*c;
				 			   		 interest=a/interest;
				 			   		 if(interest>1)
				 			   		 	fo1<<subset[i]<<",("<<lhs[subset[i]]<<") "<<"\t,------->\t,"<<subset[j]<<",("<<rhs[subset[j]]<<") ,"<<",items are positively correlated,"<<interest<<endl;
				 			   		 else if(interest<1)
				 			   		 	fo1<<subset[i]<<",("<<lhs[subset[i]]<<") "<<"\t,------->\t,"<<subset[j]<<",("<<rhs[subset[j]]<<") ,"<<",items are negatively correlated,"<<interest<<endl; 
				 			   		 else					 			   		  									fo1<<subset[i]<<",("<<lhs[subset[i]]<<") "<<"\t,------->\t,"<<subset[j]<<",("<<rhs[subset[j]]<<") ,"<<",items are independent,"<<interest<<endl; 
				 			   	
				 			   	}
				 			 lhs.clear();
							 rhs.clear();  	
				 			 temp2="";
				 			}
				 	  temp1="";
				 	}
				temp="";
			    }       		
       		cout<<"Association rules  succesfully generated"<<endl;
       		}  
       //check candidate itemset is empty or not		
      bool  is_candidate_empty() 	
      		{
      				 if(candidate_itemsets.empty())
      				 	return false;
      				 else
      				        return true;	
      		} 	
      	//check frequent itemset L[k] is empty or not		
      	bool  is_frequent_empty()	
      		{
      				 if(frequent_itemsets.empty())
      				 	return false;
      				 else
      				        return true;	
      		} 
      	//check frequent itemset L[k-1] is empty or not		
      	 bool  is_frequent_item_empty()	
      		{
      				 if(frequent_item.empty())
      				 	return true;
      				 else
      				        return false;	
      		} 
      	void getdata()
      		{         float support;
      			  cout<<"enter the value for minimum support\n",
			  cin>>minsup;
			  cout<<"enter the value for confidence in percentage\n",
			  cin>>confidence;
			 
			       		
      		}				
};


int main()
{ Apriori obj;
  string s,s1,s2;
  ifstream fin;
  ofstream fo1("frequent_itemset.csv");
  ofstream fo2("association_rules_confidence.csv");
  ofstream fo3("association_rules_interest_factor.csv");
  int i;
  int transnumber=0;
  char filename[50];
  cout<<"enter the file name\n",
  cin>>(filename);
  fflush(stdin);
  fin.open(filename);
  
  if(fin.fail())
	{
		cout<<"file cannot open\n";
		return 0;
	}
  while(fin)
	{
		        getline(fin,s);	
		        if(s=="")
		           continue;	        	
		        s+="\n";
		        obj.initialising_database(s,transnumber);
		        transnumber++;
		 
	}
 obj.database(transnumber);
 obj.getdata();
 
 do
 {
   obj.calculate_candidate();
   obj.calculate_frequent_item_set();  
   obj.join();
   
  
 }while(obj.is_frequent_empty());//loops until frequent itemset L[k]==empty
 	
 	
if(obj.is_frequent_item_empty()	)
      cout<<"cannot generate association rule and frequent itemset  for given support and confidence"<<endl; 
       
else  
    { obj.displayfrequent(fo1);
      obj.association_rules(fo2,fo3);
      
    }
 
       
  fin.close();
  fo1.close();
  fo2.close();
  fo3.close();
  
 
}
