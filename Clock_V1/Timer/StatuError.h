#ifndef STATUSERROR_H                                                                                                                                                        
#define STATUSERROR_H                                                                                                                                                        
                                                                                                                                                                             
/*******************************************************************************                                                                                             
 * includes                                                                                                                                                                  
 ******************************************************************************/                                                                                             
                                                                                                                                                                             
/*******************************************************************************                                                                                             
 * enumerations                                                                                                                                                              
 ******************************************************************************/                                                                                             
                                                                                                                                                                             
/**                                                                                                                                                                          
 * @brief Error code definition.                                                                                                                                             
 */                                                                                                                                                                          
typedef enum {                                                                                                                                                               
    StatusErrNone            =  0, //!< No error, success.                                                                                                                   
    StatusErrParam           =  1, //!< Invalid parameter supplied, e.g. NULL pointer supplied.                                                                              
    StatusErrParamVal        =  2, //!< Invalid parameter value supplied.                                                                                                    
    StatusErrNotSupported    =  3, //!< Functionality is not supported.                                                                                                      
    StatusErrNotImplemented  =  4, //!< Functionality is not (yet) implemented.                                                                                              
    StatusErrNotAvailable    =  5, //!< Desired entity is not available.                                                                                                     
    StatusErrNotActive       =  6, //!< Desired entity is not active.                                                                                                        
    StatusErrNotExpected     =  7, //!< Received data not expected.                                                                                                          
    StatusErrType            =  8, //!< Invalid type supplied.                                                                                                               
    StatusErrSize            =  9, //!< Invalid size supplied.                                                                                                               
    StatusErrRange           = 10, //!< Range error.                                                                                                                         
    StatusErrOutOfMemory     = 11, //!< No more memory available.                                                                                                            
    StatusErrBufferOverflow  = 12, //!< Buffer to small to yield intended data.                                                                                              
    StatusErrNoData          = 13, //!< No data available.                                                                                                                   
    StatusErrNoNet           = 14, //!< Connection lost.                                                                                                                     
    StatusErrDenied          = 15, //!< Access denied.                                                                                                                       
    StatusErrTime            = 16, //!< Timeout.                                                                                                                             
    StatusErrIo              = 17, //!< IO error while performing action.                                                                                                    
    StatusErrHandle          = 18, //!< Handle error, invalid or uninitialized handled supplied.                                                                             
    StatusErrLayer           = 19, //!< Layer error, a layer specific error code.                                                                                            
    StatusErrBusy            = 20, //!< Busy error, a command is still running.                                                                                              
    StatusErrCorrupted       = 21, //!< Message or data corrupted.                                                                                                           
    StatusErrOs              = 22, //!< Os error.                                                                                                                            
    StatusErrBufferUnderflow = 23, //!< Buffer contains less data than expected.                                                                                             
    StatusErrExecution       = 24, //!< Desired action or function couldn't be executed successful, a refined entity specific error code is available for further evaluation.
    StatusErrInterface       = 25, //!< Invalid interface Supplied.                                                                                                          
    StatusErrId              = 26, //!< Invalid id supplied.                                                                                                                 
    StatusErrOffset          = 27, //!< Invalid offset supplied.                                                                                                             
    StatusErrNotPossible     = 28, //!< Functionality can't be executed in the entities current state.                                                                       
    StatusErrIndex           = 29, //!< Invalid index supplied.                                                                                                              
    /* --- */                                                                                                                                                                
    StatusErrLast            = 30, //!< Internal Limit. Do NOT use as error code.                                                                                            
} StatusError;                                                                                                                                                               
                                                                                                                                                                             
/*******************************************************************************                                                                                             
 * functions                                                                                                                                                                 
 ******************************************************************************/                                                                                             

/**
 * @brief Convert error code to unique short name.
 *
 * @param[in]  errCode  Error code.
 *
 * @return char *
 * @retval String with corresponding short name, when code is valid.
 * @retval StringUnk marked with "unknown" when input is not valid.
 */
const char * StatusErrorToString(const StatusError errCode);

/**                                                                                                                                                                          
 * @}                                                                                                                                                                        
 */                                                                                                                                                                          
                                                                                                                                                                             
#endif /* STATUSERROR_H */                                                                                                                                                   
                                                                                                                                                                             