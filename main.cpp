#include "main.h"

// Declaring the Process Control Block Table
vector <pair <pid_t, pair<string, string> > > pcb;


/*
Function to trim the whitespaces in the end of the string input if there is any

Note: The return statement in the function was picked from online resource. Link is in the readme file
*/
string trailtrim(const string &s) {
    return regex_replace(s, regex("\\s+$"), string(""));
} 


/*
Function to store the string input in vector

Note: The function idea is picked from the online resource. Link in the readme file
*/
vector <string> store (string s){
    string word ;
    vector <string> args;
    for (int i = 0; i < s.length(); i++){
        if (s[i] == ' ' && word.size() !=0){
            args.push_back(word);
            word = "";
            
        }else{
            word += s[i];
            
        }
        
    }
    args.push_back(word);
    return args;
}

// Function to check the edge cases of number of arguments and length of each arguement
bool checkargs(vector <string> &vec){
    int MAX_ARGS {7};
    if (vec.size() > MAX_ARGS){
        cerr << "Maximum number of arguements exceeded" << endl;
        return false;
    }
    int MAX_LENGTH {20};
    for (auto i : vec){
        if (i.length() > MAX_LENGTH){
            cerr << "Invalid arguement" << endl;
            return false;
        }
    }
    
    return true;
}


// Function to check if process exist in pcb table
pid_t get_process_from_pcb (pid_t pid){
    for (auto i : pcb ){
        if (i.first == pid){
            return i.first;

        }
    }
    return 0;
}

// function used to change the status of the process in the pcb
void status_change(pid_t pid, string status){
    for (int i = 0; i < pcb.size(); i++){
        if (pcb[i].first == pid){
            pcb[i].second.second = status; 
        }
    }
}

// function to erase the process 
void erase (pid_t pid){
    for (int i = 0; i < pcb.size(); i++){
        if (pcb[i].first == pid){
            pcb.erase(pcb.begin()+i);
        }
    }
}


/*
Function to convert vector<string> to vector<char*> 

Note: The function idea is picked from the online resource. Link in the readme file
*/
vector<char *> to_char_vector(vector<string> &vec) {
    vector<char *> char_vector = vector<char *>(vec.size(),nullptr);

    for (int i = 0; i < vec.size(); i++){
        char_vector[i] = &vec[i][0];
    }

    return char_vector;
}


// function to check the if there is any output file to write information and storing it in string
string outfile (vector <string> &vec){
    string o;
    for (int i = 0; i < vec.size(); i++){
        string s = vec[i];
        if (s[0] == '>'){
            
            s.erase(0,1);
            vec.erase(vec.begin()+i);
            return s;
            
        }
    }
    return "null";
}


// Function to check if the process needs to go to backgroung
bool backgroundcheck(vector <string> &vec){
    if (vec.back() == "&"){
        return true;
    }
    return false;
}

/*
Function to get the input from the file if there is any and store it back in the vector

Note: The function code was picked from online resource. Link is in the readme file
*/
void get_input_from_file(string fileName, vector <string> &vec)
{
    vector <string> co;
    // Open the File
    ifstream in_file(fileName.c_str());
    if(!in_file)
    {
        std::cerr << "Cannot open the File : "<<fileName<<std::endl;
    }
    string str;
    // Read the next line from the input file until end
    while (getline(in_file, str))
    {
        if(str.size() > 0){
            co = store(str);
            for (int i = 0; i < co.size(); i++){
                vec.push_back(co[i]);
            }
        }
    }
    //Close The File
    in_file.close();
}

// Function which contains all the arguements
vector <string> args (vector<string> &vec){
    vector <string> x;
    for (int i = 0; i < vec.size(); i++){
        string s = vec[i];
        if (s[0] == '<'){
            s.erase(0,1);
            get_input_from_file(s,x);
        
        }else{
            x.push_back(s);
        }
        
        
    }
    if (backgroundcheck(x)){
        x.pop_back();
    }
    return x;
}

// if any miscellanous commands come in as input than new process is created
void newprocess (vector <string> &vec){

    // PCB Maximum entries capacity
    if (pcb.size() > 32){
        cerr << "Maximum Entries in the process control table exceeded"<< endl;
        return;

    }

    string x = outfile (vec);
    const char* output_file = x.c_str();
    vector<string> ar = args(vec);
    vector<char *> args = to_char_vector(ar);
    // Forking the process
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    }else if (pid == 0) {
        // Child Process
        char *argv[args.size() + 1];
        for (int i = 0; i < args.size(); i++) {
            argv[i] = args[i];
        }
        argv[args.size()] = nullptr;

        
        if (x != "null") {
            freopen(output_file, "w", stdout);
        }

        if (execvp(argv[0], argv) < 0) {
            printf("execvp error\n");
            _exit(1);
        }

    } else {
        // parent process
        pcb.push_back(make_pair(pid,make_pair(args[0],"R")));
        if(!backgroundcheck(vec)){
            waitpid(pid,nullptr,0);
            erase(pid);
        }
        
    }

    

}


