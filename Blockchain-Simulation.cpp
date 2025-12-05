#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ========================
// 1. WALLET CLASS
// ========================

class Wallet 
{

private:

    string username;

    string password;

    string walletAddress;

    string seedPhrase;
    
    string tokenNames[5] = {"BTC", "ETH", "SOL", "SUI", "TON"};

    string tokenNetworks[5] = {"Bitcoin mainnet", "ERC-20", "SPL", "SUI mainnet", "Jettons"};

    double tokenAmounts[5] = {0.00044, 0.0067, 0.18, 9.0, 4.43};

    double tokenPrices[5];

    double basePrices[5] = {90909.0, 2985.0, 83.0, 2.0, 1.58};

    double gasFees[5];
    
    double getRandomChange() 
    {
        static int counter = 0;
        counter++;

        return ((counter * 13) % 2000 - 1000) / 10000.0;
    }
    
    string generateAddress(string name) 
    {
        string addr = "addr_";

        size_t nameLength = name.length();


        size_t maxChars = (nameLength < 5) ? nameLength : 5;

        for (size_t i = 0; i < maxChars; i++)
        {
            addr += name[i];
        }

        addr += "_wallet";

        return addr;
    }
    
    string generateSeedPhrase() 
    {
        string seed = "";

        string words[] = {"alpha", "beta", "gamma", "delta", "echo", "foxtrot", "golf", "hotel"};

        for (int i = 0; i < 8; i++) 
        {

            seed += words[i];

            if (i < 7) seed += " ";

        }
        return seed;
    }
    
    void updatePrices() 
    {
        for (int i = 0; i < 5; i++) 
        {
            double changePercent = getRandomChange();
            tokenPrices[i] = basePrices[i] * (1.0 + changePercent);
        }
    }
    
    double calculateTotal() 
    {
        double total = 0;
        for (int i = 0; i < 5; i++) 
        {
            total += tokenAmounts[i] * tokenPrices[i];
        }

        return total;
    }
    
    double calculatePercentageChange(double currentTotal) 
    {
        return ((currentTotal - 100.0) / 100.0) * 100.0;
    }

public:

    Wallet() 
    {
        username = "";
        password = "";
        walletAddress = "";
        seedPhrase = "";
        
        gasFees[0] = 0.00001;
        gasFees[1] = 0.0001;
        gasFees[2] = 0.001;
        gasFees[3] = 0.1;
        gasFees[4] = 0.05;
        
        for (int i = 0; i < 5; i++) {
            tokenPrices[i] = basePrices[i];
        }
    }
    
    Wallet(string uname, double btc, double eth, double sol, double sui, double ton) 
    {
        username = "default";
        walletAddress = generateAddress(uname);
        password = "default123";
        seedPhrase = generateSeedPhrase();
        
        tokenAmounts[0] = btc;
        tokenAmounts[1] = eth;
        tokenAmounts[2] = sol;
        tokenAmounts[3] = sui;
        tokenAmounts[4] = ton;
        
        gasFees[0] = 0.00001;
        gasFees[1] = 0.0001;
        gasFees[2] = 0.001;
        gasFees[3] = 0.1;
        gasFees[4] = 0.05;
        
        for (int i = 0; i < 5; i++) 
        {
            tokenPrices[i] = basePrices[i];
        }
    }
    
    void createUserWallet(string uname, string pwd) 
    {
        username = uname;
        password = pwd;
        walletAddress = generateAddress(uname);
        seedPhrase = generateSeedPhrase();
        
        gasFees[0] = 0.00001;
        gasFees[1] = 0.0001;
        gasFees[2] = 0.001;
        gasFees[3] = 0.1;
        gasFees[4] = 0.05;
        
        for (int i = 0; i < 5; i++) 
        {
            tokenPrices[i] = basePrices[i];
        }
    }
    
    bool login(string addr, string pwd) 
    {
        return (walletAddress == addr && password == pwd);
    }
    
    bool recoverWithSeed(string inputSeed) 
    {
        return (seedPhrase == inputSeed);
    }
    
    void resetPassword(string newPwd) 
    {
        password = newPwd;
    }
    
    bool hasEnoughTokens(int tokenIndex, double amount) 
    {

        if (tokenIndex < 0 || tokenIndex >= 5) return false;
        return tokenAmounts[tokenIndex] >= amount;
    }
    
