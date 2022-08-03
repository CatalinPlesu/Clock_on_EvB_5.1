/*******************************************************************************                                 
 * includes                                                                                                      
 ******************************************************************************/                                 
#include "StatuError.h"
                                                                                                                 
/*******************************************************************************                                 
 * local vars                                                                                                    
 ******************************************************************************/                                 
                                                                                                                 
// Case of an unitialized value or a wrongly used value in place of StatusError.                                    
static const char * s_StatusErrorUnknown = "Unknown-StatusError";                                                      
                                                                                                                 
// The lookup array used for conversion from StatusError enum.                                                      
//                                                                                                               
// Every time the initial StatusError enum is changed,                                                              
//  this lookup array should be updated, too,                                                                    
//  in order to have an 1-to-1 relation.                                                                         
//                                                                                                               
static const char * s_StatusErrorToString[StatusErrLast] = {
    "StatusErrNone",
    "StatusErrParam",
    "StatusErrParamVal",
    "StatusErrNotSupported",
    "StatusErrNotImplemented",
    "StatusErrNotAvailable",
    "StatusErrNotActive",
    "StatusErrNotExpected",
    "StatusErrType",
    "StatusErrSize",
    "StatusErrRange",
    "StatusErrOutOfMemory",
    "StatusErrBufferOverflow",
    "StatusErrNoData",
    "StatusErrNoNet",
    "StatusErrDenied",
    "StatusErrTime",
    "StatusErrIo",
    "StatusErrHandle",
    "StatusErrLayer",
    "StatusErrBusy",
    "StatusErrCorrupted",
    "StatusErrOs",
    "StatusErrBufferUnderflow",
    "StatusErrExecution",
    "StatusErrInterface",
    "StatusErrId",
    "StatusErrOffset",
    "StatusErrNotPossible",
    "StatusErrIndex"
};                                                                                                               
                                                                                                                 
/*******************************************************************************                                 
 * functions                                                                                                     
 ******************************************************************************/                                 
const char * StatusErrorToString(const StatusError errCode)                                                            
{                                                                                                                
    // Enum is considered unsigned by the compiler.                                                              
    if (errCode >= StatusErrLast)                                                                                   
    {                                                                                                            
        return s_StatusErrorUnknown;                                                                                
    }                                                                                                            
                                                                                                                 
    return s_StatusErrorToString[errCode];                                                                          
}                                                                                                                
                                                                                                                 