void kill_command(pid_t pid){
    pid_t process = get_process_from_pcb(pid);
    if(process == 0){
        cout << "No process found to be killed" << endl;
    }else{
        //Resume the process so it can be terminated if it was in suspended state
        kill(pid, SIGCONT); 
        //Wait for the process to complete the execution and than kill it
        waitpid(pid, nullptr, 0);
        kill(pid, SIGKILL);
        erase(pid);
    }

    
}

void sleep_command(int sec){
    sleep(sec);
}


void suspend_command(pid_t pid){
    pid_t process = get_process_from_pcb(pid);
    if(process == 0){
        cout << "No process found to be suspended" << endl;
    }else{
        kill(pid,SIGSTOP);
        status_change(pid,"S");
    }
}


void resume_command(pid_t pid){
    pid_t process = get_process_from_pcb(pid);
    if(process == 0){
        cout << "No process found to be resumed" << endl;
    }else{
        kill(pid,SIGCONT);
        status_change(pid,"R");
    }
}


void wait_command(pid_t pid){
    pid_t process = get_process_from_pcb(pid);
    if (process == 0){
        cout << "No child process found to be waited for" << endl;
    }else {
        pid_t wait_t = waitpid(process,nullptr,0);
        if (wait_t > 0){
            erase(pid);
        }
    }
}

// Function to get user time and sys time
void usage(){
    struct rusage u {};
    getrusage(RUSAGE_CHILDREN, &u);
    int user_time = u.ru_utime.tv_sec;
    int sys_time = u.ru_stime.tv_sec;
    cout << "Resources used" << endl;
    cout << "User time = " << user_time << " seconds" << endl;
    cout << "Sys time = " << sys_time << " seconds" << endl;
}


int rusage_for_pid(pid_t pid){

    int pid_time;
    char buf[100];
    char command[100];
    sprintf(command, "ps -p %d -o time", pid);
    FILE *fp;
    fp = popen(command,"r");
    if ( fp == NULL){
        perror("Cannot open the pipe/n");
        return 0;
    }
    while (fgets(buf, 100, fp) != NULL){
        pid_time = atoi(buf);
    }
    pclose(fp);
    return pid_time;
}

void exit_command(){
    for (auto i : pcb){
        kill_command(i.first);
    }
    cout << "\n";
    usage();
    _exit(0);

}

void jobs_command(){
    cout << "\nRunning Process" << endl;
    if (pcb.size()==0){
        cout << "Processes =      0 active" << endl;
    }else{
        cout << "#      PID S SEC COMMAND" << endl;
        int S_NO {0};
        for (int i = 0; i < pcb.size(); i++){
            pid_t pid = pcb[i].first;
            string command = pcb[i].second.first;
            string status = pcb[i].second.second;
            int pid_time = rusage_for_pid(pid);
            cout << S_NO << ":  " << pid << " " << status << "    " << pid_time << " " << command << endl;
            S_NO++;
        }
        cout << "Processes =      " << pcb.size() << " active" << endl;

    }
    usage();
}



void commandcall(string line){
    string trimmed_input = trailtrim(line);
    vector <string> args = store(trimmed_input);

    if (checkargs(args)){
        string command = args.at(0);
        if (command == "kill"){
            kill_command(stoi(args.at(1)));
        } else if (command == "jobs"){
            jobs_command();
        } else if (command == "exit"){
            exit_command();
        } else if (command ==  "wait"){ 
            wait_command(stoi(args.at(1)));
        } else if (command == "resume"){
            resume_command(stoi(args.at(1)));
        } else if (command == "sleep"){
            int sec = stoi(args.at(1));
            sleep_command(sec);
        } else if (command == "suspend"){
            suspend_command(stoi(args.at(1)));
        } else if (!command.empty()){
            newprocess(args);
            
        }
    }

}

// signal handling to remove the completed background process from the pcb
void backgroundsignalhandle(int sig){
    pid_t pid = waitpid(-1,nullptr,WNOHANG);
    erase(pid);

}

void handlesignal() {

    signal(SIGCHLD, &backgroundsignalhandle);
}

int main(){

    handlesignal();



    int MAX_LENGTH {100};

    for (;;){
        cout << "CMPUT 379: ";
        string line;
        getline(cin,line);
        if(line.length() > MAX_LENGTH){
            cerr << "please type the input again" << endl;
        }else{
            commandcall(line);
            
        }
    }
    return 0;
}