    int getTokenIndex(string tokenName) 
    {
        for (int i = 0; i < 5; i++) 
        {
            if (tokenNames[i] == tokenName) 
            {
                return i;
            }
        }

        return -1;
    }
    
    double getGasFee(int tokenIndex) 
    {

        if (tokenIndex >= 0 && tokenIndex < 5) 
        {
            return gasFees[tokenIndex];
        }

        return 0.0;
    }
    
    void deductTokens(int tokenIndex, double amount) 
    {
        if (tokenIndex >= 0 && tokenIndex < 5) 
        {
            tokenAmounts[tokenIndex] -= amount;
        }
    }
    
    void showDashboard() 
    {

        updatePrices();

        double total = calculateTotal();

        double percentChange = calculatePercentageChange(total);
        
        cout << "\n=== WALLET DASHBOARD ===" << endl;
        cout << "Username: " << username << endl;
        cout << "Address: " << walletAddress << endl;
        cout << "Total Balance: $" << fixed << setprecision(2) << total;
        
        if (percentChange >= 0) 
        {

            cout << " (+" << setprecision(1) << percentChange << "%)" << endl;

        } 
        
        else 
        {
            cout << " (" << setprecision(1) << percentChange << "%)" << endl;
        }
        
        cout << "\nToken         Amount       Price/Token   24h Change    Value" << endl;
        cout << "-------------------------------------------------------------" << endl;
        
        for (int i = 0; i < 5; i++) 
        {
            double value = tokenAmounts[i] * tokenPrices[i];

            double priceChange = ((tokenPrices[i] - basePrices[i]) / basePrices[i]) * 100.0;
            
            cout << left << setw(10) << tokenNames[i] << "  ";
            
            if (tokenNames[i] == "BTC" || tokenNames[i] == "ETH") 
            {
                cout << setw(12) << fixed << setprecision(5) << tokenAmounts[i];
            } 
            
            else 
            {
                cout << setw(12) << fixed << setprecision(2) << tokenAmounts[i];
            }
            
            cout << "  $" << setw(11) << fixed << setprecision(0) << tokenPrices[i];

            cout << "  ";
            if (priceChange >= 0) 
            {
                cout << "+" << setprecision(1) << priceChange << "%";
            } 
            
            else 
            {
                cout << setprecision(1) << priceChange << "%";
            }

            cout << "      $" << fixed << setprecision(2) << value << endl;
        }

        cout << endl;
    }
    
    void showCreationDetails() 
    {
        cout << "\n*** WALLET CREATED! ***" << endl;

        cout << "Username: " << username << endl;

        cout << "Address: " << walletAddress << endl;

        cout << "\n!!! IMPORTANT: SAVE YOUR SEED PHRASE !!!" << endl;

        cout << "Seed Phrase: " << seedPhrase << endl;

        cout << "!!! You will need this to recover your wallet!\n" << endl;
    }
    
    string getAddress() 
    { return walletAddress; }

    string getUsername() 
    { 
        return username; 
    }

    string getTokenName(int index) 
    { 
        if (index >= 0 && index < 5) return tokenNames[index];
        return "";
    }

    string getTokenNetwork(int index) 
    { 
        if (index >= 0 && index < 5) return tokenNetworks[index];

        return "";
    }

    double getTokenAmount(int index) 
    { 
        if (index >= 0 && index < 5) return tokenAmounts[index];
        return 0.0;
    }

    bool isEmpty() {
        return username.empty();
    }
};

// ========================
// 2. TRANSACTION CLASS
// ========================

class Transaction 
{
public:

    string sender;
    string receiver;
    string token;
    string network;
    double amount;
    double gasFee;
    string status;
    string txID;
    int blockNumber;
    
    Transaction() 
    {
        sender = "";
        receiver = "";
        token = "";
        network = "";
        amount = 0.0;
        gasFee = 0.0;
        status = "Pending";
        txID = "";
        blockNumber = -1;
    }
    
    Transaction(string s, string r, string t, string n, double a, double g, string stat = "Pending", int block = -1) 
    {
        sender = s;
        receiver = r;
        token = t;
        network = n;
        amount = a;
        gasFee = g;
        status = stat;
        blockNumber = block;
        
        static int txCounter = 1000;
        txID = "TX" + to_string(txCounter++);
    }
    
