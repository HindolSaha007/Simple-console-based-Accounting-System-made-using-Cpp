#include<iostream>
#include<string.h>
#include<iomanip>
#include<cmath>
#include<fstream>

using namespace std;

class Journal
{
	protected:
		struct journal_line
		{
			char date[11];
			char dr_entry[30];
			double amount;
			char cr_entry[30];
			char narration[30];	
		}jl;
	
	public:
		//Entering a new Journal entry
		void new_journal_entry()
		{
			double test=0;
			ofstream write("Journal Entries.bin", ios::binary | ios::app);
			
			cout<<"\nEnter the date(dd-mm-yyyy): ";
			cin.getline(jl.date, 11);
			
			cout<<"Enter the Debit side entry: ";
			cin.getline(jl.dr_entry, 30);
			
			cout<<"Enter the Credit side entry: ";
			cin.getline(jl.cr_entry, 30);
			
			cout<<"Enter the amount: ";
			cin>>test;
			if (test < 0 || test == 0)
			{
				cout<<"\nAmount cannot be negative or zero.";
				return;
			}
			else
			{
				jl.amount = test;
				cin.ignore();
			}
			
			cout<<"Enter the narration('Being' prefix is necessary): ";
			cin.getline(jl.narration, 30);
			
			write.write((char*)&jl, sizeof(Journal));
			cout<<"\nEntry successfully stored.";
		}
		
		//Displaying the Journal
		void display_journal()
		{
			ifstream read("Journal Entries.bin", ios::binary);
			
			if (!read)
			{
				cout<<"\nOops it seems there's no entry.";
			}
			else
			{
				cout<<fixed<<setprecision(2);
				cout<<"\n\n";
				cout<<left<<setw(12)<<"Date"
				<<left<<setw(30)<<"Particulars"
				<<right<<setw(10)<<"Dr."
				<<right<<setw(10)<<"Cr.";
				cout<<"\n__________________________________________________________________\n\n";
				
				while(read.read((char*)this, sizeof(Journal)))
				{
					cout<<left<<setw(12)<<jl.date
					<<left<<setw(30)<<jl.dr_entry
					<<right<<setw(13)<<jl.amount<<"\n";
					
					cout<<left<<setw(12)<<""
					<<left<<"  To "<<setw(30)<<jl.cr_entry
					<<right<<setw(18)<<jl.amount<<"\n";
					
					cout<<left<<setw(12)<<""
					<<left<<setw(30)<<jl.narration;
					
					cout<<"\n\n";
				}
			}
			read.close();
		}
};

class Ledger : public Journal
{
	protected:
		struct Ledger_line
		{
			char acc_name[30];
			char date[11];
			char particulars[30];
			bool is_debit;
			double amt;
		}ll;
	
	public:
		//Defining Ledger line
		void ledger_define()
		{
			ifstream read("Journal Entries.bin", ios::binary);
			
			if (!read)
			{
				cout<<"\nNo entries to post";
			}
			else
			{
				ofstream write("Ledger Acs.bin",ios::binary | ios::app);
				while(read.read((char*)&jl, sizeof(jl)))
				{
					//Debited A/cs
				
					strcpy(ll.acc_name, jl.dr_entry);
					strcpy(ll.date, jl.date);
					strcpy(ll.particulars, jl.cr_entry);
					ll.amt = jl.amount;
					ll.is_debit = true;
					write.write((char*)&ll, sizeof(ll));
					
					//Credited A/cs
					
					strcpy(ll.acc_name, jl.cr_entry);
					strcpy(ll.particulars, jl.dr_entry);
					ll.is_debit = false;
					write.write((char*)&ll, sizeof(ll));
				}
				write.close();
			}
			read.close();
		}
		
		//Check availability of an Account
		int is_avl(char x[30])
		{
			ifstream read("Ledger Acs.bin", ios::binary);
			
			while(read.read((char*)&ll, sizeof(ll)))
			{
				if (strcmp(x, ll.acc_name)==0)
				{
					return 1;
				}
			}
			read.close();
			return 0;
		}
		
