#ifndef _RTENGINE_H_
#define _RTENGINE_H_

/*
	WHAT IF:
	
	ANALYSIS STARTS FROM THE END REPLAY. This way we can start analyzing why the final result

*/



/*	TO DO:

. Module system? ReadRegister -> to action analysis -> to grouped action passage -> to macro value
									                                             \-> to micro value 	
.

*/




/*
	Delete as they are defined
*/

extern class RTEngineInstance;

extern class RTPassage;

typedef struct _RTDataStruct {
}
RTDataStruct;


/**
 * The major version number. Different major version numbers may indicate that the
 * interface has changed and a simple re-linking is not sufficient.
 */
#define RTENGINE_LIB_MAJOR           "1"

/** The minor version number. */
#define RTENGINE_LIB_MINOR           "0"

/** The maintenance version number. */
#define RTENGINE_LIB_MAINTENANCE     "0"

/** The version number as a dotted string, for example "1.0.0". */
#define RTENGINE_LIB_DOTTED_VERSION  RTENGINE_LIB_MAJOR "." RTENGINE_LIB_MINOR "." RTENGINE_LIB_MAINTENANCE


/** A module specific function should return RT_RETURN_NOT_IMPLEMENTED when a function
    is not implemented by the module. Since the value of this define is set to 100 it does not interfere
    with any RT native StormValue return messages (except return)*/
#define RT_RETURN_OK							0
#define RT_RETURN_NOT_IMPLEMENTED               100
#define RT_RETURN_ILLEGAL_SOURCE_ID             101
#define RT_RETURN_MAX_NOF_MODULES_REACHED       102
#define RT_RETURN_ILLEGAL_MODULE_INDEX          103
#define RT_RETURN_BUFFER_OVERFLOW               104
#define RT_RETURN_PASSAGE_DROPPED                 105
#define RT_RETURN_TIME_ERROR                    106
#define RT_RETURN_SETTING_NOT_ALLOWED           107



///** Represents an RTData instance. */
//typedef struct _RTDataStruct
//{
//  struct _RTDataStruct   *self;
//  int                             nofpassages;
//  RTPassage                    passages[RT_MAX_NR_IMAGES];        /** RTPassage is a pointer to opaque data type */ 
//  double                          timestamps[RT_MAX_NR_IMAGES];    /**< Timestamp of passage in seconds (and fraction of seconds) */
//  int                             sourceIds[RT_MAX_NR_IMAGES];  /**< ID of originating passagesource */ 
//  void                            *pUserData;
//  void                            (*userDataReleaseFunc)(void* userdata);
//  QThread_Semaphore               userDataIsInClone; /** count is more than 0 when userdata is still in a cloned instance of this data struct */
//  void                            *moduleData[MAX_NOF_MODULES];  /**< holds the data for each module */
//  int                             moduleReturnValue[MAX_NOF_MODULES];
//  int                             haserror;
//  int                             nofModules;  
//  void                            *pPrivate; /**< private field, do not use */
//} RTDataStruct;
//


/** PRE: should be called from one thread at a time */
typedef int (*RTEngineInitFunc)(void* initdata);

/** PRE: should be called from  one thread at a time */
typedef int (*RTEngineFinitFunc)(void);



/* I want these to work similar to Mt in intradaLive core,  but maybe I should create an instance for StormObject which contains the info for */

extern class RTEngine;
typedef RTEngine * RTEngineContext;

extern class ValueEngine;
typedef ValueEngine * ValueEngineContext;


/**
 * Initializes RTCore and all installed modules, should be called after RTCoreAddModule calls.
 *
 * This function should be called only once.
 * 
 *
 * @retval RT_RETURN_OK
 * @retval RT_RETURN_OUT_OF_MEMORY
 * @returns  All other RTInitLibrary return values
 *
 * @see RTCoreFinalize
 * @see RTCoreAddModule
 */
//extern int RTCoreInit(const char *company_name,
//                                 const char *license_key,
//								 const char* options,
//                                 void* initdata);

extern int RTCoreInit(const char *instance_name,
								 const char* options,
                                 RTEngineInstance* reference);


/**
 * Finalizes the RTCore library.
 *
 * The library should first have been initialized by calling RTCoreInitLibrary().
 * Note that this call does NOT call RTCoreDestroyFrameSource. Each created 
 * framesource has to be destroyed before calling finalize!
 *
 * @retval RT_RETURN_OK
 * @retval RT_RETURN_NOT_FOUND
 * @retval RT_RETURN_ALREADY_LINKED_ENGINE
 *
 */
extern int RTLinkToValueEngine();


/**
 * Finalizes the RTCore library.
 *
 * The library should first have been initialized by calling RTCoreInitLibrary().
 * Note that this call does NOT call RTCoreDestroyFrameSource. Each created 
 * framesource has to be destroyed before calling finalize!
 *
 * @retval RT_RETURN_OK
 *
 * @see RTCoreInit
 */
extern int RTCoreFinalize(void);



/**
 * Creates a data container
 *
 * @retval RT_RETURN_OK
 * @retval RT_RETURN_LIB_NOT_INITIALIZED
 * @retval RT_RETURN_OUT_OF_MEMORY
 */
extern int RTCoreCreateData(RTDataStruct** data);

/**
 * Makes a clone (copy) of the src data container. Note that any userdata in the data is not cloned but only the 
 * reference is copied to the clone. Therefore a change in the userdata effects all cloned data instances. However
 * only when the last clone of a data instance is destroyed any userdata in the instance is destroyed.
 *
 * @retval RT_RETURN_OK
 * @retval RT_RETURN_LIB_NOT_INITIALIZED
 * @retval RT_RETURN_OUT_OF_MEMORY
 */
extern int RTCoreCloneData(RTDataStruct* srcdata, RTDataStruct** cloneddata);

/**
 * Adds an image/passage to a data container
 *
 * @retval RT_RETURN_OK
 * @retval RT_RETURN_LIB_NOT_INITIALIZED
 * @retval RT_RETURN_ILLEGAL_DATA
 * @retval RT_RETURN_ILLEGAL_NR_IMAGES - There is already a passage in the data container, only 1 passage is allowed
 */
extern int RTCoreDataAddpassage(RTDataStruct* data, RTPassage passage, 
                                       double passagetime, int passageSourceId);


/**
 * Process the data.
 *
 * The function pushes the data struct containing at least a passage into the  
 * the chain of processing modules. The passage in the data container should not 
 * be destroyed by the user since it is placed as reference in the data struct. The passage can 
 * also not be used by the user anymore because destroy of the passage can happen during processing
 * by one of the modules. The same applies for userdata added to the data container
 *
 * @param[in]   passage     Reference to the passage to be processed
 * @param[in]   userdata  Reference to some user data. The userdata reference is stored in the data struct
 *
 * @retval RT_RETURN_OK
 * @retval RT_RETURN_LIB_NOT_INITIALIZED
 * @retval RT_RETURN_ILLEGAL_IMAGE
 * @retval RT_RETURN_ILLEGAL_NULL_POINTER
 * @retval RT_RETURN_INTERNAL_ERROR - passage has illegal/non existing sourceId
 */
extern int RTCoreProcess(RTDataStruct* data);





#endif //_RTENGINE_H_