    void display() 
    {
        cout << "[" << txID << "] " << sender << " -> " << receiver << endl;
        cout << "   Token: " << token << " | Network: " << network << endl;
        cout << "   Amount: " << fixed << setprecision(6) << amount << " " << token;
        cout << " | Gas Fee: " << fixed << setprecision(6) << gasFee << " " << token << endl;
        cout << "   Status: " << status;

        if (status == "Confirmed") 
        {
            cout << " (Block: " << blockNumber << ")";
        }

        cout << "\n" << endl;
    }
};

// ========================
// 3. BLOCK CLASS
// ========================

class Block 
{
public:

    int blockNumber;
    Transaction transaction;
    string prevHash;
    string currentHash;
    string timestamp;
    
    Block() 
    {
        blockNumber = 0;
        transaction = Transaction();
        prevHash = "";
        currentHash = "";
        timestamp = "";
    }
    
    Block(int num, Transaction tx, string prev = "") 
    {
        blockNumber = num;
        transaction = tx;
        prevHash = prev;
        currentHash = calculateHash();
        timestamp = "2024-12-01 10:00:00";
    }
    
    string calculateHash() 
    {
        return "Block_" + to_string(blockNumber) + "_Hash";
    }
    
    void display() 
    {
        cout << "=== BLOCK " << blockNumber << " ===" << endl;
        cout << "Hash: " << currentHash << endl;
        cout << "Previous Hash: " << prevHash << endl;
        cout << "Timestamp: " << timestamp << endl;
        cout << "Transaction: " << transaction.txID << endl;
        cout << "-----------------------------------" << endl;
    }
};

// ========================
// 4. BLOCKCHAIN LINKED LIST
// ========================

struct BlockNode 
{
    Block block;
    BlockNode* next;
    
    BlockNode(Block b) : block(b), next(nullptr) {}
};

class Blockchain 
{
private:

    BlockNode* head;
    BlockNode* tail;
    int blockCount;
    
public:
    Blockchain() {
        head = nullptr;
        tail = nullptr;
        blockCount = 0;
    }
    
    void addBlock(Transaction tx) {
        blockCount++;
        string prevHash = (tail == nullptr) ? "Genesis" : tail->block.currentHash;
        Block newBlock(blockCount, tx, prevHash);
        
        BlockNode* newNode = new BlockNode(newBlock);
        
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        
        tx.blockNumber = blockCount;
        tx.status = "Confirmed";
    }
    
    void displayChain() 
    {
        cout << "\n=== BLOCKCHAIN ===" << endl;
        cout << "[NOTE: Real blockchain uses mining pools, not FIFO queues." << endl;
        cout << " This is a simplified simulation for learning.]\n" << endl;
        
        if (head == nullptr) {
            cout << "Blockchain is empty (no blocks mined yet)." << endl;
            return;
        }
        
        BlockNode* current = head;
        while (current != nullptr) {
            current->block.display();
            current = current->next;
        }
        cout << "Total Blocks: " << blockCount << endl;
    }
    
    int getBlockCount() {
        return blockCount;
    }
};

// ========================
// 5. MINER CLASS
// ========================

class Miner 
{
private:

    int minerID;
    
public:

    Miner(int id) 
    {
        minerID = id;
    }
    
    bool validateBalance(Wallet* sender, string tokenName, double amount, double gasFee) {

        int tokenIndex = sender->getTokenIndex(tokenName);

        if (tokenIndex == -1) 
        {
            return false;
        }

        return sender->hasEnoughTokens(tokenIndex, amount + gasFee);
    }
    
    bool validateRecipient(string address, Wallet wallets[]) 
    {
        for (int i = 0; i < 4; i++) 
        {
            if (wallets[i].getAddress() == address) 
            {
                return true;
            }
        }

        return false;
    }
    
    void broadcast(string message) 
    {
        cout << "   Miner " << minerID << ": " << message << endl;
    }
    
