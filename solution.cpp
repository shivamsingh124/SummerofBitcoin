// #Shivam Singh
//https://github.com/shivamsingh124/SummerofBitcoin/tree/main
//Time Complexity O(n^2)

#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


string input = "mempool.csv"; //Input
set<string>included_trx_ids;
int totalBlockWeight = 4000000;  // maximum block weight

class TransactionValue
 {
public:
  string tx_id;
  int fee;
  int weight;                // To store individual data of input
  vector<string>parent;
};

pair<string, TransactionValue*> addTransactionValue(vector<string>& trxValue)
 {

  auto trx = new TransactionValue(); // trx is the new TransactionValue object
  trx->tx_id = trxValue[0];
  trx->fee = stoi(trxValue[1]);        // String input
  trx->weight = stoi(trxValue[2]);
  vector<string> pid;
  for (int i = 3; i < trxValue.size(); i++) 
  {
    pid.push_back(trxValue[i]);
  }
  trx->parent = pid;
  return
   {
       trxValue[0], trx
  };

}

// read the cvs file
void readCSV(string filename, unordered_map<string, TransactionValue*>& trx) 
{   //O(1)

  ifstream fin(input);
  vector<string> trxDetails;
  string temp, line, word;
  getline(fin, line);
  while (getline(fin, line))
   {
    trxDetails.clear();
    stringstream s(line);
    while (getline(s, word, ','))
     {
      trxDetails.push_back(word);
    }
    pair<string, TransactionValue*> p = addTransactionValue(trxDetails);
    trx[p.first] = p.second;
  }
  fin.close();

  cout << "Total Transactions Read: " << trx.size() << endl;

}

void Out(vector<string>& output, string filename)
 {
  ofstream myfile(filename);
  for (auto s : output)
   {
    myfile << s << endl;
  }
  myfile.close();
}

bool isValidTransaction(TransactionValue* trx, set<string>& included_trx_ids)
 { // To Check All The parent Are Already Present in Block
  for (auto parent : trx->parent)
   {
    if (included_trx_ids.find(parent) == included_trx_ids.end())
      return false;
  }
  return true;
}

int main()
 {
  unordered_map<string, TransactionValue*> trx;
  readCSV(input, trx);
  set<pair<float, TransactionValue*>, greater<pair<float, TransactionValue*>>> transactions;
  set<string> Transaction_included;
  vector<string> output; //OUTPUT
  for (auto i : trx) 
  {
    transactions.insert({(float)i.second->fee / (float)i.second->weight, i.second});    // FEE / WEIGHT RATIO REFERENCE FRACTIONAL KNAPSACK .
  }
  int curBlockW = 0;
  int totalFee = 0;
  while (!transactions.empty() && (curBlockW < totalBlockWeight)) 
  {
    bool flag = false;
    for (auto itr = transactions.begin(); itr != transactions.end(); itr++)
     {
      TransactionValue* currentTransaction = (*itr).second;
      int currFee = currentTransaction->fee;
      int currWeight = currentTransaction->weight;
      if (isValidTransaction(currentTransaction, included_trx_ids) && curBlockW + currWeight <= totalBlockWeight) {
        curBlockW += currWeight;
        Transaction_included.insert(currentTransaction->tx_id);        //Updating
        output.push_back(currentTransaction->tx_id);
        totalFee += currFee;
        transactions.erase(itr);               //Earsing from the current set
        flag = true;
        break;
      }
    }
    if (!flag) 
    {
      break;
    }
  }
  cout << "Total Numbers Of Transactions " << Transaction_included.size() << endl;
  cout << "Total fee in current block: " << totalFee << endl;
  cout << "Total weight in current block: " << curBlockW << endl;
  Out(output, "block.txt");

  return 0;
}