		//Create and search any particular Account
		void search_and_create_acc()
		{
			char name_of_acc[30];
			int v;
			double dr_total=0, cr_total=0, balance=0;
			
			cout<<"\nEnter the name of the Account: ";
			cin.getline(name_of_acc, 30);
			
			ifstream read("Ledger Acs.bin", ios::binary);
			if (!read)
			{
				cout<<"\nThere's no entries.";
			}
			else
			{
				v = is_avl(name_of_acc);
				
				if (v==1)
				{	
					cout<<fixed<<setprecision(2);
						
					cout<<"\n\n";	
					cout<<left<<setw(25)<<"Dr."
					<<right<<setw(85)<<"Cr."
					<<"\n__________________________________________________________________________________________________________________"
					<<"\n"<<left<<setw(12)<<"Date"
					<<left<<setw(25)<<"Particulars"
					<<right<<setw(15)<<"Amount"<<"    |"
					<<left<<setw(12)<<"Date"
					<<left<<setw(25)<<"Particulars"
					<<right<<setw(15)<<"Amount"<<"\n";
					cout<<"__________________________________________________________________________________________________________________\n";
					
					while(read.read((char*)&ll, sizeof(ll)))
					{
						if (strcmp(ll.acc_name, name_of_acc)==0)
						{
							if (ll.is_debit==true)
							{
								cout<<left<<setw(12)<<ll.date
								<<left<<"To "<<setw(25)<<ll.particulars
								<<right<<setw(15)<<ll.amt;
								cout<<"\n";
								dr_total = dr_total+ll.amt;
							}
							else
							{
								cout<<left<<setw(12)<<""
								<<left<<setw(25)<<""
								<<left<<setw(15)<<""<<"     "
								
								<<left<<setw(12)<<ll.date
								<<left<<"By "<<setw(25)<<ll.particulars
								<<right<<setw(15)<<ll.amt;
								cout<<"\n";
								cr_total = cr_total+ll.amt;
							}
						}
					}
					if (dr_total > cr_total)
					{
						balance = dr_total - cr_total;
						cout<<left<<setw(12)<<""
						<<left<<setw(25)<<""
						<<left<<setw(15)<<""<<"     "
						
						<<left<<setw(12)<<""
						<<left<<"By Balance c/d"<<setw(20)<<""
						<<left<<setw(15)<<balance;
						cout<<"\n";
						
						cout<<"__________________________________________________________________________________________________________________\n";
						
						cout<<left<<setw(12)<<""
						<<left<<setw(25)<<""<<"    "
						<<right<<setw(15)<<dr_total;
						
						cout<<left<<setw(12)<<""
						<<left<<setw(25)<<""<<"    "
						<<right<<setw(15)<<dr_total;
					}
					
					else if (dr_total < cr_total)
					{
						balance = cr_total - dr_total;
						cout<<left<<setw(12)<<""
						<<left<<"To Balance b/d"<<setw(20)<<""
						<<left<<setw(15)<<balance;
						
						cout<<"\n__________________________________________________________________________________________________________________\n";
						
						cout<<left<<setw(12)<<""
						<<left<<setw(25)<<""<<"    "
						<<right<<setw(15)<<cr_total;
						
						cout<<left<<setw(12)<<""
						<<left<<setw(25)<<""<<"    "
						<<right<<setw(15)<<cr_total;
					}
				}
				
				else
				{
					cout<<"\nThere's no account as such";
				}
			}
			read.close();
			remove("Ledger Acs.bin");
		}
};

//Trial Balance
class trial_balance : public Journal
{
	protected:
		struct t_b
		{
			char name_of_the_acc[30];
			double dr_amt;
			double cr_amt;
		}trial;
		
	public:
		//Transfering all Dr entries
		void t_balance_dr()
		{
			ofstream write("Trial balance.bin", ios::binary | ios::trunc);
			write.close();
			ifstream read("Journal Entries.bin", ios::binary);
			
			while (read.read((char*)&jl, sizeof(jl)))
			{
				bool found = false;
				
				ifstream read2("Trial balance.bin", ios::binary);
				while(read2.read((char*)&trial, sizeof(trial)))
				{
					if (strcmp(jl.dr_entry, trial.name_of_the_acc)==0)
					{
						found = true;
						break;
					}
				}
				read2.close();
				
				if(!found)
					{
						ofstream write("Trial balance.bin", ios::binary | ios::app);
						strcpy(trial.name_of_the_acc, jl.dr_entry);
						trial.dr_amt = 0;
						trial.cr_amt = 0;
						write.write((char*)&trial, sizeof(trial));
						write.close();
					}
			}
		}
		
		//Transfering all Cr entries
		void t_balance_cr()
		{
			ofstream w("Trial balance.bin", ios::binary | ios::app);
			w.close();
			ifstream r("Journal Entries.bin", ios::binary);
			
			while (r.read((char*)&jl, sizeof(jl)))
			{
				bool found = false;
			
				ifstream r2("Trial balance.bin", ios::binary);
				while(r2.read((char*)&trial, sizeof(trial)))
				{
					if (strcmp(jl.cr_entry, trial.name_of_the_acc)==0)
					{
						found = true;
						break;
					}
				}
				r2.close();
				
				if(!found)
					{
						ofstream w("Trial balance.bin", ios::binary | ios::app);
						strcpy(trial.name_of_the_acc, jl.cr_entry);
						trial.dr_amt = 0;
						trial.cr_amt = 0;
						w.write((char*)&trial, sizeof(trial));
						w.close();
					}
			}
		}
		