    bool voteOnTransaction(Transaction tx, Wallet* sender, Wallet wallets[]) 
    {
        cout << "   Miner " << minerID << ": ";
        
        bool balanceOK = validateBalance(sender, tx.token, tx.amount, tx.gasFee);
        if (!balanceOK) 
        {
            cout << "[FAIL] Insufficient " << tx.token << " balance" << endl;
            return false;
        }

        cout << "[OK] Balance check passed" << endl;
        
        bool recipientOK = validateRecipient(tx.receiver, wallets);

        if (!recipientOK) 
        {
            cout << "[FAIL] Invalid recipient address" << endl;
            return false;
        }

        cout << "[OK] Recipient valid" << endl;
        
        return true;
    }
};

// ========================
// 6. TRANSACTION QUEUE (FIFO)
// ========================

class TransactionQueue 
{
private:

    Transaction* queue[10];
    int front, rear, size;
    
public:

    TransactionQueue() 
    {
        front = 0;
        rear = -1;
        size = 0;
        initializeDummyTransactions();
    }
    
    void initializeDummyTransactions() 
    {
        enqueue(new Transaction("addr_Alice_wallet", "addr_Bob_wallet", 

                               "BTC", "Bitcoin mainnet", 0.0001, 0.00001, "Pending"));

        enqueue(new Transaction("addr_Bob_wallet", "addr_Charlie_wallet", 

                               "ETH", "ERC-20", 0.5, 0.0001, "Pending"));

        enqueue(new Transaction("addr_Charlie_wallet", "addr_Alice_wallet", 

                               "SOL", "SPL", 10.0, 0.001, "Pending"));
    }
    
    void enqueue(Transaction* tx) {
        if (size < 10) 
        {
            rear = (rear + 1) % 10;
            queue[rear] = tx;
            size++;
        }
    }
    
    Transaction* dequeue() 
    {
        if (size == 0) return nullptr;
        
        Transaction* tx = queue[front];

        front = (front + 1) % 10;

        size--;

        return tx;
    }
    
    bool isFull() {
        return size == 10;
    }
    
    bool isEmpty() 
    {
        return size == 0;
    }
    
    void displayQueue() 
    {
        cout << "\n=== TRANSACTION QUEUE (FIFO) ===" << endl;
        cout << "[NOTE: Real blockchain uses mining pools, not FIFO queues." << endl;
        cout << " This is a simplified simulation for learning.]\n" << endl;
        
        if (size == 0) 
        {
            cout << "Queue is empty." << endl;
            return;
        }
        
        int index = front;

        for (int i = 0; i < size; i++) 
        {
            cout << i+1 << ". ";

            if (i < 3) 
            {
                cout << "[DUMMY] ";
            } 
            
            else 
            {
                cout << "[YOURS] ";
            }

            cout << queue[index]->sender << " -> " << queue[index]->receiver;
            cout << ": " << queue[index]->amount << " " << queue[index]->token << endl;
            index = (index + 1) % 10;
        }
    }
    
    int getSize() 
    {
        return size;
    }
    
    void delayWithDots(int seconds) 
    {
        for (int i = 0; i < seconds; i++) 
        {
            cout << ".";

            for (int j = 0; j < 300000000; j++) 
            {
                // Delay loop
            }
        }
    }
    
    void processQueue() 
    {
        cout << "\n[WAIT] Processing 3 dummy transactions in queue..." << endl;
        
        for (int i = 1; i <= 3; i++) 
        {
            cout << "\nProcessing Dummy " << i << " ";
            delayWithDots(5);
            cout << " [OK]" << endl;
            
            Transaction* dummy = dequeue();
            if (dummy) delete dummy;
        }
        
        cout << "\n[MINER] Now processing YOUR transaction..." << endl;
    }
};

// ========================
// 7. WALLET MANAGER
// ========================

class WalletManager 
{
private:

    Wallet wallets[4];

    int userWalletIndex;

    Transaction* transactions[50];

    int transactionCount;

    bool dummyTransactionsAdded;

    TransactionQueue queue;

    Blockchain blockchain;

    Miner miners[3];
    
public:

    WalletManager() : miners{Miner(1), Miner(2), Miner(3)} 
    {
        transactionCount = 0;
        userWalletIndex = -1;
        dummyTransactionsAdded = false;
        
        wallets[0] = Wallet("Alice", 0.001, 0.01, 0.5, 15.0, 10.0);
        wallets[1] = Wallet("Bob", 0.0005, 0.008, 0.3, 12.0, 8.0);
        wallets[2] = Wallet("Charlie", 0.0008, 0.012, 0.4, 18.0, 12.0);
    }
    
