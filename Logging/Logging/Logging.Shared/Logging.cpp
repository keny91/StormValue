#include "Logging.h"
#include "qthreads.h"




static void WriteLine(logdata* dataLog, char * input) 
{
	fprintf(MessageLogFile, "%s \n", input);
}



void InitLog(char* name, logType log_type)
{

	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	char* alt;
	char buffer[200];
	char*path = ".//logs//";
	char timebuffer[80];

	strftime(timebuffer, 80, "_%Y-%m-%d.", now);

	switch (log_type)
	{
	case ErrorLog:

		strcat(buffer, path);
		strcat(buffer, name);
		strcat(buffer, timebuffer);
		strcat(buffer, ErrorLogName);

		//alt = ;
		ErrorLogFile = fopen(buffer, "w");
		break;

	case WarningLog:
		strcat(buffer, path);
		strcat(buffer, name);
		strcat(buffer, timebuffer);
		strcat(buffer, WarningLogName);

		//alt = ;
		WarningLogFile = fopen(buffer, "w");
		break;

	case MessageLog:
		strcat(buffer, path);
		strcat(buffer, name);
		strcat(buffer, timebuffer);
		strcat(buffer, MessageLogName);

		//alt = ;
		MessageLogFile = fopen(buffer, "w");
		break;

	default:
		break;
	}

};


void InitLogs()
{
	InitLog("debug", ErrorLog);
	InitLog("debug", WarningLog);
	InitLog("debug", MessageLog);

};


void LogMessage(LoggingObjectStruct *log,char * input) {
	FILE* aux = fopen(log->logs[0].filename, "w+");
	if (aux == NULL) {
		// do nothing
	}
	else
	{
		// assing the reference created to
		MessageLogFile = aux;
	}
	QThread_Mutex_P(log->mutex);
	WriteLine(log->logs, strcat("REPORT LOG ENTRY:",input));
	//fprintf(MessageLogFile, "%s \n", input);
#ifdef DEBUG_GE
	//print to console
	printf(MessageLog, "GE_ENGINE_MESSAGE: %s \n", input);
#endif

	QThread_Mutex_V(log->mutex);
};



void LogWarning(LoggingObjectStruct *log, char * input) {
	FILE* aux = fopen(WarningLogName, "w+");
	if (aux == NULL) {
		// do nothing
	}
	else
	{

		// assing the reference created to
		WarningLogFile = aux;
	}
	QThread_Mutex_P(log->mutex);
	WriteLine(log->logs, strcat("WARNING LOG ENTRY:", input));
	QThread_Mutex_V(log->mutex);

};


void LogError(LoggingObjectStruct *log, char * input) {
	FILE* aux = fopen(WarningLogName, "w+");
	if (aux == NULL) {
		// do nothing
	}
	else
	{
		// assing the reference created to 
		ErrorLogFile = aux;
	}
	QThread_Mutex_P(log->mutex);
	WriteLine(log->logs, strcat("ERROR LOG ENTRY:", input));
	QThread_Mutex_V(log->mutex);

};




void CloseFile(FILE* log) {
	fclose(log);
};

void CloseLogFiles() {
	CloseFile(ErrorLogFile);
	CloseFile(MessageLogFile);
	CloseFile(WarningLogFile);
};

