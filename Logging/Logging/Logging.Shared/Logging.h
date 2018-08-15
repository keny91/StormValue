#ifndef _LOGGING_H_
#define _LOGGING_H_ 

#include <stdio.h>
#include <fstream>
#include <time.h>
using namespace std;

#define ErrorLogName "error_log.txt"
#define WarningLogName "warning_log.txt"
#define MessageLogName "message_log.txt"

#define LoggingPath

//using std;
FILE* ErrorLogFile;
FILE* WarningLogFile;
FILE* MessageLogFile;



#define MAX_FILENAME_SIZE 512
/** The maximum size of a message in bytes. */
#define LOG_MAX_MESSAGE_SIZE  256
/** length of the longest log header string */
#define LOG_HEADER_MAX_LENGTH  9
/** length of longest time string */
#define LOG_TIME_MAX_LENGHT 25
/** max nof files to log to */
#define LOG_MAX_NOF_LOGS 5



enum logType
{
	ErrorLog, WarningLog, MessageLog
};


typedef struct _logdata
{
	logType logto;
	FILE* fp;
	char filename[MAX_FILENAME_SIZE];
	char filenameWithNr[MAX_FILENAME_SIZE + 3];
	size_t maxFileSize;
	int level; /* levels:  1 - only errors, 2 - errors + warnings, 3 - errors + warnings + notifications */
} logdata;


typedef struct _LoggingObjectStruct
{
	QThread_Mutex mutex;
	logdata logs[LOG_MAX_NOF_LOGS];
	int nofLogs;
	int prefixWithTime;
	int useLocalTime;
#if defined PLATFORM_Win32 && defined SUPPORT_EVENT_LOG
	gcroot<System::Diagnostics::EventLog^> systemLog;
#endif
} LoggingObjectStruct;


extern void InitLog(char* name, logType log_type);

extern void InitLogs();

extern void LogMessage(char * input);

extern void LogWarning(char * input);

extern void LogError(char * input);

extern void CloseFile(FILE* log);

extern void CloseLogFiles();


#endif