    ~WalletManager() 
    {
        for (int i = 0; i < transactionCount; i++) 
        {
            delete transactions[i];
        }
    }
    
    void createUserWallet(string uname, string pwd) 
    {
        wallets[3].createUserWallet(uname, pwd);
        userWalletIndex = 3;
    }
    
    bool loginUserWallet(string addr, string pwd) 
    {
        if (userWalletIndex == -1) return false;
        return wallets[3].login(addr, pwd);
    }
    
    Wallet* getUserWallet() 
    {
        if (userWalletIndex == -1) return nullptr;
        return &wallets[userWalletIndex];
    }
    
    void showRecipientWallets() 
    {
        cout << "\n=== AVAILABLE RECIPIENTS ===" << endl;

        for (int i = 0; i < 3; i++) 
        {
            cout << i+1 << ". " << wallets[i].getUsername() 
                 << " - " << wallets[i].getAddress() << endl;
        }
    }
    
    Wallet* getRecipientWallet(int index) 
    {
        if (index >= 0 && index < 3) 
        {
            return &wallets[index];
        }

        return nullptr;
    }
    
    void addTransaction(Transaction* tx) 
    {
        if (transactionCount < 50) 
        {
            transactions[transactionCount] = tx;
            transactionCount++;
        }
    }
    
    void showTransactionHistory() 
    {
        cout << "\n=== TRANSACTION HISTORY (Confirmed) ===" << endl;

        bool hasConfirmed = false;
        
        for (int i = 0; i < transactionCount; i++) 
        {
            if (transactions[i]->status == "Confirmed") 
            {
                transactions[i]->display();
                hasConfirmed = true;
            }
        }
        
        if (!hasConfirmed) 
        {
            cout << "No confirmed transactions yet." << endl;
        }
    }
    
    bool simulateMining(Transaction* tx, Wallet* sender) 
    {
        cout << "\n[INFO] Transaction Submitted!" << endl;
        
        queue.displayQueue();
        queue.processQueue();
        
        cout << "\n[MINER] Validating with Network..." << endl;
        cout << "\n=== VALIDATOR CHECKS ===" << endl;
        
        bool allValid = true;

        for (int i = 0; i < 3; i++) 
        {
            if (!miners[i].voteOnTransaction(*tx, sender, wallets)) 
            {
                allValid = false;
            }
        }
        
        if (!allValid) 
        {
            cout << "\n[ERROR] Transaction rejected by validators!" << endl;

            return false;
        }
        
        cout << "\n[MINER] Broadcasting to network..." << endl;

        for (int i = 0; i < 3; i++) 
        {
            miners[i].broadcast("Received transaction, beginning mining...");
        }
        
        cout << "\n[MINER] Mining in progress... ";

        queue.delayWithDots(2);

        cout << "\n[   ] Block 1/3" << endl;

        queue.delayWithDots(2);

        cout << "[---] Block 2/3" << endl;
        queue.delayWithDots(2);
        cout << "[---] Block 3/3" << endl;
        queue.delayWithDots(1);
        
        cout << "\n[SUCCESS] Transaction Successful!" << endl;
        
        blockchain.addBlock(*tx);

        tx->status = "Confirmed";
        
        int tokenIndex = sender->getTokenIndex(tx->token);

        if (tokenIndex != -1) 
        {
            double totalDeduct = tx->amount + tx->gasFee;

            sender->deductTokens(tokenIndex, totalDeduct);

            cout << "[BALANCE] Updated: -" << totalDeduct << " " << tx->token << endl;
        }
        
        addTransaction(tx);

        cout << "[HISTORY] Transaction added to history" << endl;
        
        blockchain.displayChain();
        
        return true;
    }
    
    void showBlockchain() 
    {
        blockchain.displayChain();
    }
};

