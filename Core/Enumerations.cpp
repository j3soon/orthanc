/**
 * Orthanc - A Lightweight, RESTful DICOM Store
 * Copyright (C) 2012-2015 Sebastien Jodogne, Medical Physics
 * Department, University Hospital of Liege, Belgium
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * In addition, as a special exception, the copyright holders of this
 * program give permission to link the code of its release with the
 * OpenSSL project's "OpenSSL" library (or with modified versions of it
 * that use the same license as the "OpenSSL" library), and distribute
 * the linked executables. You must obey the GNU General Public License
 * in all respects for all of the code used other than "OpenSSL". If you
 * modify file(s) with this exception, you may extend this exception to
 * your version of the file(s), but you are not obligated to do so. If
 * you do not wish to do so, delete this exception statement from your
 * version. If you delete this exception statement from all source files
 * in the program, then also delete it here.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/


#include "PrecompiledHeaders.h"
#include "Enumerations.h"

#include "OrthancException.h"
#include "Toolbox.h"

#include <string.h>

namespace Orthanc
{
  // This function is autogenerated by the script
  // "Resources/GenerateErrorCodes.py"
  const char* EnumerationToString(ErrorCode error)
  {
    switch (error)
    {
      case ErrorCode_InternalError:
        return "Internal error";

      case ErrorCode_Success:
        return "Success";

      case ErrorCode_Plugin:
        return "Error encountered within the plugin engine";

      case ErrorCode_NotImplemented:
        return "Not implemented yet";

      case ErrorCode_ParameterOutOfRange:
        return "Parameter out of range";

      case ErrorCode_NotEnoughMemory:
        return "Not enough memory";

      case ErrorCode_BadParameterType:
        return "Bad type for a parameter";

      case ErrorCode_BadSequenceOfCalls:
        return "Bad sequence of calls";

      case ErrorCode_InexistentItem:
        return "Accessing an inexistent item";

      case ErrorCode_BadRequest:
        return "Bad request";

      case ErrorCode_NetworkProtocol:
        return "Error in the network protocol";

      case ErrorCode_SystemCommand:
        return "Error while calling a system command";

      case ErrorCode_Database:
        return "Error with the database engine";

      case ErrorCode_UriSyntax:
        return "Badly formatted URI";

      case ErrorCode_InexistentFile:
        return "Inexistent file";

      case ErrorCode_CannotWriteFile:
        return "Cannot write to file";

      case ErrorCode_BadFileFormat:
        return "Bad file format";

      case ErrorCode_Timeout:
        return "Timeout";

      case ErrorCode_UnknownResource:
        return "Unknown resource";

      case ErrorCode_IncompatibleDatabaseVersion:
        return "Incompatible version of the database";

      case ErrorCode_FullStorage:
        return "The file storage is full";

      case ErrorCode_CorruptedFile:
        return "Corrupted file (e.g. inconsistent MD5 hash)";

      case ErrorCode_InexistentTag:
        return "Inexistent tag";

      case ErrorCode_ReadOnly:
        return "Cannot modify a read-only data structure";

      case ErrorCode_IncompatibleImageFormat:
        return "Incompatible format of the images";

      case ErrorCode_IncompatibleImageSize:
        return "Incompatible size of the images";

      case ErrorCode_SharedLibrary:
        return "Error while using a shared library (plugin)";

      case ErrorCode_UnknownPluginService:
        return "Plugin invoking an unknown service";

      case ErrorCode_UnknownDicomTag:
        return "Unknown DICOM tag";

      case ErrorCode_BadJson:
        return "Cannot parse a JSON document";

      case ErrorCode_Unauthorized:
        return "Bad credentials were provided to an HTTP request";

      case ErrorCode_BadFont:
        return "Badly formatted font file";

      case ErrorCode_DatabasePlugin:
        return "The plugin implementing a custom database back-end does not fulfill the proper interface";

      case ErrorCode_StorageAreaPlugin:
        return "Error in the plugin implementing a custom storage area";

      case ErrorCode_EmptyRequest:
        return "The request is empty";

      case ErrorCode_NotAcceptable:
        return "Cannot send a response which is acceptable according to the Accept HTTP header";

      case ErrorCode_SQLiteNotOpened:
        return "SQLite: The database is not opened";

      case ErrorCode_SQLiteAlreadyOpened:
        return "SQLite: Connection is already open";

      case ErrorCode_SQLiteCannotOpen:
        return "SQLite: Unable to open the database";

      case ErrorCode_SQLiteStatementAlreadyUsed:
        return "SQLite: This cached statement is already being referred to";

      case ErrorCode_SQLiteExecute:
        return "SQLite: Cannot execute a command";

      case ErrorCode_SQLiteRollbackWithoutTransaction:
        return "SQLite: Rolling back a nonexistent transaction (have you called Begin()?)";

      case ErrorCode_SQLiteCommitWithoutTransaction:
        return "SQLite: Committing a nonexistent transaction";

      case ErrorCode_SQLiteRegisterFunction:
        return "SQLite: Unable to register a function";

      case ErrorCode_SQLiteFlush:
        return "SQLite: Unable to flush the database";

      case ErrorCode_SQLiteCannotRun:
        return "SQLite: Cannot run a cached statement";

      case ErrorCode_SQLiteCannotStep:
        return "SQLite: Cannot step over a cached statement";

      case ErrorCode_SQLiteBindOutOfRange:
        return "SQLite: Bing a value while out of range (serious error)";

      case ErrorCode_SQLitePrepareStatement:
        return "SQLite: Cannot prepare a cached statement";

      case ErrorCode_SQLiteTransactionAlreadyStarted:
        return "SQLite: Beginning the same transaction twice";

      case ErrorCode_SQLiteTransactionCommit:
        return "SQLite: Failure when committing the transaction";

      case ErrorCode_SQLiteTransactionBegin:
        return "SQLite: Cannot start a transaction";

      case ErrorCode_DirectoryOverFile:
        return "The directory to be created is already occupied by a regular file";

      case ErrorCode_FileStorageCannotWrite:
        return "Unable to create a subdirectory or a file in the file storage";

      case ErrorCode_DirectoryExpected:
        return "The specified path does not point to a directory";

      case ErrorCode_HttpPortInUse:
        return "The TCP port of the HTTP server is already in use";

      case ErrorCode_DicomPortInUse:
        return "The TCP port of the DICOM server is already in use";

      case ErrorCode_BadHttpStatusInRest:
        return "This HTTP status is not allowed in a REST API";

      case ErrorCode_RegularFileExpected:
        return "The specified path does not point to a regular file";

      case ErrorCode_PathToExecutable:
        return "Unable to get the path to the executable";

      case ErrorCode_MakeDirectory:
        return "Cannot create a directory";

      case ErrorCode_BadApplicationEntityTitle:
        return "An application entity title (AET) cannot be empty or be longer than 16 characters";

      case ErrorCode_NoCFindHandler:
        return "No request handler factory for DICOM C-FIND SCP";

      case ErrorCode_NoCMoveHandler:
        return "No request handler factory for DICOM C-MOVE SCP";

      case ErrorCode_NoCStoreHandler:
        return "No request handler factory for DICOM C-STORE SCP";

      case ErrorCode_NoApplicationEntityFilter:
        return "No application entity filter";

      case ErrorCode_NoSopClassOrInstance:
        return "DicomUserConnection: Unable to find the SOP class and instance";

      case ErrorCode_NoPresentationContext:
        return "DicomUserConnection: No acceptable presentation context for modality";

      case ErrorCode_DicomFindUnavailable:
        return "DicomUserConnection: The C-FIND command is not supported by the remote SCP";

      case ErrorCode_DicomMoveUnavailable:
        return "DicomUserConnection: The C-MOVE command is not supported by the remote SCP";

      case ErrorCode_CannotStoreInstance:
        return "Cannot store an instance";

      case ErrorCode_CreateDicomNotString:
        return "Only string values are supported when creating DICOM instances";

      case ErrorCode_CreateDicomOverrideTag:
        return "Trying to override a value inherited from a parent module";

      case ErrorCode_CreateDicomUseContent:
        return "Use \"Content\" to inject an image into a new DICOM instance";

      case ErrorCode_CreateDicomNoPayload:
        return "No payload is present for one instance in the series";

      case ErrorCode_CreateDicomUseDataUriScheme:
        return "The payload of the DICOM instance must be specified according to Data URI scheme";

      case ErrorCode_CreateDicomBadParent:
        return "Trying to attach a new DICOM instance to an inexistent resource";

      case ErrorCode_CreateDicomParentIsInstance:
        return "Trying to attach a new DICOM instance to an instance (must be a series, study or patient)";

      case ErrorCode_CreateDicomParentEncoding:
        return "Unable to get the encoding of the parent resource";

      case ErrorCode_UnknownModality:
        return "Unknown modality";

      case ErrorCode_BadJobOrdering:
        return "Bad ordering of filters in a job";

      case ErrorCode_JsonToLuaTable:
        return "Cannot convert the given JSON object to a Lua table";

      case ErrorCode_CannotCreateLua:
        return "Cannot create the Lua context";

      case ErrorCode_CannotExecuteLua:
        return "Cannot execute a Lua command";

      case ErrorCode_LuaAlreadyExecuted:
        return "Arguments cannot be pushed after the Lua function is executed";

      case ErrorCode_LuaBadOutput:
        return "The Lua function does not give the expected number of outputs";

      case ErrorCode_NotLuaPredicate:
        return "The Lua function is not a predicate (only true/false outputs allowed)";

      case ErrorCode_LuaReturnsNoString:
        return "The Lua function does not return a string";

      case ErrorCode_StorageAreaAlreadyRegistered:
        return "Another plugin has already registered a custom storage area";

      case ErrorCode_DatabaseBackendAlreadyRegistered:
        return "Another plugin has already registered a custom database back-end";

      case ErrorCode_DatabaseNotInitialized:
        return "Plugin trying to call the database during its initialization";

      case ErrorCode_SslDisabled:
        return "Orthanc has been built without SSL support";

      case ErrorCode_CannotOrderSlices:
        return "Unable to order the slices of the series";

      default:
        if (error >= ErrorCode_START_PLUGINS)
        {
          return "Error encountered within some plugin";
        }
        else
        {
          return "Unknown error code";
        }
    }
  }


  const char* EnumerationToString(HttpMethod method)
  {
    switch (method)
    {
      case HttpMethod_Get:
        return "GET";

      case HttpMethod_Post:
        return "POST";

      case HttpMethod_Delete:
        return "DELETE";

      case HttpMethod_Put:
        return "PUT";

      default:
        return "?";
    }
  }


  const char* EnumerationToString(HttpStatus status)
  {
    switch (status)
    {
    case HttpStatus_100_Continue:
      return "Continue";

    case HttpStatus_101_SwitchingProtocols:
      return "Switching Protocols";

    case HttpStatus_102_Processing:
      return "Processing";

    case HttpStatus_200_Ok:
      return "OK";

    case HttpStatus_201_Created:
      return "Created";

    case HttpStatus_202_Accepted:
      return "Accepted";

    case HttpStatus_203_NonAuthoritativeInformation:
      return "Non-Authoritative Information";

    case HttpStatus_204_NoContent:
      return "No Content";

    case HttpStatus_205_ResetContent:
      return "Reset Content";

    case HttpStatus_206_PartialContent:
      return "Partial Content";

    case HttpStatus_207_MultiStatus:
      return "Multi-Status";

    case HttpStatus_208_AlreadyReported:
      return "Already Reported";

    case HttpStatus_226_IMUsed:
      return "IM Used";

    case HttpStatus_300_MultipleChoices:
      return "Multiple Choices";

    case HttpStatus_301_MovedPermanently:
      return "Moved Permanently";

    case HttpStatus_302_Found:
      return "Found";

    case HttpStatus_303_SeeOther:
      return "See Other";

    case HttpStatus_304_NotModified:
      return "Not Modified";

    case HttpStatus_305_UseProxy:
      return "Use Proxy";

    case HttpStatus_307_TemporaryRedirect:
      return "Temporary Redirect";

    case HttpStatus_400_BadRequest:
      return "Bad Request";

    case HttpStatus_401_Unauthorized:
      return "Unauthorized";

    case HttpStatus_402_PaymentRequired:
      return "Payment Required";

    case HttpStatus_403_Forbidden:
      return "Forbidden";

    case HttpStatus_404_NotFound:
      return "Not Found";

    case HttpStatus_405_MethodNotAllowed:
      return "Method Not Allowed";

    case HttpStatus_406_NotAcceptable:
      return "Not Acceptable";

    case HttpStatus_407_ProxyAuthenticationRequired:
      return "Proxy Authentication Required";

    case HttpStatus_408_RequestTimeout:
      return "Request Timeout";

    case HttpStatus_409_Conflict:
      return "Conflict";

    case HttpStatus_410_Gone:
      return "Gone";

    case HttpStatus_411_LengthRequired:
      return "Length Required";

    case HttpStatus_412_PreconditionFailed:
      return "Precondition Failed";

    case HttpStatus_413_RequestEntityTooLarge:
      return "Request Entity Too Large";

    case HttpStatus_414_RequestUriTooLong:
      return "Request-URI Too Long";

    case HttpStatus_415_UnsupportedMediaType:
      return "Unsupported Media Type";

    case HttpStatus_416_RequestedRangeNotSatisfiable:
      return "Requested Range Not Satisfiable";

    case HttpStatus_417_ExpectationFailed:
      return "Expectation Failed";

    case HttpStatus_422_UnprocessableEntity:
      return "Unprocessable Entity";

    case HttpStatus_423_Locked:
      return "Locked";

    case HttpStatus_424_FailedDependency:
      return "Failed Dependency";

    case HttpStatus_426_UpgradeRequired:
      return "Upgrade Required";

    case HttpStatus_500_InternalServerError:
      return "Internal Server Error";

    case HttpStatus_501_NotImplemented:
      return "Not Implemented";

    case HttpStatus_502_BadGateway:
      return "Bad Gateway";

    case HttpStatus_503_ServiceUnavailable:
      return "Service Unavailable";

    case HttpStatus_504_GatewayTimeout:
      return "Gateway Timeout";

    case HttpStatus_505_HttpVersionNotSupported:
      return "HTTP Version Not Supported";

    case HttpStatus_506_VariantAlsoNegotiates:
      return "Variant Also Negotiates";

    case HttpStatus_507_InsufficientStorage:
      return "Insufficient Storage";

    case HttpStatus_509_BandwidthLimitExceeded:
      return "Bandwidth Limit Exceeded";

    case HttpStatus_510_NotExtended:
      return "Not Extended";

    default:
      throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  const char* EnumerationToString(ResourceType type)
  {
    switch (type)
    {
      case ResourceType_Patient:
        return "Patient";

      case ResourceType_Study:
        return "Study";

      case ResourceType_Series:
        return "Series";

      case ResourceType_Instance:
        return "Instance";
      
      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  const char* EnumerationToString(ImageFormat format)
  {
    switch (format)
    {
      case ImageFormat_Png:
        return "Png";

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  const char* EnumerationToString(Encoding encoding)
  {
    switch (encoding)
    {
      case Encoding_Ascii:
        return "Ascii";

      case Encoding_Utf8:
        return "Utf8";

      case Encoding_Latin1:
        return "Latin1";

      case Encoding_Latin2:
        return "Latin2";

      case Encoding_Latin3:
        return "Latin3";

      case Encoding_Latin4:
        return "Latin4";

      case Encoding_Latin5:
        return "Latin5";

      case Encoding_Cyrillic:
        return "Cyrillic";

      case Encoding_Windows1251:
        return "Windows1251";

      case Encoding_Arabic:
        return "Arabic";

      case Encoding_Greek:
        return "Greek";

      case Encoding_Hebrew:
        return "Hebrew";

      case Encoding_Thai:
        return "Thai";

      case Encoding_Japanese:
        return "Japanese";

      case Encoding_Chinese:
        return "Chinese";

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  const char* EnumerationToString(PhotometricInterpretation photometric)
  {
    switch (photometric)
    {
      case PhotometricInterpretation_RGB:
        return "RGB";

      case PhotometricInterpretation_Monochrome1:
        return "Monochrome1";

      case PhotometricInterpretation_Monochrome2:
        return "Monochrome2";

      case PhotometricInterpretation_ARGB:
        return "ARGB";

      case PhotometricInterpretation_CMYK:
        return "CMYK";

      case PhotometricInterpretation_HSV:
        return "HSV";

      case PhotometricInterpretation_Palette:
        return "Palette color";

      case PhotometricInterpretation_YBRFull:
        return "YBR full";

      case PhotometricInterpretation_YBRFull422:
        return "YBR full 422";

      case PhotometricInterpretation_YBRPartial420:
        return "YBR partial 420"; 

      case PhotometricInterpretation_YBRPartial422:
        return "YBR partial 422"; 

      case PhotometricInterpretation_YBR_ICT:
        return "YBR ICT"; 

      case PhotometricInterpretation_YBR_RCT:
        return "YBR RCT"; 

      case PhotometricInterpretation_Unknown:
        return "Unknown";

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  const char* EnumerationToString(RequestOrigin origin)
  {
    switch (origin)
    {
      case RequestOrigin_Unknown:
        return "Unknown";

      case RequestOrigin_DicomProtocol:
        return "DicomProtocol";

      case RequestOrigin_Http:
        return "Http";

      case RequestOrigin_Plugins:
        return "Plugins";

      case RequestOrigin_Lua:
        return "Lua";

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  const char* EnumerationToString(LogLevel level)
  {
    switch (level)
    {
      case LogLevel_Error:
        return "ERROR";

      case LogLevel_Warning:
        return "WARNING";

      case LogLevel_Info:
        return "INFO";

      case LogLevel_Trace:
        return "TRACE";

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  Encoding StringToEncoding(const char* encoding)
  {
    std::string s(encoding);
    Toolbox::ToUpperCase(s);

    if (s == "UTF8")
    {
      return Encoding_Utf8;
    }

    if (s == "ASCII")
    {
      return Encoding_Ascii;
    }

    if (s == "LATIN1")
    {
      return Encoding_Latin1;
    }

    if (s == "LATIN2")
    {
      return Encoding_Latin2;
    }

    if (s == "LATIN3")
    {
      return Encoding_Latin3;
    }

    if (s == "LATIN4")
    {
      return Encoding_Latin4;
    }

    if (s == "LATIN5")
    {
      return Encoding_Latin5;
    }

    if (s == "CYRILLIC")
    {
      return Encoding_Cyrillic;
    }

    if (s == "WINDOWS1251")
    {
      return Encoding_Windows1251;
    }

    if (s == "ARABIC")
    {
      return Encoding_Arabic;
    }

    if (s == "GREEK")
    {
      return Encoding_Greek;
    }

    if (s == "HEBREW")
    {
      return Encoding_Hebrew;
    }

    if (s == "THAI")
    {
      return Encoding_Thai;
    }

    if (s == "JAPANESE")
    {
      return Encoding_Japanese;
    }

    if (s == "CHINESE")
    {
      return Encoding_Chinese;
    }

    throw OrthancException(ErrorCode_ParameterOutOfRange);
  }


  ResourceType StringToResourceType(const char* type)
  {
    std::string s(type);
    Toolbox::ToUpperCase(s);

    if (s == "PATIENT" || s == "PATIENTS")
    {
      return ResourceType_Patient;
    }
    else if (s == "STUDY" || s == "STUDIES")
    {
      return ResourceType_Study;
    }
    else if (s == "SERIES")
    {
      return ResourceType_Series;
    }
    else if (s == "INSTANCE"  || s == "IMAGE" || 
             s == "INSTANCES" || s == "IMAGES")
    {
      return ResourceType_Instance;
    }

    throw OrthancException(ErrorCode_ParameterOutOfRange);
  }


  ImageFormat StringToImageFormat(const char* format)
  {
    std::string s(format);
    Toolbox::ToUpperCase(s);

    if (s == "PNG")
    {
      return ImageFormat_Png;
    }

    throw OrthancException(ErrorCode_ParameterOutOfRange);
  }


  LogLevel StringToLogLevel(const char *level)
  {
    if (strcmp(level, "ERROR") == 0)
    {
      return LogLevel_Error;
    }
    else if (strcmp(level, "WARNING") == 0)
    {
      return LogLevel_Warning;
    }
    else if (strcmp(level, "INFO") == 0)
    {
      return LogLevel_Info;
    }
    else if (strcmp(level, "TRACE") == 0)
    {
      return LogLevel_Trace;
    }
    else 
    {
      throw OrthancException(ErrorCode_InternalError);
    }
  }


  unsigned int GetBytesPerPixel(PixelFormat format)
  {
    switch (format)
    {
      case PixelFormat_Grayscale8:
        return 1;

      case PixelFormat_Grayscale16:
      case PixelFormat_SignedGrayscale16:
        return 2;

      case PixelFormat_RGB24:
        return 3;

      case PixelFormat_RGBA32:
        return 4;

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  bool GetDicomEncoding(Encoding& encoding,
                        const char* specificCharacterSet)
  {
    std::string s = specificCharacterSet;
    Toolbox::ToUpperCase(s);

    // http://www.dabsoft.ch/dicom/3/C.12.1.1.2/
    // https://github.com/dcm4che/dcm4che/blob/master/dcm4che-core/src/main/java/org/dcm4che3/data/SpecificCharacterSet.java
    if (s == "ISO_IR 6" ||
        s == "ISO_IR 192" ||
        s == "ISO 2022 IR 6")
    {
      encoding = Encoding_Utf8;
    }
    else if (s == "ISO_IR 100" ||
             s == "ISO 2022 IR 100")
    {
      encoding = Encoding_Latin1;
    }
    else if (s == "ISO_IR 101" ||
             s == "ISO 2022 IR 101")
    {
      encoding = Encoding_Latin2;
    }
    else if (s == "ISO_IR 109" ||
             s == "ISO 2022 IR 109")
    {
      encoding = Encoding_Latin3;
    }
    else if (s == "ISO_IR 110" ||
             s == "ISO 2022 IR 110")
    {
      encoding = Encoding_Latin4;
    }
    else if (s == "ISO_IR 148" ||
             s == "ISO 2022 IR 148")
    {
      encoding = Encoding_Latin5;
    }
    else if (s == "ISO_IR 144" ||
             s == "ISO 2022 IR 144")
    {
      encoding = Encoding_Cyrillic;
    }
    else if (s == "ISO_IR 127" ||
             s == "ISO 2022 IR 127")
    {
      encoding = Encoding_Arabic;
    }
    else if (s == "ISO_IR 126" ||
             s == "ISO 2022 IR 126")
    {
      encoding = Encoding_Greek;
    }
    else if (s == "ISO_IR 138" ||
             s == "ISO 2022 IR 138")
    {
      encoding = Encoding_Hebrew;
    }
    else if (s == "ISO_IR 166" || s == "ISO 2022 IR 166")
    {
      encoding = Encoding_Thai;
    }
    else if (s == "ISO_IR 13" || s == "ISO 2022 IR 13")
    {
      encoding = Encoding_Japanese;
    }
    else if (s == "GB18030")
    {
      encoding = Encoding_Chinese;
    }
    /*
      else if (s == "ISO 2022 IR 149")
      {
      TODO
      }
      else if (s == "ISO 2022 IR 159")
      {
      TODO
      }
      else if (s == "ISO 2022 IR 87")
      {
      TODO
      }
    */
    else
    {
      return false;
    }

    // The encoding was properly detected
    return true;
  }


  ResourceType GetChildResourceType(ResourceType type)
  {
    switch (type)
    {
      case ResourceType_Patient:
        return ResourceType_Study;

      case ResourceType_Study:
        return ResourceType_Series;
        
      case ResourceType_Series:
        return ResourceType_Instance;

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  ResourceType GetParentResourceType(ResourceType type)
  {
    switch (type)
    {
      case ResourceType_Study:
        return ResourceType_Patient;
        
      case ResourceType_Series:
        return ResourceType_Study;

      case ResourceType_Instance:
        return ResourceType_Series;

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  DicomModule GetModule(ResourceType type)
  {
    switch (type)
    {
      case ResourceType_Patient:
        return DicomModule_Patient;

      case ResourceType_Study:
        return DicomModule_Study;
        
      case ResourceType_Series:
        return DicomModule_Series;

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }



  const char* GetDicomSpecificCharacterSet(Encoding encoding)
  {
    // http://www.dabsoft.ch/dicom/3/C.12.1.1.2/
    switch (encoding)
    {
      case Encoding_Utf8:
      case Encoding_Ascii:
        return "ISO_IR 192";

      case Encoding_Latin1:
        return "ISO_IR 100";

      case Encoding_Latin2:
        return "ISO_IR 101";

      case Encoding_Latin3:
        return "ISO_IR 109";

      case Encoding_Latin4:
        return "ISO_IR 110";

      case Encoding_Latin5:
        return "ISO_IR 148";

      case Encoding_Cyrillic:
        return "ISO_IR 144";

      case Encoding_Arabic:
        return "ISO_IR 127";

      case Encoding_Greek:
        return "ISO_IR 126";

      case Encoding_Hebrew:
        return "ISO_IR 138";

      case Encoding_Japanese:
        return "ISO_IR 13";

      case Encoding_Chinese:
        return "GB18030";

      case Encoding_Thai:
        return "ISO_IR 166";

      default:
        throw OrthancException(ErrorCode_ParameterOutOfRange);
    }
  }


  // This function is autogenerated by the script
  // "Resources/GenerateErrorCodes.py"
  HttpStatus ConvertErrorCodeToHttpStatus(ErrorCode error)
  {
    switch (error)
    {
      case ErrorCode_Success:
        return HttpStatus_200_Ok;

      case ErrorCode_ParameterOutOfRange:
        return HttpStatus_400_BadRequest;

      case ErrorCode_BadParameterType:
        return HttpStatus_400_BadRequest;

      case ErrorCode_InexistentItem:
        return HttpStatus_404_NotFound;

      case ErrorCode_BadRequest:
        return HttpStatus_400_BadRequest;

      case ErrorCode_UriSyntax:
        return HttpStatus_400_BadRequest;

      case ErrorCode_InexistentFile:
        return HttpStatus_404_NotFound;

      case ErrorCode_BadFileFormat:
        return HttpStatus_400_BadRequest;

      case ErrorCode_UnknownResource:
        return HttpStatus_404_NotFound;

      case ErrorCode_InexistentTag:
        return HttpStatus_404_NotFound;

      case ErrorCode_BadJson:
        return HttpStatus_400_BadRequest;

      case ErrorCode_Unauthorized:
        return HttpStatus_401_Unauthorized;

      case ErrorCode_NotAcceptable:
        return HttpStatus_406_NotAcceptable;

      default:
        return HttpStatus_500_InternalServerError;
    }
  }


  bool IsUserContentType(FileContentType type)
  {
    return (type >= FileContentType_StartUser &&
            type <= FileContentType_EndUser);
  }
}