		//Displaying the main Trial Balance
		void display_trial_balance()
		{
			char date_of_tb[11];
			cout<<"\nEnter the date on which the Trial Balance is made (dd-mm-yyyy):";
			cin.getline(date_of_tb, 11);

			double gr_ttl_dr=0, gr_ttl_cr=0;
			cout<<fixed<<setprecision(2);
			
			ifstream read("Trial Balance.bin", ios::binary);
			
			if(!read)
			{
				cout<<"\n\nNo data available";
			}
			else
			{
				cout<<"\n\n\t\t\tTrial Balance as on "<<date_of_tb<<"\n";
				cout<<"----------------------------------------------------------------------------\n";
			
				cout<<left<<setw(30)<<"Particulars"
				<<left<<setw(10)<<"Dr        |   "
				<<left<<setw(10)<<"Cr";
				cout<<"\n";
				cout<<"----------------------------------------------------------------------------\n";
				while(read.read((char*)&trial, sizeof(trial)))
				{
					double dr_amt=0, cr_amt=0, total_bl=0, dbl=0;
					
					ifstream read2("Journal Entries.bin", ios::binary);
					while(read2.read((char*)&jl, sizeof(jl)))
					{
						if (strcmp(trial.name_of_the_acc, jl.dr_entry)==0)
						{
							dr_amt = dr_amt + jl.amount;
						}
						
						if (strcmp(trial.name_of_the_acc, jl.cr_entry)==0)
						{
							cr_amt = cr_amt + jl.amount;
						}
					}
					read2.close();
					//Deciding which side the balance goes
					total_bl = dr_amt - cr_amt;
					
					if (total_bl > 0)
					{
						cout<<left<<setw(30)<<trial.name_of_the_acc
						<<left<<setw(10)<<total_bl<<"| "
						<<left<<setw(30)<<""
						<<left<<setw(10)<<"";
						cout<<"\n";
						gr_ttl_dr = gr_ttl_dr + total_bl;
					}
					else if (total_bl < 0)
					{
						cout<<left<<setw(30)<<trial.name_of_the_acc
						<<right<<setw(14)<<"    |   "
						<<left<<setw(10)<<-total_bl;
						cout<<"\n";
						gr_ttl_cr = gr_ttl_cr + abs(total_bl);
					}
				}
				cout<<"----------------------------------------------------------------------------\n";
				cout<<right<<setw(30)<<""
				<<left<<setw(10)<<gr_ttl_dr<<"   "
				<<left<<setw(10)<<gr_ttl_cr;
				cout<<"\n";
				
				cout<<right<<setw(30)<<""
				<<left<<setw(10)<<"=========="<<"   "
				<<left<<setw(10)<<"==========";
			}
			
			read.close();
			remove("Trial balance.bin");
		}
};

int main()
{
	class Journal jr;
	class Ledger ld;
	class trial_balance tb;
	
	int ch;
	
	while(1)
	{
		cout<<"\n\n     =======================\n";
		cout<<"         ACCOUNT MANAGER\n";
		cout<<"     =======================\n";
		cout<<"\n       Main Menu\n       _________\n";
		cout<<"\n       1. Create a new Journal.";
		cout<<"\n       2. Enter new Journal entry.";
		cout<<"\n       3. Display Journal";
		cout<<"\n       4. Check Ledger A/c by name";
		cout<<"\n       5. Display Trial Balance";
		cout<<"\n       6. Exit process";
		cout<<"\n\n      Enter your choice (1-5): ";
		cin>>ch;
		cin.ignore();
		
		switch(ch)
		{
			case 1:
				{
					char ch1[1];
					cout<<"\nCreating a new Journal will delete the previous records.\nDo you still want to continue? (Y / N): ";
					cin>>ch1;
					cin.ignore();
					if (strcmp(ch1, "Y")==0)
					{
						remove("Journal Entries.bin");
						jr.new_journal_entry();
					}
					else if (strcmp(ch1, "N")==0)
					{
						continue;
					}
					else
					{
						cout<<"Choose from (Y / N)";
					}
				}
				break;
				
			case 2:
				{
					ld.new_journal_entry();
				}
				break;
				
			case 3:
				{
					ld.display_journal();
				}
				break;
				
			case 4:
				{
					ld.ledger_define();
					ld.search_and_create_acc();
				}
				break;
				
			case 5:
				{
					tb.t_balance_dr();
					tb.t_balance_cr();
					tb.display_trial_balance();
				}
			break;
				
			case 6:
			{
				cout<<"\n\nProcess exited successfully.";
				exit(1);
			}
			break;
				
			default:
				{
					cout<<"\n\tWrong choice.\n\tChoose from option(1-5)";
				}
				break;
		}
	}
}