// ========================
// 8. MAIN PROGRAM
// ========================
int main() 
{
    WalletManager manager;

    int choice;

    string username, password, address, seedInput;

    char recoveryChoice;

    int loginAttempts = 0;
    
    cout << "========================================" << endl;

    cout << "   BLOCKCHAIN WALLET SIMULATOR v3.0" << endl;

    cout << "        WITH MINING SYSTEM" << endl;

    cout << "========================================\n" << endl;
    
    while (true) 
    {
        cout << "=== MAIN MENU ===" << endl;
        cout << "1. Create Wallet" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        
        if (choice == 1) {
            cout << "\n=== CREATE WALLET ===" << endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            
            manager.createUserWallet(username, password);
            manager.getUserWallet()->showCreationDetails();
            
        } else if (choice == 2) {
            if (manager.getUserWallet() == nullptr || manager.getUserWallet()->isEmpty()) {
                cout << "\n*** No wallet found! Please create a wallet first. ***\n" << endl;
                continue;
            }
            
            loginAttempts = 0;
            
            while (loginAttempts < 3) {
                cout << "\n=== LOGIN ===" << endl;
                cout << "Enter wallet address: ";
                cin >> address;
                cout << "Enter password: ";
                cin >> password;
                
                if (manager.loginUserWallet(address, password)) {
                    cout << "\n*** LOGIN SUCCESSFUL! ***" << endl;
                    
                    int walletChoice;
                    do {
                        manager.getUserWallet()->showDashboard();
                        
                        cout << "=== WALLET MENU ===" << endl;
                        cout << "1. Refresh Dashboard" << endl;
                        cout << "2. Send Tokens" << endl;
                        cout << "3. Receive Tokens" << endl;
                        cout << "4. View Transaction History" << endl;
                        cout << "5. View Blockchain" << endl;
                        cout << "6. Logout" << endl;
                        cout << "Choice: ";
                        cin >> walletChoice;
                        
                        switch (walletChoice) {
                            case 1:
                                break;
                                
                            case 2: {
                                cout << "\n=== SEND TOKENS ===" << endl;
                                cout << "Select Token:" << endl;
                                for (int i = 0; i < 5; i++) {
                                    cout << i+1 << ". " << manager.getUserWallet()->getTokenName(i)
                                         << " (" << manager.getUserWallet()->getTokenNetwork(i) << ")" << endl;
                                }
                                
                                int tokenChoice;
                                cout << "Choice (1-5): ";
                                cin >> tokenChoice;
                                
                                if (tokenChoice < 1 || tokenChoice > 5) {
                                    cout << "\n*** Invalid token choice! ***\n" << endl;
                                    break;
                                }
                                
                                tokenChoice--;
                                
                                string tokenName = manager.getUserWallet()->getTokenName(tokenChoice);
                                string network = manager.getUserWallet()->getTokenNetwork(tokenChoice);
                                double available = manager.getUserWallet()->getTokenAmount(tokenChoice);
                                double gasFee = manager.getUserWallet()->getGasFee(tokenChoice);
                                
                                cout << "\n--- Token Details ---" << endl;
                                cout << "Selected: " << tokenName << endl;
                                cout << "Network: " << network << endl;
                                cout << "Available: " << fixed << setprecision(6) << available << " " << tokenName << endl;
                                cout << "Network Gas Fee: " << fixed << setprecision(6) << gasFee << " " << tokenName << endl;
                                
                                manager.showRecipientWallets();
                                
                                int recipientChoice;
                                cout << "\nSelect recipient (1-3): ";
                                cin >> recipientChoice;
                                
                                if (recipientChoice < 1 || recipientChoice > 3) {
                                    cout << "\n*** Invalid recipient choice! ***\n" << endl;
                                    break;
                                }
                                
                                Wallet* recipient = manager.getRecipientWallet(recipientChoice - 1);
                                if (!recipient) {
                                    cout << "\n*** Invalid recipient! ***\n" << endl;
                                    break;
                                }
                                
                                string recipientAddress = recipient->getAddress();
                                string recipientName = recipient->getUsername();
                                
                                double amount;
                                cout << "Amount to send: ";
                                cin >> amount;
                                
                                if (amount <= 0) {
                                    cout << "\n*** Amount must be positive! ***\n" << endl;
                                    break;
                                }
                                
                                double totalNeeded = amount + gasFee;
                                if (!manager.getUserWallet()->hasEnoughTokens(tokenChoice, totalNeeded)) {
                                    cout << "\n*** INSUFFICIENT BALANCE! ***" << endl;
                                    cout << "Available: " << fixed << setprecision(6) << available << " " << tokenName << endl;
                                    cout << "Required: " << fixed << setprecision(6) << totalNeeded << " " << tokenName 
                                         << " (amount + gas)" << endl;
                                    break;
                                }
                                
                                cout << "\n=== TRANSACTION PREVIEW ===" << endl;
                                cout << "From:     " << manager.getUserWallet()->getAddress() << endl;
                                cout << "To:       " << recipientName << " (" << recipientAddress << ")" << endl;
                                cout << "Token:    " << tokenName << endl;
                                cout << "Network:  " << network << endl;
                                cout << "-------------------------------------------" << endl;
                                cout << "Amount:   " << fixed << setprecision(6) << amount << " " << tokenName << endl;
                                cout << "Gas Fee:  " << fixed << setprecision(6) << gasFee << " " << tokenName << endl;
                                cout << "-------------------------------------------" << endl;
                                cout << "TOTAL:    " << fixed << setprecision(6) << totalNeeded << " " << tokenName << endl;
                                
                                char confirm;
                                cout << "\nConfirm transaction? [Y/N]: ";
                                cin >> confirm;
                                
                                if (confirm == 'Y' || confirm == 'y') {
                                    Transaction* newTx = new Transaction(
                                        manager.getUserWallet()->getAddress(),
                                        recipientAddress,
                                        tokenName,
                                        network,
                                        amount,
                                        gasFee,
                                        "Pending"
                                    );
                                    
                                    bool success = manager.simulateMining(newTx, manager.getUserWallet());
                                    
                                    if (!success) {
                                        delete newTx;
                                    }
                                } else {
                                    cout << "\n*** Transaction cancelled. ***\n" << endl;
                                }
                                break;
                            }
                                
                            case 3:
                                cout << "\n=== RECEIVE TOKENS ===" << endl;
                                cout << "Your wallet address: " << manager.getUserWallet()->getAddress() << endl;
                                cout << "Share this address to receive tokens!\n" << endl;
                                break;
                                
                            case 4:
                                manager.showTransactionHistory();
                                break;
                                
                            case 5:
                                manager.showBlockchain();
                                break;
                                
                            case 6:
                                cout << "\n*** Logging out... ***\n" << endl;
                                break;
                                
                            default:
                                cout << "\n*** Invalid choice! ***\n" << endl;
                        }
                        
                    } while (walletChoice != 6);
                    
                    break;
                    
                } else {
                    loginAttempts++;
                    cout << "\n*** LOGIN FAILED! (Attempt " << loginAttempts << "/3) ***" << endl;
                    
                    if (loginAttempts < 3) {
                        cout << "Forgot password/address? [R]ecover wallet or [T]ry again? ";
                        cin >> recoveryChoice;
                        
                        if (recoveryChoice == 'R' || recoveryChoice == 'r') {
                            cout << "\n=== WALLET RECOVERY ===" << endl;
                            cout << "Enter your 8-word seed phrase: ";
                            cin.ignore();
                            getline(cin, seedInput);
                            
                            if (manager.getUserWallet()->recoverWithSeed(seedInput)) {
                                cout << "\n*** WALLET RECOVERED! ***" << endl;
                                cout << "Your wallet address: " << manager.getUserWallet()->getAddress() << endl;
                                cout << "Please set a new password: ";
                                cin >> password;
                                
                                manager.getUserWallet()->resetPassword(password);
                                cout << "\n*** Password updated! Please login again. ***\n" << endl;
                                break;
                            } else {
                                cout << "\n*** Invalid seed phrase! Recovery failed. ***\n" << endl;
                                break;
                            }
                        }
                    } else {
                        cout << "\n*** Maximum login attempts reached! ***" << endl;
                        cout << "*** Please use wallet recovery option. ***\n" << endl;
                        break;
                    }
                }
            }
            
        } else if (choice == 3) {
            cout << "\n========================================" << endl;
            cout << "Thank you for using Blockchain Wallet!" << endl;
            cout << "========================================\n" << endl;
            break;
        } else {
            cout << "\n*** Invalid choice! Please select 1-3. ***\n" << endl;
        }
        
        cout << "--------------------\n" << endl;
    }
    
    return 0;
}