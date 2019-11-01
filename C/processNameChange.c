#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<unistd.h> 
// Here process changes its own name during runtime
int main(int argc, char* argv[]) 
{ 
    char buff[1024]; // Buffer to read lines 
    char new_name[1028]; // Buffer to store new process name 
  
    char *ptr = NULL; 
    FILE *fp  = NULL; 
  
    a: memset(buff,'\0', sizeof(buff)); // Setting the memory with NULLs 
    memset(new_name,'\0', sizeof(new_name)); // Setting the memory with NULLs 
  
    // Introduce constant 3 bytes '123' in the beginning  
    // of every name that we change our process name to.  
    // So that we can at-least easily track our process name 
    // when we check it using ps command. Note that 
    // this is only for practice purpose otherwise there 
    // is no need for a set of constant bytes like these. 
    new_name[0] = '1'; 
    new_name[1] = '2'; 
    new_name[2] = '3'; 
  
    // Run the command 'ps -aef > ps.txt' 
    // This command will store the result of 'ps -aef' in a text file 'ps.txt' 
    // The files would have entries like : 
      // UID        PID  PPID  C STIME TTY          TIME CMD 
      // root         1     0  0 20:49 ?        00:00:00 /sbin/init 
      // root         2     0  0 20:49 ?        00:00:00 [kthreadd] 
      // root         3     2  0 20:49 ?        00:00:00 [migration/0] 
      // root         4     2  0 20:49 ?        00:00:00 [ksoftirqd/0] 
  
    system("/bin/sh -c 'ps -aef > ps.txt'"); 
  
  
    // Open the file 'ps.txt' 
    fp = fopen("ps.txt", "r"); 
  
    if(NULL == fp) 
    { 
        printf("\n File open failed\n"); 
        return -1; 
    } 
  
    // Get each line from file until the whole file is read or some error occurs 
    while(NULL != fgets(buff, sizeof(buff), fp)) 
    { 
        // Search for the character '[' in the line fetched from file. 
        // This is because most of the process names are enclosed in '[' brackets. 
        // For example : 
        // root         2     0  0 20:49 ?        00:00:00 [kthreadd] 
        ptr = strchr(buff, '['); 
  
        unsigned int len = strlen(buff); 
  
        if(NULL == ptr) 
        { 
            // Search for the character '/' in the line fetched from file. 
            // This is because many of the process names are start with '/'. 
            // For example : 
            // root         1     0  0 20:49 ?        00:00:00 /sbin/init 
            ptr = strchr(buff, '/'); 
        } 
        if(NULL != ptr) 
        { 
            // If any one of '[' or '/' is found than copy the complete process 
            // name in the buffer which already holds 123 as its first three bytes. 
            // Make sure that you do not overwrite the first three bytes of the buffer 
            // new_name which contains 123 as its first three bytes 
            strncat((new_name+3), ptr, ((buff + len-1) - ptr)); 
        } 
        else 
        { 
            // If the line fetched does not contain either of '[' or '/' 
            // Then use a default process name '/bin/bash' 
            ptr = "/bin/bash"; 
            strncpy((new_name+3), ptr, strlen(ptr)); 
        } 
  
        // Since by now we have the new_name buffer filled with 
        // new process name so copy this name to arg[0] so as to  
        // change our process name.   
        strncpy(argv[0], new_name, sizeof(new_name)); 
  
        printf("\n %s \n", new_name); 
  
        //A delay of eight seconds so that you can run the command 'ps -aef' 
        // and check the new name of our process. :-) 
        sleep(8); 
          
        //Time to fetch a new line from ps.txt so just reset 
        // the buffer new_name with NULL bytes except the first 
        // three bytes which are '123'.  
        memset((new_name+3),'\0', sizeof(new_name)); 
    } 
  
    // Seems like either we are done reading all the lines 
    // from ps.txt or fgets() encountered some error. In either 
    // of the case, just close the file descriptor 
    fclose(fp); 
  
    // Since we do not want to stop even now, so lets re run the 
    // whole cycle again from running the command 'ps -aef > ps.txt' 
    // to reading each line using fgets() and changing the our process 
    // name accordingly 
    goto a; 
  
    return 0; 
} 
